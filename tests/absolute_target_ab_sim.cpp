#include "../kalman/AlphaBetaTracker.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <random>

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

struct NoiseMetrics {
    double predictionMin = 0.0;
    double predictionMax = 0.0;
    double predictionRms = 0.0;
    double velocityRms = 0.0;
};

struct StopMetrics {
    double maxPostStopPredictionDeg = 0.0;
    double finalAngleErrorDeg = 0.0;
    double finalVelocityDegPerSec = 0.0;
};

struct StepMetrics {
    double maxPredictionDeg = 0.0;
    double finalAngleDeg = 0.0;
    double finalVelocityDegPerSec = 0.0;
};

NoiseMetrics evaluateStationaryNoise(double alpha, double beta, double speedLimit)
{
    AlphaBetaTracker tracker(alpha, beta, AngleMode::Clamp, -100.0, 100.0);
    tracker.setSpeedLimit(speedLimit);
    tracker.setInnovationGate(0.05, 0.20, 4.0, 0.5, 3);
    tracker.init(kTargetOuterDeg, 0);

    std::mt19937 rng(20261030u);
    std::uniform_real_distribution<double> noise(-0.002, 0.002);
    NoiseMetrics metrics;
    bool first = true;
    double predictionSq = 0.0;
    double velocitySq = 0.0;
    int count = 0;

    for (int step = 1; step <= 750; ++step) {
        const double measured = std::round(
                    (kTargetOuterDeg + noise(rng)) / 0.002) * 0.002;
        tracker.update(true, measured,
                       static_cast<qint64>(step * kDeltaTimeMs));
        if (step < 150)
            continue;

        const double prediction = tracker.predict(1.0);
        if (first) {
            metrics.predictionMin = prediction;
            metrics.predictionMax = prediction;
            first = false;
        } else {
            metrics.predictionMin = std::min(metrics.predictionMin, prediction);
            metrics.predictionMax = std::max(metrics.predictionMax, prediction);
        }
        predictionSq += prediction * prediction;
        velocitySq += tracker.velocity() * tracker.velocity();
        ++count;
    }

    metrics.predictionRms = std::sqrt(predictionSq / count);
    metrics.velocityRms = std::sqrt(velocitySq / count);
    return metrics;
}

StopMetrics evaluateStopResponse(double alpha, double beta, double speedLimit)
{
    AlphaBetaTracker tracker(alpha, beta, AngleMode::Clamp, -100.0, 100.0);
    tracker.setSpeedLimit(speedLimit);
    tracker.setInnovationGate(0.05, 0.20, 4.0, 0.5, 3);
    tracker.init(0.0, 0);

    std::mt19937 rng(20261031u);
    std::uniform_real_distribution<double> noise(-0.002, 0.002);
    StopMetrics metrics;

    for (int step = 1; step <= 500; ++step) {
        const double t = step * kDtSec;
        const double target = std::min(5.0, t * 1.0);
        const double measured = std::round((target + noise(rng)) / 0.002) * 0.002;
        tracker.update(true, measured,
                       static_cast<qint64>(step * kDeltaTimeMs));

        if (t > 5.0 && t <= 8.0) {
            metrics.maxPostStopPredictionDeg = std::max(
                        metrics.maxPostStopPredictionDeg,
                        tracker.predict(1.0));
        }
    }

    metrics.finalAngleErrorDeg =
            std::abs(tracker.angle() - 5.0);
    metrics.finalVelocityDegPerSec =
            std::abs(tracker.velocity());
    return metrics;
}

StepMetrics evaluateOneDegreeStep(double alpha, double beta, double speedLimit,
                                  double gateMin = 0.05,
                                  double gateMax = 0.20,
                                  int reacquireFrames = 3)
{
    AlphaBetaTracker tracker(alpha, beta, AngleMode::Clamp, -100.0, 100.0);
    tracker.setSpeedLimit(speedLimit);
    tracker.setInnovationGate(gateMin, gateMax, 4.0, 0.5, reacquireFrames);
    tracker.init(0.0, 0);

    std::mt19937 rng(20261101u);
    std::uniform_real_distribution<double> noise(-0.002, 0.002);
    StepMetrics metrics;

    for (int step = 1; step <= 450; ++step) {
        const double t = step * kDtSec;
        const double target = t >= 2.0 ? 1.0 : 0.0;
        const double measured = std::round((target + noise(rng)) / 0.002) * 0.002;
        tracker.update(true, measured,
                       static_cast<qint64>(step * kDeltaTimeMs));
        if (t >= 2.0) {
            metrics.maxPredictionDeg = std::max(
                        metrics.maxPredictionDeg, tracker.predict(1.0));
        }
    }

    metrics.finalAngleDeg = tracker.angle();
    metrics.finalVelocityDegPerSec = std::abs(tracker.velocity());
    return metrics;
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

    const NoiseMetrics defaultNoise = evaluateStationaryNoise(0.50, 0.15, 14.0);
    const NoiseMetrics cappedNoise = evaluateStationaryNoise(0.50, 0.05, 1.5);
    const NoiseMetrics fastAngleNoise = evaluateStationaryNoise(0.50, 0.05, 3.0);
    const NoiseMetrics midAngleNoise = evaluateStationaryNoise(0.40, 0.05, 3.0);
    const NoiseMetrics balancedNoise = evaluateStationaryNoise(0.35, 0.08, 4.0);
    const NoiseMetrics smoothNoise = evaluateStationaryNoise(0.30, 0.05, 3.0);
    const NoiseMetrics slowNoise = evaluateStationaryNoise(0.15, 0.01, 0.8);
    std::printf(
        "stationary noise prediction range: default=%.4f, "
        "alpha.50/beta.05/speed1.5=%.4f, "
        "alpha.50/beta.05=%.4f, alpha.40/beta.05=%.4f, "
        "alpha.35/beta.08=%.4f, alpha.30/beta.05=%.4f, "
        "alpha.15/beta.01=%.4f deg\n",
        defaultNoise.predictionMax - defaultNoise.predictionMin,
        cappedNoise.predictionMax - cappedNoise.predictionMin,
        fastAngleNoise.predictionMax - fastAngleNoise.predictionMin,
        midAngleNoise.predictionMax - midAngleNoise.predictionMin,
        balancedNoise.predictionMax - balancedNoise.predictionMin,
        smoothNoise.predictionMax - smoothNoise.predictionMin,
        slowNoise.predictionMax - slowNoise.predictionMin);
    std::printf(
        "stationary noise velocity RMS: default=%.4f, "
        "alpha.50/beta.05/speed1.5=%.4f, "
        "alpha.50/beta.05=%.4f, alpha.40/beta.05=%.4f, "
        "alpha.35/beta.08=%.4f, alpha.30/beta.05=%.4f, "
        "alpha.15/beta.01=%.4f deg/s\n",
        defaultNoise.velocityRms, cappedNoise.velocityRms,
        fastAngleNoise.velocityRms,
        midAngleNoise.velocityRms, balancedNoise.velocityRms,
        smoothNoise.velocityRms, slowNoise.velocityRms);

    check(smoothNoise.predictionMax - smoothNoise.predictionMin
              < defaultNoise.predictionMax - defaultNoise.predictionMin,
          "lower alpha/beta should reduce stationary prediction spread");
    check(slowNoise.predictionMax - slowNoise.predictionMin < 0.03,
          "selected slow AB parameters should keep prediction spread below 0.03 deg");
    check(slowNoise.velocityRms < 0.005,
          "selected slow AB parameters should keep velocity RMS below 0.005 deg/s");

    const StopMetrics defaultStop = evaluateStopResponse(0.50, 0.15, 14.0);
    const StopMetrics cappedStop = evaluateStopResponse(0.50, 0.05, 1.5);
    const StopMetrics fastAngleStop = evaluateStopResponse(0.50, 0.05, 3.0);
    const StopMetrics midAngleStop = evaluateStopResponse(0.40, 0.05, 3.0);
    const StopMetrics balancedStop = evaluateStopResponse(0.35, 0.08, 4.0);
    const StopMetrics smoothStop = evaluateStopResponse(0.30, 0.05, 3.0);
    const StopMetrics slowStop = evaluateStopResponse(0.15, 0.01, 0.8);
    std::printf(
        "post-stop max prediction: default=%.4f, "
        "alpha.50/beta.05/speed1.5=%.4f, "
        "alpha.50/beta.05=%.4f, alpha.40/beta.05=%.4f, "
        "alpha.35/beta.08=%.4f, alpha.30/beta.05=%.4f, "
        "alpha.15/beta.01=%.4f deg\n",
        defaultStop.maxPostStopPredictionDeg,
        cappedStop.maxPostStopPredictionDeg,
        fastAngleStop.maxPostStopPredictionDeg,
        midAngleStop.maxPostStopPredictionDeg,
        balancedStop.maxPostStopPredictionDeg,
        smoothStop.maxPostStopPredictionDeg,
        slowStop.maxPostStopPredictionDeg);
    std::printf(
        "post-stop velocity: default=%.4f, "
        "alpha.50/beta.05/speed1.5=%.4f, "
        "alpha.50/beta.05=%.4f, alpha.40/beta.05=%.4f, "
        "alpha.35/beta.08=%.4f, alpha.30/beta.05=%.4f, "
        "alpha.15/beta.01=%.4f deg/s\n",
        defaultStop.finalVelocityDegPerSec,
        cappedStop.finalVelocityDegPerSec,
        fastAngleStop.finalVelocityDegPerSec,
        midAngleStop.finalVelocityDegPerSec,
        balancedStop.finalVelocityDegPerSec,
        smoothStop.finalVelocityDegPerSec,
        slowStop.finalVelocityDegPerSec);

    const StepMetrics currentStep =
            evaluateOneDegreeStep(0.20, 0.03, 1.5);
    const StepMetrics relaxedStep =
            evaluateOneDegreeStep(0.15, 0.015, 1.0);
    const StepMetrics verySlowStep =
            evaluateOneDegreeStep(0.15, 0.010, 0.8);
    const StepMetrics selectedStep =
            evaluateOneDegreeStep(0.30, 0.03, 2.0, 0.10, 0.50, 3);
    std::printf(
        "1deg step max prediction: alpha.20/beta.03=%.4f, "
        "alpha.15/beta.015=%.4f, alpha.15/beta.010=%.4f, "
        "selected=%.4f deg\n",
        currentStep.maxPredictionDeg,
        relaxedStep.maxPredictionDeg,
        verySlowStep.maxPredictionDeg,
        selectedStep.maxPredictionDeg);
    std::printf(
        "1deg step final velocity: alpha.20/beta.03=%.4f, "
        "alpha.15/beta.015=%.4f, alpha.15/beta.010=%.4f, "
        "selected=%.4f deg/s\n",
        currentStep.finalVelocityDegPerSec,
        relaxedStep.finalVelocityDegPerSec,
        verySlowStep.finalVelocityDegPerSec,
        selectedStep.finalVelocityDegPerSec);
    check(selectedStep.finalAngleDeg > 0.99,
          "selected AB gate must eventually accept a persistent 1 deg target");

    if (gFailures == 0) {
        std::printf("absolute_target_ab_sim: PASS\n");
        return 0;
    }
    std::printf("absolute_target_ab_sim: %d failure(s)\n", gFailures);
    return 1;
}
