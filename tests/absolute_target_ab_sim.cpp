#include "../kalman/AlphaBetaTracker.h"

#include <cmath>
#include <cstdio>

namespace {

constexpr double kDtSec = 0.02;
constexpr double kDeltaTimeMs = 20.0;
constexpr double kTargetOuterDeg = 10.0;
constexpr double kServoTauSec = 0.08;

int gFailures = 0;

void check(bool condition, const char *message)
{
    if (condition)
        return;
    std::printf("FAIL: %s\n", message);
    ++gFailures;
}

} // namespace

int main()
{
    ABTrackManager relativeTracker(SeekerType::Image);
    ABTrackManager absoluteTracker(SeekerType::Image);
    relativeTracker.AzTracker().setClampRange(-100.0, 100.0);
    absoluteTracker.AzTracker().setClampRange(-100.0, 100.0);

    double turntableDeg = 0.0;
    relativeTracker.Init(kTargetOuterDeg - turntableDeg, 0.0, 0);
    absoluteTracker.Init(kTargetOuterDeg, 0.0, 0);

    for (int step = 1; step <= 20; ++step) {
        const double t = step * kDtSec;
        const double target = kTargetOuterDeg;
        const double relative = target - turntableDeg;
        const qint64 timestampMs =
                static_cast<qint64>(std::round(t * 1000.0));

        relativeTracker.FeedData(timestampMs, relative, 0.0);
        absoluteTracker.FeedData(timestampMs, target, 0.0);

        // 目标静止，转台按一阶伺服接近目标。
        const double plantAlpha =
                1.0 - std::exp(-kDtSec / kServoTauSec);
        turntableDeg += (target - turntableDeg) * plantAlpha;
    }

    const double relativePrediction =
            relativeTracker.GenAxisPacket(true, 0, 1.0).angle1;
    const double relativeCommand =
            turntableDeg + relativePrediction;
    const double absoluteCommand =
            absoluteTracker.GenAxisPacket(true, 0, 1.0).angle1;

    std::printf(
        "static 10deg target: turntable=%.4f, "
        "relative+feedback=%.4f, absoluteAB=%.4f\n",
        turntableDeg, relativeCommand, absoluteCommand);

    check(std::abs(absoluteCommand - kTargetOuterDeg) < 0.05,
          "absolute-target AB should remain on the stationary target");
    check(std::abs(relativeCommand - kTargetOuterDeg)
              > std::abs(absoluteCommand - kTargetOuterDeg),
          "relative-error AB should be worse than absolute-target AB");

    if (gFailures == 0) {
        std::printf("absolute_target_ab_sim: PASS\n");
        return 0;
    }
    std::printf("absolute_target_ab_sim: %d failure(s)\n", gFailures);
    return 1;
}
