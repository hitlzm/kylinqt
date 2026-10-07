#ifndef VIDEOLATENCYPROBE_H
#define VIDEOLATENCYPROBE_H

#include <QByteArray>
#include <QElapsedTimer>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QVector>

class QTimer;

// ══════════════════════════════════════════════════════════════════════
// 编译期默认值 —— 想在代码里"改宏"就改这里，不用每次敲环境变量。
// 也可以在 CMake 里用 target_compile_definitions 覆盖（见 CMakeLists.txt）。
//
//   KYLIN_LATPROBE_DEFAULT_ON   1 = 程序一启动就打开探针；0 = 默认关闭
//   KYLIN_LATPROBE_DEFAULT_CSV  默认 CSV 路径，必须是带引号的字符串，
//                               空串 "" 表示不写文件
//
// 优先级：环境变量 > 编译期宏 > 默认关闭
//   KYLIN_LATPROBE=1 / =0   显式打开 / 关闭（覆盖宏）
//   KYLIN_LATPROBE_CSV=<路径>
//   KYLIN_LATPROBE_IFACE=<网卡名>
//   KYLIN_LATPROBE_SHIFT=<帧数>
//   运行中还可以用 videoPlayer.setLatencyProbeEnabled(true) 随时开关
// ══════════════════════════════════════════════════════════════════════
#ifndef KYLIN_LATPROBE_DEFAULT_ON
#define KYLIN_LATPROBE_DEFAULT_ON 0
#endif

#ifndef KYLIN_LATPROBE_DEFAULT_CSV
#define KYLIN_LATPROBE_DEFAULT_CSV ""
#endif

/**
 * @brief 视频接收侧延迟探针（仅导引头直显模式）
 *
 * ── 测量口径 ─────────────────────────────────────────────────────────
 *   本机 UDP socket 收到某帧首个 TS 包（T1）  →  该帧在屏幕上换页显示（T2）
 *   延迟 = T2 - T1
 *
 *   两个打点都取本机单调时钟，所以**不需要**与源端对时；PTS 只当"帧的身份
 *   标识"用来把同一个帧在两侧对上，不参与任何时间差计算。
 *
 * ── 两个打点从哪来 ───────────────────────────────────────────────────
 *   1) 抓包线程 Capture：另开一路 UDP socket 加入同一组播组（组播允许多个
 *      接收者，不影响 mpv 正常收流），解析 MPEG-TS 视频 PES 头部的 PTS，
 *      记录每帧首个 TS 包的到达时刻。
 *   2) 显示打点：VlcVideoItem 在渲染出新的 mpv 帧之后调用
 *      onFrameDisplayed()，传入 mpv 的 video-pts（当前显示帧）。
 *
 * ── 开关（默认关闭；关闭时不抓包、不查询 mpv、不产生任何开销）─────────
 *   KYLIN_LATPROBE=1              打开探针
 *   KYLIN_LATPROBE_CSV=<路径>     把每个样本写成 CSV（便于出报告）
 *   KYLIN_LATPROBE_SHIFT=<帧数>   修正 PTS 基准的整帧偏差，默认 0
 *   KYLIN_LATPROBE_IFACE=<网卡名> 指定加入组播的网卡，默认系统默认路由
 *   也可以直接调用 setEnabled(true) 程序内打开。
 *
 * ── 限制 ─────────────────────────────────────────────────────────────
 *   - 只支持 UDP 组播地址（udp://@226.0.0.80:8001 这种）。RTSP/TCP 不支持，
 *     会打一条告警并保持静默。
 *   - 软件口径的终点是"渲染完成、即将换页"，比肉眼可见延迟少 0~2 帧
 *     （60Hz 下每帧 16.7ms）。出报告时要么注明口径，要么补上这个常数。
 *   - 仅测量导引头直显路径（cpuFrameConsumer=false）。CCD 模式显示的帧
 *     经过回读与检测处理，本探针不做处理。
 */
class VideoLatencyProbe : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    struct Stats {
        qint64  samples   = 0;      // 有效样本数
        qint64  misses    = 0;      // 显示帧在到达表里查不到（丢包/换流等）
        double  avgMs     = 0.0;
        double  p50Ms     = 0.0;
        double  p95Ms     = 0.0;
        double  p99Ms     = 0.0;
        double  minMs     = 0.0;
        double  maxMs     = 0.0;
        bool    anchorReady = false;
        quint64 anchorPts   = 0;    // 原始 PTS 基准（90kHz）
        double  ptsStepMs   = 0.0;  // 实测帧间隔，用于自检
    };

    explicit VideoLatencyProbe(QObject *parent = nullptr);
    ~VideoLatencyProbe() override;

    // ── 环境变量开关 ──
    static bool    envEnabled();
    static QString envCsvPath();
    static int     envFrameShift();

    // ── 开关 ──
    bool isEnabled() const;
    void setEnabled(bool on);

    // ── 码流 ──
    /// 传入播放地址；仅 udp:// 组播地址会被接受并开始抓包
    bool setStreamUrl(const QString &url);

    /// mpv 即将 loadfile 时调用：把之后收到的第一帧作为 PTS 基准
    void notePlaybackStart();

    // ── 显示侧打点（渲染线程调用）──
    void onFrameDisplayed(double mpvPtsSeconds);

    // ── 统计 ──
    Stats stats() const;
    void  resetStats();

signals:
    void enabledChanged();
    void statsUpdated();

private:
    class Capture;   // 抓包线程，定义在 .cpp

    void onCaptureArrival(quint64 pts90k, qint64 tArriveMs);
    void startCapture(const QString &host, quint16 port);
    void stopCapture();

    void writeCsvRow(quint64 pts, qint64 tArriveMs,
                     qint64 tDisplayMs, double latencyMs);
    void flushCsv();
    void onStatTimer();

    bool    m_enabled    = false;
    QString m_url;
    QString m_csvPath;
    int     m_frameShift = 0;

    Capture *m_capture = nullptr;

    mutable QMutex         m_mutex;
    QHash<quint64, qint64> m_arrive;    // PTS(90kHz) → 到达时刻(ms)
    QVector<quint64>       m_order;     // 到达顺序，用于淘汰旧条目
    QVector<double>        m_samples;   // 延迟样本(ms)

    quint64 m_anchorPts    = 0;         // PTS 基准：mpv 归一化后的 0 点
    bool    m_anchorReady  = false;
    bool    m_anchorPending = false;    // 等待下一帧作为基准
    bool    m_modeConfirmed = false;    // 是否已确定 mpv 是否归一化 PTS
    bool    m_useRawPts     = false;    // true = 直接用原始 PTS 匹配
    qint64  m_missStreak    = 0;        // 连续查不到，用于给出诊断提示

    qint64  m_misses       = 0;
    double  m_stepSum      = 0.0;
    qint64  m_stepCount    = 0;
    quint64 m_lastArrivalPts = 0;

    QTimer  *m_statTimer   = nullptr;
    qint64   m_lastLogMs   = 0;
    QByteArray m_csvBuf;
    bool     m_csvHeaderWritten = false;
};

#endif // VIDEOLATENCYPROBE_H
