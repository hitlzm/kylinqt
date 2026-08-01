#include "LogManager.h"
#include <QDateTime>
#include <QMetaObject>

LogManager* LogManager::s_instance = nullptr;

LogManager* LogManager::instance()
{
    // 首次调用在 main.cpp 中，先于所有工作线程启动，无需加锁
    if (!s_instance) {
        s_instance = new LogManager(nullptr);
    }
    return s_instance;
}

LogManager::LogManager(QObject *parent)
    : QObject(parent)
{
    m_lastCCDLog.start();
    m_lastImageLog.start();
    m_lastLaserLog.start();
}

// ── Q_PROPERTY 读取（主线程）──

QStringList LogManager::logEntries() const
{
    return m_logEntries;
}

// ── 内部：主线程执行的追加操作 ──

void LogManager::appendEntry(const QString &entry)
{
    m_logEntries.append(entry);

    // 超过上限时从头部裁剪
    while (m_logEntries.size() > m_maxEntries) {
        m_logEntries.removeFirst();
    }

    emit logEntriesChanged();
}

void LogManager::clearLog()
{
    m_logEntries.clear();
    emit logEntriesChanged();
}

// ── 三类导引头追踪日志（任意线程可调用）──

void LogManager::logCCDTracking(double azimuth, double pitch)
{
    if (m_lastCCDLog.elapsed() < kThrottleMs)
        return;
    m_lastCCDLog.restart();

    QString entry = QString("[%1] [CCD] 方位角=%2°  俯仰角=%3°")
                        .arg(formatTimestamp())
                        .arg(formatAngle(azimuth))
                        .arg(formatAngle(pitch));

    QMetaObject::invokeMethod(this, "appendEntry", Qt::QueuedConnection,
                              Q_ARG(QString, entry));
}

void LogManager::logImageTracking(double azimuth, double pitch)
{
    if (m_lastImageLog.elapsed() < kThrottleMs)
        return;
    m_lastImageLog.restart();

    QString entry = QString("[%1] [图像] 方位角=%2°  俯仰角=%3°")
                        .arg(formatTimestamp())
                        .arg(formatAngle(azimuth))
                        .arg(formatAngle(pitch));

    QMetaObject::invokeMethod(this, "appendEntry", Qt::QueuedConnection,
                              Q_ARG(QString, entry));
}

void LogManager::logLaserTracking(double azimuth, double pitch)
{
    if (m_lastLaserLog.elapsed() < kThrottleMs)
        return;
    m_lastLaserLog.restart();

    QString entry = QString("[%1] [激光] 方位角=%2°  俯仰角=%3°")
                        .arg(formatTimestamp())
                        .arg(formatAngle(azimuth))
                        .arg(formatAngle(pitch));

    QMetaObject::invokeMethod(this, "appendEntry", Qt::QueuedConnection,
                              Q_ARG(QString, entry));
}

// ── 通用日志接口 ──

void LogManager::addLog(const QString &source, const QString &msg)
{
    QString entry;
    if (source.isEmpty()) {
        entry = QString("[%1] %2").arg(formatTimestamp(), msg);
    } else {
        entry = QString("[%1] [%2] %3").arg(formatTimestamp(), source, msg);
    }

    QMetaObject::invokeMethod(this, "appendEntry", Qt::QueuedConnection,
                              Q_ARG(QString, entry));
}

// ── 格式化辅助 ──

QString LogManager::formatTimestamp()
{
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}

QString LogManager::formatAngle(double angle)
{
    if (angle >= 0.0) {
        return QString("+%1").arg(angle, 0, 'f', 2);
    } else {
        return QString("%1").arg(angle, 0, 'f', 2);  // 负号自带
    }
}
