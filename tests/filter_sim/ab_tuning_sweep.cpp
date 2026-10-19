// Targeted Alpha-Beta parameter sweep using the production class.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <random>
#include <vector>

#include "kalman/AlphaBetaTracker.h"

namespace
{
constexpr double PI = 3.14159265358979323846;
std::mt19937 g_rng(20261017u);

double gauss(double sigma)
{
    std::normal_distribution<double> d(0.0, sigma);
    return d(g_rng);
}

struct Result
{
    double trackRms = 0.0;
    double packetRms = 0.0;
    double stillPacketRms = 0.0;
    double outlierPeak = 0.0;
    double stopTail = 0.0;
    double stepOvershoot = 0.0;
    double stepSettle = -1.0;
};

struct Acc
{
    double sq = 0.0;
    int n = 0;
    void add(double e)
    {
        sq += e * e;
        n++;
    }
    double rms() const { return n ? std::sqrt(sq / n) : 0.0; }
};

Result evaluate(double alpha, double beta, double hz)
{
    const double dt = 1.0 / hz;
    const double taus[4] = {0.25, 0.50, 0.75, 1.00};
    Result out;

    auto make = [&]()
    {
        ABTrackManager mgr(hz >= 75.0 ? SeekerType::Laser : SeekerType::Image);
        mgr.AzTracker().setAlpha(alpha);
        mgr.AzTracker().setBeta(beta);
        mgr.ElTracker().setAlpha(alpha);
        mgr.ElTracker().setBeta(beta);
        mgr.AzTracker().disableInnovationGate();
        mgr.ElTracker().disableInnovationGate();
        mgr.Init(0.0, 0.0, 0);
        return mgr;
    };

    // 0.25 Hz, +/-6 deg maneuver.
    {
        ABTrackManager mgr = make();
        g_rng.seed(20261017u);
        Acc track, packet;
        const int n = static_cast<int>(20.0 * hz);
        for (int k = 1; k <= n; ++k)
        {
            const double t = k * dt;
            const double truth = 6.0 * std::sin(2.0 * PI * 0.25 * t);
            mgr.FeedData(static_cast<qint64>(std::llround(t * 1000.0)),
                         truth + gauss(0.05), 0.0);
            if (t >= 4.0)
                track.add(mgr.AzTracker().angle() - truth);

            const double nearest = std::round(t);
            if (t >= 4.0 && std::fabs(t - nearest) <= 0.51 * dt)
            {
                const sendExGuideData p = mgr.GenAxisPacket(true, 0);
                const double pred[4] = {p.angle1, p.angle2, p.angle3, p.angle4};
                for (int h = 0; h < 4; ++h)
                    packet.add(pred[h] -
                               6.0 * std::sin(2.0 * PI * 0.25 * (t + taus[h])));
            }
        }
        out.trackRms = track.rms();
        out.packetRms = packet.rms();
    }

    // Still target: long-horizon packet jitter caused by estimated velocity.
    {
        ABTrackManager mgr = make();
        g_rng.seed(20261017u);
        Acc packet;
        const int n = static_cast<int>(20.0 * hz);
        for (int k = 1; k <= n; ++k)
        {
            const double t = k * dt;
            mgr.FeedData(static_cast<qint64>(std::llround(t * 1000.0)),
                         gauss(0.05), 0.0);
            const double nearest = std::round(t);
            if (t >= 4.0 && std::fabs(t - nearest) <= 0.51 * dt)
            {
                const sendExGuideData p = mgr.GenAxisPacket(true, 0);
                packet.add(p.angle1);
                packet.add(p.angle2);
                packet.add(p.angle3);
                packet.add(p.angle4);
            }
        }
        out.stillPacketRms = packet.rms();
    }

    // Single-frame 5 deg outlier.
    {
        ABTrackManager mgr = make();
        g_rng.seed(20261017u);
        const int n = static_cast<int>(8.0 * hz);
        for (int k = 1; k <= n; ++k)
        {
            const double t = k * dt;
            double z = gauss(0.05);
            if (std::fabs(t - 3.0) <= 0.51 * dt)
                z += 5.0;
            mgr.FeedData(static_cast<qint64>(std::llround(t * 1000.0)), z, 0.0);
            if (t >= 3.0)
                out.outlierPeak =
                    std::max(out.outlierPeak, std::fabs(mgr.AzTracker().angle()));
        }
    }

    // 2 deg/s ramp, then hold 10 deg.
    {
        ABTrackManager mgr = make();
        g_rng.seed(20261017u);
        Acc tail;
        const int n = static_cast<int>(15.0 * hz);
        for (int k = 1; k <= n; ++k)
        {
            const double t = k * dt;
            const double truth = t < 5.0 ? 2.0 * t : 10.0;
            mgr.FeedData(static_cast<qint64>(std::llround(t * 1000.0)),
                         truth + gauss(0.05), 0.0);
            if (t >= 10.0)
                tail.add(mgr.AzTracker().angle() - truth);
        }
        out.stopTail = tail.rms();
    }

    // Noise-free 5 deg step: check stability and settling.
    {
        ABTrackManager mgr = make();
        const int n = static_cast<int>(5.0 * hz);
        int lastOutside = -1;
        for (int k = 1; k <= n; ++k)
        {
            const double t = k * dt;
            const double truth = 5.0;
            mgr.FeedData(static_cast<qint64>(std::llround(t * 1000.0)),
                         truth, 0.0);
            const double e = mgr.AzTracker().angle() - truth;
            out.stepOvershoot = std::max(out.stepOvershoot, e);
            if (std::fabs(e) > 0.05)
                lastOutside = k;
        }
        out.stepSettle = (lastOutside + 1 < n) ? (lastOutside + 1) * dt : -1.0;
    }

    return out;
}
} // namespace

int main()
{
    struct Pair { double a; double b; };
    const Pair pairs[] = {
        {0.85, 0.05},
        {0.85, 0.20},
        {0.70, 0.10},
        {0.70, 0.25},
        {0.60, 0.10},
        {0.60, 0.25},
        {0.50, 0.05},
        {0.50, 0.15},
        {0.40, 0.05},
        {0.40, 0.10},
        {0.30, 0.02},
        {0.30, 0.05},
        {0.20, 0.01},
    };

    std::printf("rate  alpha  beta  trackRMS  packet1sRMS  stillPacketRMS  "
                "outlierPeak  stopTail  stepOver  stepSettle\n");
    for (double hz : {100.0, 50.0})
    {
        for (const Pair &p : pairs)
        {
            const Result r = evaluate(p.a, p.b, hz);
            std::printf("%4.0f  %5.2f  %5.2f  %9.4f  %11.4f  %14.4f  "
                        "%11.4f  %8.4f  %8.4f  %10.3f\n",
                        hz, p.a, p.b, r.trackRms, r.packetRms,
                        r.stillPacketRms, r.outlierPeak, r.stopTail,
                        r.stepOvershoot, r.stepSettle);
        }
        std::printf("\n");
    }
    return 0;
}
