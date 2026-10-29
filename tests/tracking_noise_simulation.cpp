#include "../utils/tracking_target_smoother.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <random>
#include <vector>

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kDt = 0.02;
constexpr double kSensorLsb = 0.002;
constexpr double kServoTauSec = 0.08;

struct Metrics {
    double commandRangeDeg = 0.0;
    int commandReversals = 0;
    double finalErrorDeg = 0.0;
    double maxOvershootDeg = 0.0;
    double settleTimeSec = -1.0;
    double trackingRmsDeg = 0.0;
    double maxCommandStepDeg = 0.0;
    double commandStepRmsDeg = 0.0;
};

double quantize(double value)
{
    return std::round(value / kSensorLsb) * kSensorLsb;
}

double measurementNoise(std::mt19937 &rng)
{
    std::uniform_real_distribution<double> distribution(-0.002, 0.002);
    return distribution(rng);
}

Metrics simulate(double nearSpeedDegPerSec,
                 int segment,
                 bool directBaseline = false,
                 double nearAccelerationDegPerSec2 = 3.0,
                 double inputFilterTauSec = 0.04,
                 bool useMedian3 = false,
                 double zeroDeadbandDeg = 0.004,
                 double maxAccelerationDegPerSec2 = 10.0,
                 double zeroDeadbandReleaseDeg = 0.006,
                 double filterFastSlewThresholdDeg = 0.03)
{
    TrackingTargetSmoother::Config config;
    config.zeroDeadbandDeg = zeroDeadbandDeg;
    config.zeroDeadbandReleaseDeg = zeroDeadbandReleaseDeg;
    config.nearTrackRadiusDeg = 1.0;
    config.nearMaxSpeedDegPerSec = nearSpeedDegPerSec;
    config.nearMaxAccelerationDegPerSec2 = nearAccelerationDegPerSec2;
    config.farTrackRadiusDeg = 3.0;
    config.offsetFilterTimeConstantSec = inputFilterTauSec;
    config.offsetFilterFastSlewThresholdDeg = filterFastSlewThresholdDeg;
    config.maxSpeedDegPerSec = 12.0;
    config.maxAccelerationDegPerSec2 = maxAccelerationDegPerSec2;
    config.settleDeadbandDeg = 0.01;

    TrackingTargetSmoother smoother(config);
    std::mt19937 rng(20261028u + segment);
    const double plantAlpha = 1.0 - std::exp(-kDt / kServoTauSec);

    double target = 0.0;
    double actual = 0.0;
    if (segment == 2)
        actual = 5.0;
    smoother.reset(actual);

    double previousCommand = actual;
    int previousDirection = 0;
    int reversals = 0;
    double minCommand = previousCommand;
    double maxCommand = previousCommand;
    double maxStep = 0.0;
    double commandStepSq = 0.0;
    double errorSq = 0.0;
    double overshoot = 0.0;
    int samples = 0;
    int settleSamples = 0;
    double settleTimeSec = -1.0;
    std::array<double, 3> medianHistory = {{0.0, 0.0, 0.0}};
    int medianCount = 0;

    const int totalSteps = segment == 2 ? 1500 : 500;
    for (int step = 0; step < totalSteps; ++step) {
        const double timeSec = step * kDt;
        if (segment == 1)
            target = timeSec >= 1.0 ? 10.0 : 0.0;
        else if (segment == 2)
            target = 5.0 + std::sin(2.0 * kPi * 0.2 * timeSec);

        double measuredOffset =
                quantize(target - actual + measurementNoise(rng));
        if (useMedian3) {
            medianHistory[0] = medianHistory[1];
            medianHistory[1] = medianHistory[2];
            medianHistory[2] = measuredOffset;
            ++medianCount;
            if (medianCount >= 3) {
                std::array<double, 3> sorted = medianHistory;
                std::sort(sorted.begin(), sorted.end());
                measuredOffset = sorted[1];
            }
        }

        double command = actual + measuredOffset;
        if (!directBaseline) {
            command = smoother.update(actual, measuredOffset, kDt).positionDeg;
        }

        const double delta = command - previousCommand;
        maxStep = std::max(maxStep, std::abs(delta));
        commandStepSq += delta * delta;
        if (std::abs(delta) > 1e-9) {
            const int direction = delta > 0.0 ? 1 : -1;
            if (previousDirection != 0 && direction != previousDirection)
                ++reversals;
            previousDirection = direction;
        }
        minCommand = std::min(minCommand, command);
        maxCommand = std::max(maxCommand, command);
        previousCommand = command;

        actual += (command - actual) * plantAlpha;

        if (segment == 0 && timeSec >= 1.0) {
            errorSq += (target - actual) * (target - actual);
            ++samples;
        } else if (segment == 1 && timeSec >= 1.0) {
            errorSq += (target - actual) * (target - actual);
            overshoot = std::max(overshoot, actual - target);
            if (std::abs(target - actual) <= 0.05) {
                ++settleSamples;
                if (settleTimeSec < 0.0 && settleSamples >= 50)
                    settleTimeSec = timeSec - 1.0;
            } else {
                settleSamples = 0;
            }
        } else if (segment == 2 && timeSec >= 5.0) {
            errorSq += (target - actual) * (target - actual);
            ++samples;
        }
    }

    Metrics metrics;
    metrics.commandRangeDeg = maxCommand - minCommand;
    metrics.commandReversals = reversals;
    metrics.settleTimeSec = settleTimeSec;
    metrics.finalErrorDeg = target - actual;
    metrics.maxOvershootDeg = std::max(0.0, overshoot);
    metrics.trackingRmsDeg =
            samples > 0 ? std::sqrt(errorSq / samples) : 0.0;
    metrics.maxCommandStepDeg = maxStep;
    metrics.commandStepRmsDeg =
            totalSteps > 0 ? std::sqrt(commandStepSq / totalSteps) : 0.0;
    return metrics;
}

int countReversals(double nearSpeedDegPerSec)
{
    TrackingTargetSmoother::Config config;
    config.nearMaxSpeedDegPerSec = nearSpeedDegPerSec;
    TrackingTargetSmoother smoother(config);
    std::mt19937 rng(20261028u);

    double actual = 0.0;
    double previousCommand = actual;
    int previousDirection = 0;
    int reversals = 0;
    smoother.reset(actual);

    for (int step = 0; step < 500; ++step) {
        const double measuredOffset = quantize(measurementNoise(rng));
        const double command =
                smoother.update(actual, measuredOffset, kDt).positionDeg;
        const double delta = command - previousCommand;
        if (std::abs(delta) > 1e-9) {
            const int direction = delta > 0.0 ? 1 : -1;
            if (previousDirection != 0 && direction != previousDirection)
                ++reversals;
            previousDirection = direction;
        }
        previousCommand = command;
        actual += (command - actual)
                * (1.0 - std::exp(-kDt / kServoTauSec));
    }
    return reversals;
}

void printMetrics(const char *name, const Metrics &metrics)
{
    std::printf(
        "%-24s commandRange=%7.4f deg, finalError=%7.4f deg, "
        "overshoot=%7.4f deg, settle=%6.2f s, trackingRms=%7.4f deg, "
        "maxCmdStep=%7.4f deg, stepRms=%7.4f deg, reversals=%d\n",
        name, metrics.commandRangeDeg, metrics.finalErrorDeg,
        metrics.maxOvershootDeg, metrics.settleTimeSec,
        metrics.trackingRmsDeg, metrics.maxCommandStepDeg,
        metrics.commandStepRmsDeg, metrics.commandReversals);
}

} // namespace

int main()
{
    std::printf("Tracking simulation: sensor=0.002 deg, noise=+-0.002 deg, "
                "dt=20 ms, servo tau=%.2f s\n", kServoTauSec);

    const Metrics baseline = simulate(1.5, 0, true, 4.0, 0.0);
    const Metrics staticNoise = simulate(1.5, 0, false);
    std::printf("Static target reversals: direct=%d, smoother=%d\n",
                baseline.commandReversals, countReversals(1.5));
    printMetrics("static noise (direct)", baseline);
    printMetrics("static noise (smoother)", staticNoise);

    const Metrics step = simulate(1.5, 1, false);
    printMetrics("10 deg acquisition", step);

    for (double filterTau : {0.0, 0.02, 0.04, 0.06, 0.08}) {
        char name[64];
        std::snprintf(name, sizeof(name), "10 deg filterTau %.2fs", filterTau);
        printMetrics(name, simulate(1.5, 1, false, 4.0, filterTau));
    }

    for (double nearSpeed : {0.5, 1.0, 1.5, 2.0}) {
        char name[64];
        std::snprintf(name, sizeof(name), "sine 0.2Hz +-1deg @%.1fdeg/s",
                      nearSpeed);
        printMetrics(name, simulate(nearSpeed, 2, false));
    }

    Metrics tuned;
    for (double nearAcceleration : {1.0, 2.0, 3.0, 4.0}) {
        char name[64];
        std::snprintf(name, sizeof(name), "sine 0.2Hz accel %.1fdeg/s2",
                      nearAcceleration);
        const Metrics metrics =
                simulate(1.5, 2, false, nearAcceleration);
        printMetrics(name, metrics);
        if (std::abs(nearAcceleration - 3.0) < 1e-9)
            tuned = metrics;
    }

    for (double filterTau : {0.0, 0.04, 0.06, 0.08, 0.12}) {
        char name[64];
        std::snprintf(name, sizeof(name), "sine 0.2Hz filterTau %.2fs",
                      filterTau);
        const Metrics metrics =
                simulate(1.5, 2, false, 4.0, filterTau);
        printMetrics(name, metrics);
    }

    for (double filterTau : {0.0, 0.04, 0.08}) {
        char name[64];
        std::snprintf(name, sizeof(name), "sine median3 filterTau %.2fs",
                      filterTau);
        const Metrics metrics =
                simulate(1.5, 2, false, 4.0, filterTau, true);
        printMetrics(name, metrics);
    }

    for (double deadband : {0.02, 0.04, 0.06, 0.08, 0.10}) {
        char name[64];
        std::snprintf(name, sizeof(name), "10 deg deadband %.2fdeg",
                      deadband);
        printMetrics(name, simulate(1.5, 1, false, 4.0, 0.08, false, deadband));

        std::snprintf(name, sizeof(name), "sine deadband %.2fdeg", deadband);
        printMetrics(name, simulate(1.5, 2, false, 4.0, 0.08, false, deadband));
    }

    for (double maxAcceleration : {4.0, 6.0, 8.0}) {
        char name[64];
        std::snprintf(name, sizeof(name), "10 deg maxAccel %.1f",
                      maxAcceleration);
        printMetrics(name, simulate(1.5, 1, false, 4.0, 0.08,
                                    false, 0.04, maxAcceleration));
        std::snprintf(name, sizeof(name), "sine maxAccel %.1f",
                      maxAcceleration);
        printMetrics(name, simulate(1.5, 2, false, 4.0, 0.08,
                                    false, 0.04, maxAcceleration));
    }

    struct DeadbandCase {
        double enter;
        double release;
        double filterTau;
    };
    const DeadbandCase deadbandCases[] = {
        {0.04, 0.10, 0.08},
        {0.01, 0.04, 0.08},
        {0.01, 0.04, 0.12},
        {0.01, 0.06, 0.12},
        {0.01, 0.08, 0.12},
        {0.01, 0.10, 0.12},
        {0.02, 0.06, 0.12}
    };
    for (const DeadbandCase &item : deadbandCases) {
        char name[64];
        std::snprintf(name, sizeof(name),
                      "case %.2f/%.2f/tau%.2f static",
                      item.enter, item.release, item.filterTau);
        printMetrics(name, simulate(1.5, 0, false, 4.0,
                                    item.filterTau, false, item.enter,
                                    6.0, item.release));

        std::snprintf(name, sizeof(name),
                      "case %.2f/%.2f/tau%.2f sine",
                      item.enter, item.release, item.filterTau);
        printMetrics(name, simulate(1.5, 2, false, 4.0,
                                    item.filterTau, false, item.enter,
                                    6.0, item.release));
    }

    struct ApproachCase {
        const char *name;
        double filterTau;
        double fastThreshold;
        double release;
        double maxAcceleration;
    };
    const ApproachCase approachCases[] = {
        {"approach current", 0.10, 0.15, 0.010, 10.0},
        {"approach fast02", 0.10, 0.02, 0.010, 10.0},
        {"approach fast03", 0.10, 0.03, 0.010, 10.0},
        {"approach fastNear10", 0.10, 0.05, 0.010, 10.0},
        {"approach tau04", 0.04, 0.03, 0.006, 10.0},
        {"approach tau06_10", 0.06, 0.05, 0.010, 10.0},
        {"approach tau06_rel06", 0.06, 0.05, 0.006, 10.0},
        {"approach tau06_fast03", 0.06, 0.03, 0.010, 10.0},
        {"approach maxAcc6", 0.10, 0.15, 0.010, 6.0},
        {"approach fastNear", 0.10, 0.05, 0.010, 6.0},
        {"approach tau06", 0.06, 0.05, 0.010, 6.0},
        {"approach release06", 0.10, 0.05, 0.006, 6.0}
    };
    for (const ApproachCase &item : approachCases) {
        printMetrics(item.name,
                     simulate(1.5, 1, false, 3.0, item.filterTau,
                              false, 0.004, item.maxAcceleration,
                              item.release, item.fastThreshold));
        std::printf("  sine check: ");
        printMetrics(item.name,
                     simulate(1.5, 2, false, 3.0, item.filterTau,
                              false, 0.004, item.maxAcceleration,
                              item.release, item.fastThreshold));
    }

    struct JitterCase {
        const char *name;
        double nearSpeed;
        double nearAcceleration;
        double filterTau;
        double fastThreshold;
        double enter;
        double release;
    };
    const JitterCase jitterCases[] = {
        {"jitter current", 1.5, 3.0, 0.06, 0.05, 0.004, 0.006},
        {"jitter hold06", 1.5, 3.0, 0.06, 0.05, 0.006, 0.012},
        {"jitter hold08", 1.5, 3.0, 0.06, 0.05, 0.008, 0.014},
        {"jitter accel2", 1.5, 2.0, 0.06, 0.05, 0.004, 0.006},
        {"jitter slow12", 1.2, 2.5, 0.06, 0.05, 0.004, 0.006},
        {"jitter slowHold", 1.2, 2.5, 0.06, 0.05, 0.006, 0.012},
        {"jitter soft1", 1.0, 2.0, 0.08, 0.10, 0.006, 0.012},
        {"jitter soft2", 1.2, 2.5, 0.08, 0.08, 0.006, 0.012},
        {"jitter soft3", 1.0, 2.0, 0.08, 0.08, 0.004, 0.010},
        {"jitter soft4", 1.0, 2.0, 0.10, 0.15, 0.006, 0.012}
    };
    for (const JitterCase &item : jitterCases) {
        char name[64];
        std::snprintf(name, sizeof(name), "%s static", item.name);
        printMetrics(name, simulate(item.nearSpeed, 0, false,
                                    item.nearAcceleration, item.filterTau,
                                    false, item.enter, 10.0, item.release,
                                    item.fastThreshold));
        std::snprintf(name, sizeof(name), "%s approach", item.name);
        printMetrics(name, simulate(item.nearSpeed, 1, false,
                                    item.nearAcceleration, item.filterTau,
                                    false, item.enter, 10.0, item.release,
                                    item.fastThreshold));
        std::snprintf(name, sizeof(name), "%s sine", item.name);
        printMetrics(name, simulate(item.nearSpeed, 2, false,
                                    item.nearAcceleration, item.filterTau,
                                    false, item.enter, 10.0, item.release,
                                    item.fastThreshold));
    }

    const bool passed =
            baseline.commandReversals > 0
            && staticNoise.commandReversals == 0
            && staticNoise.commandRangeDeg <= 1e-9
            && step.settleTimeSec > 0.0
            && step.settleTimeSec < 4.0
            && step.maxOvershootDeg < 0.1
            && std::abs(step.finalErrorDeg) < 0.1
            && tuned.trackingRmsDeg < 0.25
            && tuned.maxCommandStepDeg < 0.03;

    std::printf("tracking_noise_simulation: %s\n",
                passed ? "PASS" : "FAIL");
    return passed ? 0 : 1;
}
