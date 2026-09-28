#include "DataRecorder.h"
#include "../vlcvideo/VlcVideoItem.h"

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

// ── 串口原始帧（图像 / 激光分开保存，互不混写）──
void DataRecorder::onImageFrame(const QByteArray &frame)
{
    if (!m_saving)
        return;
    ensureImageFile();
    // 十六进制 ASCII 文本：原始帧字节多为非 UTF-8 二进制值，
    // 直接落盘会导致麒麟等 Linux 文本查看器报“字符编码错误”。
    // 每帧一行，空格分隔，可还原为原始字节（去掉空格后 hex → bytes）。
    m_imageBuffer.append(frame.toHex(' '));
    m_imageBuffer.append('\n');
}

void DataRecorder::onLaserFrame(const QByteArray &frame)
{
    if (!m_saving)
        return;
    ensureLaserFile();
    m_laserBuffer.append(frame.toHex(' '));
    m_laserBuffer.append('\n');
}

// ── 转台周期状态帧 ──
void DataRecorder::onTurntableFrame(const StatusFeedbackHex &frame)
{
    if (!m_saving)
        return;
    ensureCsvFile();
    if (m_csvOpenFailed)
        return;

    // 一行一帧。角度/控制偏差按工程单位（0.0001°）保留 4 位小数，
    // 序号 + 主机毫秒时间用于事后核对转台的固定周期。
    QString line;
    line.reserve(160);
    line += QString::number(++m_turntableSeq);
    line += ',';
    line += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    line += ',';
    line += QString::number(frame.m_time);
    line += ',';
    line += QString::number(frame.m_ctlnumber);
    line += ','; line += QString::number(frame.m_inner_statusnumber);
    line += ','; line += QString::number(frame.m_inner_angle, 'f', 4);
    line += ','; line += QString::number(frame.m_inner_ctlDeviation, 'f', 4);
    line += ','; line += QString::number(frame.m_middle_statusnumber);
    line += ','; line += QString::number(frame.m_middle_angle, 'f', 4);
    line += ','; line += QString::number(frame.m_middle_ctlDeviation, 'f', 4);
    line += ','; line += QString::number(frame.m_outter_statusnumber);
    line += ','; line += QString::number(frame.m_outter_angle, 'f', 4);
    line += ','; line += QString::number(frame.m_outter_ctlDeviation, 'f', 4);
    line += ','; line += QString::number(frame.m_hasSecPulse);
    line += ','; line += QString::number(frame.m_cmdHint);
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

    m_imagePath = makeUniquePath(m_sessionDir + "/image" + m_sessionTime + ".txt");
    m_imageFile.setFileName(m_imagePath);
    if (!m_imageFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_imageOpenFailed = true;   // 打开失败：停止重试，避免每帧重复报错
        emit errorOccurred("无法创建图像数据文件: " + m_imagePath);
        return;
    }
    qDebug() << "[DataRecorder] 图像数据保存到:" << m_imagePath;
}

void DataRecorder::ensureLaserFile()
{
    if (m_laserFile.isOpen() || m_laserOpenFailed)
        return;

    m_laserPath = makeUniquePath(m_sessionDir + "/laser" + m_sessionTime + ".txt");
    m_laserFile.setFileName(m_laserPath);
    if (!m_laserFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_laserOpenFailed = true;
        emit errorOccurred("无法创建激光数据文件: " + m_laserPath);
        return;
    }
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
    // 表头（与 onTurntableFrame 的列序严格一致；全 ASCII，便于脚本与表格软件读取）
    m_csvFile.write("seq,host_time,ms_time,ctlnumber,"
                    "inner_status,inner_angle,inner_dev,"
                    "middle_status,middle_angle,middle_dev,"
                    "outter_status,outter_angle,outter_dev,"
                    "sec_pulse,cmd_hint\n");
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
