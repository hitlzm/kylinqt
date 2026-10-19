// Ex-guide rate/trajectory comparison.
//
// This harness compiles and calls the production implementations directly:
//   - kalman/AlphaBetaTracker.cpp
//   - opencv/pixelkalmantracker.cpp
//
// Angle input rates model the current serial paths:
//   laser: 100 Hz (m_filterTime += 10 ms)
//   image:  50 Hz (m_filterTime += 20 ms)
//
// Pixel rates model local detector frame rates (30 and 50 fps).  The pixel
// tracker is not part of the angle ex-guide packet path; it is included here
// only to characterize its position prediction independently.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

#include "kalman/AlphaBetaTracker.h"
#include "opencv/pixelkalmantracker.h"

namespace
{
constexpr double PI = 3.14159265358979323846;

std::mt19937 g_rng(20261017u);

double gauss(double sigma)
{
    if (sigma <= 0.0)
        return 0.0;
    std::normal_distribution<double> d(0.0, sigma);
    return d(g_rng);
}

struct Sample
{
    double t = 0.0;
    double truth = 0.0;
    double meas = 0.0;
    double estimate = 0.0;
    double velocity = 0.0;
    double error = 0.0;
    double pred[4] = {0.0, 0.0, 0.0, 0.0};
    int packet = 0;
};

struct Summary
{
    std::string filter;
    std::string scenario;
    double rate = 0.0;
    std::string unit;
    double rmse = 0.0;
    double mae = 0.0;
    double maxAbsError = 0.0;
    double tailStd = 0.0;
    double finalError = 0.0;
    double maxAbsOutput = 0.0;
    double maxAbsVelocity = 0.0;
    double predRmse[4] = {0.0, 0.0, 0.0, 0.0};
    double settleTime = -1.0;
    double outlierPeak = 0.0;
    int gateRejects = 0;
};

const double TAU[4] = {0.25, 0.50, 0.75, 1.00};

bool isPacketTime(double t, double dt)
{
    const double nearest = std::round(t);
    return nearest >= 1.0 && std::fabs(t - nearest) <= 0.51 * dt;
}

void saveTrace(const std::string &name, const std::vector<Sample> &trace)
{
    const std::string path = "out/rate_" + name + ".csv";
    FILE *fp = std::fopen(path.c_str(), "w");
    if (!fp)
        return;

    std::fprintf(fp, "t,truth,meas,estimate,velocity,error,p1,p2,p3,p4,packet\n");
    for (const Sample &s : trace)
    {
        std::fprintf(fp,
                     "%.6f,%.9f,%.9f,%.9f,%.9f,%.9f,"
                     "%.9f,%.9f,%.9f,%.9f,%d\n",
                     s.t, s.truth, s.meas, s.estimate, s.velocity, s.error,
                     s.pred[0], s.pred[1], s.pred[2], s.pred[3], s.packet);
    }
    std::fclose(fp);
}

Summary calculateSummary(const std::string &filter,
                         const std::string &scenario,
                         double rate,
                         const std::string &unit,
                         const std::vector<Sample> &trace,
                         double evalStart,
                         double settleStart = -1.0,
                         double settleBand = 0.0,
                         double outlierTime = -1.0)
{
    Summary s;
    s.filter = filter;
    s.scenario = scenario;
    s.rate = rate;
    s.unit = unit;

    std::vector<const Sample *> eval;
    for (const Sample &x : trace)
        if (x.t >= evalStart)
            eval.push_back(&x);
    if (eval.empty())
        return s;

    double sq = 0.0;
    for (const Sample *x : eval)
    {
        const double e = x->error;
        sq += e * e;
        s.maxAbsError = std::max(s.maxAbsError, std::fabs(e));
        s.maxAbsOutput = std::max(s.maxAbsOutput, std::fabs(x->estimate));
        s.maxAbsVelocity = std::max(s.maxAbsVelocity, std::fabs(x->velocity));
    }

    s.rmse = std::sqrt(sq / static_cast<double>(eval.size()));
    s.mae = 0.0;
    for (const Sample *x : eval)
        s.mae += std::fabs(x->error);
    s.mae /= static_cast<double>(eval.size());
    s.finalError = eval.back()->error;

    const size_t tailStart = eval.size() * 3 / 4;
    double tailSum = 0.0;
    double tailSq = 0.0;
    for (size_t i = tailStart; i < eval.size(); ++i)
    {
        tailSum += eval[i]->error;
        tailSq += eval[i]->error * eval[i]->error;
    }
    const double n = static_cast<double>(eval.size() - tailStart);
    const double tailMean = tailSum / n;
    s.tailStd = std::sqrt(std::max(0.0, tailSq / n - tailMean * tailMean));

    if (settleStart >= 0.0 && settleBand > 0.0)
    {
        size_t startIndex = 0;
        while (startIndex < trace.size() && trace[startIndex].t < settleStart)
            startIndex++;

        int lastOutside = -1;
        size_t windowBegin = startIndex;
        double windowSum = 0.0;
        for (size_t i = startIndex; i < trace.size(); ++i)
        {
            // Judge convergence from a 0.5 s moving average.  A raw noisy
            // sample can cross any narrow threshold even after the mean error
            // has reached zero.
            windowSum += trace[i].error;
            while (windowBegin < i && trace[i].t - trace[windowBegin].t > 0.5)
            {
                windowSum -= trace[windowBegin].error;
                windowBegin++;
            }
            const double avg = windowSum / static_cast<double>(i - windowBegin + 1);
            if (std::fabs(avg) > settleBand)
                lastOutside = static_cast<int>(i);
        }
        if (lastOutside < 0)
            s.settleTime = 0.0;
        else if (lastOutside + 1 < static_cast<int>(trace.size()))
            s.settleTime = trace[lastOutside + 1].t - settleStart;
        else
            s.settleTime = -1.0;
    }

    if (outlierTime >= 0.0)
    {
        for (const Sample &x : trace)
        {
            if (x.t >= outlierTime)
                s.outlierPeak = std::max(s.outlierPeak, std::fabs(x.error));
        }
    }

    return s;
}

void measureAnglePackets(const std::function<double(double)> &truth,
                         const std::vector<Sample> &trace,
                         double evalStart,
                         Summary &s)
{
    double sq[4] = {0.0, 0.0, 0.0, 0.0};
    int count = 0;
    for (const Sample &x : trace)
    {
        if (!x.packet || x.t < evalStart)
            continue;
        for (int h = 0; h < 4; ++h)
        {
            const double e = x.pred[h] - truth(x.t + TAU[h]);
            sq[h] += e * e;
        }
        count++;
    }
    if (count == 0)
        return;
    for (int h = 0; h < 4; ++h)
        s.predRmse[h] = std::sqrt(sq[h] / count);
}

void measurePixelPackets(const std::function<double(double)> &truth,
                         const std::vector<Sample> &trace,
                         double evalStart,
                         Summary &s)
{
    double sq[4] = {0.0, 0.0, 0.0, 0.0};
    int count = 0;
    for (const Sample &x : trace)
    {
        if (!x.packet || x.t < evalStart)
            continue;
        for (int h = 0; h < 4; ++h)
        {
            const double e = x.pred[h] - truth(x.t + TAU[h]);
            sq[h] += e * e;
        }
        count++;
    }
    if (count == 0)
        return;
    for (int h = 0; h < 4; ++h)
        s.predRmse[h] = std::sqrt(sq[h] / count);
}

Summary runAlphaBeta(const std::string &name,
                     double hz,
                     const std::function<double(double)> &truth,
                     double sigma,
                     double duration,
                     double evalStart,
                     double outlierTime = -1.0,
                     double outlierAdd = 0.0,
                     double settleStart = -1.0,
                     double settleBand = 0.0,
                     bool gateEnabled = false)
{
    const double dt = 1.0 / hz;
    ABTrackManager mgr(hz >= 75.0 ? SeekerType::Laser : SeekerType::Image);
    if (gateEnabled)
    {
        const double gateMin = (hz >= 75.0) ? 0.30 : 0.50;
        const double gateMax = (hz >= 75.0) ? 1.00 : 1.50;
        mgr.AzTracker().setInnovationGate(gateMin, gateMax, 4.0, 0.5, 3);
        mgr.ElTracker().setInnovationGate(gateMin, gateMax, 4.0, 0.5, 3);
    }
    else
    {
        mgr.AzTracker().disableInnovationGate();
        mgr.ElTracker().disableInnovationGate();
    }
    mgr.Init(truth(0.0), truth(0.0), 0);
    g_rng.seed(20261017u);

    std::vector<Sample> trace;
    const int count = static_cast<int>(std::ceil(duration * hz));
    for (int k = 1; k <= count; ++k)
    {
        const double t = k * dt;
        Sample s;
        s.t = t;
        s.truth = truth(t);
        s.meas = s.truth + gauss(sigma);
        if (outlierTime >= 0.0 && std::fabs(t - outlierTime) <= 0.51 * dt)
            s.meas += outlierAdd;

        mgr.FeedData(static_cast<qint64>(std::llround(t * 1000.0)),
                     s.meas, s.meas);
        s.estimate = mgr.AzTracker().angle();
        s.velocity = mgr.AzTracker().velocity();
        s.error = s.estimate - s.truth;
        if (isPacketTime(t, dt))
        {
            const sendExGuideData pkt = mgr.GenAxisPacket(true, std::llround(t));
            s.pred[0] = pkt.angle1;
            s.pred[1] = pkt.angle2;
            s.pred[2] = pkt.angle3;
            s.pred[3] = pkt.angle4;
            s.packet = 1;
        }
        trace.push_back(s);
    }

    saveTrace("ab_" + name, trace);
    Summary s = calculateSummary("alpha-beta", name, hz, "deg", trace,
                                 evalStart, settleStart, settleBand, outlierTime);
    measureAnglePackets(truth, trace, evalStart, s);
    s.gateRejects = mgr.AzTracker().rejectedMeasurementCount();
    return s;
}

Summary runPixel(const std::string &name,
                 double fps,
                 const std::function<double(double)> &truthX,
                 double sigma,
                 double duration,
                 double evalStart,
                 float processNoise = 1e-2f,
                 double settleStart = -1.0,
                 double settleBand = 0.0)
{
    const float dt = static_cast<float>(1.0 / fps);
    PixelKalmanTracker tracker;
    tracker.setProcessNoiseVel(processNoise);
    g_rng.seed(20261017u);

    tracker.feed(static_cast<float>(truthX(0.0) + gauss(sigma)),
                 360.0f + static_cast<float>(gauss(sigma)), 0.0f);

    std::vector<Sample> trace;
    const int count = static_cast<int>(std::ceil(duration * fps));
    for (int k = 1; k <= count; ++k)
    {
        const double t = k / fps;
        Sample s;
        s.t = t;
        s.truth = truthX(t);
        s.meas = truthX(t) + gauss(sigma);
        tracker.feed(static_cast<float>(s.meas),
                     360.0f + static_cast<float>(gauss(sigma)), dt);
        s.estimate = tracker.filteredX();
        s.error = s.estimate - s.truth;
        if (isPacketTime(t, dt))
        {
            float py = 0.0f;
            for (int h = 0; h < 4; ++h)
            {
                float px = 0.0f;
                tracker.predictPosition(static_cast<float>(TAU[h]), px, py);
                s.pred[h] = px;
            }
            s.packet = 1;
        }
        trace.push_back(s);
    }

    saveTrace("pixel_" + name, trace);
    Summary s = calculateSummary("pixel-kalman", name, fps, "px", trace,
                                 evalStart, settleStart, settleBand);
    measurePixelPackets(truthX, trace, evalStart, s);
    return s;
}

void printSummary(const Summary &s)
{
    std::printf("%-13s %-25s %6.1f Hz  %-3s  rmse=%8.3f  max=%8.3f  "
                "tailStd=%7.3f  final=%+8.3f  pred=[%7.3f %7.3f %7.3f %7.3f]  reject=%d\n",
                s.filter.c_str(), s.scenario.c_str(), s.rate, s.unit.c_str(),
                s.rmse, s.maxAbsError, s.tailStd, s.finalError,
                s.predRmse[0], s.predRmse[1], s.predRmse[2], s.predRmse[3],
                s.gateRejects);
}

void saveSummary(const std::vector<Summary> &all)
{
    FILE *fp = std::fopen("out/rate_summary.csv", "w");
    if (!fp)
        return;
    std::fprintf(fp,
                 "filter,scenario,rate_hz,unit,tracking_rmse,tracking_mae,"
                 "max_abs_error,tail_std,final_error,max_abs_output,"
                 "max_abs_velocity,pred_rmse_025,pred_rmse_050,pred_rmse_075,"
                 "pred_rmse_100,settle_s,outlier_peak,gate_rejects\n");
    for (const Summary &s : all)
    {
        std::fprintf(fp,
                     "%s,%s,%.1f,%s,%.9f,%.9f,%.9f,%.9f,%.9f,%.9f,%.9f,"
                     "%.9f,%.9f,%.9f,%.9f,%.9f,%.9f,%d\n",
                     s.filter.c_str(), s.scenario.c_str(), s.rate, s.unit.c_str(),
                     s.rmse, s.mae, s.maxAbsError, s.tailStd, s.finalError,
                     s.maxAbsOutput, s.maxAbsVelocity,
                     s.predRmse[0], s.predRmse[1], s.predRmse[2], s.predRmse[3],
                     s.settleTime, s.outlierPeak, s.gateRejects);
    }
    std::fclose(fp);
}
} // namespace

int main()
{
    std::printf("Ex-guide rate/trajectory comparison (production filter sources)\n");
    std::printf("Angle packet horizons: 0.25 / 0.50 / 0.75 / 1.00 s\n\n");

    {
        ABTrackManager laser(SeekerType::Laser);
        ABTrackManager image(SeekerType::Image);
        ABTrackManager ccdWide(SeekerType::CCD_Wide);
        ABTrackManager ccdTele(SeekerType::CCD_Tele);
        std::printf("Default gates enabled: laser=%d/%.2f image=%d/%.2f "
                    "ccdWide=%d/%.2f ccdTele=%d/%.2f\n\n",
                    laser.AzTracker().innovationGateEnabled(),
                    laser.AzTracker().innovationGate(),
                    image.AzTracker().innovationGateEnabled(),
                    image.AzTracker().innovationGate(),
                    ccdWide.AzTracker().innovationGateEnabled(),
                    ccdWide.AzTracker().innovationGate(),
                    ccdTele.AzTracker().innovationGateEnabled(),
                    ccdTele.AzTracker().innovationGate());
    }

    std::vector<Summary> all;

    const auto angleStill = [](double) { return 0.0; };
    const auto angleLinear = [](double t) { return 2.0 * t; };
    const auto angleStop = [](double t) { return t < 5.0 ? 2.0 * t : 10.0; };
    const auto angleSine = [](double t)
    {
        return 6.0 * std::sin(2.0 * PI * 0.25 * t);
    };
    const auto angleJump = [](double t)
    {
        return t < 3.0 ? 0.0 : 5.0;
    };

    // Laser angle path: 10 ms serial period in serialport_laser.cpp.
    all.push_back(runAlphaBeta("laser_static", 100.0, angleStill, 0.03,
                               20.0, 2.0));
    all.push_back(runAlphaBeta("laser_linear", 100.0, angleLinear, 0.03,
                               8.0, 2.0));
    all.push_back(runAlphaBeta("laser_stop", 100.0, angleStop, 0.03,
                               15.0, 8.0, -1.0, 0.0, 5.0, 0.05));
    all.push_back(runAlphaBeta("laser_sine_025hz", 100.0, angleSine, 0.03,
                               20.0, 4.0));
    all.push_back(runAlphaBeta("laser_outlier_5deg", 100.0, angleStill, 0.03,
                               10.0, 2.0, 3.0, 5.0));
    all.push_back(runAlphaBeta("laser_static_gated", 100.0, angleStill, 0.03,
                               20.0, 2.0, -1.0, 0.0, -1.0, 0.0, true));
    all.push_back(runAlphaBeta("laser_linear_gated", 100.0, angleLinear, 0.03,
                               8.0, 2.0, -1.0, 0.0, -1.0, 0.0, true));
    all.push_back(runAlphaBeta("laser_stop_gated", 100.0, angleStop, 0.03,
                               15.0, 8.0, -1.0, 0.0, 5.0, 0.05, true));
    all.push_back(runAlphaBeta("laser_sine_025hz_gated", 100.0, angleSine, 0.03,
                               20.0, 4.0, -1.0, 0.0, -1.0, 0.0, true));
    all.push_back(runAlphaBeta("laser_outlier_5deg_gated", 100.0, angleStill, 0.03,
                               10.0, 2.0, 3.0, 5.0, -1.0, 0.0, true));
    all.push_back(runAlphaBeta("laser_jump_5deg_gated", 100.0, angleJump, 0.03,
                               6.0, 2.0, 3.0, 0.0, -1.0, 0.0, true));

    // Image angle path: 20 ms serial period in serialport_image.cpp.
    all.push_back(runAlphaBeta("image_static", 50.0, angleStill, 0.05,
                               20.0, 2.0));
    all.push_back(runAlphaBeta("image_linear", 50.0, angleLinear, 0.05,
                               8.0, 2.0));
    all.push_back(runAlphaBeta("image_stop", 50.0, angleStop, 0.05,
                               15.0, 8.0, -1.0, 0.0, 5.0, 0.05));
    all.push_back(runAlphaBeta("image_sine_025hz", 50.0, angleSine, 0.05,
                               20.0, 4.0));
    all.push_back(runAlphaBeta("image_outlier_5deg", 50.0, angleStill, 0.05,
                               10.0, 2.0, 3.0, 5.0));
    all.push_back(runAlphaBeta("image_static_gated", 50.0, angleStill, 0.05,
                               20.0, 2.0, -1.0, 0.0, -1.0, 0.0, true));
    all.push_back(runAlphaBeta("image_linear_gated", 50.0, angleLinear, 0.05,
                               8.0, 2.0, -1.0, 0.0, -1.0, 0.0, true));
    all.push_back(runAlphaBeta("image_stop_gated", 50.0, angleStop, 0.05,
                               15.0, 8.0, -1.0, 0.0, 5.0, 0.05, true));
    all.push_back(runAlphaBeta("image_sine_025hz_gated", 50.0, angleSine, 0.05,
                               20.0, 4.0, -1.0, 0.0, -1.0, 0.0, true));
    all.push_back(runAlphaBeta("image_outlier_5deg_gated", 50.0, angleStill, 0.05,
                               10.0, 2.0, 3.0, 5.0, -1.0, 0.0, true));
    all.push_back(runAlphaBeta("image_jump_5deg_gated", 50.0, angleJump, 0.05,
                               6.0, 2.0, 3.0, 0.0, -1.0, 0.0, true));

    const auto pixelStill = [](double) { return 640.0; };
    const auto pixelLinear = [](double t) { return 300.0 + 40.0 * t; };
    const auto pixelStop = [](double t) { return t < 5.0 ? 300.0 + 33.0 * t : 465.0; };
    const auto pixelSine = [](double t)
    {
        return 640.0 + 120.0 * std::sin(2.0 * PI * 0.25 * t);
    };

    all.push_back(runPixel("pixel30_static", 30.0, pixelStill, 3.0, 20.0, 2.0));
    all.push_back(runPixel("pixel30_linear", 30.0, pixelLinear, 3.0, 20.0, 2.0));
    all.push_back(runPixel("pixel30_stop", 30.0, pixelStop, 3.0, 20.0, 8.0,
                           1e-2f, 5.0, 5.0));
    all.push_back(runPixel("pixel30_sine_025hz", 30.0, pixelSine, 3.0, 20.0, 4.0));
    all.push_back(runPixel("pixel30_sine_q1", 30.0, pixelSine, 3.0, 20.0, 4.0,
                           1.0f));
    all.push_back(runPixel("pixel30_sine_q100", 30.0, pixelSine, 3.0, 20.0, 4.0,
                           100.0f));
    all.push_back(runPixel("pixel30_stop_q100", 30.0, pixelStop, 3.0, 20.0, 8.0,
                           100.0f, 5.0, 5.0));
    all.push_back(runPixel("pixel50_static", 50.0, pixelStill, 3.0, 20.0, 2.0));
    all.push_back(runPixel("pixel50_linear", 50.0, pixelLinear, 3.0, 20.0, 2.0));
    all.push_back(runPixel("pixel50_sine_025hz", 50.0, pixelSine, 3.0, 20.0, 4.0));

    for (const Summary &s : all)
        printSummary(s);
    saveSummary(all);
    std::printf("\nCSV: out/rate_*.csv, out/rate_summary.csv\n");
    return 0;
}
