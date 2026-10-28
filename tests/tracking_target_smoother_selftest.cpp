#include "../utils/tracking_target_smoother.h"

#include <cmath>
#include <cstdio>

namespace {

constexpr double kTolerance = 1e-6;

bool closeTo(double actual, double expected, double tolerance = kTolerance)
{
    return std::abs(actual - expected) <= tolerance;
}

int g_failures = 0;

void check(bool condition, const char *message)
{
    if (condition)
        return;
    std::printf("FAIL: %s\n", message);
    ++g_failures;
}

void testSmallOffsetPassesThrough()
{
    TrackingTargetSmoother smoother;
    smoother.reset(12.0);
    const auto result = smoother.update(12.0, 1.0, 0.02);
    check(closeTo(result.positionDeg, 13.0), "small offset should follow directly");
    check(!result.ramping, "small offset should not enter ramp mode");
}

void testLargeOffsetHonorsSpeedAndAcceleration()
{
    TrackingTargetSmoother::Config config;
    config.rampEnterOffsetDeg = 3.0;
    config.rampExitOffsetDeg = 2.0;
    config.maxSpeedDegPerSec = 10.0;
    config.maxAccelerationDegPerSec2 = 10.0;
    TrackingTargetSmoother smoother(config);
    smoother.reset(0.0);

    constexpr double dt = 0.02;
    constexpr double targetAbs = 20.0;
    double previousPosition = 0.0;
    double previousAverageVelocity = 0.0;
    bool sawRamp = false;

    for (int i = 0; i < 400; ++i) {
        // 固定世界目标、转台持续跟随：反馈角变化后，导引头相对偏差同步缩小。
        const double feedback = previousPosition;
        const double offset = targetAbs - feedback;
        const auto result = smoother.update(feedback, offset, dt);
        const double positionStep = std::abs(result.positionDeg - previousPosition);
        const double averageVelocity = positionStep / dt;
        const double velocityStep =
                std::abs(averageVelocity - previousAverageVelocity);

        check(positionStep <= config.maxSpeedDegPerSec * dt + 1e-6,
              "large offset exceeded maximum tracking speed");
        if (result.ramping) {
            check(velocityStep <=
                      config.maxAccelerationDegPerSec2 * dt + 1e-6,
                  "large offset exceeded maximum tracking acceleration");
        }
        sawRamp = sawRamp || result.ramping;
        previousPosition = result.positionDeg;
        previousAverageVelocity = averageVelocity;
    }

    check(sawRamp, "large offset should enter ramp mode");
    check(closeTo(previousPosition, targetAbs, 1e-4),
          "large offset should converge to target");
    check(!smoother.ramping(),
          "large offset should leave ramp mode after settling");
}

void testMovingTargetReversesWithoutJump()
{
    TrackingTargetSmoother smoother;
    smoother.reset(0.0);

    double feedback = 0.0;
    for (int i = 0; i < 80; ++i) {
        const auto result = smoother.update(feedback, 15.0 - feedback, 0.02);
        feedback = result.positionDeg;
    }
    const double beforeReversal = feedback;

    // 目标从绝对 15° 突然改到 2°。先求出正确保留符号的相对偏差，
    // 再确认第一拍仍受速度和加速度限制。
    const double offsetAfterChange = 2.0 - feedback;
    const auto reversed = smoother.update(feedback, offsetAfterChange, 0.02);
    check(std::abs(reversed.positionDeg - beforeReversal) <= 0.2 + 1e-6,
          "target reversal should remain slew limited");
}

void testPreviewGeneratesOneSecondRamp()
{
    TrackingTargetSmoother smoother;
    const std::array<double, 4> offsets = {{20.0, 20.0, 20.0, 20.0}};
    const auto positions = smoother.preview(0.0, offsets, 0.25);

    check(closeTo(positions[0], 0.625, 1e-4),
          "preview first point should begin the acceleration ramp");
    double previous = 0.0;
    double previousVelocity = 0.0;
    for (double position : positions) {
        const double velocity = (position - previous) / 0.25;
        check(std::abs(velocity - previousVelocity) <= 10.0 * 0.25 + 1e-6,
              "preview point exceeded maximum acceleration");
        check(std::abs(position - previous) <= 10.0 * 0.25 + 1e-6,
              "preview point exceeded maximum speed");
        previous = position;
        previousVelocity = velocity;
    }
}

void testInvalidMeasurementHoldsLastTarget()
{
    TrackingTargetSmoother smoother;
    smoother.reset(4.0);
    const auto result = smoother.update(4.0, NAN, 0.02);
    check(closeTo(result.positionDeg, 4.0),
          "non-finite seeker offset should hold last target");
}

void testRampStartDoesNotInheritDirectModeVelocity()
{
    TrackingTargetSmoother smoother;
    smoother.reset(0.0);

    // 小偏差直通时若反馈角跳变，内部观测速度会很大；紧接着进入斜坡时
    // 必须从零速度重新加速。
    smoother.update(100.0, 0.5, 0.02);
    const auto result = smoother.update(100.5, 20.0, 0.02);
    check(result.ramping, "large offset should enter ramp mode");
    check(std::abs(result.positionDeg - 100.5) <= 10.0 * 0.02 * 0.02 + 1e-6,
          "ramp start must not inherit direct-mode velocity");
}

} // namespace

int main()
{
    testSmallOffsetPassesThrough();
    testLargeOffsetHonorsSpeedAndAcceleration();
    testMovingTargetReversesWithoutJump();
    testPreviewGeneratesOneSecondRamp();
    testInvalidMeasurementHoldsLastTarget();
    testRampStartDoesNotInheritDirectModeVelocity();

    if (g_failures == 0) {
        std::printf("tracking_target_smoother_selftest: PASS\n");
        return 0;
    }
    std::printf("tracking_target_smoother_selftest: %d failure(s)\n", g_failures);
    return 1;
}
