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
    // 析构时兜底：停止录制并收尾文件（正常流程应走 stopAll）
    m_flushTimer.stop();
    flushAll();
    if (m_image.file.isOpen())
        m_image.file.close();
    if (m_laser.file.isOpen())
        m_laser.file.close();
    if (m_turntable.file.isOpen())
        m_turntable.file.close();
    if (m_videoItem && m_videoItem->recording())
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

// ── 开始/停止保存（三路各自独立，视频按请求计数共用一份）──

// 单路开始：建目录、复位该路状态、置位标记、登记一次视频请求
bool DataRecorder::startSource(SourceState &st, const char *name)
{
    if (st.saving) {
        emit errorOccurred(QString("%1数据已经在保存中，请先停止再开始").arg(QString::fromUtf8(name)));
        return false;
    }
    // 没有别的录制在用视频时，才需要新开一份录像；此时若上一段还在转封装就等一等
    if (m_videoRefCount == 0 && m_ffmpegProc.state() != QProcess::NotRunning) {
        emit errorOccurred("上一段视频还在转封装，请稍后再试");
        return false;
    }

    const QDateTime now = QDateTime::currentDateTime();
    const QString dir = m_saveDir + "/" + now.toString("M.d");   // 例如 /data/savedata/9.28
    if (!QDir().mkpath(dir)) {
        emit errorOccurred("无法创建保存目录: " + dir);
        return false;
    }

    // 复位该路状态（文件懒创建：第一帧到达时才真正建文件）
    st.dir = dir;
    st.time = now.toString("HHmmss");    // 例如 143025
    st.path.clear();
    st.openFailed = false;
    st.buffer.clear();
    if (st.file.isOpen())                // 上一段若异常未关闭，兜底关闭避免继续写旧文件
        st.file.close();
    st.saving = true;

    acquireVideoRecording(dir, st.time);
    updateFlushTimer();
    return true;
}

// 单路停止：冲刷并关闭该路文件，并释放一次视频请求
bool DataRecorder::stopSource(SourceState &st, const char *name)
{
    if (!st.saving)
        return false;

    st.saving = false;
    flushSource(st, name);
    if (st.file.isOpen())
        st.file.close();

    releaseVideoRecording();   // 该路对应的那次视频请求结束
    updateFlushTimer();
    return true;
}

// 任一路在保存就保持落盘定时器运行；全部停止时收尾
void DataRecorder::updateFlushTimer()
{
    if (anySaving()) {
        if (!m_flushTimer.isActive())
            m_flushTimer.start();
    } else {
        m_flushTimer.stop();
        flushAll();
    }
}

bool DataRecorder::startImageSave()
{
    const bool wasSaving = anySaving();
    if (!startSource(m_image, "图像"))
        return false;
    emit savingImageChanged();
    if (!wasSaving)
        emit savingChanged();
    return true;
}

void DataRecorder::stopImageSave()
{
    if (!stopSource(m_image, "图像"))
        return;
    emit savingImageChanged();
    if (!anySaving())
        emit savingChanged();
}

bool DataRecorder::startLaserSave()
{
    const bool wasSaving = anySaving();
    if (!startSource(m_laser, "激光"))
        return false;
    emit savingLaserChanged();
    if (!wasSaving)
        emit savingChanged();
    return true;
}

void DataRecorder::stopLaserSave()
{
    if (!stopSource(m_laser, "激光"))
        return;
    emit savingLaserChanged();
    if (!anySaving())
        emit savingChanged();
}

bool DataRecorder::startTurntableSave()
{
    const bool wasSaving = anySaving();
    if (!startSource(m_turntable, "转台"))
        return false;
    m_turntableSeq = 0;                 // 序号从 1 重新开始
    emit savingTurntableChanged();
    if (!wasSaving)
        emit savingChanged();
    return true;
}

void DataRecorder::stopTurntableSave()
{
    if (!stopSource(m_turntable, "转台"))
        return;
    emit savingTurntableChanged();
    if (!anySaving())
        emit savingChanged();
}

void DataRecorder::stopAll()
{
    stopTurntableSave();
    stopLaserSave();
    stopImageSave();
    // 兜底：万一计数和实际状态不一致，也要把视频收尾掉
    if (m_videoRefCount > 0) {
        m_videoRefCount = 0;
        releaseVideoRecording();
    }
}

// ── 视频：多个保存请求共用一份录像 ──
void DataRecorder::acquireVideoRecording(const QString &dir, const QString &time)
{
    ++m_videoRefCount;
    if (m_videoRefCount == 1) {
        // 第一个请求：用它的时间开一份新录像
        m_videoRecordLost = false;
        startVideoRecord(dir, time);
    } else {
        qDebug() << "[DataRecorder] 已有录像在用，本次请求共用同一份:" << m_videoMp4Path;
    }
}

void DataRecorder::releaseVideoRecording()
{
    if (m_videoRefCount > 0)
        --m_videoRefCount;
    if (m_videoRefCount > 0)
        return;   // 还有别的保存在用这份录像

    // 计数归零：真正停止录制并收尾（此时计数已为 0，状态回调不会误判成“被换源打断”）
    stopVideoRecord();

    if (m_videoMp4Path.isEmpty())
        return;   // 本次没有开过录制
    if (!QFile::exists(m_videoTsPath)) {
        // 没接视频/没在播时不会产生 .ts，这属于正常情况：跳过转封装，不报错
        qDebug() << "[DataRecorder] 未产生录像文件，跳过转封装:" << m_videoTsPath;
        m_videoMp4Path.clear();
        return;
    }
    // 转封装进行中说明中断那次已经在转，等它自己完成
    if (m_ffmpegProc.state() == QProcess::NotRunning)
        startRemux();
}

// ── B帧枚举字段 → 中文（不写码值；只有映射表里没有的取值才回退成 未知(0xNN)，避免丢信息）──
static QString imageUnknownText(int code)
{
    const QString hex = QString::number(static_cast<uint>(code) & 0xFFu, 16)
                            .rightJustified(2, QLatin1Char('0')).toUpper();
    return QStringLiteral("未知(0x%1)").arg(hex);
}

// 字节11: 当前工作通道
static QString imageWorkChannelText(int v)
{
    switch (v) {
    case 0x00: return QStringLiteral("电视");
    case 0x01: return QStringLiteral("红外");
    default:   return imageUnknownText(v);
    }
}

// 字节18: 光学工作状态
static QString imageOpticalWorkStateText(int v)
{
    switch (v) {
    case 0x02: return QStringLiteral("搜索状态（预置状态）");
    case 0x03: return QStringLiteral("跟踪状态");
    case 0x04: return QStringLiteral("框架角电锁零位状态");
    case 0x05: return QStringLiteral("记忆状态");
    case 0x06: return QStringLiteral("解锁状态");
    default:   return imageUnknownText(v);
    }
}

// 字节32: 跟踪状态
static QString imageTrackingStateText(int v)
{
    switch (v) {
    case 0x00: return QStringLiteral("默认");
    case 0x11: return QStringLiteral("搜索中");
    case 0x22: return QStringLiteral("目标丢失");
    case 0x33: return QStringLiteral("目标锁定");
    case 0x44: return QStringLiteral("记忆状态");
    default:   return imageUnknownText(v);
    }
}

// 字节33: 跟踪器状态
static QString imageTrackerStateText(int v)
{
    switch (v) {
    case 0x00: return QStringLiteral("空闲状态");
    case 0x01: return QStringLiteral("跟踪状态");
    case 0x02: return QStringLiteral("识别状态");
    case 0x03: return QStringLiteral("匹配状态");
    case 0x04: return QStringLiteral("仅识别状态");
    default:   return imageUnknownText(v);
    }
}

// ── 图像 B 帧（解析后的工程量，一行一帧）──
void DataRecorder::onImageFrame()
{
    if (!m_image.saving || !m_imageData)
        return;
    // 懒创建：该路第一帧到达时才建文件并写表头（列序必须与下面写行严格一致）
    static const QByteArray kImageHeader = QByteArrayLiteral(
            "\xEF\xBB\xBF"
            "时间,B帧流水号,当前工作通道,俯仰视线角速度(°/s),偏航视线角速度(°/s),"
            "光学工作状态,俯仰框架角(°),偏航框架角(°),"
            "俯仰陀螺(°/s),偏航陀螺(°/s),跟踪状态,跟踪器状态,"
            "红外帧编号,电视帧编号\n");
    ensureSourceFile(m_image, QStringLiteral("image"), kImageHeader, "图像");
    if (m_image.openFailed)
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

    m_image.buffer.append(line.toUtf8());
}

// 字节24: 增益状态（协议 0xB0~0xB4 为 5 级起控状态 → 中文）
static QString laserGainStatusText(int v)
{
    switch (v) {
    case 0xB0: return QStringLiteral("起控1级");
    case 0xB1: return QStringLiteral("起控2级");
    case 0xB2: return QStringLiteral("起控3级");
    case 0xB3: return QStringLiteral("起控4级");
    case 0xB4: return QStringLiteral("起控5级");
    default:   return imageUnknownText(v);   // 与图像同一套回退：未知(0xNN)
    }
}

// ── 激光接收帧（解析后的工程量，一行一帧）──
void DataRecorder::onLaserFrame()
{
    if (!m_laser.saving || !m_laserData)
        return;
    // 懒创建：该路第一帧到达时才建文件并写表头（列序必须与下面写行严格一致）
    static const QByteArray kLaserHeader = QByteArrayLiteral(
            "\xEF\xBB\xBF"
            "时间,光轴方位角(°),光轴俯仰角(°),"
            "方位陀螺输出角速度(°/s),俯仰陀螺输出角速度(°/s),"
            "方位视线角速度(°/s),俯仰视线角速度(°/s),"
            "方位偏差角(°),俯仰偏差角(°),激光周期(ms),增益状态,"
            "第一象限能量强度,第二象限能量强度,第三象限能量强度,第四象限能量强度\n");
    ensureSourceFile(m_laser, QStringLiteral("laser"), kLaserHeader, "激光");
    if (m_laser.openFailed)
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

    m_laser.buffer.append(line.toUtf8());
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
    if (!m_turntable.saving)
        return;
    // 懒创建：该路第一帧到达时才建文件并写表头（列序必须与下面写行严格一致）
    static const QByteArray kTurntableHeader = QByteArrayLiteral(
            "\xEF\xBB\xBF"
            "序号,转台毫秒时间,指令提示,"
            "内框状态,内框角度(°),内框控制偏差(°),"
            "中框状态,中框角度(°),中框控制偏差(°),"
            "外框状态,外框角度(°),外框控制偏差(°),"
            "秒脉冲\n");
    ensureSourceFile(m_turntable, QStringLiteral("turntable"), kTurntableHeader, "转台");
    if (m_turntable.openFailed)
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

    m_turntable.buffer.append(line.toUtf8());
}

// ── 内部 ──
// 某一路的数据文件懒创建：第一帧到达时才建文件并写表头（header 需与写行顺序一致）
void DataRecorder::ensureSourceFile(SourceState &st, const QString &prefix,
                                    const QByteArray &header, const char *name)
{
    if (st.file.isOpen() || st.openFailed)
        return;

    st.path = makeUniquePath(st.dir + "/" + prefix + st.time + ".csv");
    st.file.setFileName(st.path);
    if (!st.file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        st.openFailed = true;   // 打开失败：停止重试，避免每帧重复报错
        emit errorOccurred(QString("无法创建%1数据文件: %2").arg(QString::fromUtf8(name), st.path));
        return;
    }
    st.file.write(header);
    qDebug() << "[DataRecorder]" << name << "数据保存到:" << st.path;
}

// 某一路缓冲落盘
void DataRecorder::flushSource(SourceState &st, const char *name)
{
    if (!st.file.isOpen() || st.buffer.isEmpty())
        return;
    const qint64 written = st.file.write(st.buffer);
    if (written < 0) {
        emit errorOccurred(QString("%1数据写入失败: %2").arg(QString::fromUtf8(name), st.path));
        return;   // 保留缓冲，下次 flush 重试
    }
    st.buffer.clear();
}

void DataRecorder::flushAll()
{
    flushSource(m_image, "图像");
    flushSource(m_laser, "激光");
    flushSource(m_turntable, "转台");
}

void DataRecorder::startVideoRecord(const QString &dir, const QString &time)
{
    if (!m_videoItem) {
        // 没有视频播放器就只保存数据，不产生视频文件（不弹错误）
        qWarning() << "[DataRecorder] 未找到视频播放器，本次只保存数据、不保存视频";
        return;
    }
    if (!m_videoItem->isPlaying())
        qWarning() << "[DataRecorder] 当前视频未在播放，录像可能为空（有画面后再录才会有内容）";
    // ts 与 mp4 成对判重，避免 -y 覆盖同秒内上一份录像
    QString stem = "video" + time;
    QString tsBase = dir + "/" + stem + ".ts";
    QString mp4Path = dir + "/" + stem + ".mp4";
    int n = 1;
    while (QFile::exists(tsBase) || QFile::exists(mp4Path)) {
        stem = "video" + time + "_" + QString::number(n++);
        tsBase = dir + "/" + stem + ".ts";
        mp4Path = dir + "/" + stem + ".mp4";
    }
    m_videoTsPath = tsBase;
    m_videoMp4Path = mp4Path;
    m_videoItem->startRecord(m_videoTsPath);
    qDebug() << "[DataRecorder] 开始录像:" << m_videoTsPath;
}

void DataRecorder::stopVideoRecord()
{
    if (m_videoItem)
        m_videoItem->stopRecord();
}

// 视频录制状态变化：录制请求还存在（计数>0）时由 true 变 false，说明被换源/重连打断了
//（VlcVideoItem::releasePlayer() → stopRecord()；正常收尾时计数已先减到 0，不会走到这里）
void DataRecorder::handleVideoRecordingChanged()
{
    if (m_videoRefCount <= 0 || !m_videoItem)
        return;
    if (m_videoItem->recording() || m_videoRecordLost)
        return;   // 只处理 true -> false，且每次会话只处理一次

    m_videoRecordLost = true;

    // 视频录制就地停止，本会话不再续录：把已录到的部分立刻转封装，
    // 这样界面上立刻能拿到可用的 mp4，而不是等到最后一路停止时才发现内容缺失。
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
