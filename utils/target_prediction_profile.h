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

// 绝对目标与当前反馈的偏差越小，允许交给 AB 的预测速度越少。
// 分段而不是单一阈值，避免在目标停止位置附近发生预测开/关跳变。
inline double nearTargetVelocityBlend(double relativeDistanceDeg)
{
    if (!std::isfinite(relativeDistanceDeg) || relativeDistanceDeg <= 0.0)
        return 0.0;
    if (relativeDistanceDeg < 0.10)
        return 0.0;
    if (relativeDistanceDeg < 1.0) {
        const double t = (relativeDistanceDeg - 0.10) / 0.90;
        return 0.10 * smoothStep01(t);
    }
    if (relativeDistanceDeg < 3.0) {
        const double t = (relativeDistanceDeg - 1.0) / 2.0;
        return 0.10 + 0.90 * smoothStep01(t);
    }
    return 1.0;
}

} // namespace target_prediction

#endif // TARGET_PREDICTION_PROFILE_H
