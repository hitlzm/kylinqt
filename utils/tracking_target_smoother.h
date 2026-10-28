#ifndef TRACKING_TARGET_SMOOTHER_H
#define TRACKING_TARGET_SMOOTHER_H

#include <algorithm>
#include <array>
#include <cmath>

// 单轴外引导目标整形器。
//
// 输入是“转台当前反馈角 + 导引头相对偏差”，输出是可直接写入 0x0A/0x0B
// 协议的绝对目标角。偏差小于阈值时保持原来的直接跟踪；偏差超过阈值时，
// 输出按最大速度和最大加速度逼近目标的斜坡，避免大阶跃让转台全速运行。
//
// 进入斜坡后使用较小的退出阈值，并且要等输出真正追上目标才退出，避免
// 目标在阈值附近抖动时在“直通”和“斜坡”之间反复切换。
class TrackingTargetSmoother
{
public:
    struct Config {
        double rampEnterOffsetDeg = 3.0;
        double rampExitOffsetDeg = 2.0;
        double maxSpeedDegPerSec = 10.0;
        double maxAccelerationDegPerSec2 = 10.0;
        double settleDeadbandDeg = 0.01;
    };

    struct StepResult {
        double positionDeg = 0.0;
        double velocityDegPerSec = 0.0;
        bool ramping = false;
    };

    TrackingTargetSmoother()
    {
        setConfig(Config());
    }

    explicit TrackingTargetSmoother(const Config &config)
    {
        setConfig(config);
    }

    void setConfig(const Config &config)
    {
        m_config = config;
        if (!std::isfinite(m_config.rampEnterOffsetDeg) || m_config.rampEnterOffsetDeg < 0.0)
            m_config.rampEnterOffsetDeg = 3.0;
        if (!std::isfinite(m_config.rampExitOffsetDeg) || m_config.rampExitOffsetDeg < 0.0)
            m_config.rampExitOffsetDeg = std::min(2.0, m_config.rampEnterOffsetDeg);
        m_config.rampExitOffsetDeg = std::min(
                    m_config.rampExitOffsetDeg, m_config.rampEnterOffsetDeg);
        if (!std::isfinite(m_config.maxSpeedDegPerSec) || m_config.maxSpeedDegPerSec <= 0.0)
            m_config.maxSpeedDegPerSec = 10.0;
        if (!std::isfinite(m_config.maxAccelerationDegPerSec2)
                || m_config.maxAccelerationDegPerSec2 <= 0.0) {
            m_config.maxAccelerationDegPerSec2 = 10.0;
        }
        if (!std::isfinite(m_config.settleDeadbandDeg) || m_config.settleDeadbandDeg < 0.0)
            m_config.settleDeadbandDeg = 0.01;
    }

    const Config &config() const
    {
        return m_config;
    }

    bool initialized() const
    {
        return m_initialized;
    }

    bool ramping() const
    {
        return m_ramping;
    }

    void reset(double feedbackDeg)
    {
        if (std::isfinite(feedbackDeg)) {
            m_positionDeg = feedbackDeg;
            m_velocityDegPerSec = 0.0;
            m_initialized = true;
        } else {
            m_positionDeg = 0.0;
            m_velocityDegPerSec = 0.0;
            m_initialized = false;
        }
        m_ramping = false;
    }

    StepResult update(double feedbackDeg, double requestedOffsetDeg, double dtSec)
    {
        if (!std::isfinite(feedbackDeg)) {
            return currentResult();
        }
        if (!m_initialized)
            reset(feedbackDeg);

        // 单帧异常数据不应把已经整形好的目标状态破坏掉。
        if (!std::isfinite(requestedOffsetDeg) || !std::isfinite(dtSec) || dtSec <= 0.0)
            return currentResult();

        const double absOffset = std::abs(requestedOffsetDeg);
        const double desiredPosition = feedbackDeg + requestedOffsetDeg;

        if (!m_ramping && absOffset > m_config.rampEnterOffsetDeg) {
            m_ramping = true;
            // 直通模式的速度只用于状态观测，不能带入新斜坡，否则反馈跳变
            // 可能在进入斜坡的第一拍产生一次大角度推进。
            m_velocityDegPerSec = 0.0;
        }

        if (!m_ramping) {
            // 小偏差沿用原来的直接跟踪，保证正常小视场跟踪没有额外滞后。
            const double oldPosition = m_positionDeg;
            m_positionDeg = desiredPosition;
            m_velocityDegPerSec =
                    (m_positionDeg - oldPosition) / dtSec;
            return currentResult();
        }

        const double error = desiredPosition - m_positionDeg;
        const double maxVelocityStep =
                m_config.maxAccelerationDegPerSec2 * dtSec;

        // 先按剩余距离计算允许的制动速度，再叠加加速度约束。这样接近目标
        // 时会提前减速，而不是等到越过目标才由位置钳位强制停车。
        const double stopLimitedVelocity = std::sqrt(
                    2.0 * m_config.maxAccelerationDegPerSec2 * std::abs(error));
        const double velocityMagnitude = std::min(
                    m_config.maxSpeedDegPerSec,
                    std::min(stopLimitedVelocity, std::abs(error) / dtSec));
        const double wantedVelocity = error >= 0.0
                ? velocityMagnitude : -velocityMagnitude;
        m_velocityDegPerSec += std::max(
                    -maxVelocityStep,
                    std::min(maxVelocityStep, wantedVelocity - m_velocityDegPerSec));

        const double oldPosition = m_positionDeg;
        m_positionDeg += m_velocityDegPerSec * dtSec;

        // 不允许因为采样间隔或目标修正越过新目标。
        if ((oldPosition <= desiredPosition && m_positionDeg > desiredPosition)
                || (oldPosition >= desiredPosition && m_positionDeg < desiredPosition)) {
            m_positionDeg = desiredPosition;
            m_velocityDegPerSec = 0.0;
        }

        if (!std::isfinite(m_positionDeg) || !std::isfinite(m_velocityDegPerSec)) {
            reset(feedbackDeg);
            return currentResult();
        }

        // 退出斜坡需要同时满足两个条件：导引头偏差已回到小角度，且本次
        // 整形输出已经追上目标。后者防止大目标突然变小时输出瞬间跳变。
        if (absOffset <= m_config.rampExitOffsetDeg
                && std::abs(desiredPosition - m_positionDeg)
                    <= m_config.settleDeadbandDeg) {
            m_positionDeg = desiredPosition;
            m_velocityDegPerSec = 0.0;
            m_ramping = false;
        }

        return currentResult();
    }

    // 1 秒协议一次写四个 250 ms 点。这里从当前反馈重新生成一条轨迹，
    // 不推进常驻的 20 ms 状态；下一包收到反馈后再重新生成，形成闭环修正。
    template <std::size_t N>
    std::array<double, N> preview(double feedbackDeg,
                                  const std::array<double, N> &requestedOffsetsDeg,
                                  double intervalSec) const
    {
        std::array<double, N> positions;
        positions.fill(feedbackDeg);
        if (!std::isfinite(feedbackDeg)
                || !std::isfinite(intervalSec) || intervalSec <= 0.0) {
            return positions;
        }

        TrackingTargetSmoother trial(m_config);
        trial.reset(feedbackDeg);
        for (std::size_t i = 0; i < N; ++i) {
            if (!std::isfinite(requestedOffsetsDeg[i])) {
                positions[i] = trial.m_positionDeg;
                continue;
            }
            trial.update(feedbackDeg, requestedOffsetsDeg[i], intervalSec);
            positions[i] = trial.m_positionDeg;
        }
        return positions;
    }

private:
    StepResult currentResult() const
    {
        StepResult result;
        result.positionDeg = m_positionDeg;
        result.velocityDegPerSec = m_velocityDegPerSec;
        result.ramping = m_ramping;
        return result;
    }

    Config m_config;
    bool m_initialized = false;
    bool m_ramping = false;
    double m_positionDeg = 0.0;
    double m_velocityDegPerSec = 0.0;
};

#endif // TRACKING_TARGET_SMOOTHER_H
