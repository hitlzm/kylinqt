#ifndef TRACKING_TARGET_SMOOTHER_H
#define TRACKING_TARGET_SMOOTHER_H

#include <algorithm>
#include <array>
#include <cmath>

// 单轴外引导目标整形器。
//
// 输入是“转台当前反馈角 + 导引头相对偏差”，输出是可直接写入 0x0A/0x0B
// 协议的绝对目标角。所有目标变化都经过速度和加速度限制，不再使用小角度
// 直通或“冻结到退出门限后突然恢复”的硬切换。
//
// 导引头角分辨率通常明显低于转台。零点附近先扣除软死区，再以低速连续跟进；
// 偏差超过近目标半径后，速度和加速度上限连续过渡到远目标跟踪能力。这样既
// 保留 0.002° 导引头分辨率下的近零响应，也不会形成较大的死区振荡。
class TrackingTargetSmoother
{
public:
    struct Config {
        double zeroDeadbandDeg = 0.004;
        double zeroDeadbandReleaseDeg = 0.006;
        double fineTrackRadiusDeg = 0.10;
        double fineMaxSpeedDegPerSec = 1.5;
        double fineMaxAccelerationDegPerSec2 = 3.0;
        double nearTrackRadiusDeg = 1.0;
        double nearMaxSpeedDegPerSec = 1.5;
        double nearMaxAccelerationDegPerSec2 = 3.0;
        double farTrackRadiusDeg = 3.0;
        double offsetFilterTimeConstantSec = 0.04;
        double offsetFilterFastTimeConstantSec = 0.02;
        double offsetFilterFastSlewThresholdDeg = 0.03;
        double offsetFilterFastResetDeltaDeg = 0.5;
        double offsetFilterSignResetMinDeg = 0.006;
        bool offsetFilterResetOnSignChange = true;
        double maxSpeedDegPerSec = 12.0;
        double maxAccelerationDegPerSec2 = 10.0;
        double settleDeadbandDeg = 0.002;
    };

    struct StepResult {
        double positionDeg = 0.0;
        double velocityDegPerSec = 0.0;
        bool ramping = false;
        bool zeroHeld = false;
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
        if (!std::isfinite(m_config.zeroDeadbandDeg)
                || m_config.zeroDeadbandDeg < 0.0) {
            m_config.zeroDeadbandDeg = 0.004;
        }
        if (!std::isfinite(m_config.zeroDeadbandReleaseDeg)
                || m_config.zeroDeadbandReleaseDeg < 0.0) {
            m_config.zeroDeadbandReleaseDeg = 0.006;
        }
        m_config.zeroDeadbandReleaseDeg = std::max(
                    m_config.zeroDeadbandDeg,
                    m_config.zeroDeadbandReleaseDeg);
        if (!std::isfinite(m_config.maxSpeedDegPerSec) || m_config.maxSpeedDegPerSec <= 0.0)
            m_config.maxSpeedDegPerSec = 10.0;
        if (!std::isfinite(m_config.maxAccelerationDegPerSec2)
                || m_config.maxAccelerationDegPerSec2 <= 0.0) {
            m_config.maxAccelerationDegPerSec2 = 6.0;
        }
        if (!std::isfinite(m_config.nearTrackRadiusDeg)
                || m_config.nearTrackRadiusDeg <= 0.0) {
            m_config.nearTrackRadiusDeg = 1.0;
        }
        m_config.nearTrackRadiusDeg = std::max(
                    m_config.nearTrackRadiusDeg, m_config.zeroDeadbandDeg);
        if (!std::isfinite(m_config.fineTrackRadiusDeg)
                || m_config.fineTrackRadiusDeg <= 0.0) {
            m_config.fineTrackRadiusDeg = 0.10;
        }
        m_config.fineTrackRadiusDeg = std::max(
                    m_config.zeroDeadbandDeg,
                    std::min(m_config.fineTrackRadiusDeg,
                             m_config.nearTrackRadiusDeg));
        if (!std::isfinite(m_config.farTrackRadiusDeg)
                || m_config.farTrackRadiusDeg <= m_config.nearTrackRadiusDeg) {
            m_config.farTrackRadiusDeg =
                    std::max(3.0, m_config.nearTrackRadiusDeg);
        }
        if (!std::isfinite(m_config.nearMaxSpeedDegPerSec)
                || m_config.nearMaxSpeedDegPerSec <= 0.0) {
            m_config.nearMaxSpeedDegPerSec = 1.5;
        }
        m_config.nearMaxSpeedDegPerSec = std::min(
                    m_config.nearMaxSpeedDegPerSec,
                    m_config.maxSpeedDegPerSec);
        if (!std::isfinite(m_config.nearMaxAccelerationDegPerSec2)
                || m_config.nearMaxAccelerationDegPerSec2 <= 0.0) {
            m_config.nearMaxAccelerationDegPerSec2 = 3.0;
        }
        m_config.nearMaxAccelerationDegPerSec2 = std::min(
                    m_config.nearMaxAccelerationDegPerSec2,
                    m_config.maxAccelerationDegPerSec2);
        if (!std::isfinite(m_config.fineMaxSpeedDegPerSec)
                || m_config.fineMaxSpeedDegPerSec <= 0.0) {
            m_config.fineMaxSpeedDegPerSec = 1.5;
        }
        m_config.fineMaxSpeedDegPerSec = std::min(
                    m_config.fineMaxSpeedDegPerSec,
                    m_config.nearMaxSpeedDegPerSec);
        if (!std::isfinite(m_config.fineMaxAccelerationDegPerSec2)
                || m_config.fineMaxAccelerationDegPerSec2 <= 0.0) {
            m_config.fineMaxAccelerationDegPerSec2 = 3.0;
        }
        m_config.fineMaxAccelerationDegPerSec2 = std::min(
                    m_config.fineMaxAccelerationDegPerSec2,
                    m_config.nearMaxAccelerationDegPerSec2);
        if (!std::isfinite(m_config.offsetFilterTimeConstantSec)
                || m_config.offsetFilterTimeConstantSec < 0.0) {
            m_config.offsetFilterTimeConstantSec = 0.04;
        }
        if (!std::isfinite(m_config.offsetFilterFastTimeConstantSec)
                || m_config.offsetFilterFastTimeConstantSec < 0.0) {
            m_config.offsetFilterFastTimeConstantSec = 0.02;
        }
        if (!std::isfinite(m_config.offsetFilterFastSlewThresholdDeg)
                || m_config.offsetFilterFastSlewThresholdDeg < 0.0) {
            m_config.offsetFilterFastSlewThresholdDeg = 0.03;
        }
        if (!std::isfinite(m_config.offsetFilterFastResetDeltaDeg)
                || m_config.offsetFilterFastResetDeltaDeg < 0.0) {
            m_config.offsetFilterFastResetDeltaDeg = 0.5;
        }
        if (!std::isfinite(m_config.offsetFilterSignResetMinDeg)
                || m_config.offsetFilterSignResetMinDeg < 0.0) {
            m_config.offsetFilterSignResetMinDeg = 0.006;
        }
        if (!std::isfinite(m_config.settleDeadbandDeg) || m_config.settleDeadbandDeg < 0.0)
            m_config.settleDeadbandDeg = 0.002;
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

    bool zeroHeld() const
    {
        return m_zeroHeld;
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
        m_zeroHeld = false;
        m_filterInitialized = false;
        m_filteredOffsetDeg = 0.0;
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

        double filteredOffset = requestedOffsetDeg;
        if (m_config.offsetFilterTimeConstantSec > 0.0) {
            if (!m_filterInitialized) {
                m_filteredOffsetDeg = requestedOffsetDeg;
                m_filterInitialized = true;
            } else if (std::abs(requestedOffsetDeg)
                       + m_config.offsetFilterFastResetDeltaDeg
                       < std::abs(m_filteredOffsetDeg)) {
                // 大偏差突然回到零附近时必须快速释放滤波状态，否则旧的大
                // 偏差会继续推动转台，造成明显过冲。普通小噪声不会触发。
                m_filteredOffsetDeg = requestedOffsetDeg;
            } else if (m_config.offsetFilterResetOnSignChange
                       && requestedOffsetDeg * m_filteredOffsetDeg < 0.0
                       && std::max(std::abs(requestedOffsetDeg),
                                   std::abs(m_filteredOffsetDeg))
                           > m_config.offsetFilterSignResetMinDeg) {
                // 目标越过视轴时立即释放旧的符号，避免滤波滞后继续把转台
                // 推过目标。只有超过独立门限的符号变化才触发，防止零点附近
                // 的 ±0.002° 量化噪声反复绕过滤波器。
                m_filteredOffsetDeg = requestedOffsetDeg;
            } else {
                const double innovation =
                        std::abs(requestedOffsetDeg - m_filteredOffsetDeg);
                const double timeConstant =
                        innovation > m_config.offsetFilterFastSlewThresholdDeg
                        ? m_config.offsetFilterFastTimeConstantSec
                        : m_config.offsetFilterTimeConstantSec;
                const double alpha = timeConstant > 0.0
                        ? 1.0 - std::exp(-dtSec / timeConstant)
                        : 1.0;
                m_filteredOffsetDeg +=
                        alpha * (requestedOffsetDeg - m_filteredOffsetDeg);
            }
            filteredOffset = m_filteredOffsetDeg;
        }

        const double absOffset = std::abs(filteredOffset);
        if (m_zeroHeld) {
            if (absOffset < m_config.zeroDeadbandReleaseDeg) {
                m_ramping = false;
                m_velocityDegPerSec = 0.0;
                return currentResult();
            }
            m_zeroHeld = false;
        } else if (absOffset <= m_config.zeroDeadbandDeg) {
            // 零点死区内保持上一有效目标。不要改成当前反馈角，否则转台会
            // 先反向回追反馈，再从另一侧重新启动，形成更大的往复摆动。
            m_zeroHeld = true;
            m_ramping = false;
            m_velocityDegPerSec = 0.0;
            return currentResult();
        }

        m_ramping = true;

        // 软死区保证恢复跟踪时从零连续增加，不会直接跳到完整偏差。
        const double effectiveOffset =
                filteredOffset
                - std::copysign(m_config.zeroDeadbandDeg, filteredOffset);
        const double desiredPosition = feedbackDeg + effectiveOffset;
        const double error = desiredPosition - m_positionDeg;

        // 近目标采用低速、低加速度；越过近目标半径后连续过渡到远目标能力。
        double speedLimit = m_config.maxSpeedDegPerSec;
        double accelerationLimit = m_config.maxAccelerationDegPerSec2;
        if (absOffset < m_config.farTrackRadiusDeg) {
            double blend = 0.0;
            if (absOffset <= m_config.fineTrackRadiusDeg) {
                speedLimit = m_config.fineMaxSpeedDegPerSec;
                accelerationLimit = m_config.fineMaxAccelerationDegPerSec2;
            } else if (absOffset <= m_config.nearTrackRadiusDeg) {
                blend = (absOffset - m_config.fineTrackRadiusDeg)
                        / (m_config.nearTrackRadiusDeg
                           - m_config.fineTrackRadiusDeg);
                blend = std::max(0.0, std::min(1.0, blend));
                speedLimit = m_config.fineMaxSpeedDegPerSec
                        + blend * (m_config.nearMaxSpeedDegPerSec
                                   - m_config.fineMaxSpeedDegPerSec);
                accelerationLimit = m_config.fineMaxAccelerationDegPerSec2
                        + blend * (m_config.nearMaxAccelerationDegPerSec2
                                   - m_config.fineMaxAccelerationDegPerSec2);
            } else {
                blend = (absOffset - m_config.nearTrackRadiusDeg)
                        / (m_config.farTrackRadiusDeg
                           - m_config.nearTrackRadiusDeg);
                blend = std::max(0.0, std::min(1.0, blend));
                speedLimit = m_config.nearMaxSpeedDegPerSec
                        + blend * (m_config.maxSpeedDegPerSec
                                   - m_config.nearMaxSpeedDegPerSec);
                accelerationLimit = m_config.nearMaxAccelerationDegPerSec2
                        + blend * (m_config.maxAccelerationDegPerSec2
                                   - m_config.nearMaxAccelerationDegPerSec2);
            }
        }
        const double maxVelocityStep = accelerationLimit * dtSec;

        // 先按剩余距离计算允许的制动速度，再叠加加速度约束。这样接近目标
        // 时会提前减速，而不是等到越过目标才由位置钳位强制停车。
        const double stopLimitedVelocity = std::sqrt(
                    2.0 * accelerationLimit * std::abs(error));
        const double velocityMagnitude = std::min(
                    speedLimit,
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

        if (std::abs(desiredPosition - m_positionDeg)
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
        result.zeroHeld = m_zeroHeld;
        return result;
    }

    Config m_config;
    bool m_initialized = false;
    bool m_ramping = false;
    bool m_zeroHeld = false;
    bool m_filterInitialized = false;
    double m_filteredOffsetDeg = 0.0;
    double m_positionDeg = 0.0;
    double m_velocityDegPerSec = 0.0;
};

#endif // TRACKING_TARGET_SMOOTHER_H
