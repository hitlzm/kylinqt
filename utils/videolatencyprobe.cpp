#include "videolatencyprobe.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QThread>
#include <QTimer>
#include <QUdpSocket>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdlib>
#include <utility>

// ══════════════════════════════════════════════════════════════════════════
// 常量与工具
// ══════════════════════════════════════════════════════════════════════════

namespace {

constexpr quint64 kPtsMask = 0x1FFFFFFFFull;      // MPEG-TS PTS 为 33 bit
constexpr int     kTsSize  = 188;                 // 标准 TS 包长
constexpr int     kOrderMax = 4096;               // 到达表保留的最大帧数（约 2 分钟）
constexpr int     kSampleMax = 2000000;           // 样本上限，防止长跑吃内存
constexpr double  kLatencyLoMs = 0.0;             // 合理区间下界
constexpr double  kLatencyHiMs = 5000.0;          // 合理区间上界（超过视为异常样本）

/// 本机单调时钟（毫秒）。不要用 QDateTime：它会被 NTP 调整。
/// 注意 Qt 5.12 的 QElapsedTimer::msecsSinceReference() 还是成员函数，
/// 所以这里用一个只启动一次的静态实例，elapsed() 本身不修改状态、可跨线程读。
inline qint64 monoMs()
{
    static QElapsedTimer timer = [] {
        QElapsedTimer t;
        t.start();
        return t;
    }();
    return timer.elapsed();
}

/// 从 PES 头部解析 PTS（90kHz，33 bit）
bool parsePesPts(const uchar *pes, int len, quint64 &pts)
{
    if (len < 14)
        return false;
    if (!(pes[0] == 0x00 && pes[1] == 0x00 && pes[2] == 0x01))
        return false;
    const int streamId = pes[3];
    if (streamId < 0xE0 || streamId > 0xEF)      // 视频 ES
        return false;
    if ((pes[6] & 0xC0) != 0x80)                 // '10' 前缀
        return false;
    if (!((pes[7] >> 6) & 0x02))                 // PTS_DTS_flags：有 PTS
        return false;

    pts = (static_cast<quint64>((pes[9]  >> 1) & 0x07) << 30)
        | (static_cast<quint64>(pes[10]) << 22)
        | (static_cast<quint64>((pes[11] >> 1) & 0x7F) << 15)
        | (static_cast<quint64>(pes[12]) << 7)
        | (static_cast<quint64>((pes[13] >> 1) & 0x7F));
    return true;
}

/// 识别 TS 包长。返回步长（188/192/204），start 输出首个同步字节的偏移。
int detectTsPacketSize(const uchar *p, int len, int &start)
{
    start = 0;
    if (len < kTsSize)
        return 0;

    for (int lead = 0; lead < 4 && lead < len; ++lead) {
        if (p[lead] != 0x47)
            continue;
        const int candidates[3] = {188, 192, 204};
        for (int stride : candidates) {
            const int next = lead + stride;
            if (next + 1 >= len) {
                if (lead == 0 && stride == 188) {   // 只收到单个包
                    start = 0;
                    return 188;
                }
                continue;
            }
            if (p[next] == 0x47) {
                start = lead;
                return stride;
            }
        }
        if (lead == 0) {                            // 兜底
            start = 0;
            return 188;
        }
    }
    return 0;
}

/// 解析 udp://@226.0.0.80:8001 形式的地址
bool parseUdpUrl(const QString &url, QString &host, quint16 &port)
{
    QString s = url.trimmed();
    if (!s.startsWith(QStringLiteral("udp://"), Qt::CaseInsensitive))
        return false;
    s = s.mid(6);
    if (s.startsWith(QLatin1Char('@')))
        s = s.mid(1);
    const int q = s.indexOf(QLatin1Char('?'));
    if (q >= 0)
        s = s.left(q);
    const int slash = s.indexOf(QLatin1Char('/'));
    if (slash >= 0)
        s = s.left(slash);
    const int colon = s.lastIndexOf(QLatin1Char(':'));
    if (colon <= 0)
        return false;
    bool ok = false;
    const uint p = s.mid(colon + 1).toUInt(&ok);
    if (!ok || p == 0 || p > 65535)
        return false;
    host = s.left(colon);
    port = static_cast<quint16>(p);
    return !host.isEmpty();
}

/// 计算分位数（sorted 必须已升序）
double percentile(const QVector<double> &sorted, double q)
{
    if (sorted.isEmpty())
        return 0.0;
    const double pos = q * (sorted.size() - 1);
    const int lo = static_cast<int>(std::floor(pos));
    const int hi = static_cast<int>(std::ceil(pos));
    if (lo == hi)
        return sorted[lo];
    const double frac = pos - lo;
    return sorted[lo] * (1.0 - frac) + sorted[hi] * frac;
}

} // namespace

// ══════════════════════════════════════════════════════════════════════════
// Capture —— 抓包线程
//
// 独立线程 + 独立 socket，只做"收包 → 解析 PTS → 上报到达时刻"，
// 不触碰播放/解码/渲染/检测任何现有链路。
// ══════════════════════════════════════════════════════════════════════════

class VideoLatencyProbe::Capture : public QThread
{
public:
    Capture(VideoLatencyProbe *probe, QString host, quint16 port, QString ifaceName)
        : m_probe(probe)
        , m_host(std::move(host))
        , m_port(port)
        , m_ifaceName(std::move(ifaceName))
    {
        setObjectName(QStringLiteral("LatProbeCapture"));
    }

    void stop() { m_stop.store(true, std::memory_order_relaxed); }
    bool failed() const { return m_failed.load(std::memory_order_relaxed); }

protected:
    void run() override
    {
        QUdpSocket socket;
        if (!socket.bind(QHostAddress::AnyIPv4, m_port,
                         QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
            // 少数平台要求直接绑到组播地址上
            if (!socket.bind(QHostAddress(m_host), m_port,
                             QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
                qWarning() << "[LatProbe] 绑定 UDP 端口失败:" << m_port
                           << socket.errorString()
                           << "（若与播放器争用端口，可把探针放到独立进程里跑）";
                m_failed.store(true, std::memory_order_relaxed);
                return;
            }
        }

        const QHostAddress group(m_host);
        bool joined = false;
        if (!m_ifaceName.isEmpty()) {
            const QNetworkInterface iface =
                    QNetworkInterface::interfaceFromName(m_ifaceName);
            if (!iface.isValid()) {
                qWarning() << "[LatProbe] 找不到网卡" << m_ifaceName
                           << "，改用默认接口加入组播";
                joined = socket.joinMulticastGroup(group);
            } else {
                joined = socket.joinMulticastGroup(group, iface);
            }
        } else {
            joined = socket.joinMulticastGroup(group);
        }
        if (!joined) {
            qWarning() << "[LatProbe] 加入组播组失败:" << m_host
                       << socket.errorString();
            m_failed.store(true, std::memory_order_relaxed);
            return;
        }

        qDebug() << "[LatProbe] 抓包线程就绪, 组播" << m_host << ":" << m_port
                 << "网卡" << (m_ifaceName.isEmpty() ? QStringLiteral("<默认>") : m_ifaceName);

        while (!m_stop.load(std::memory_order_relaxed)) {
            if (socket.waitForReadyRead(100))
                drain(socket);
        }

        socket.leaveMulticastGroup(group);
        socket.close();
        qDebug() << "[LatProbe] 抓包线程退出";
    }

private:
    void drain(QUdpSocket &socket)
    {
        while (socket.hasPendingDatagrams()) {
            const qint64 pending = socket.pendingDatagramSize();
            if (pending <= 0)
                break;
            QByteArray datagram;
            datagram.resize(static_cast<int>(pending));
            if (socket.readDatagram(datagram.data(), datagram.size()) < 0)
                break;
            // 取包后立刻打时间戳，减少排队误差
            const qint64 tArrive = monoMs();
            feed(reinterpret_cast<const uchar *>(datagram.constData()),
                 datagram.size(), tArrive);
        }
    }

    void feed(const uchar *data, int len, qint64 tArriveMs)
    {
        int start = 0;
        const int stride = detectTsPacketSize(data, len, start);
        if (stride <= 0)
            return;

        for (int off = start; off + kTsSize <= len; off += stride) {
            const uchar *p = data + off;
            if (p[0] != 0x47)
                continue;

            const bool pusi = (p[1] & 0x40) != 0;          // 新 PES 起始
            const int  afc  = (p[3] >> 4) & 0x03;          // adaptation control
            if (!pusi || afc == 0 || afc == 2)             // 只取带负载的帧首包
                continue;

            int hdrLen = 4;
            if (afc == 3)
                hdrLen += 1 + p[4];                        // 跳过 adaptation field
            if (hdrLen + 14 > kTsSize)
                continue;

            quint64 pts = 0;
            if (!parsePesPts(p + hdrLen, kTsSize - hdrLen, pts))
                continue;

            m_probe->onCaptureArrival(pts, tArriveMs);
        }
    }

    VideoLatencyProbe *m_probe = nullptr;
    QString            m_host;
    quint16            m_port = 0;
    QString            m_ifaceName;
    std::atomic<bool>  m_stop{false};
    std::atomic<bool>  m_failed{false};
};

// ══════════════════════════════════════════════════════════════════════════
// VideoLatencyProbe
// ══════════════════════════════════════════════════════════════════════════

VideoLatencyProbe::VideoLatencyProbe(QObject *parent)
    : QObject(parent)
    , m_enabled(envEnabled())
    , m_csvPath(envCsvPath())
    , m_frameShift(envFrameShift())
{
    m_statTimer = new QTimer(this);
    m_statTimer->setInterval(1000);
    connect(m_statTimer, &QTimer::timeout, this, &VideoLatencyProbe::onStatTimer);

    if (m_enabled) {
        m_lastLogMs = monoMs();
        m_statTimer->start();
        qDebug() << "[LatProbe] 已启用  开关来源: 环境变量 KYLIN_LATPROBE 或编译期宏 "
                    "KYLIN_LATPROBE_DEFAULT_ON"
                 << " CSV:" << (m_csvPath.isEmpty() ? QStringLiteral("<不写文件>") : m_csvPath)
                 << " PTS 整帧修正:" << m_frameShift;
    }
}

VideoLatencyProbe::~VideoLatencyProbe()
{
    if (m_statTimer)
        m_statTimer->stop();
    stopCapture();
    flushCsv();
}

bool VideoLatencyProbe::envEnabled()
{
    // 环境变量显式设置时以它为准（便于临时开关）；
    // 没设置时用编译期宏 KYLIN_LATPROBE_DEFAULT_ON。
    if (qEnvironmentVariableIsSet("KYLIN_LATPROBE")) {
        const QByteArray v = qgetenv("KYLIN_LATPROBE");
        return !v.isEmpty() && v != "0" && v.toLower() != "false";
    }
    return KYLIN_LATPROBE_DEFAULT_ON != 0;
}

QString VideoLatencyProbe::envCsvPath()
{
    if (qEnvironmentVariableIsSet("KYLIN_LATPROBE_CSV"))
        return QString::fromLocal8Bit(qgetenv("KYLIN_LATPROBE_CSV"));
    return QString::fromUtf8(KYLIN_LATPROBE_DEFAULT_CSV);
}

int VideoLatencyProbe::envFrameShift()
{
    bool ok = false;
    const int v = QString::fromLocal8Bit(qgetenv("KYLIN_LATPROBE_SHIFT")).toInt(&ok);
    return ok ? v : 0;
}

bool VideoLatencyProbe::isEnabled() const
{
    return m_enabled;
}

void VideoLatencyProbe::setEnabled(bool on)
{
    if (m_enabled == on)
        return;
    m_enabled = on;

    if (on) {
        m_lastLogMs = monoMs();
        m_statTimer->start();
        qDebug() << "[LatProbe] 探针已打开";
        if (!m_url.isEmpty())
            setStreamUrl(m_url);          // 补上之前设置的地址
    } else {
        stopCapture();
        m_statTimer->stop();
        qDebug() << "[LatProbe] 探针已关闭";
    }
    emit enabledChanged();
}

bool VideoLatencyProbe::setStreamUrl(const QString &url)
{
    m_url = url;
    if (!m_enabled)
        return false;                      // 关闭状态下不建立任何连接

    QString host;
    quint16 port = 0;
    if (!parseUdpUrl(url, host, port)) {
        if (!url.isEmpty())
            qWarning() << "[LatProbe] 当前只支持 udp:// 组播地址，已忽略:" << url;
        stopCapture();
        return false;
    }

    startCapture(host, port);
    return true;
}

void VideoLatencyProbe::startCapture(const QString &host, quint16 port)
{
    stopCapture();

    const QString ifaceName = QString::fromLocal8Bit(qgetenv("KYLIN_LATPROBE_IFACE"));
    m_capture = new Capture(this, host, port, ifaceName);
    m_capture->start();
}

void VideoLatencyProbe::stopCapture()
{
    if (!m_capture)
        return;
    m_capture->stop();
    if (!m_capture->wait(3000))
        qWarning() << "[LatProbe] 抓包线程未在 3s 内退出";
    delete m_capture;
    m_capture = nullptr;
}

void VideoLatencyProbe::notePlaybackStart()
{
    QMutexLocker lock(&m_mutex);
    // 下一帧到达时重新锚定 PTS 基准（换流、重连都会走到这里）
    m_anchorReady = false;
    m_anchorPending = true;
}

void VideoLatencyProbe::onCaptureArrival(quint64 pts90k, qint64 tArriveMs)
{
    QMutexLocker lock(&m_mutex);

    // ── 建立 PTS 基准 ──
    // mpv 把时间轴归一化到 0，而 TS 里的 PTS 是原始值，两者相差一个常数
    // （就是首帧的原始 PTS）。这里用"loadfile 之后收到的第一帧"作为基准。
    if (!m_anchorReady && (m_anchorPending || m_lastArrivalPts == 0)) {
        m_anchorPts = pts90k;
        m_anchorReady = true;
        m_anchorPending = false;
        qDebug() << "[LatProbe] PTS 基准已锚定: 原始 PTS =" << pts90k;
    }

    if (m_arrive.contains(pts90k))
        return;                            // 同一帧的后续包 / 重传

    m_arrive.insert(pts90k, tArriveMs);
    m_order.append(pts90k);

    if (m_lastArrivalPts != 0) {
        const quint64 d = (pts90k - m_lastArrivalPts) & kPtsMask;
        if (d > 0 && d < 90000ull) {       // 小于 1s 视为正常帧间隔
            m_stepSum += static_cast<double>(d);
            ++m_stepCount;
        }
    }
    m_lastArrivalPts = pts90k;

    if (m_order.size() > kOrderMax) {
        const int drop = m_order.size() - kOrderMax;
        for (int i = 0; i < drop; ++i)
            m_arrive.remove(m_order[i]);
        m_order.remove(0, drop);
    }
}

void VideoLatencyProbe::onFrameDisplayed(double mpvPtsSeconds)
{
    if (!m_enabled)
        return;

    const quint64 mpvPts =
            static_cast<quint64>(llround(mpvPtsSeconds * 90000.0)) & kPtsMask;
    const qint64 tDisplay = monoMs();

    QMutexLocker lock(&m_mutex);
    if (!m_anchorReady || m_order.isEmpty())
        return;

    const double step = (m_stepCount > 0)
            ? (m_stepSum / static_cast<double>(m_stepCount))
            : 3003.0;                       // 默认按 30fps 估计
    const qint64 stepTicks = qMax<qint64>(1, qRound64(step));

    // 原始 PTS = mpv 归一化 PTS + 基准（+可选整帧修正）
    const qint64 shift = static_cast<qint64>(m_frameShift) * stepTicks;
    const quint64 shiftedBase =
            static_cast<quint64>(m_anchorPts + static_cast<quint64>(shift));

    // 查表：只做 ±1 tick 的邻域探测（约 11us），绝不跨帧搜索
    const auto lookup = [this](quint64 k) -> qint64 {
        qint64 t = m_arrive.value(k, -1);
        if (t < 0) t = m_arrive.value((k - 1) & kPtsMask, -1);
        if (t < 0) t = m_arrive.value((k + 1) & kPtsMask, -1);
        return t;
    };

    quint64 key = (mpvPts + shiftedBase) & kPtsMask;
    qint64  tArrive = -1;

    if (!m_modeConfirmed) {
        // 首帧：先按"mpv 把时间轴归一化到 0"试，不中再按"PTS 保持原始值"试
        tArrive = lookup(key);
        if (tArrive >= 0) {
            m_modeConfirmed = true;
            m_useRawPts = false;
        } else {
            const quint64 rawKey =
                    (mpvPts + static_cast<quint64>(shift)) & kPtsMask;
            tArrive = lookup(rawKey);
            if (tArrive >= 0) {
                m_modeConfirmed = true;
                m_useRawPts = true;
                qDebug() << "[LatProbe] mpv 的 video-pts 未做归一化，改按原始 PTS 匹配";
            }
        }
    } else {
        key = m_useRawPts
                ? ((mpvPts + static_cast<quint64>(shift)) & kPtsMask)
                : ((mpvPts + shiftedBase) & kPtsMask);
        tArrive = lookup(key);
    }

    if (tArrive < 0) {
        if (++m_missStreak == 60 && m_samples.isEmpty()) {
            qWarning() << "[LatProbe] 连续 60 帧匹配不到到达记录。常见原因："
                       << "① 组播地址/网卡不对；② PTS 基准差了几帧，"
                       << "可用环境变量 KYLIN_LATPROBE_SHIFT=<帧数> 逐帧试（±1~3）；"
                       << "③ 视频 PID 不是 0xE0~0xEF 的视频 ES。";
        }
        ++m_misses;
        return;
    }
    m_missStreak = 0;

    const double latency = static_cast<double>(tDisplay - tArrive);
    if (latency < kLatencyLoMs || latency > kLatencyHiMs) {
        ++m_misses;                        // 异常样本（换流/长暂停等）
        return;
    }

    if (m_samples.size() < kSampleMax)
        m_samples.append(latency);
    writeCsvRow(key, tArrive, tDisplay, latency);
}

void VideoLatencyProbe::writeCsvRow(quint64 pts, qint64 tArriveMs,
                                    qint64 tDisplayMs, double latencyMs)
{
    if (m_csvPath.isEmpty())
        return;
    if (!m_csvHeaderWritten) {
        m_csvHeaderWritten = true;
        m_csvBuf += QByteArray(
            "# VideoLatencyProbe  接收侧延迟 (ms)\n"
            "# 口径: 本机收到该帧首个 TS 包 -> 渲染换页。单调时钟, 无需对时。\n"
            "# 软件口径比肉眼可见延迟少 0~2 帧。\n");
        m_csvBuf += "# 会话开始(墙钟 ms),"
                    + QByteArray::number(QDateTime::currentMSecsSinceEpoch()) + "\n";
        m_csvBuf += "pts90k,t_arrive_ms,t_display_ms,latency_ms\n";
    }
    m_csvBuf += QByteArray::number(static_cast<qulonglong>(pts)) + ','
              + QByteArray::number(static_cast<qlonglong>(tArriveMs)) + ','
              + QByteArray::number(static_cast<qlonglong>(tDisplayMs)) + ','
              + QByteArray::number(latencyMs, 'f', 3) + '\n';
}

void VideoLatencyProbe::flushCsv()
{
    QByteArray pending;
    {
        QMutexLocker lock(&m_mutex);
        if (m_csvBuf.isEmpty())
            return;
        pending.swap(m_csvBuf);
    }
    QFile f(m_csvPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "[LatProbe] 无法写入 CSV:" << m_csvPath << f.errorString();
        return;
    }
    f.write(pending);
    f.close();
}

VideoLatencyProbe::Stats VideoLatencyProbe::stats() const
{
    Stats s;
    QVector<double> copy;
    {
        QMutexLocker lock(&m_mutex);
        copy = m_samples;
        s.samples     = m_samples.size();
        s.misses      = m_misses;
        s.anchorPts   = m_anchorPts;
        s.anchorReady = m_anchorReady;
        if (m_stepCount > 0)
            s.ptsStepMs = m_stepSum / static_cast<double>(m_stepCount) / 90.0;
    }
    if (copy.isEmpty())
        return s;

    std::sort(copy.begin(), copy.end());
    double sum = 0.0;
    for (double v : copy)
        sum += v;
    s.avgMs = sum / static_cast<double>(copy.size());
    s.minMs = copy.first();
    s.maxMs = copy.last();
    s.p50Ms = percentile(copy, 0.50);
    s.p95Ms = percentile(copy, 0.95);
    s.p99Ms = percentile(copy, 0.99);
    return s;
}

void VideoLatencyProbe::resetStats()
{
    QMutexLocker lock(&m_mutex);
    m_samples.clear();
    m_misses = 0;
}

void VideoLatencyProbe::onStatTimer()
{
    flushCsv();

    if (m_capture && m_capture->failed()) {
        qWarning() << "[LatProbe] 抓包失败，探针停止";
        setEnabled(false);
        return;
    }

    const qint64 now = monoMs();
    if (now - m_lastLogMs >= 5000) {
        m_lastLogMs = now;
        const Stats s = stats();
        if (s.samples > 0) {
            qDebug().nospace()
                << "[LatProbe] 接收侧延迟  样本=" << s.samples
                << "  平均=" << QString::number(s.avgMs, 'f', 1) << "ms"
                << "  P50=" << QString::number(s.p50Ms, 'f', 1)
                << "  P95=" << QString::number(s.p95Ms, 'f', 1)
                << "  P99=" << QString::number(s.p99Ms, 'f', 1)
                << "  最小=" << QString::number(s.minMs, 'f', 1)
                << "  最大=" << QString::number(s.maxMs, 'f', 1)
                << "  查不到=" << s.misses
                << "  帧间隔=" << QString::number(s.ptsStepMs, 'f', 2) << "ms";
        } else {
            qDebug() << "[LatProbe] 尚无有效样本  miss=" << s.misses
                     << " 锚定=" << s.anchorReady;
        }
    }

    emit statsUpdated();
}
