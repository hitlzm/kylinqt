#ifndef TARGET_PREDICTION_PROFILE_H
#define TARGET_PREDICTION_PROFILE_H

#include <algorithm>
#include <cmath>

namespace target_prediction {

inline double smoothStep01(double value)
{
    value = std::max(0.0, std::min(1.0, value));
    return value * value * (3.0 - 2.0 * value);
}

// 根据目标自身速度决定预测前馈强度。
//
// 不能按“目标与转台反馈的距离”关断预测：正常跟踪时这个距离通常很小，
// 但对运动目标仍需要速度前馈。这里只抑制静止目标上 AB 的残余速度，
// 目标开始持续运动后快速恢复到完整预测。
inline double targetVelocityBlend(double targetVelocityDegPerSec)
{
    if (!std::isfinite(targetVelocityDegPerSec))
        return 0.0;

    const double speed = std::abs(targetVelocityDegPerSec);
    constexpr double kStopVelocityDegPerSec = 0.03;
    constexpr double kFullPredictionVelocityDegPerSec = 0.12;

    if (speed <= kStopVelocityDegPerSec)
        return 0.0;
    if (speed >= kFullPredictionVelocityDegPerSec)
        return 1.0;

    return smoothStep01(
                (speed - kStopVelocityDegPerSec)
                / (kFullPredictionVelocityDegPerSec
                   - kStopVelocityDegPerSec));
}

} // namespace target_prediction

#endif // TARGET_PREDICTION_PROFILE_H
