#ifndef CCDLATENCYPROBE_H
#define CCDLATENCYPROBE_H

#include <QByteArray>
#include <QObject>
#include <QString>
#include <QVector>

#include <atomic>

class QMutex;
class QTimer;

/**
 * @brief CCD 直显进程内延迟探针
 *
 * 测量口径：
 *   mpv 输出一帧可渲染的新视频帧（T1）
 *       -> Qt 完成该帧的 FBO 绘制（T2）
 *
 * 该探针只用于 CCD 且视觉模型推理关闭时的 GPU 直显路径。CCD 默认使用
 * RTSP，现有 VideoLatencyProbe 的 MPEG-TS 旁路抓包无法建立 PTS 到达表，
 * 因此这里采用进程内双点计时。它不包含网络接收、解复用和解码时间，
 * 结果不能与导引头“本机收到 TS 包 -> 显示”的接收侧延迟直接比较。
 *
 * 开关：
 *   KYLIN_LATPROBE=1          随视频探针一起启用
 *   KYLIN_CCDLAT=1            单独启用（也可显式设为 0 覆盖）
 *   KYLIN_CCDLAT_CSV=<路径>   将逐帧样本写入独立 CSV
 */
class CcdLatencyProbe : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    struct Stats {
        qint64 samples = 0;
        qint64 misses = 0;
        double avgMs = 0.0;
        double p50Ms = 0.0;
        double p95Ms = 0.0;
        double p99Ms = 0.0;
        double minMs = 0.0;
        double maxMs = 0.0;
    };

    explicit CcdLatencyProbe(QObject *parent = nullptr);
    ~CcdLatencyProbe() override;

    static bool envEnabled();
    static QString envCsvPath();

    bool isEnabled() const;
    void setEnabled(bool on);

    /// 开始加载新流时丢弃上一路尚未完成的帧内计时。
    void notePlaybackStart();

    /// 渲染线程：mpv 已输出一帧可渲染的新视频帧。
    void onFrameDecoded();

    /// 渲染线程：本帧的 Qt 绘制已完成。
    void onFrameRendered(bool newFrameRendered);

    Stats stats() const;
    void resetStats();

signals:
    void enabledChanged();
    void statsUpdated();

private slots:
    void onStatTimer();

private:
    void writeCsvRow(qint64 tDecodedNs, qint64 tRenderNs, double latencyMs);
    void flushCsv();

    std::atomic<bool> m_enabled{false};
    QString m_csvPath;

    mutable QMutex *m_mutex = nullptr;
    qint64 m_decodedNs = -1;
    qint64 m_samples = 0;
    qint64 m_misses = 0;
    QVector<double> m_samplesMs;

    QTimer *m_statTimer = nullptr;
    qint64 m_lastLogNs = 0;
    QByteArray m_csvBuf;
    bool m_csvHeaderWritten = false;
};

#endif // CCDLATENCYPROBE_H
