#ifndef EXGUIDE_TIMESYNC_H
#define EXGUIDE_TIMESYNC_H

#include <QElapsedTimer>
#include <QtGlobal>
#include <algorithm>
#include <cstdint>

// 1s外引导（协议0x0A，四个250ms插值点）的绝对时间调度。
// 每组数据在目标秒开始前200ms发送，避免恰好在周期边界发包导致超时。
class PeriodicExGuideSchedule
{
public:
    enum {
        kSecondMs = 1000,
        kSendLeadMs = 200,
        kLoopSeconds = 14400
    };

    struct Slot {
        qint64 rawTargetSecond = 0;
        uint32_t targetSecond = 0;
        qint64 delayUntilSendMs = 0;
        double firstPredictionDelaySec = 0.25;
    };

    void restart()
    {
        m_clock.start();
    }

    bool isValid() const
    {
        return m_clock.isValid();
    }

    qint64 elapsedMs() const
    {
        return m_clock.isValid() ? m_clock.elapsed() : 0;
    }

    // 返回当前应发送的槽位。运行延迟跨过目标秒边界时自动跳到下一目标秒。
    Slot currentSlot() const
    {
        const qint64 nowMs = elapsedMs();
        Slot slot;
        slot.rawTargetSecond = nowMs / kSecondMs + 1;
        slot.targetSecond = static_cast<uint32_t>(
            slot.rawTargetSecond % kLoopSeconds);

        const qint64 targetStartMs = slot.rawTargetSecond * kSecondMs;
        const qint64 sendAtMs = targetStartMs - kSendLeadMs;
        slot.delayUntilSendMs = std::max<qint64>(0, sendAtMs - nowMs);

        const double secondsToTargetStart = static_cast<double>(
            std::max<qint64>(0, targetStartMs - nowMs)) / kSecondMs;
        slot.firstPredictionDelaySec = secondsToTargetStart + 0.25;
        return slot;
    }

    // 发送完指定槽位后，计算到下一槽位发送时刻的延时。
    qint64 delayUntilNextSendMs(qint64 sentRawTargetSecond) const
    {
        const qint64 nowMs = elapsedMs();
        const qint64 nextSendAtMs =
                (sentRawTargetSecond + 1) * kSecondMs - kSendLeadMs;
        return std::max<qint64>(1, nextSendAtMs - nowMs);
    }

private:
    QElapsedTimer m_clock;
};

#endif // EXGUIDE_TIMESYNC_H
