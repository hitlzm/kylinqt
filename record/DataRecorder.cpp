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
    connect(&m_flushTimer, &QTimer::timeout, this, &DataRecorder::flushTxt);

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
    flushTxt();
    if (m_txtFile.isOpen())
        m_txtFile.close();
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
    m_txtPath.clear();
    m_txtOpenFailed = false;
    m_videoTsPath.clear();
    m_videoMp4Path.clear();

    m_saving = true;
    emit savingChanged();

    startVideoRecord();
    m_flushTimer.start();
    qDebug() << "[DataRecorder] 开始保存，目录:" << m_sessionDir
             << " txt:" << m_txtPath << " video:" << m_videoTsPath;
    return true;
}

void DataRecorder::stopSave()
{
    if (!m_saving)
        return;

    m_saving = false;
    emit savingChanged();

    m_flushTimer.stop();
    flushTxt();
    if (m_txtFile.isOpen())
        m_txtFile.close();

    stopVideoRecord();

    // 视频开录过才转封装
    if (!m_videoMp4Path.isEmpty())
        startRemux();
}

// ── 串口原始帧 ──
void DataRecorder::onImageFrame(const QByteArray &frame)
{
    if (!m_saving)
        return;
    ensureTxtFile();
    // 十六进制 ASCII 文本：原始帧字节多为非 UTF-8 二进制值，
    // 直接落盘会导致麒麟等 Linux 文本查看器报“字符编码错误”。
    // 每帧一行，空格分隔，可还原为原始字节（去掉空格后 hex → bytes）。
    m_txtBuffer.append(frame.toHex(' '));
    m_txtBuffer.append('\n');
}

void DataRecorder::onLaserFrame(const QByteArray &frame)
{
    if (!m_saving)
        return;
    ensureTxtFile();
    m_txtBuffer.append(frame.toHex(' '));
    m_txtBuffer.append('\n');
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

void DataRecorder::ensureTxtFile()
{
    if (m_txtFile.isOpen() || m_txtOpenFailed)
        return;

    m_txtPath = makeUniquePath(m_sessionDir + "/txt" + m_sessionTime + ".txt");
    m_txtFile.setFileName(m_txtPath);
    if (!m_txtFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_txtOpenFailed = true;   // 打开失败：停止重试，避免每帧重复报错
        emit errorOccurred("无法创建串口数据文件: " + m_txtPath);
        return;
    }
    qDebug() << "[DataRecorder] 串口数据保存到:" << m_txtPath;
}

void DataRecorder::flushTxt()
{
    if (!m_txtFile.isOpen() || m_txtBuffer.isEmpty())
        return;
    const qint64 written = m_txtFile.write(m_txtBuffer);
    if (written < 0) {
        emit errorOccurred("串口数据写入失败: " + m_txtPath);
        return;   // 保留缓冲，下次 flush 重试
    }
    m_txtBuffer.clear();
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
