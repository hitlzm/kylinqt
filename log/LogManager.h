#pragma once

#include <QObject>
#include <QStringList>
#include <QElapsedTimer>

/**
 * @brief 全局单例日志管理器
 *
 * 为三类外导引源（CCD、图像导引头、激光导引头）提供外引导模式下的追踪信息日志接口。
 * 日志条目仅在 ExGuide 定时器回调中产生，而非每次收到导引头原始数据。
 *
 * 节流策略：统一 200ms 最小间隔。1s 模式（1000ms 周期）自然通过；
 * 5ms 模式（5~15ms 周期）被限制为最多 ~5 条/秒，避免日志洪水。
 *
 * 线程安全：logXXX() 可从任意工作线程调用，通过 QMetaObject::invokeMethod
 * 将写操作排队到主线程执行。各 QElapsedTimer 仅被单一工作线程访问，无竞争。
 */
class LogManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList logEntries READ logEntries NOTIFY logEntriesChanged)

public:
    static LogManager* instance();

    QStringList logEntries() const;

    // ── 三类导引头外引导追踪日志（任意线程可调用，线程安全）──
    void logCCDTracking(double azimuth, double pitch);
    void logImageTracking(double azimuth, double pitch);
    void logLaserTracking(double azimuth, double pitch);

    // ── 通用日志接口 ──
    void addLog(const QString &source, const QString &msg);

public slots:
    void clearLog();

signals:
    void logEntriesChanged();

private:
    // 单例：禁止外部构造 / 拷贝 / 移动
    explicit LogManager(QObject *parent = nullptr);
    Q_DISABLE_COPY(LogManager)

    Q_INVOKABLE void appendEntry(const QString &entry);
    static QString formatAngle(double angle);
    static QString formatTimestamp();

    static LogManager *s_instance;
    QStringList m_logEntries;
    int m_maxEntries = 500;

    // 高频限流：每种导引源独立计时，仅被各自的单一工作线程访问
    QElapsedTimer m_lastCCDLog;
    QElapsedTimer m_lastImageLog;
    QElapsedTimer m_lastLaserLog;
    static constexpr int kThrottleMs = 200;
};
