#include "ccdlatencyprobe.h"

#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

#include <algorithm>
#include <cmath>

#ifndef KYLIN_CCDLAT_DEFAULT_ON
#define KYLIN_CCDLAT_DEFAULT_ON 0
#endif

#ifndef KYLIN_CCDLAT_DEFAULT_CSV
#define KYLIN_CCDLAT_DEFAULT_CSV ""
#endif

namespace {

constexpr int kSampleMax = 2000000;
constexpr double kLatencyLoMs = 0.0;
constexpr double kLatencyHiMs = 5000.0;
constexpr qint64 kLogIntervalNs = 5LL * 1000 * 1000 * 1000;

inline qint64 monoNs()
{
    static QElapsedTimer timer = [] {
        QElapsedTimer t;
        t.start();
        return t;
    }();
    return timer.nsecsElapsed();
}

bool parseBool(const QByteArray &raw, bool fallback)
{
    const QByteArray v = raw.trimmed().toLower();
    if (v.isEmpty())
        return fallback;
    if (v == "0" || v == "false" || v == "off" || v == "no")
        return false;
    return v == "1" || v == "true" || v == "on" || v == "yes";
}

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

CcdLatencyProbe::CcdLatencyProbe(QObject *parent)
    : QObject(parent)
    , m_enabled(envEnabled())
    , m_csvPath(envCsvPath())
    , m_mutex(new QMutex)
{
    m_statTimer = new QTimer(this);
    m_statTimer->setInterval(1000);
    connect(m_statTimer, &QTimer::timeout, this, &CcdLatencyProbe::onStatTimer);

    if (m_enabled.load(std::memory_order_relaxed)) {
        m_lastLogNs = monoNs();
        m_statTimer->start();
        qDebug() << "[CcdLatProbe] 已启用  CCD直显口径: mpv解码帧就绪 -> Qt本帧绘制"
                 << " CSV:" << (m_csvPath.isEmpty() ? QStringLiteral("<不写文件>") : m_csvPath);
    }
}

CcdLatencyProbe::~CcdLatencyProbe()
{
    if (m_statTimer)
        m_statTimer->stop();
    flushCsv();
    delete m_mutex;
    m_mutex = nullptr;
}

bool CcdLatencyProbe::envEnabled()
{
    if (qEnvironmentVariableIsSet("KYLIN_CCDLAT"))
        return parseBool(qgetenv("KYLIN_CCDLAT"), KYLIN_CCDLAT_DEFAULT_ON != 0);
    if (qEnvironmentVariableIsSet("KYLIN_LATPROBE"))
        return parseBool(qgetenv("KYLIN_LATPROBE"), KYLIN_CCDLAT_DEFAULT_ON != 0);
    return KYLIN_CCDLAT_DEFAULT_ON != 0;
}

QString CcdLatencyProbe::envCsvPath()
{
    if (qEnvironmentVariableIsSet("KYLIN_CCDLAT_CSV"))
        return QString::fromLocal8Bit(qgetenv("KYLIN_CCDLAT_CSV"));
    return QString::fromUtf8(KYLIN_CCDLAT_DEFAULT_CSV);
}

bool CcdLatencyProbe::isEnabled() const
{
    return m_enabled.load(std::memory_order_relaxed);
}

void CcdLatencyProbe::setEnabled(bool on)
{
    if (m_enabled.exchange(on, std::memory_order_relaxed) == on)
        return;

    if (on) {
        m_lastLogNs = monoNs();
        m_statTimer->start();
        qDebug() << "[CcdLatProbe] CCD直显延迟探针已打开";
    } else {
        {
            QMutexLocker lock(m_mutex);
            m_decodedNs = -1;
        }
        flushCsv();
        m_statTimer->stop();
        qDebug() << "[CcdLatProbe] CCD直显延迟探针已关闭";
    }
    emit enabledChanged();
}

void CcdLatencyProbe::notePlaybackStart()
{
    QMutexLocker lock(m_mutex);
    m_decodedNs = -1;
}

void CcdLatencyProbe::onFrameDecoded()
{
    if (!m_enabled.load(std::memory_order_relaxed))
        return;
    QMutexLocker lock(m_mutex);
    m_decodedNs = monoNs();
}

void CcdLatencyProbe::onFrameRendered(bool newFrameRendered)
{
    if (!newFrameRendered || !m_enabled.load(std::memory_order_relaxed))
        return;

    const qint64 tRenderNs = monoNs();
    QMutexLocker lock(m_mutex);
    if (m_decodedNs < 0)
        return;

    const qint64 tDecodedNs = m_decodedNs;
    m_decodedNs = -1;
    const double latency = static_cast<double>(tRenderNs - tDecodedNs) / 1.0e6;
    if (latency < kLatencyLoMs || latency > kLatencyHiMs) {
        ++m_misses;
        return;
    }

    ++m_samples;
    if (m_samplesMs.size() < kSampleMax)
        m_samplesMs.append(latency);
    writeCsvRow(tDecodedNs, tRenderNs, latency);
}

CcdLatencyProbe::Stats CcdLatencyProbe::stats() const
{
    Stats s;
    QVector<double> copy;
    {
        QMutexLocker lock(m_mutex);
        s.samples = m_samples;
        s.misses = m_misses;
        copy = m_samplesMs;
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

void CcdLatencyProbe::resetStats()
{
    QMutexLocker lock(m_mutex);
    m_decodedNs = -1;
    m_samples = 0;
    m_misses = 0;
    m_samplesMs.clear();
}

void CcdLatencyProbe::writeCsvRow(qint64 tDecodedNs, qint64 tRenderNs,
                                  double latencyMs)
{
    if (m_csvPath.isEmpty())
        return;
    if (!m_csvHeaderWritten) {
        m_csvHeaderWritten = true;
        m_csvBuf += QByteArray(
            "# CcdLatencyProbe  CCD直显延迟 (ms)\n"
            "# 口径: mpv解码帧就绪 -> Qt完成本帧绘制。\n"
            "# 适用: CCD且视觉模型推理关闭；不含网络接收、解复用和解码。\n");
        m_csvBuf += "# 会话开始(墙钟 ms),"
                    + QByteArray::number(QDateTime::currentMSecsSinceEpoch()) + "\n";
        m_csvBuf += "t_decoded_ms,t_render_ms,latency_ms\n";
    }
    m_csvBuf += QByteArray::number(static_cast<double>(tDecodedNs) / 1.0e6, 'f', 3) + ','
              + QByteArray::number(static_cast<double>(tRenderNs) / 1.0e6, 'f', 3) + ','
              + QByteArray::number(latencyMs, 'f', 3) + '\n';
}

void CcdLatencyProbe::flushCsv()
{
    QByteArray pending;
    {
        QMutexLocker lock(m_mutex);
        if (m_csvBuf.isEmpty())
            return;
        pending.swap(m_csvBuf);
    }

    QFile f(m_csvPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "[CcdLatProbe] 无法写入 CSV:" << m_csvPath << f.errorString();
        return;
    }

    qint64 total = 0;
    while (total < pending.size()) {
        const qint64 written =
                f.write(pending.constData() + total, pending.size() - total);
        if (written <= 0)
            break;
        total += written;
    }
    if (total < pending.size() || !f.flush()) {
        if (total < pending.size()) {
            QMutexLocker lock(m_mutex);
            m_csvBuf.prepend(pending.mid(static_cast<int>(total)));
        }
        qWarning() << "[CcdLatProbe] CSV 写入失败:" << m_csvPath << f.errorString();
    }
    f.close();
}

void CcdLatencyProbe::onStatTimer()
{
    flushCsv();

    const qint64 now = monoNs();
    if (now - m_lastLogNs < kLogIntervalNs)
        return;
    m_lastLogNs = now;

    const Stats s = stats();
    if (s.samples > 0) {
        qDebug().nospace()
            << "[CcdLatProbe] CCD直显延迟  样本=" << s.samples
            << "  平均=" << QString::number(s.avgMs, 'f', 3) << "ms"
            << "  P50=" << QString::number(s.p50Ms, 'f', 3)
            << "  P95=" << QString::number(s.p95Ms, 'f', 3)
            << "  P99=" << QString::number(s.p99Ms, 'f', 3)
            << "  最小=" << QString::number(s.minMs, 'f', 3)
            << "  最大=" << QString::number(s.maxMs, 'f', 3)
            << "  异常=" << s.misses;
    } else {
        qDebug() << "[CcdLatProbe] 尚无有效样本  异常=" << s.misses;
    }

    emit statsUpdated();
}
