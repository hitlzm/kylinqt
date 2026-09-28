#include "DataRecorder.h"
#include "../vlcvideo/VlcVideoItem.h"
#include "../serialport/serialport_image.h"
#include "../serialport/serialport_laser.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

DataRecorder::DataRecorder(QObject *parent)
    : QObject(parent)
{
    m_flushTimer.setInterval(200);
    m_flushTimer.setTimerType(Qt::CoarseTimer);
    // image/laser txt 与转台 csv 共用同一个落盘节拍
    connect(&m_flushTimer, &QTimer::timeout, this, &DataRecorder::flushAll);

    // ffmpeg 转封装结束：成功删 TS，失败保留原始文件
    connect(&m_ffmpegProc,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus status) {
        // FailedToStart 等错误已由 errorOccurred 处理并清空路径，避免重复上报
        if (m_videoMp4Path.isEmpty())
            return;
        const bool ok = (exitCode == 0 && status == QProcess::NormalExit);
        if (ok) {
            QFile::remove(m_videoTsPath);
            qDebug() << "[DataRecorder] 视频转封装完成:" << m_videoMp4Path;
        } else {
            emit errorOccurred(QString("视频转封装失败(ffmpeg exit=%1)，已保留原始文件：%2")
                               .arg(exitCode).arg(m_videoTsPath));    //可支持QML弹窗提示
        }
        emit videoConvertFinished(ok, m_videoMp4Path);   //可支持QML弹窗提示
        m_videoMp4Path.clear();
    });

    // ffmpeg 启动失败（找不到程序等）也要提示
    connect(&m_ffmpegProc, &QProcess::errorOccurred,
            this, [this](QProcess::ProcessError err) {
        if (err == QProcess::FailedToStart) {
            emit errorOccurred("无法启动 ffmpeg，请检查 ffmpegPath 配置: " + m_ffmpegPath);
            emit videoConvertFinished(false, m_videoMp4Path);
            m_videoMp4Path.clear();
        }
    });
}

DataRecorder::~DataRecorder()
{
    // 析构时兜底：停止录制并收尾文件（正常流程应走 stopSave）
    m_flushTimer.stop();
    flushAll();
    if (m_imageFile.isOpen())
        m_imageFile.close();
    if (m_laserFile.isOpen())
        m_laserFile.close();
    if (m_csvFile.isOpen())
        m_csvFile.close();
    if (m_videoItem && m_saving)
        m_videoItem->stopRecord();
}

// ── 配置 ──
void DataRecorder::setSaveDir(const QString &dir)
{
    if (m_saveDir != dir) {
        m_saveDir = dir;
        emit saveDirChanged();
    }
}

void DataRecorder::setFfmpegPath(const QString &path)
{
    if (m_ffmpegPath != path) {
        m_ffmpegPath = path;
        emit ffmpegPathChanged();
    }
}

void DataRecorder::setVideoItem(VlcVideoItem *item)
{
    m_videoItem = item;
    if (item) {
        // 换源/重连会走 VlcVideoItem::releasePlayer() → stopRecord()，
        // 那里把 recording 置 false 并发出本信号，是中途发现录制被打断的唯一入口
        connect(item, &VlcVideoItem::recordingChanged,
                this, &DataRecorder::handleVideoRecordingChanged,
                Qt::UniqueConnection);
    }
}

void DataRecorder::setImageData(ImageData *data)
{
    m_imageData = data;
}

void DataRecorder::setLaserData(LaserData *data)
{
    m_laserData = data;
}

// ── 开始/停止保存 ──
bool DataRecorder::startSave()
{
    if (m_saving) {
        emit errorOccurred("已经在保存中，请先停止再开始");
        return false;
    }
    if (m_ffmpegProc.state() != QProcess::NotRunning) {
        emit errorOccurred("上一段视频还在转封装，请稍后再试");
        return false;
    }

    const QDateTime now = QDateTime::currentDateTime();
    m_sessionDate = now.toString("M.d");      // 8.21
    m_sessionTime = now.toString("HHmmss");   // 143025

    if (!ensureSaveFolder())
        return false;

    // 复位上一会话的路径/状态（本会话懒创建时会重新生成）
    m_imagePath.clear();
    m_imageOpenFailed = false;
    m_laserPath.clear();
    m_laserOpenFailed = false;
    m_csvPath.clear();
    m_csvOpenFailed = false;
    m_turntableSeq = 0;
    m_videoTsPath.clear();
    m_videoMp4Path.clear();
    m_videoRecordLost = false;
    // 上一会话若异常未关闭，这里兜底关闭，避免继续写旧文件
    if (m_imageFile.isOpen())
        m_imageFile.close();
    if (m_laserFile.isOpen())
        m_laserFile.close();
    if (m_csvFile.isOpen())
        m_csvFile.close();

    m_saving = true;
    emit savingChanged();

    startVideoRecord();
    m_flushTimer.start();
    qDebug() << "[DataRecorder] 开始保存，目录:" << m_sessionDir
             << " image:" << m_imagePath << " laser:" << m_laserPath
             << " csv:" << m_csvPath
             << " video:" << m_videoTsPath;
    return true;
}

void DataRecorder::stopSave()
{
    if (!m_saving)
        return;

    m_saving = false;
    emit savingChanged();

    m_flushTimer.stop();
    flushAll();
    if (m_imageFile.isOpen())
        m_imageFile.close();
    if (m_laserFile.isOpen())
        m_laserFile.close();
    if (m_csvFile.isOpen())
        m_csvFile.close();

    stopVideoRecord();

    // 视频开录过才转封装；若录制中途被换源/重连打断，那次收尾里已经转过了
    //（m_videoMp4Path 会被转封装结束回调清空，转封装进行中则这里不再重复启动）
    if (!m_videoMp4Path.isEmpty() && m_ffmpegProc.state() == QProcess::NotRunning)
        startRemux();
}

// ── B帧枚举字段 → 直观文本（保留原始码值，便于与协议核对）──
static QString imageCodeText(int code, const char *name)
{
    const QString hex = QString::number(static_cast<uint>(code) & 0xFFu, 16)
                            .rightJustified(2, QLatin1Char('0')).toUpper();
    return QString("%1(0x%2)").arg(QString::fromUtf8(name), hex);
}

// 字节11: 当前工作通道
static QString imageWorkChannelText(int v)
{
    switch (v) {
    case 0x00: return imageCodeText(v, "电视");
    case 0x01: return imageCodeText(v, "红外");
    default:   return imageCodeText(v, "未定义");
    }
}

// 字节18: 光学工作状态
static QString imageOpticalWorkStateText(int v)
{
    switch (v) {
    case 0x02: return imageCodeText(v, "搜索状态（预置状态）");
    case 0x03: return imageCodeText(v, "跟踪状态");
    case 0x04: return imageCodeText(v, "框架角电锁零位状态");
    case 0x05: return imageCodeText(v, "记忆状态");
    case 0x06: return imageCodeText(v, "解锁状态");
    default:   return imageCodeText(v, "未定义");
    }
}

// 字节32: 跟踪状态
static QString imageTrackingStateText(int v)
{
    switch (v) {
    case 0x00: return imageCodeText(v, "默认");
    case 0x11: return imageCodeText(v, "搜索中");
    case 0x22: return imageCodeText(v, "目标丢失");
    case 0x33: return imageCodeText(v, "目标锁定");
    case 0x44: return imageCodeText(v, "记忆状态");
    default:   return imageCodeText(v, "未定义");
    }
}

// 字节33: 跟踪器状态
static QString imageTrackerStateText(int v)
{
    switch (v) {
    case 0x00: return imageCodeText(v, "空闲状态");
    case 0x01: return imageCodeText(v, "跟踪状态");
    case 0x02: return imageCodeText(v, "识别状态");
    case 0x03: return imageCodeText(v, "匹配状态");
    case 0x04: return imageCodeText(v, "仅识别状态");
    default:   return imageCodeText(v, "未定义");
    }
}

// ── 图像 B 帧（解析后的工程量，一行一帧）──
void DataRecorder::onImageFrame()
{
    if (!m_saving || !m_imageData)
        return;
    ensureImageFile();
    if (m_imageOpenFailed)
        return;

    // 时间：由 B 帧“返回时间戳1(毫秒) + 返回时间戳2(微秒)”合并后的北京时间
    //（与界面显示同源，ImageData 在解析时已按 UTC+8 折算好；未收到时间同步前为空）
    const QString timeText = (m_imageData->msTime() > 0) ? m_imageData->recvTimeText()
                                                         : QString();

    QString line;
    line.reserve(200);
    line += timeText;
    line += ','; line += QString::number(m_imageData->bFrameSequence());
    line += ','; line += imageWorkChannelText(m_imageData->currentWorkChannel());
    line += ','; line += QString::number(m_imageData->pitchLosAngVel(), 'f', 4);
    line += ','; line += QString::number(m_imageData->yawLosAngVel(), 'f', 4);
    line += ','; line += imageOpticalWorkStateText(m_imageData->opticalWorkState());
    line += ','; line += QString::number(m_imageData->pitchFrameAngle(), 'f', 4);
    line += ','; line += QString::number(m_imageData->yawFrameAngle(), 'f', 4);
    line += ','; line += QString::number(m_imageData->pitchGyro(), 'f', 4);
    line += ','; line += QString::number(m_imageData->yawGyro(), 'f', 4);
    line += ','; line += imageTrackingStateText(m_imageData->trackingState());
    line += ','; line += imageTrackerStateText(m_imageData->trackerState());
    line += ','; line += QString::number(m_imageData->infraredFrameNum());
    line += ','; line += QString::number(m_imageData->cbhTv4405());
    line += '\n';

    m_imageBuffer.append(line.toUtf8());
}

// 字节24: 增益状态（0xB0~0xB4 五级起控状态，与界面显示一致用十六进制）
static QString laserGainStatusText(int v)
{
    return QStringLiteral("0x")
            + QString::number(static_cast<uint>(v) & 0xFFu, 16)
                      .rightJustified(2, QLatin1Char('0')).toUpper();
}

// ── 激光接收帧（解析后的工程量，一行一帧）──
void DataRecorder::onLaserFrame()
{
    if (!m_saving || !m_laserData)
        return;
    ensureLaserFile();
    if (m_laserOpenFailed)
        return;

    // 时间：由接收帧“返回时间戳1(毫秒) + 返回时间戳2(微秒)”合并后的北京时间
    //（解析时已按 UTC+8 折算好；未收到时间同步前为空）
    const QString timeText = (m_laserData->msTime() > 0) ? m_laserData->recvTimeText()
                                                         : QString();

    QString line;
    line.reserve(220);
    line += timeText;
    line += ','; line += QString::number(m_laserData->opticalAzimuth(), 'f', 4);
    line += ','; line += QString::number(m_laserData->opticalPitch(), 'f', 4);
    line += ','; line += QString::number(m_laserData->gyroAzimuthRate(), 'f', 4);
    line += ','; line += QString::number(m_laserData->gyroPitchRate(), 'f', 4);
    line += ','; line += QString::number(m_laserData->losAzimuthRate(), 'f', 4);
    line += ','; line += QString::number(m_laserData->losPitchRate(), 'f', 4);
    line += ','; line += QString::number(m_laserData->deviationAzimuth(), 'f', 4);
    line += ','; line += QString::number(m_laserData->deviationPitch(), 'f', 4);
    line += ','; line += QString::number(m_laserData->laserPeriod(), 'f', 4);
    line += ','; line += laserGainStatusText(m_laserData->gainStatus());
    line += ','; line += QString::number(m_laserData->quadrant1Energy(), 'f', 4);
    line += ','; line += QString::number(m_laserData->quadrant2Energy(), 'f', 4);
    line += ','; line += QString::number(m_laserData->quadrant3Energy(), 'f', 4);
    line += ','; line += QString::number(m_laserData->quadrant4Energy(), 'f', 4);
    line += '\n';

    m_laserBuffer.append(line.toUtf8());
}

// 转台三轴状态（convertHexStatusToLegacy 得到的编码）→ 中文，与界面显示同一套叫法
static QString turntableAxisStatusText(int code)
{
    switch (code) {
    case 0x00: return QStringLiteral("空闲");
    case 0x01: return QStringLiteral("伺服");
    case 0x02: return QStringLiteral("回零执行中");
    case 0x03: return QStringLiteral("位置执行中");
    case 0x04: return QStringLiteral("速率执行中");
    case 0x05: return QStringLiteral("速率稳定");
    case 0x06: return QStringLiteral("摇摆执行中");
    case 0x07: return QStringLiteral("摇摆稳定");
    case 0x08: return QStringLiteral("停车执行中");
    case 0x09: return QStringLiteral("跟踪模式1");      // 250ms 跟踪
    case 0x0A: return QStringLiteral("停止跟踪");
    case 0x0B: return QStringLiteral("跟踪模式2");      // 5ms 跟踪
    case 0x0F: return QStringLiteral("速度环模式");
    case 0x1F: return QStringLiteral("驱动器报警");
    case 0x20: return QStringLiteral("伺服超差报警");
    case 0x21: return QStringLiteral("正向限位报警");
    case 0x22: return QStringLiteral("逆向限位报警");
    case 0x23: return QStringLiteral("时钟同步报警");
    case 0x24: return QStringLiteral("初始化报警");
    case 0x25: return QStringLiteral("限位开关同时导通");
    case 0x26: return QStringLiteral("编码器故障报警");
    case 0x29: return QStringLiteral("瞬态电流报警");
    case 0x2A: return QStringLiteral("连续电流报警");
    default:   return QStringLiteral("未知(0x%1)")
                      .arg(QString::number(code, 16).rightJustified(2, QLatin1Char('0')).toUpper());
    }
}

// 转台指令提示（接收帧 byte26 低6位）→ 中文指令名，与界面 ctlNumberText 同一套叫法
static QString turntableCmdHintText(int code)
{
    switch (code) {
    case 0x00: return QStringLiteral("无");        // 未收到有效指令
    case 0x01: return QStringLiteral("使能");
    case 0x02: return QStringLiteral("停车");
    case 0x03: return QStringLiteral("回零");
    case 0x04: return QStringLiteral("位置");
    case 0x05: return QStringLiteral("速率");
    case 0x06: return QStringLiteral("摇摆");
    case 0x0A: return QStringLiteral("250ms跟踪");
    case 0x0C: return QStringLiteral("5ms跟踪");
    case 0x10: return QStringLiteral("时间设置");
    case 0x11: return QStringLiteral("跟踪修正");
    case 0x1F: return QStringLiteral("复位");
    default:   return QStringLiteral("未知(0x%1)")
                      .arg(QString::number(code, 16).rightJustified(2, QLatin1Char('0')).toUpper());
    }
}

// ── 转台周期状态帧 ──
void DataRecorder::onTurntableFrame(const StatusFeedbackHex &frame)
{
    if (!m_saving)
        return;
    ensureCsvFile();
    if (m_csvOpenFailed)
        return;

    // 一行一帧。角度/控制偏差按工程单位（0.0001°）保留 4 位小数；
    // 状态与指令提示存中文（不存代号），序号用于事后核对转台的固定周期。
    QString line;
    line.reserve(160);
    line += QString::number(++m_turntableSeq);
    line += ','; line += QString::number(frame.m_time);
    line += ','; line += turntableCmdHintText(frame.m_cmdHint);
    line += ','; line += turntableAxisStatusText(frame.m_inner_statusnumber);
    line += ','; line += QString::number(frame.m_inner_angle, 'f', 4);
    line += ','; line += QString::number(frame.m_inner_ctlDeviation, 'f', 4);
    line += ','; line += turntableAxisStatusText(frame.m_middle_statusnumber);
    line += ','; line += QString::number(frame.m_middle_angle, 'f', 4);
    line += ','; line += QString::number(frame.m_middle_ctlDeviation, 'f', 4);
    line += ','; line += turntableAxisStatusText(frame.m_outter_statusnumber);
    line += ','; line += QString::number(frame.m_outter_angle, 'f', 4);
    line += ','; line += QString::number(frame.m_outter_ctlDeviation, 'f', 4);
    line += ','; line += (frame.m_hasSecPulse ? QStringLiteral("有") : QStringLiteral("无"));
    line += '\n';

    m_csvBuffer.append(line.toUtf8());
}

// ── 内部 ──
bool DataRecorder::ensureSaveFolder()
{
    m_sessionDir = m_saveDir + "/" + m_sessionDate;
    if (!QDir().mkpath(m_sessionDir)) {
        emit errorOccurred("无法创建保存目录: " + m_sessionDir);
        return false;
    }
    return true;
}

void DataRecorder::ensureImageFile()
{
    if (m_imageFile.isOpen() || m_imageOpenFailed)
        return;

    m_imagePath = makeUniquePath(m_sessionDir + "/image" + m_sessionTime + ".csv");
    m_imageFile.setFileName(m_imagePath);
    if (!m_imageFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_imageOpenFailed = true;   // 打开失败：停止重试，避免每帧重复报错
        emit errorOccurred("无法创建图像数据文件: " + m_imagePath);
        return;
    }
    // 表头（与 onImageFrame 的列序严格一致；带 UTF-8 BOM，方便表格软件识别中文表头）
    m_imageFile.write("\xEF\xBB\xBF"
                      "时间,B帧流水号,当前工作通道,俯仰视线角速度(°/s),偏航视线角速度(°/s),"
                      "光学工作状态,俯仰框架角(°),偏航框架角(°),"
                      "俯仰陀螺(°/s),偏航陀螺(°/s),跟踪状态,跟踪器状态,"
                      "红外帧编号,电视帧编号\n");
    qDebug() << "[DataRecorder] 图像数据保存到:" << m_imagePath;
}

void DataRecorder::ensureLaserFile()
{
    if (m_laserFile.isOpen() || m_laserOpenFailed)
        return;

    m_laserPath = makeUniquePath(m_sessionDir + "/laser" + m_sessionTime + ".csv");
    m_laserFile.setFileName(m_laserPath);
    if (!m_laserFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_laserOpenFailed = true;
        emit errorOccurred("无法创建激光数据文件: " + m_laserPath);
        return;
    }
    // 表头（与 onLaserFrame 的列序严格一致；带 UTF-8 BOM，方便表格软件识别中文表头）
    m_laserFile.write("\xEF\xBB\xBF"
                      "时间,光轴方位角(°),光轴俯仰角(°),"
                      "方位陀螺输出角速度(°/s),俯仰陀螺输出角速度(°/s),"
                      "方位视线角速度(°/s),俯仰视线角速度(°/s),"
                      "方位偏差角(°),俯仰偏差角(°),激光周期(ms),增益状态,"
                      "第一象限能量强度,第二象限能量强度,第三象限能量强度,第四象限能量强度\n");
    qDebug() << "[DataRecorder] 激光数据保存到:" << m_laserPath;
}

void DataRecorder::flushImage()
{
    if (!m_imageFile.isOpen() || m_imageBuffer.isEmpty())
        return;
    const qint64 written = m_imageFile.write(m_imageBuffer);
    if (written < 0) {
        emit errorOccurred("图像数据写入失败: " + m_imagePath);
        return;   // 保留缓冲，下次 flush 重试
    }
    m_imageBuffer.clear();
}

void DataRecorder::flushLaser()
{
    if (!m_laserFile.isOpen() || m_laserBuffer.isEmpty())
        return;
    const qint64 written = m_laserFile.write(m_laserBuffer);
    if (written < 0) {
        emit errorOccurred("激光数据写入失败: " + m_laserPath);
        return;   // 保留缓冲，下次 flush 重试
    }
    m_laserBuffer.clear();
}

void DataRecorder::ensureCsvFile()
{
    if (m_csvFile.isOpen() || m_csvOpenFailed)
        return;

    m_csvPath = makeUniquePath(m_sessionDir + "/turntable" + m_sessionTime + ".csv");
    m_csvFile.setFileName(m_csvPath);
    if (!m_csvFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_csvOpenFailed = true;   // 打开失败：停止重试，避免每帧重复报错
        emit errorOccurred("无法创建转台数据文件: " + m_csvPath);
        return;
    }
    // 表头（与 onTurntableFrame 的列序严格一致；带 UTF-8 BOM，方便表格软件识别中文表头）
    m_csvFile.write("\xEF\xBB\xBF"
                    "序号,转台毫秒时间,指令提示,"
                    "内框状态,内框角度(°),内框控制偏差(°),"
                    "中框状态,中框角度(°),中框控制偏差(°),"
                    "外框状态,外框角度(°),外框控制偏差(°),"
                    "秒脉冲\n");
    qDebug() << "[DataRecorder] 转台数据保存到:" << m_csvPath;
}

void DataRecorder::flushCsv()
{
    if (!m_csvFile.isOpen() || m_csvBuffer.isEmpty())
        return;
    const qint64 written = m_csvFile.write(m_csvBuffer);
    if (written < 0) {
        emit errorOccurred("转台数据写入失败: " + m_csvPath);
        return;   // 保留缓冲，下次 flush 重试
    }
    m_csvBuffer.clear();
}

void DataRecorder::flushAll()
{
    flushImage();
    flushLaser();
    flushCsv();
}

void DataRecorder::startVideoRecord()
{
    if (!m_videoItem) {
        emit errorOccurred("视频录制不可用：未找到视频播放器");
        return;
    }
    // ts 与 mp4 成对判重，避免 -y 覆盖同秒内上一会话生成的 mp4
    QString stem = "video" + m_sessionTime;
    QString tsBase = m_sessionDir + "/" + stem + ".ts";
    QString mp4Path = m_sessionDir + "/" + stem + ".mp4";
    int n = 1;
    while (QFile::exists(tsBase) || QFile::exists(mp4Path)) {
        stem = "video" + m_sessionTime + "_" + QString::number(n++);
        tsBase = m_sessionDir + "/" + stem + ".ts";
        mp4Path = m_sessionDir + "/" + stem + ".mp4";
    }
    m_videoTsPath = tsBase;
    m_videoMp4Path = mp4Path;
    m_videoItem->startRecord(m_videoTsPath);
}

void DataRecorder::stopVideoRecord()
{
    if (m_videoItem)
        m_videoItem->stopRecord();
}

// 视频录制状态变化：保存期间由 true 变 false，说明录制被换源/重连打断了
//（VlcVideoItem::releasePlayer() → stopRecord()；正常收尾时 m_saving 已先置 false，不会走到这里）
void DataRecorder::handleVideoRecordingChanged()
{
    if (!m_saving || !m_videoItem)
        return;
    if (m_videoItem->recording() || m_videoRecordLost)
        return;   // 只处理 true -> false，且每次会话只处理一次

    m_videoRecordLost = true;

    // 视频录制就地停止，本会话不再续录：把已录到的部分立刻转封装，
    // 这样界面上立刻能拿到可用的 mp4，而不是等到 stopSave 才发现内容缺失。
    QString detail;
    if (m_videoMp4Path.isEmpty()) {
        detail = "本次会话没有产生录像文件";
    } else if (!QFile::exists(m_videoTsPath)) {
        detail = "没有产生录像文件: " + m_videoTsPath;
        m_videoMp4Path.clear();
    } else if (m_ffmpegProc.state() == QProcess::NotRunning) {
        startRemux();
        detail = "已保留中断前的录像并转为 MP4: " + m_videoMp4Path;
    } else {
        detail = "上一段转封装仍在进行，完成后即出 MP4";
    }

    const QString msg = "视频源已切换或重连，本会话的视频录制已停止（" + detail + "）";
    qWarning().noquote() << "[DataRecorder]" << msg;
    emit videoRecordInterrupted(msg);
}

void DataRecorder::startRemux()
{
    if (!QFile::exists(m_videoTsPath)) {
        emit errorOccurred("TS 文件不存在，无法转封装: " + m_videoTsPath);
        m_videoMp4Path.clear();
        return;
    }

    QStringList args;
    args << "-y"
         << "-i" << m_videoTsPath
         << "-c" << "copy"
         << "-movflags" << "+faststart"
         << m_videoMp4Path;
    qDebug() << "[DataRecorder] ffmpeg:" << m_ffmpegPath << args.join(' ');
    m_ffmpegProc.start(m_ffmpegPath, args);
}

QString DataRecorder::makeUniquePath(const QString &basePath) const
{
    if (!QFile::exists(basePath))
        return basePath;

    const QFileInfo fi(basePath);
    const QString dir = fi.path();
    const QString base = fi.completeBaseName();
    const QString suffix = fi.suffix();
    for (int i = 1; i < 1000; ++i) {
        const QString candidate = dir + "/" + base + "_" + QString::number(i)
                                  + (suffix.isEmpty() ? QString() : "." + suffix);
        if (!QFile::exists(candidate))
            return candidate;
    }
    return basePath;   // 理论不可达：1..999 都用满时退回原路径
}
