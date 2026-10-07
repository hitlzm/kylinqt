#include "sendlatencyprobe.h"

#if SENDLAT_COMPILED

#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include <QString>
#include <QVector>
#include <algorithm>

// ══════════════════════════════════════════════════════════════════════════
// 使能开关的存储：默认取编译期 SENDLAT_DEFAULT_ON，install() 里再被
// 环境变量覆盖。
// ══════════════════════════════════════════════════════════════════════════
std::atomic<bool> SendLat::g_enabled{ SENDLAT_DEFAULT_ON != 0 };

namespace {

// ── 可调参数 ─────────────────────────────────────────────────────────────
const qint64 kStaleNs      = 500LL * 1000 * 1000;  // 令牌超过 500ms 未配对即视为过期
const qint64 kLatchNs      = 1000LL * 1000 * 1000; // "本次请求已取样"标志的有效期
const int    kMaxPending   = 64;                   // 待配对令牌上限
const int    kMaxSamples   = 20000;                // 保留的原始样本上限
const int    kDefaultEvery = 50;                   // 默认每 50 个样本打印一次汇总

const char *kChannelKeys[SendLat::ChannelCount] = { "image", "laser", "turntable" };

// 进程级单调时钟：主线程与工作线程共用同一个参考点，所以差值可直接跨线程比较。
// Qt 5.12 没有 QElapsedTimer::nsecsSinceReference()，这里用一个进程内唯一的
// 参考计时器 + nsecsElapsed() 达到同样的效果（纳秒分辨率）。
struct MonoClock {
    MonoClock() { t.start(); }
    QElapsedTimer t;
};

const QElapsedTimer &referenceTimer()
{
    static MonoClock c;     // C++11 起函数局部静态初始化线程安全
    return c.t;
}

inline qint64 monoNs() { return referenceTimer().nsecsElapsed(); }

// 队列里等待配对的"点击时刻"
struct Token { qint64 t1Ns; };

struct ChannelState {
    QQueue<Token>   pending;                  // 已点击、尚未发出的请求
    bool            latched      = false;     // 本次请求是否已经取过样
    qint64          latchedAtNs  = 0;         // 取样的时刻，用于判断闩锁是否还新鲜
    QVector<double> samplesMs;                // 原始样本(ms)，最多 kMaxSamples 条
    QByteArray      csvRows;                  // 逐条 CSV（内存缓存，退出时落盘）
    qint64 samplesTotal = 0;                  // 有效样本总数（可能大于 samplesMs.size()）
    qint64 coalesced    = 0;                  // 同一次点击的后续帧
    qint64 ignored      = 0;                  // 无对应点击的写入
    qint64 dropped      = 0;                  // 被 discardClick 丢弃的请求
    qint64 stale        = 0;                  // 过期令牌
    qint64 nextSummary  = kDefaultEvery;      // 下次打印汇总的样本数阈值
};

// 最近秩法取百分位下标，n 为样本数
int nearestRankIndex(int n, double p)
{
    if (n <= 0) return -1;
    const int idx = static_cast<int>(p * (n - 1) + 0.5);
    return qBound(0, idx, n - 1);
}

bool parseBool(const QByteArray &raw, bool fallback)
{
    const QByteArray v = raw.trimmed().toLower();
    if (v.isEmpty()) return fallback;
    return v == "1" || v == "true" || v == "on" || v == "yes";
}

class Probe
{
public:
    static Probe &instance()
    {
        static Probe p;
        return p;
    }

    void configure(bool verbose, int everyN, const QString &csvPath)
    {
        QMutexLocker lk(&m_mutex);
        m_verbose = verbose;
        m_everyN  = everyN > 0 ? everyN : 0;
        m_csvPath = csvPath;
        for (int ch = 0; ch < SendLat::ChannelCount; ++ch) {
            ChannelState &s = m_ch[ch];
            s.nextSummary = s.samplesTotal + (m_everyN > 0 ? m_everyN : kDefaultEvery);
        }
    }

    void setCsvPath(const QString &path)
    {
        QMutexLocker lk(&m_mutex);
        m_csvPath = path;
    }

    // 开关切换时清空配对状态，避免半路启停留下配不上的陈旧令牌
    void resetPairing()
    {
        QMutexLocker lk(&m_mutex);
        for (int ch = 0; ch < SendLat::ChannelCount; ++ch) {
            m_ch[ch].pending.clear();
            m_ch[ch].latched = false;
        }
    }

    void resetStats()
    {
        QMutexLocker lk(&m_mutex);
        for (int ch = 0; ch < SendLat::ChannelCount; ++ch) {
            ChannelState &s = m_ch[ch];
            s.pending.clear();
            s.latched = false;
            s.latchedAtNs = 0;
            s.samplesMs.clear();
            s.csvRows.clear();
            s.samplesTotal = 0;
            s.coalesced    = 0;
            s.ignored      = 0;
            s.dropped      = 0;
            s.stale        = 0;
            s.nextSummary  = m_everyN > 0 ? m_everyN : kDefaultEvery;
        }
    }

    // 主线程：人工点击
    void markClick(int ch)
    {
        if (ch < 0 || ch >= SendLat::ChannelCount) return;
        const qint64 now = monoNs();
        QMutexLocker lk(&m_mutex);
        ChannelState &s = m_ch[ch];
        if (s.pending.size() >= kMaxPending) s.pending.dequeue();
        Token t;
        t.t1Ns = now;
        s.pending.enqueue(t);
        s.latched = false;      // 新的一次点击 → 重新武装，下一帧重新取样
    }

    // 工作线程：请求被丢弃
    void discardClick(int ch)
    {
        if (ch < 0 || ch >= SendLat::ChannelCount) return;
        QMutexLocker lk(&m_mutex);
        ChannelState &s = m_ch[ch];
        if (!s.pending.isEmpty()) {
            s.pending.dequeue();
            s.dropped++;
        }
        s.latched = true;       // 本次请求已结束，后续帧不再取样
        s.latchedAtNs = monoNs();
    }

    // 工作线程：真正 write 之前
    void markSend(int ch)
    {
        if (ch < 0 || ch >= SendLat::ChannelCount) return;

        const qint64 now = monoNs();

        bool    printOne    = false;
        double  sampleMs    = 0.0;
        qint64  sampleIndex = 0;
        qint64  summaryAt   = -1;

        {
            QMutexLocker lk(&m_mutex);
            ChannelState &s = m_ch[ch];

            // 同一次点击的补发拍不重复取样
            if (s.latched) {
                if (now - s.latchedAtNs <= kLatchNs) {
                    s.coalesced++;
                    return;
                }
                // 闩锁已过期：上一次请求早就结束了，这是一次新的写入
                s.latched = false;
            }

            bool hit = false;
            while (!s.pending.isEmpty()) {
                const Token t = s.pending.dequeue();
                const qint64 delta = now - t.t1Ns;
                if (delta > kStaleNs) {     // 请求丢失后残留的令牌，丢弃
                    s.stale++;
                    continue;
                }
                sampleMs = static_cast<double>(delta) / 1e6;
                hit = true;
                break;
            }

            if (!hit) {                     // 自动发送源 / 其它未打点入口
                s.ignored++;
                s.latched = true;
                s.latchedAtNs = now;
                return;
            }

            s.latched = true;
            s.latchedAtNs = now;
            s.samplesTotal++;
            sampleIndex = s.samplesTotal;
            if (s.samplesMs.size() < kMaxSamples) s.samplesMs.append(sampleMs);

            if (!m_csvPath.isEmpty()) {
                s.csvRows += kChannelKeys[ch];
                s.csvRows += ',';
                s.csvRows += QByteArray::number(sampleIndex);
                s.csvRows += ',';
                s.csvRows += QByteArray::number(sampleMs, 'f', 3);
                s.csvRows += '\n';
            }

            if (m_verbose) printOne = true;

            if (m_everyN > 0 && s.samplesTotal >= s.nextSummary) {
                s.nextSummary = s.samplesTotal + m_everyN;
                summaryAt = s.samplesTotal;
            }
        }

        // 打印放在锁外、且在 write 之前：记录到的样本值不受影响，
        // 但这一拍的写入会被打印耗时略微推后。
        if (printOne) {
            // 一律用 ASCII 输出：终端里不受代码页影响，也方便 grep 与出报告
            qDebug().noquote() << QStringLiteral("[SendLat] %1  #%2  latency = %3 ms")
                                      .arg(QLatin1String(kChannelKeys[ch]))
                                      .arg(sampleIndex)
                                      .arg(sampleMs, 0, 'f', 3);
        }
        if (summaryAt > 0) dumpSummary();
    }

    SendLat::Stats stats(int ch) const
    {
        QMutexLocker lk(&m_mutex);
        return statsLocked(ch);
    }

    void dumpSummary()
    {
        QString    text;
        QByteArray csv;
        QString    csvPath;

        {
            QMutexLocker lk(&m_mutex);
            csvPath = m_csvPath;
            for (int ch = 0; ch < SendLat::ChannelCount; ++ch) {
                const ChannelState &s = m_ch[ch];
                if (s.samplesTotal == 0 && s.coalesced == 0 && s.ignored == 0
                    && s.dropped == 0 && s.stale == 0) {
                    continue;
                }

                const SendLat::Stats st = statsLocked(ch);
                text += QStringLiteral(
                            "[SendLat] %1  samples=%2  avg=%3ms  p50=%4  p95=%5  p99=%6  "
                            "min=%7  max=%8  coalesced=%9  unmatched=%10  dropped=%11  stale=%12\n")
                            .arg(QLatin1String(kChannelKeys[ch]))
                            .arg(st.samples)
                            .arg(st.avgMs, 0, 'f', 3)
                            .arg(st.p50Ms, 0, 'f', 3)
                            .arg(st.p95Ms, 0, 'f', 3)
                            .arg(st.p99Ms, 0, 'f', 3)
                            .arg(st.minMs, 0, 'f', 3)
                            .arg(st.maxMs, 0, 'f', 3)
                            .arg(st.coalesced)
                            .arg(st.ignored)
                            .arg(st.dropped)
                            .arg(st.stale);

                if (!csvPath.isEmpty() && !s.csvRows.isEmpty()) csv += s.csvRows;
            }
        }

        if (text.isEmpty()) return;

        if (!csvPath.isEmpty() && !csv.isEmpty()) {
            QFile f(csvPath);
            if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                f.write("channel,index,send_latency_ms\n");
                f.write(csv);
                f.close();
            } else {
                qWarning().noquote() << QStringLiteral("[SendLat] cannot write csv: %1").arg(csvPath);
            }
        }

        // 汇总一次成型，避免多行交错
        qDebug().noquote() << text;
    }

private:
    Probe() = default;
    Probe(const Probe &) = delete;
    Probe &operator=(const Probe &) = delete;

    // 调用者必须已持有 m_mutex
    SendLat::Stats statsLocked(int ch) const
    {
        SendLat::Stats st;
        if (ch < 0 || ch >= SendLat::ChannelCount) return st;
        const ChannelState &s = m_ch[ch];

        st.samples   = s.samplesTotal;
        st.coalesced = s.coalesced;
        st.ignored   = s.ignored;
        st.dropped   = s.dropped;
        st.stale     = s.stale;

        if (!s.samplesMs.isEmpty()) {
            QVector<double> v = s.samplesMs;
            std::sort(v.begin(), v.end());
            const int n = v.size();
            double sum = 0.0;
            for (int i = 0; i < n; ++i) sum += v.at(i);
            st.avgMs = sum / n;
            st.minMs = v.first();
            st.maxMs = v.last();
            st.p50Ms = v.at(nearestRankIndex(n, 0.50));
            st.p95Ms = v.at(nearestRankIndex(n, 0.95));
            st.p99Ms = v.at(nearestRankIndex(n, 0.99));
        }
        return st;
    }

    mutable QMutex m_mutex;
    ChannelState   m_ch[SendLat::ChannelCount];
    bool           m_verbose = true;
    int            m_everyN  = kDefaultEvery;
    QString        m_csvPath;
};

// 进程启动时读一次环境变量并注册退出汇总
struct AutoInstall {
    AutoInstall() { SendLat::install(); }
} g_autoInstall;

} // namespace

// ══════════════════════════════════════════════════════════════════════════
// 对外接口
// ══════════════════════════════════════════════════════════════════════════

namespace SendLat {

const char *channelName(int ch)
{
    if (ch < 0 || ch >= ChannelCount) return "?";
    return kChannelKeys[ch];
}

bool envEnabled()
{
    return parseBool(qgetenv("KYLIN_SENDLAT"), SENDLAT_DEFAULT_ON != 0);
}

void install()
{
    g_enabled.store(envEnabled(), std::memory_order_relaxed);

    const bool verbose = parseBool(qgetenv("KYLIN_SENDLAT_VERBOSE"), true);

    bool ok = false;
    const int every = QString::fromLatin1(qgetenv("KYLIN_SENDLAT_EVERY")).toInt(&ok);

    const QString csvPath = QString::fromLocal8Bit(qgetenv("KYLIN_SENDLAT_CSV"));

    Probe::instance().configure(verbose, ok ? every : kDefaultEvery, csvPath);

    // 进程退出（QCoreApplication 析构）时自动打印一次汇总
    qAddPostRoutine(SendLat::dumpSummary);
}

void setEnabled(bool on)
{
    const bool prev = g_enabled.exchange(on, std::memory_order_relaxed);
    if (prev == on) return;

    if (!on) Probe::instance().dumpSummary();   // 关闭前先出一次汇总
    Probe::instance().resetPairing();           // 半路启停不留陈旧令牌

    qDebug().noquote() << QStringLiteral("[SendLat] probe %1")
                              .arg(on ? QStringLiteral("enabled") : QStringLiteral("disabled"));
}

void markClickImpl(int ch)    { Probe::instance().markClick(ch); }
void markSendImpl(int ch)     { Probe::instance().markSend(ch); }
void discardClickImpl(int ch) { Probe::instance().discardClick(ch); }

Stats stats(int ch)           { return Probe::instance().stats(ch); }
void  resetStats()            { Probe::instance().resetStats(); }
void  dumpSummary()           { Probe::instance().dumpSummary(); }
void  setCsvPath(const QString &path) { Probe::instance().setCsvPath(path); }

} // namespace SendLat

#endif // SENDLAT_COMPILED
