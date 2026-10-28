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

void testSmallOffsetUsesLowSpeedTracking()
{
    TrackingTargetSmoother smoother;
    smoother.reset(12.0);
    const auto result = smoother.update(12.0, 0.5, 0.02);
    check(std::abs(result.positionDeg - 12.0) <= 1.0 * 0.02 + 1e-6,
          "small offset should use near-target speed limit");
    check(result.ramping, "small nonzero offset should use continuous tracking");
}

void testLargeOffsetHonorsSpeedAndAcceleration()
{
    TrackingTargetSmoother::Config config;
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
        if (result.ramping && std::abs(offset) > 1.0) {
            check(velocityStep <=
                      config.maxAccelerationDegPerSec2 * dt + 1e-6,
                  "large offset exceeded maximum tracking acceleration");
        }
        sawRamp = sawRamp || result.ramping;
        previousPosition = result.positionDeg;
        previousAverageVelocity = averageVelocity;
    }

    check(sawRamp, "large offset should enter ramp mode");
    check(std::abs(previousPosition - targetAbs) <=
              config.zeroDeadbandDeg + 1e-6,
          "large offset should converge into the soft deadband");
    check(!smoother.ramping() && smoother.zeroHeld(),
          "large offset should stop ramping inside the zero-hold band");
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
    check(std::abs(reversed.positionDeg - beforeReversal)
              <= smoother.config().maxSpeedDegPerSec * 0.02 + 1e-6,
          "target reversal should remain slew limited");
}

void testPreviewGeneratesOneSecondRamp()
{
    TrackingTargetSmoother smoother;
    const std::array<double, 4> offsets = {{20.0, 20.0, 20.0, 20.0}};
    const auto positions = smoother.preview(0.0, offsets, 0.25);

    check(positions[0] > 0.0 && positions[0] < positions[3],
          "preview should generate a continuous trajectory");
    double previous = 0.0;
    double previousVelocity = 0.0;
    for (double position : positions) {
        const double velocity = (position - previous) / 0.25;
        check(std::abs(velocity - previousVelocity)
                  <= smoother.config().maxAccelerationDegPerSec2 * 0.25 + 1e-6,
              "preview point exceeded maximum acceleration");
        check(std::abs(position - previous)
                  <= smoother.config().maxSpeedDegPerSec * 0.25 + 1e-6,
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

void testNearZeroHoldsAgainstSensorQuantization()
{
    TrackingTargetSmoother::Config config;
    config.zeroDeadbandDeg = 0.04;
    TrackingTargetSmoother smoother(config);
    smoother.reset(37.0);

    const auto entered = smoother.update(37.0, 0.02, 0.02);
    check(entered.zeroHeld, "small offset should enter soft deadband");
    check(closeTo(entered.positionDeg, 37.0),
          "soft deadband should hold the last valid target");

    // 导引头在零位附近按 0.02° 分辨率跳动，转台目标必须保持不动。
    const double jitter[] = {0.04, 0.02, 0.00, -0.02, 0.02, 0.04};
    const double feedbackJitter[] = {37.0001, 36.9999, 37.0002,
                                     37.0000, 37.0001, 36.9998};
    for (int i = 0; i < 6; ++i) {
        const auto held = smoother.update(feedbackJitter[i], jitter[i], 0.02);
        check(held.zeroHeld, "small jitter should remain in soft deadband");
        check(closeTo(held.positionDeg, 37.0),
              "sensor quantization must not move the held target");
    }

    TrackingTargetSmoother::StepResult resumed;
    for (int i = 0; i < 20 && smoother.zeroHeld(); ++i)
        resumed = smoother.update(37.0001, 0.09, 0.02);
    check(!smoother.zeroHeld(),
          "persistent offset above deadband should resume soft tracking");
    check(std::abs(resumed.positionDeg - 37.0)
              <= smoother.config().nearMaxSpeedDegPerSec * 0.02 * 20 + 1e-6,
          "soft-deadband exit must remain low-speed limited");
}

void testZeroHoldEntryRemainsSlewLimited()
{
    TrackingTargetSmoother smoother;
    smoother.reset(0.0);

    double beforeEntry = 0.0;
    for (int i = 0; i < 20; ++i) {
        const auto ramping = smoother.update(0.0, 20.0, 0.02);
        beforeEntry = ramping.positionDeg;
    }

    // 突然进入导引头零点区时只冻结上一有效目标，不能回追当前反馈。
    const auto entered = smoother.update(0.0, 0.02, 0.02);
    check(entered.zeroHeld, "small offset should enter soft deadband");
    check(closeTo(entered.positionDeg, beforeEntry),
          "deadband entry must not command a reverse correction");
}

} // namespace

int main()
{
    testSmallOffsetUsesLowSpeedTracking();
    testLargeOffsetHonorsSpeedAndAcceleration();
    testMovingTargetReversesWithoutJump();
    testPreviewGeneratesOneSecondRamp();
    testInvalidMeasurementHoldsLastTarget();
    testNearZeroHoldsAgainstSensorQuantization();
    testZeroHoldEntryRemainsSlewLimited();

    if (g_failures == 0) {
        std::printf("tracking_target_smoother_selftest: PASS\n");
        return 0;
    }
    std::printf("tracking_target_smoother_selftest: %d failure(s)\n", g_failures);
    return 1;
}
