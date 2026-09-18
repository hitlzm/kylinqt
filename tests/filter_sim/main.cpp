// ═══════════════════════════════════════════════════════════════════════════
// 滤波器预测能力实测（不改动工程源码，直接编译 kalman/ 下的真实实现）
//
//   被测对象 1：kalman/AlphaBetaTracker.cpp —— 外引导模式的 α-β 预测
//   被测对象 2：kalman/seekKalman.cpp      —— 当前统计模型(CS)卡尔曼预测
//
//   验证目标：
//     A. 是否存在数据振荡（超调/回摆/极限环/发散的符号反复）
//     B. 输出数据能否为 0（能否收敛到 0、能否编码成 0、是否存在零点偏置）
//
//   编译运行：见同目录 build_and_run.ps1
//   输出：stdout 摘要 + out/*.csv 明细
// ═══════════════════════════════════════════════════════════════════════════

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>

#include "kalman/AlphaBetaTracker.h"
#include "kalman/seekKalman.h"

// ─────────────────────────── 基础工具 ───────────────────────────

static const double PI = 3.14159265358979323846;

static std::mt19937 g_rng(20260918u);

static double gauss(double sigma)
{
    std::normal_distribution<double> d(0.0, sigma);
    return d(g_rng);
}

struct Trace
{
    std::vector<double> t;     // 秒
    std::vector<double> meas;  // 测量
    std::vector<double> y;     // 滤波输出（位置）
    std::vector<double> v;     // 速度/角速度
    std::vector<double> a;     // 加速度（卡尔曼第三状态）
};

static void saveCsv(const std::string &file,
                    const std::string &header,
                    const Trace &tr)
{
    FILE *fp = std::fopen(file.c_str(), "w");
    if (!fp)
        return;
    std::fprintf(fp, "%s\n", header.c_str());
    for (size_t i = 0; i < tr.t.size(); ++i)
    {
        std::fprintf(fp, "%.6f", tr.t[i]);
        if (!tr.meas.empty()) std::fprintf(fp, ",%.9f", tr.meas[i]);
        std::fprintf(fp, ",%.12f", tr.y[i]);
        if (!tr.v.empty()) std::fprintf(fp, ",%.12f", tr.v[i]);
        if (!tr.a.empty()) std::fprintf(fp, ",%.12f", tr.a[i]);
        std::fprintf(fp, "\n");
    }
    std::fclose(fp);
}

struct Analysis
{
    double minY = 0, maxY = 0, finalY = 0;
    double firstZeroT = -1;     // 首次 |y| <= 1e-9 的时刻
    double firstExactZeroT = -1;// 首次 y == 0.0 的时刻
    int exactZeroCount = 0;     // 恰好等于 0.0 的采样点个数
    int signChanges = 0;        // 相对参考值的符号变化次数（振荡迹象）
    double overshoot = 0;       // max(y - ref)
    double undershoot = 0;      // min(y - ref)
    double settleT = -1;        // 最后一次进入 ±band 的时刻
    double tailMean = 0, tailStd = 0, tailMin = 0, tailMax = 0;
    int tailCount = 0;
};

// band 用于判定settle，ref 是每个时刻的期望值（默认恒为常量 refConst）
static Analysis analyze(const Trace &tr,
                        const std::vector<double> &ref,
                        double band)
{
    Analysis an;
    if (tr.y.empty())
        return an;

    an.minY = tr.y[0];
    an.maxY = tr.y[0];

    int lastOutOfBand = -1;
    for (size_t i = 0; i < tr.y.size(); ++i)
    {
        const double e = tr.y[i] - ref[i];
        an.minY = std::min(an.minY, tr.y[i]);
        an.maxY = std::max(an.maxY, tr.y[i]);
        an.overshoot = std::max(an.overshoot, e);
        an.undershoot = std::min(an.undershoot, e);

        if (std::fabs(tr.y[i]) <= 1e-9 && an.firstZeroT < 0)
            an.firstZeroT = tr.t[i];
        if (tr.y[i] == 0.0)
        {
            if (an.firstExactZeroT < 0)
                an.firstExactZeroT = tr.t[i];
            an.exactZeroCount++;
        }
        if (i > 0)
        {
            const double ePrev = tr.y[i - 1] - ref[i - 1];
            if ((e > 0 && ePrev < 0) || (e < 0 && ePrev > 0))
                an.signChanges++;
        }
        if (std::fabs(e) > band)
            lastOutOfBand = static_cast<int>(i);
    }
    an.finalY = tr.y.back();
    an.settleT = (lastOutOfBand + 1 < static_cast<int>(tr.y.size()))
                     ? tr.t[lastOutOfBand + 1]
                     : -1.0;

    // 尾段统计（最后 25%）
    const size_t start = tr.y.size() * 3 / 4;
    double s = 0, s2 = 0, mn = 1e300, mx = -1e300;
    for (size_t i = start; i < tr.y.size(); ++i)
    {
        const double e = tr.y[i] - ref[i];
        s += e;
        s2 += e * e;
        mn = std::min(mn, e);
        mx = std::max(mx, e);
    }
    an.tailCount = static_cast<int>(tr.y.size() - start);
    an.tailMean = s / an.tailCount;
    an.tailStd = std::sqrt(std::max(0.0, s2 / an.tailCount - an.tailMean * an.tailMean));
    an.tailMin = mn;
    an.tailMax = mx;
    return an;
}

static std::vector<double> constRef(size_t n, double c)
{
    return std::vector<double>(n, c);
}

static void printAnalysis(const char *tag, const Analysis &an)
{
    std::printf("%-42s min=%-11.6f max=%-11.6f 末值=%-11.6f 超调=%-11.6f 下冲=%-11.6f\n",
                tag, an.minY, an.maxY, an.finalY, an.overshoot, an.undershoot);
    std::printf("%-42s 首次|y|<=1e-9 t=%-8.3f 首次y==0 t=%-8.3f 恰好0个数=%-5d 符号翻转=%-5d 稳定时刻=%.3f\n",
                "", an.firstZeroT, an.firstExactZeroT, an.exactZeroCount,
                an.signChanges, an.settleT);
    std::printf("%-42s 尾段偏差 mean=%-11.8f std=%-11.8f [%.8f, %.8f]\n",
                "", an.tailMean, an.tailStd, an.tailMin, an.tailMax);
}

static void rule(const char *title)
{
    std::printf("\n══════════════════════════════════════════════════════════════════════\n");
    std::printf("%s\n", title);
    std::printf("══════════════════════════════════════════════════════════════════════\n");
}

// ═══════════════════════════════════════════════════════════════════════════
// 一、α-β 预测器（外引导模式）
// ═══════════════════════════════════════════════════════════════════════════

static const double AB_DT = 0.02;  // 图像导引头虚拟时间步长 20ms

// 1.1 未初始化时输出什么（外引导刚进入、还没收到第一帧时下发什么）
static void ab_uninitialized()
{
    rule("1.1  α-β：未收到第一帧测量时的输出（predict 未初始化）");

    AlphaBetaTracker trk(0.85, 0.05, AngleMode::Clamp, -18.0, 18.0);
    std::printf("initialized=%d lost=%d angle=%.12f vel=%.12f\n",
                (int)trk.initialized(), (int)trk.lost(), trk.angle(), trk.velocity());
    std::printf("predict(0.25)=%.12f predict(0.50)=%.12f predict(0.75)=%.12f predict(1.00)=%.12f\n",
                trk.predict(0.25), trk.predict(0.50), trk.predict(0.75), trk.predict(1.00));
    std::printf("→ 4 个预测点全为 0，发包时转台会被指令到 \"当前角 + 0\"\n");
}

// 1.2 目标从 18° 跳回 0°：会不会冲过 0、会不会振荡、能不能停在 0
static void ab_step_to_zero()
{
    rule("1.2  α-β：目标从 18° 跳到 0°（限幅 ±18°）——能否收敛到 0、是否过零振荡");

    AlphaBetaTracker trk(0.85, 0.05, AngleMode::Clamp, -18.0, 18.0);
    qint64 tms = 0;
    trk.init(18.0, tms);

    Trace tr;
    for (int k = 0; k < 600; ++k)  // 12 s
    {
        tms += 20;
        trk.update(true, 0.0, tms);
        tr.t.push_back(tms / 1000.0);
        tr.meas.push_back(0.0);
        tr.y.push_back(trk.angle());
        tr.v.push_back(trk.velocity());
    }
    saveCsv("out/ab_step_to_zero.csv", "t,meas,angle,velocity", tr);

    const Analysis an = analyze(tr, constRef(tr.y.size(), 0.0), 0.01);
    printAnalysis("α-β 18°→0°", an);

    // 首次过零时刻与最大负冲
    double minv = 1e300, minvT = 0, maxV = 0;
    for (size_t i = 0; i < tr.y.size(); ++i)
    {
        if (tr.y[i] < minv) { minv = tr.y[i]; minvT = tr.t[i]; }
        maxV = std::max(maxV, std::fabs(tr.v[i]));
    }
    std::printf("最低点=%.6f° (t=%.3fs)  期间最大|角速度|=%.3f°/s\n", minv, minvT, maxV);
}

// 1.3 阶跃超调量：0 → 若干角度
static void ab_step_overshoot()
{
    rule("1.3  α-β：阶跃响应超调（0 → 目标角，限幅 ±18°）");

    const double targets[] = {1.0, 2.0, 5.0, 10.0, 18.0};
    for (double tg : targets)
    {
        AlphaBetaTracker trk(0.85, 0.05, AngleMode::Clamp, -18.0, 18.0);
        qint64 tms = 0;
        trk.init(0.0, tms);

        Trace tr;
        for (int k = 0; k < 500; ++k)
        {
            tms += 20;
            trk.update(true, tg, tms);
            tr.t.push_back(tms / 1000.0);
            tr.y.push_back(trk.angle());
            tr.v.push_back(trk.velocity());
        }
        const Analysis an = analyze(tr, constRef(tr.y.size(), tg), 0.05);
        std::printf("阶跃到 %5.1f°: 最大超调=%7.4f° (%5.2f%%)  最大下冲=%7.4f°  符号翻转=%d  ±0.05°稳定时刻=%.3fs  尾段均值偏差=%+.6f\n",
                    tg, an.overshoot, 100.0 * an.overshoot / tg, an.undershoot,
                    an.signChanges, an.settleT, an.tailMean);
    }
}

// 1.4 噪声输入下能否停留在 0（外引导中目标居中、只有检测抖动）
static void ab_noise_at_zero()
{
    rule("1.4  α-β：目标居中(真值 0°)、测量带噪声——输出能否为 0、抖动多大");

    const double sigmas[] = {0.02, 0.05, 0.20, 0.50};
    for (double sg : sigmas)
    {
        // 图像导引头路径：α=0.85 β=0.05 dt=20ms
        AlphaBetaTracker trk(0.85, 0.05, AngleMode::Clamp, -18.0, 18.0);
        // 对照：常用平滑参数 α=0.3 β=0.02
        AlphaBetaTracker ref(0.30, 0.02, AngleMode::Clamp, -18.0, 18.0);

        qint64 tms = 0;
        trk.init(0.0, tms);
        ref.init(0.0, tms);

        Trace tr, tr2;
        for (int k = 0; k < 1500; ++k)  // 30 s
        {
            const double z = gauss(sg);
            tms += 20;
            trk.update(true, z, tms);
            ref.update(true, z, tms);
            tr.t.push_back(tms / 1000.0);
            tr.y.push_back(trk.angle());
            tr.v.push_back(trk.velocity());
            tr2.t.push_back(tms / 1000.0);
            tr2.y.push_back(ref.angle());
            tr2.v.push_back(ref.velocity());
        }
        if (sg == 0.05)
            saveCsv("out/ab_noise.csv", "t,angle,velocity", tr);

        const Analysis a1 = analyze(tr, constRef(tr.y.size(), 0.0), 0.0);
        const Analysis a2 = analyze(tr2, constRef(tr2.y.size(), 0.0), 0.0);
        std::printf("测量σ=%.2f°  α=0.85/β=0.05: 尾段std=%.6f° 极值[%+.4f,%+.4f] 恰好0=%d 符号翻转=%d | "
                    "α=0.30/β=0.02: 尾段std=%.6f° 恰好0=%d\n",
                    sg, a1.tailStd, a1.tailMin, a1.tailMax, a1.exactZeroCount, a1.signChanges,
                    a2.tailStd, a2.exactZeroCount);
    }
}

// 1.5 测量超出限幅范围（目标出视场）后再回到视场内：限幅会不会造成振荡
static void ab_clamp_windup()
{
    rule("1.5  α-β：测量超限(25°)持续 2s 再回到 5°——限幅饱和后的回摆");

    AlphaBetaTracker trk(0.85, 0.05, AngleMode::Clamp, -18.0, 18.0);
    qint64 tms = 0;
    trk.init(0.0, tms);

    Trace tr;
    for (int k = 0; k < 250; ++k)  // 5 s
    {
        tms += 20;
        const double meas = (k < 100) ? 25.0 : 5.0;  // 前 2s 出视场
        trk.update(true, meas, tms);
        tr.t.push_back(tms / 1000.0);
        tr.meas.push_back(meas);
        tr.y.push_back(trk.angle());
        tr.v.push_back(trk.velocity());
    }
    saveCsv("out/ab_clamp_windup.csv", "t,meas,angle,velocity", tr);

    double maxAbsV = 0;
    int satCount = 0;
    for (size_t i = 0; i < tr.y.size(); ++i)
    {
        maxAbsV = std::max(maxAbsV, std::fabs(tr.v[i]));
        if (std::fabs(tr.y[i]) >= 18.0 - 1e-9)
            satCount++;
    }
    std::printf("饱和(贴住 ±18°)采样数=%d/250  最大|角速度|=%.3f°/s（内部限速 180°/s）\n",
                satCount, maxAbsV);
    std::printf("饱和期间 angle=%.6f  velocity=%.6f\n", tr.y[99], tr.v[99]);

    // 回到视场后的恢复段：2.0s 之后
    Trace rec;
    for (size_t i = 100; i < tr.y.size(); ++i)
    {
        rec.t.push_back(tr.t[i]);
        rec.y.push_back(tr.y[i]);
        rec.v.push_back(tr.v[i]);
    }
    const Analysis an = analyze(rec, constRef(rec.y.size(), 5.0), 0.05);
    printAnalysis("回到视场后(参考 5°)", an);
}

// ─────────────────────────────────────────────────────────────────────────
// 修正前的 α-β 逻辑复刻（仅测试用，用于“改前/改后”对照）
//   旧 update()  : dt<1ms（含 0 与负值）一律压成 1ms，不区分重复帧/中断
//   旧 correct() : 残差直接进入速度积分，无抗饱和
// ─────────────────────────────────────────────────────────────────────────
struct LegacyAB
{
    double angle = 0.0, vel = 0.0;
    qint64 ts = 0;
    bool   inited = false;
    double alpha = 0.85, beta = 0.05, vmax = 180.0, lo = -27.0, hi = 27.0;

    double clampA(double a) const { return a < lo ? lo : (a > hi ? hi : a); }

    void initAt(double a, qint64 t)
    {
        inited = true;
        angle = clampA(a);
        vel = 0.0;
        ts = t;
    }

    void update(double z, qint64 t)
    {
        if (!inited) { initAt(z, t); return; }

        double dt = (t - ts) / 1000.0;
        if (dt < 0.001) dt = 0.001;          // ← 旧行为
        ts = t;

        const double pred = clampA(angle + vel * dt);
        const double res  = z - pred;        // Clamp 模式无角度回绕
        angle = clampA(pred + alpha * res);
        vel   = vel + beta * res / dt;
        if (vel >  vmax) vel =  vmax;
        if (vel < -vmax) vel = -vmax;
    }

    double predict(double tau) const { return clampA(angle + vel * tau); }
};

// 1.6 α-β 抗饱和 + 时间戳守卫（改前/改后对照）
static void ab_guard_tests()
{
    rule("1.6  α-β 抗饱和 + 时间戳守卫（改前 = 旧逻辑复刻，改后 = 现工程实现）");

    // ── (a) 限幅饱和回摆 ──
    {
        std::printf("\n(a) 限幅饱和：测量 25°（超限）持续 2s → 回到 5°（限幅 ±18°，20ms/帧）\n");
        const int N1 = 100, N2 = 150;
        LegacyAB lg; lg.lo = -18; lg.hi = 18;
        AlphaBetaTracker nt(0.85, 0.05, AngleMode::Clamp, -18.0, 18.0);
        qint64 t = 0;
        lg.initAt(0.0, t);
        nt.init(0.0, t);

        double maxVl = 0, maxVn = 0, maxOl = -1e9, maxOn = -1e9;
        double areaL = 0, areaN = 0;               // 回视场后的 |误差| 时间积分
        int satFrames = 0, lastOutL = -1, lastOutN = -1;
        for (int k = 0; k < N1 + N2; ++k)
        {
            t += 20;
            const double z = (k < N1) ? 25.0 : 5.0;
            lg.update(z, t);
            nt.update(true, z, t);

            if (k < N1)
            {
                maxVl = std::max(maxVl, std::fabs(lg.vel));
                maxVn = std::max(maxVn, std::fabs(nt.velocity()));
                if (nt.saturated()) satFrames++;
            }
            else
            {
                maxOl = std::max(maxOl, lg.angle);
                maxOn = std::max(maxOn, nt.angle());
                areaL += std::fabs(lg.angle - 5.0) * 0.02;
                areaN += std::fabs(nt.angle() - 5.0) * 0.02;
                if (std::fabs(lg.angle - 5.0) > 0.05) lastOutL = k;
                if (std::fabs(nt.angle() - 5.0) > 0.05) lastOutN = k;
            }
        }
        std::printf("  饱和期间最大|角速度|：改前 %6.2f°/s   改后 %6.2f°/s\n", maxVl, maxVn);
        std::printf("  饱和采样数(现实现 saturated 标志)：%d/%d\n", satFrames, N1);
        std::printf("  回到 5° 后的最大角度：改前 %7.4f°（超调 %+.4f°）  改后 %7.4f°（超调 %+.4f°）\n",
                    maxOl, maxOl - 5.0, maxOn, maxOn - 5.0);
        std::printf("  回视场后恢复时间(±0.05°)：改前 %.3fs   改后 %.3fs\n",
                    (lastOutL + 1) * 0.02, (lastOutN + 1) * 0.02);
        std::printf("  回视场后 |误差| 时间积分：改前 %.3f°·s   改后 %.3f°·s\n", areaL, areaN);
    }

    // ── (b) 同一毫秒两帧 ──
    {
        std::printf("\n(b) 同一毫秒两帧（dt=0，CCD 真实毫秒时间戳常见）\n");
        const double residuals[] = {0.5, 20.0};
        for (double res : residuals)
        {
            LegacyAB lg;
            AlphaBetaTracker nt(0.85, 0.05, AngleMode::Clamp, -27.0, 27.0);
            lg.initAt(10.0, 1000);
            nt.init(10.0, 1000);

            lg.update(10.0 + res, 1000);                 // 旧：dt 被压成 1ms
            const bool ok = nt.update(true, 10.0 + res, 1000);   // 新：整帧丢弃
            std::printf("  残差 %5.1f°: 改前 velocity=%8.3f°/s   改后 velocity=%6.3f°/s（update 返回 %s）\n",
                        res, lg.vel, nt.velocity(), ok ? "true" : "false→丢弃该帧");
        }
    }

    // ── (c) 时间戳回退（系统校时）──
    {
        std::printf("\n(c) 时间戳回退（例如系统校时导致 -40ms）\n");
        LegacyAB lg;
        AlphaBetaTracker nt(0.85, 0.05, AngleMode::Clamp, -27.0, 27.0);
        lg.initAt(10.0, 1000);
        nt.init(10.0, 1000);

        lg.update(10.5, 960);
        const bool ok = nt.update(true, 10.5, 960);
        std::printf("  改前 velocity=%8.3f°/s   改后 velocity=%6.3f°/s（update 返回 %s）\n",
                    lg.vel, nt.velocity(), ok ? "true" : "false→丢弃该帧");
    }

    // ── (d) 数据中断 2s ──
    {
        std::printf("\n(d) 数据中断 2s（目标在中断期间已停住）\n");
        LegacyAB lg;
        AlphaBetaTracker nt(0.85, 0.05, AngleMode::Clamp, -27.0, 27.0);
        qint64 t = 0;
        lg.initAt(10.0, t);
        nt.init(10.0, t);

        for (int k = 0; k < 40; ++k)     // 先建立 5°/s 的速度
        {
            t += 20;
            const double z = 10.0 + 5.0 * (t / 1000.0);
            lg.update(z, t);
            nt.update(true, z, t);
        }
        const double zHold = 10.0 + 5.0 * (t / 1000.0);   // 中断期间目标停在原地
        t += 2000;

        lg.update(zHold, t);
        const bool ok = nt.update(true, zHold, t);
        std::printf("  中断前速度≈5°/s，中断后测量仍为 %.3f°\n", zHold);
        std::printf("  改前 angle=%7.3f° velocity=%7.3f°/s   改后 angle=%7.3f° velocity=%7.3f°/s（返回 %s）\n",
                    lg.angle, lg.vel, nt.angle(), nt.velocity(), ok ? "true" : "false");
        std::printf("  4 点包 改前: %.4f %.4f %.4f %.4f\n",
                    lg.predict(0.25), lg.predict(0.5), lg.predict(0.75), lg.predict(1.0));
        std::printf("  4 点包 改后: %.4f %.4f %.4f %.4f\n",
                    nt.predict(0.25), nt.predict(0.5), nt.predict(0.75), nt.predict(1.0));
    }

    // ── (e) 回归：正常跟踪不应变差 ──
    {
        std::printf("\n(e) 回归对照（正常 20ms 跟踪）\n");
        // 阶跃 0 → 5°
        LegacyAB lg; lg.lo = -18; lg.hi = 18;
        AlphaBetaTracker nt(0.85, 0.05, AngleMode::Clamp, -18.0, 18.0);
        qint64 t = 0, t2 = 0;
        lg.initAt(0.0, t);
        nt.init(0.0, t2);
        double ovL = -1e9, ovN = -1e9;
        for (int k = 0; k < 500; ++k)
        {
            t += 20; t2 += 20;
            lg.update(5.0, t);
            nt.update(true, 5.0, t2);
            ovL = std::max(ovL, lg.angle - 5.0);
            ovN = std::max(ovN, nt.angle() - 5.0);
        }
        std::printf("  阶跃 0→5° 超调：改前 %+.4f°   改后 %+.4f°\n", ovL, ovN);

        // 噪声下输出抖动（真值 0，σ=0.05°）
        g_rng.seed(20260918u);
        LegacyAB ln; ln.lo = -18; ln.hi = 18;
        AlphaBetaTracker nn(0.85, 0.05, AngleMode::Clamp, -18.0, 18.0);
        qint64 ta = 0, tb = 0;
        ln.initAt(0.0, ta);
        nn.init(0.0, tb);
        std::vector<double> zs;
        for (int k = 0; k < 1500; ++k) zs.push_back(gauss(0.05));
        double sl = 0, sn = 0;
        for (int k = 0; k < 1500; ++k)
        {
            ta += 20; tb += 20;
            ln.update(zs[k], ta);
            nn.update(true, zs[k], tb);
            const double el = ln.angle, en = nn.angle();
            sl += el * el;
            sn += en * en;
        }
        std::printf("  噪声 σ=0.05° 输出 rms：改前 %.6f°   改后 %.6f°\n",
                    std::sqrt(sl / 1500.0), std::sqrt(sn / 1500.0));
    }

    // ── (f) 模式切换未复位：陈旧角速度对“进入外引导后第 1 包”的影响 ──
    {
        std::printf("\n(f) 进入外引导时未复位（陈旧角速度）——第 1 包对比\n");

        // 用 12°/s 匀速目标建立角速度，1.2s 后角度约 14.4°（仍在 ±27° 视场内）
        auto build = [](AlphaBetaTracker &tk, qint64 &t) {
            tk.init(0.0, 0);
            t = 0;
            for (int k = 0; k < 60; ++k)
            {
                t += 20;
                tk.update(true, 12.0 * (t / 1000.0), t);
            }
        };

        AlphaBetaTracker noInit(0.85, 0.05, AngleMode::Clamp, -27.0, 27.0);
        qint64 t1 = 0;
        build(noInit, t1);
        // 外引导关闭期间不喂数据（虚拟时钟不推进，状态原样保留）
        // 重新进入外引导：目标已经停住，第 1 帧测量就是当前角度
        const double zHold = noInit.angle();
        t1 += 20;
        noInit.update(true, zHold, t1);
        std::printf("  未复位: 进入时的角速度=%.2f°/s  第 1 包 = %.3f %.3f %.3f %.3f\n",
                    noInit.velocity(),
                    noInit.predict(0.25), noInit.predict(0.5), noInit.predict(0.75), noInit.predict(1.0));

        AlphaBetaTracker withInit(0.85, 0.05, AngleMode::Clamp, -27.0, 27.0);
        qint64 t2 = 0;
        build(withInit, t2);
        t2 += 20;
        // 与现工程改法一致：虚拟时钟清零 + 用本帧测量 Init，随后同一帧按 20ms 继续喂
        withInit.init(zHold, t2 - 20);   // ← 对应 m_abMgr.Init(az, pitch, 0)
        withInit.update(true, zHold, t2);
        std::printf("  已复位: 进入时的角速度=%.2f°/s  第 1 包 = %.3f %.3f %.3f %.3f\n",
                    withInit.velocity(),
                    withInit.predict(0.25), withInit.predict(0.5),
                    withInit.predict(0.75), withInit.predict(1.0));
    }
}

// 1.7 1s 数据包（4 点预测）在运动目标下是否单调、外推误差多大
static void ab_packet_monotonic()
{
    rule("1.7  α-β：1s 数据包 4 点预测(0.25/0.50/0.75/1.0s) 的单调性与误差");

    ABTrackManager mgr(SeekerType::Image);  // 图像导引头 ±18°

    qint64 t = 0;
    mgr.Init(0.0, 0.0, t);

    // 目标做 0.5Hz、幅度 8° 正弦摆动的相对运动
    auto target = [](double sec) { return 8.0 * std::sin(2.0 * PI * 0.5 * sec); };

    Trace pk;
    std::printf("发包时刻  预测1    预测2    预测3    预测4   | 真值1    真值2    真值3    真值4   | 单调?\n");
    for (int packet = 0; packet < 8; ++packet)
    {
        for (int k = 0; k < 50; ++k)  // 每 20ms 一帧，1s 一包
        {
            const double sec = (t + 20.0) / 1000.0;
            t += 20;
            mgr.FeedData(t, target(sec), 0.0);
        }
        const sendExGuideData pkt = mgr.GenAxisPacket(true, packet);
        const double now = t / 1000.0;
        const double g1 = target(now + 0.25), g2 = target(now + 0.50);
        const double g3 = target(now + 0.75), g4 = target(now + 1.00);

        pk.t.push_back(now);
        pk.y.push_back(pkt.angle1);
        pk.v.push_back(pkt.angle2);
        pk.a.push_back(pkt.angle3);
        pk.meas.push_back(pkt.angle4);

        const bool inc = (pkt.angle1 <= pkt.angle2) && (pkt.angle2 <= pkt.angle3) && (pkt.angle3 <= pkt.angle4);
        const bool dec = (pkt.angle1 >= pkt.angle2) && (pkt.angle2 >= pkt.angle3) && (pkt.angle3 >= pkt.angle4);
        const bool gInc = (g1 <= g2) && (g2 <= g3) && (g3 <= g4);
        const bool gDec = (g1 >= g2) && (g2 >= g3) && (g3 >= g4);
        std::printf("%6.2fs  %8.4f %8.4f %8.4f %8.4f | %8.4f %8.4f %8.4f %8.4f | %s%s%s\n",
                    now, pkt.angle1, pkt.angle2, pkt.angle3, pkt.angle4,
                    g1, g2, g3, g4,
                    (inc ? "递增" : (dec ? "递减" : "非单调")),
                    ((inc || dec) != (gInc || gDec)) ? " ←与真值方向不一致" : "",
                    "");

    }

    FILE *fp = std::fopen("out/ab_packet_vs_truth.csv", "w");
    if (fp)
    {
        std::fprintf(fp, "t,p1,p2,p3,p4,g1,g2,g3,g4\n");
        // 重新算一遍真值序列（与上面同一目标函数）
        for (int packet = 0; packet < 8; ++packet)
        {
            const double now = (packet + 1) * 1.0;
            std::fprintf(fp, "%.3f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                         now, pk.y[packet], pk.v[packet], pk.a[packet], pk.meas[packet],
                         target(now + 0.25), target(now + 0.50),
                         target(now + 0.75), target(now + 1.00));
        }
        std::fclose(fp);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// 二、CS 卡尔曼（kalman/seekKalman.cpp）
// ═══════════════════════════════════════════════════════════════════════════

// 3x3 矩阵工具：用数值积分独立算出“理论正确”的 Φ、U、Q，与代码里的公式对照
struct M3
{
    double m[3][3];
};

static M3 mul(const M3 &A, const M3 &B)
{
    M3 C{};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                C.m[i][j] += A.m[i][k] * B.m[k][j];
    return C;
}

// e^{A s} 的泰勒级数（s 很小、A 元素 O(1)，30 项足够到机器精度）
static M3 expm(const M3 &A, double s, int terms = 30)
{
    M3 E{};
    for (int i = 0; i < 3; ++i) E.m[i][i] = 1.0;
    M3 term{};
    for (int i = 0; i < 3; ++i) term.m[i][i] = 1.0;
    M3 powA{};
    for (int i = 0; i < 3; ++i) powA.m[i][i] = 1.0;  // A^0
    double fact = 1.0;
    for (int n = 1; n <= terms; ++n)
    {
        powA = mul(powA, A);
        fact *= n;
        const double c = std::pow(s, n) / fact;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                E.m[i][j] += c * powA.m[i][j];
    }
    return E;
}

// 代码使用的加速度时间常数模型
static void kf_theoretical_matrices()
{
    const double a = ALPHA, T = DT;
    M3 A{};
    A.m[0][1] = 1.0;
    A.m[1][2] = 1.0;
    A.m[2][2] = -a;

    const M3 Phi = expm(A, T);

    // v(s) = Φ(s)·[0,0,1]^T ；U = ∫v ds ；Q = 2aσ² ∫v(s)v(s)^T ds
    const int N = 20000;
    const double h = T / N;
    double U[3] = {0, 0, 0};
    double Q[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    for (int k = 0; k <= N; ++k)
    {
        const double s = k * h;
        const M3 P = expm(A, s);
        const double v[3] = {P.m[0][2], P.m[1][2], P.m[2][2]};
        const double w = (k == 0 || k == N) ? 1.0 : ((k % 2) ? 4.0 : 2.0);  // Simpson 权重
        for (int i = 0; i < 3; ++i)
        {
            U[i] += w * v[i] * h / 3.0;
            for (int j = 0; j < 3; ++j)
                Q[i][j] += w * v[i] * v[j] * h / 3.0;
        }
    }
    const double c = 2.0 * a * SIGMA_A * SIGMA_A;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            Q[i][j] *= c;

    // 重要约定：论文的连续模型是 ȧ = -α·a + α·ā + w，输入项为 α·ā，
    // 所以离散输入矩阵是 U = α·∫₀^T v(s)ds（不是 ∫v）。这里补上 α 再比较。
    for (int i = 0; i < 3; ++i)
        U[i] *= a;

    CSKalmanFilter f;
    std::printf("\n【模型矩阵对照】代码公式 vs 数值积分（同一连续模型的最优离散化）\n");
    std::printf("Φ 最大偏差 = %.3e\n", std::fabs(Phi.m[0][1] - f.Phi[0][1]) +
                                            std::fabs(Phi.m[0][2] - f.Phi[0][2]) +
                                            std::fabs(Phi.m[1][2] - f.Phi[1][2]));
    std::printf("U 代码 = [%.6e, %.6e, %.6e]   U 数值(α·∫v) = [%.6e, %.6e, %.6e]\n",
                f.U[0], f.U[1], f.U[2], U[0], U[1], U[2]);
    std::printf("Q 代码                               Q 数值积分\n");
    for (int i = 0; i < 3; ++i)
        std::printf("[% .6e % .6e % .6e]     [% .6e % .6e % .6e]\n",
                    f.Q[i][0], f.Q[i][1], f.Q[i][2], Q[i][0], Q[i][1], Q[i][2]);
}

// 复刻修正前的 Q 公式（用于“改前/改后”对照，只在测试里使用）
static void setLegacyQ(CSKalmanFilter &f)
{
    const double a = ALPHA, T = DT, exp_aT = std::exp(-a * T);
    const double a2 = a * a, a3 = a2 * a;

    const double q11 = (1 - std::exp(-2*a*T) + 2*a*T + 2*a3*T*T*T/3 - 2*a2*T*T - 4*a*T*exp_aT) / (2*a3);
    const double q12 = (std::exp(-2*a*T) + 1 - 2*exp_aT + 2*a*T*exp_aT - 2*a*T + a2*T*T) / (2*a2*a);
    const double q13 = (1 - std::exp(-2*a*T) - 2*a*T*exp_aT) / (2*a3);
    const double q22 = (4*std::exp(-2*a*T) - 3 - std::exp(-2*a*T) + 2*a*T) / (2*a3);
    const double q23 = (1 - std::exp(-2*a*T)) / (2*a);
    const double q33 = (1 - std::exp(-2*a*T)) / (2*a);

    const double c = 2 * SIGMA_A * SIGMA_A;
    f.Q[0][0] = c*q11; f.Q[0][1] = c*q12; f.Q[0][2] = c*q13;
    f.Q[1][0] = c*q12; f.Q[1][1] = c*q22; f.Q[1][2] = c*q23;
    f.Q[2][0] = c*q13; f.Q[2][1] = c*q23; f.Q[2][2] = c*q33;
}

// 2.1 打印实际生效的过程噪声矩阵 Q（只读，不改代码）
static void kf_dump_Q()
{
    rule("2.1  卡尔曼：实际使用的 Q 矩阵（过程噪声协方差）诊断");

    CSKalmanFilter f;
    std::printf("α=%.3f  σ_a=%.4f  R=%.4f  T=%.3f\n", ALPHA, SIGMA_A, R_MEAS, DT);
    for (int i = 0; i < 3; ++i)
        std::printf("Q[%d] = [% .6e % .6e % .6e]\n", i, f.Q[i][0], f.Q[i][1], f.Q[i][2]);

    // 对称 3x3 矩阵是否正定：Sylvester 判据（各阶主子式 > 0）
    const double d1 = f.Q[0][0];
    const double d2 = f.Q[0][0] * f.Q[1][1] - f.Q[0][1] * f.Q[1][0];
    const double d3 = f.Q[0][0] * (f.Q[1][1] * f.Q[2][2] - f.Q[1][2] * f.Q[2][1]) -
                      f.Q[0][1] * (f.Q[1][0] * f.Q[2][2] - f.Q[1][2] * f.Q[2][0]) +
                      f.Q[0][2] * (f.Q[1][0] * f.Q[2][1] - f.Q[1][1] * f.Q[2][0]);
    std::printf("主子式: D1=%.6e  D2=%.6e  D3=%.6e\n", d1, d2, d3);
    std::printf("→ Q 是否半正定: %s (D1>0 且 D2>0 且 D3>0 才成立)\n",
                (d1 > 0 && d2 > 0 && d3 > 0) ? "是" : "否 *");
    std::printf("   Q[1][1]=%.6e  (负值意味着加速度方差被\"减掉\")\n", f.Q[1][1]);

    CSKalmanFilter legacy;
    setLegacyQ(legacy);
    std::printf("\n【修正前的旧 Q（仅测试用复刻）】\n");
    for (int i = 0; i < 3; ++i)
        std::printf("Q_old[%d] = [% .6e % .6e % .6e]\n", i,
                    legacy.Q[i][0], legacy.Q[i][1], legacy.Q[i][2]);

    kf_theoretical_matrices();
}

// 2.6 数据包对比：α-β / 卡尔曼(旧 3s 视野) / 卡尔曼(新 1s 视野)
static void packet_compare()
{
    rule("2.6  数据包对比：α-β  vs  卡尔曼(旧 3s 视野)  vs  卡尔曼(新 1s 视野)");

    const double tShort[4] = {0.25, 0.50, 0.75, 1.00};   // 新：与 α-β 一致
    const double tLong[4]  = {0.00, 1.00, 2.00, 3.00};   // 旧：3s 视野

    // 场景 A：目标静止在 0°，测量噪声 σ=0.05°（看数据包抖不抖）
    {
        ABTrackManager mgr(SeekerType::Image);
        mgr.Init(0.0, 0.0, 0);
        CSKalmanFilter kf;
        kf.Init(0.0);

        g_rng.seed(20260918u);
        qint64 t = 0;
        for (int k = 0; k < 1500; ++k)
        {
            const double z = gauss(0.05);
            t += 20;
            mgr.FeedData(t, z, 0.0);
            kf.Predict();
            kf.Update(z);
        }
        const sendExGuideData ab = mgr.GenAxisPacket(true, 30);
        std::printf("【A】目标静止(真值 0°) + 0.05° 噪声\n");
        std::printf("  α-β (1s 视野):      %+8.4f %+8.4f %+8.4f %+8.4f\n",
                    ab.angle1, ab.angle2, ab.angle3, ab.angle4);
        std::printf("  卡尔曼(旧 3s 视野):  %+8.4f %+8.4f %+8.4f %+8.4f\n",
                    kf.Extrapolate(tLong[0]), kf.Extrapolate(tLong[1]),
                    kf.Extrapolate(tLong[2]), kf.Extrapolate(tLong[3]));
        std::printf("  卡尔曼(新 1s 视野):  %+8.4f %+8.4f %+8.4f %+8.4f\n",
                    kf.Extrapolate(tShort[0]), kf.Extrapolate(tShort[1]),
                    kf.Extrapolate(tShort[2]), kf.Extrapolate(tShort[3]));
    }

    // 场景 B：目标 0.25Hz、±6° 摆动（视轴角速度峰值 9.4°/s，在转台 12°/s 能力内）
    {
        ABTrackManager mgr(SeekerType::Image);
        mgr.Init(0.0, 0.0, 0);
        CSKalmanFilter kf;
        kf.SetSigmaA(4.0);       // 用能跟机动的配置
        kf.Init(0.0);

        auto target = [](double sec) { return 6.0 * std::sin(2.0 * PI * 0.25 * sec); };

        g_rng.seed(20260918u);
        qint64 t = 0;
        for (int k = 0; k < 1500; ++k)
        {
            t += 20;
            const double sec = t / 1000.0;
            const double z = target(sec);
            mgr.FeedData(t, z, 0.0);
            kf.Predict();
            kf.Update(z);
        }
        const double now = t / 1000.0;
        const sendExGuideData ab = mgr.GenAxisPacket(true, 30);
        std::printf("\n【B】目标 0.25Hz、±6° 摆动（视轴速率峰值 9.4°/s，转台能力 12°/s 内），t=%.1fs 发的那一包\n", now);
        std::printf("  真值(τ=0.25/0.5/0.75/1s): %+8.4f %+8.4f %+8.4f %+8.4f\n",
                    target(now + 0.25), target(now + 0.50),
                    target(now + 0.75), target(now + 1.00));
        std::printf("  α-β (1s 视野):      %+8.4f %+8.4f %+8.4f %+8.4f\n",
                    ab.angle1, ab.angle2, ab.angle3, ab.angle4);
        std::printf("  卡尔曼(旧 3s 视野):  %+8.4f %+8.4f %+8.4f %+8.4f\n",
                    kf.Extrapolate(tLong[0]), kf.Extrapolate(tLong[1]),
                    kf.Extrapolate(tLong[2]), kf.Extrapolate(tLong[3]));
        std::printf("  卡尔曼(新 1s 视野):  %+8.4f %+8.4f %+8.4f %+8.4f\n",
                    kf.Extrapolate(tShort[0]), kf.Extrapolate(tShort[1]),
                    kf.Extrapolate(tShort[2]), kf.Extrapolate(tShort[3]));
    }
    std::printf("\n转台机械范围：方位 -100~100°，俯仰 -10~70°\n");
}

// 2.2 常值 0 测量：输出能否稳定在 0，加速度状态会不会自己长大
static void kf_zero_input()
{
    rule("2.2  卡尔曼：测量恒为 0（目标居中）——输出能否到 0、状态是否发散");

    CSKalmanFilter f;
    f.Init(10.0);  // 从 10° 起步，看能否收敛到 0

    Trace tr;
    double t = 0;
    for (int k = 0; k < 1500; ++k)  // 30 s
    {
        t += DT;
        f.Predict();
        f.Update(0.0);
        tr.t.push_back(t);
        tr.meas.push_back(0.0);
        tr.y.push_back(f.GetAngle());
        tr.v.push_back(f.GetRate());
        tr.a.push_back(f.GetAcc());
    }
    saveCsv("out/kf_zero_input.csv", "t,meas,angle,rate,acc", tr);

    const Analysis an = analyze(tr, constRef(tr.y.size(), 0.0), 0.001);
    printAnalysis("卡尔曼 10°→0°", an);

    double maxAbsAcc = 0, maxAbsRate = 0;
    for (size_t i = 0; i < tr.a.size(); ++i)
    {
        maxAbsAcc = std::max(maxAbsAcc, std::fabs(tr.a[i]));
        maxAbsRate = std::max(maxAbsRate, std::fabs(tr.v[i]));
    }
    std::printf("最大|角加速度|=%.6e°/s²  最大|角速度|=%.6e°/s  末尾(x0,x1,x2)=(%.3e, %.3e, %.3e)\n",
                maxAbsAcc, maxAbsRate, tr.y.back(), tr.v.back(), tr.a.back());
}

// 2.3 噪声测量：姿态估计 + 3s 外推是否振荡
static void kf_noise_and_extrapolation()
{
    rule("2.3  卡尔曼：目标静止在 0°、测量带噪声——输出振荡幅度与 3s 外推误差");

    const double sigmas[] = {0.02, 0.05, 0.20};
    for (double sg : sigmas)
    {
        CSKalmanFilter f;
        f.Init(0.0);

        Trace tr;
        double t = 0;
        double maxAbsX2 = 0;
        for (int k = 0; k < 1500; ++k)
        {
            t += DT;
            f.Predict();
            f.Update(gauss(sg));
            tr.t.push_back(t);
            tr.y.push_back(f.GetAngle());
            tr.v.push_back(f.GetRate());
            tr.a.push_back(f.GetAcc());
            maxAbsX2 = std::max(maxAbsX2, std::fabs(f.GetAcc()));
        }
        if (sg == 0.05)
            saveCsv("out/kf_noise.csv", "t,angle,rate,acc", tr);

        const Analysis an = analyze(tr, constRef(tr.y.size(), 0.0), 0.0);
        std::printf("σ=%.2f°: 输出 尾段std=%.6f° 极值[%+.4f,%+.4f] 全长min=%.4f max=%.4f 恰好0=%d 符号翻转=%d max|x2|=%.3e\n",
                    sg, an.tailStd, an.tailMin, an.tailMax, an.minY, an.maxY,
                    an.exactZeroCount, an.signChanges, maxAbsX2);
    }

    // 3s 外推在“目标静止”时本应恒等于真值 0
    {
        CSKalmanFilter f;
        f.Init(0.0);
        double t = 0;
        for (int k = 0; k < 1500; ++k)
        {
            t += DT;
            f.Predict();
            f.Update(gauss(0.05));
        }
        std::printf("噪声 σ=0.05° 时 4 点外推: E(0)=%.6f E(1)=%.6f E(2)=%.6f E(3)=%.6f （真值均为 0）\n",
                    f.Extrapolate(0.0), f.Extrapolate(1.0), f.Extrapolate(2.0), f.Extrapolate(3.0));
    }
}

// 2.4 阶跃 + 正弦跟踪，检查是否振荡
static void kf_dynamics()
{
    rule("2.4  卡尔曼：阶跃与正弦跟踪（0.5Hz、幅度 8°）");

    // 阶跃 0 → 5°
    {
        CSKalmanFilter f;
        f.Init(0.0);
        Trace tr;
        double t = 0;
        for (int k = 0; k < 500; ++k)
        {
            t += DT;
            f.Predict();
            f.Update(5.0);
            tr.t.push_back(t);
            tr.y.push_back(f.GetAngle());
            tr.v.push_back(f.GetRate());
            tr.a.push_back(f.GetAcc());
        }
        const Analysis an = analyze(tr, constRef(tr.y.size(), 5.0), 0.05);
        std::printf("阶跃到 5°: 超调=%+.6f° (%+.3f%%) 下冲=%+.6f° 极值[%.6f,%.6f] 符号翻转=%d ±0.05°稳定=%.3fs 尾段偏差=%+.6e\n",
                    an.overshoot, 100.0 * an.overshoot / 5.0, an.undershoot, an.minY, an.maxY,
                    an.signChanges, an.settleT, an.tailMean);
    }

    // 正弦跟踪
    {
        CSKalmanFilter f;
        f.Init(0.0);
        Trace tr;
        double t = 0;
        for (int k = 0; k < 1500; ++k)
        {
            t += DT;
            const double z = 8.0 * std::sin(2.0 * PI * 0.5 * t);
            f.Predict();
            f.Update(z);
            tr.t.push_back(t);
            tr.y.push_back(f.GetAngle());
            tr.v.push_back(f.GetRate());
            tr.a.push_back(f.GetAcc());
        }
        if (true)
            saveCsv("out/kf_sine.csv", "t,angle,rate,acc", tr);

        double maxErr = 0, sumSq = 0;
        double maxAbsE3 = 0;
        for (size_t i = 0; i < tr.t.size(); ++i)
        {
            const double gt = 8.0 * std::sin(2.0 * PI * 0.5 * tr.t[i]);
            const double e = tr.y[i] - gt;
            maxErr = std::max(maxErr, std::fabs(e));
            sumSq += e * e;
        }
        // 3s 外推误差（用尾部若干点统计）
        (void)maxAbsE3;
        std::printf("正弦跟踪: 跟踪误差 rms=%.6f° max=%.6f° 尾段(x0,x1,x2)=(%.4f, %.4f, %.4e)\n",
                    std::sqrt(sumSq / tr.t.size()), maxErr, tr.y.back(), tr.v.back(), tr.a.back());
        const double S = f.P[0][0] + R_MEAS;
        std::printf("           稳态 P 对角=(%.3e, %.3e, %.3e)  当前增益 K=(%.4f, %.4f, %.4f)\n",
                    f.P[0][0], f.P[1][1], f.P[2][2],
                    f.P[0][0] / S, f.P[1][0] / S, f.P[2][0] / S);
    }
}

// 2.5 长时运行：a_mean = x[2] 的自适应反馈是否会让加速度状态持续增长
static void kf_long_run()
{
    rule("2.5  卡尔曼：120s 长时运行（目标静止 + 测量噪声），检查状态是否漂移/发散");

    CSKalmanFilter f;
    f.Init(0.0);

    struct Snap { double t, x0, x1, x2; };
    std::vector<Snap> snaps;
    double t = 0;
    for (int k = 0; k < 6000; ++k)  // 120 s
    {
        t += DT;
        f.Predict();
        f.Update(gauss(0.05));
        if (k % 500 == 499)
            snaps.push_back({t, f.GetAngle(), f.GetRate(), f.GetAcc()});
    }
    std::printf("    t(s)        x0(角度)        x1(角速度)      x2(角加速度)   |x0|\n");
    for (const auto &s : snaps)
        std::printf("%8.1f  %14.6e  %14.6e  %14.6e  %10.4f\n", s.t, s.x0, s.x1, s.x2, std::fabs(s.x0));
}

// 2.6 配置对照：旧 Q / 新 Q / 新 Q+修正 U / 新 Q+修正 U+关闭均值前馈
struct KfCfg
{
    const char *name;
    bool legacyQ;
    double sigmaScale = 1.0;   // Q 按 σ_a² 缩放，用于看 σ_a 的影响
};

struct KfMetrics
{
    double zeroMin = 0, zeroMax = 0, zeroEnd = 0;
    int zeroFlips = 0;
    double noiseMaxAbs = 0, noiseTailStd = 0, noiseMaxAcc = 0;
    double stepOver = 0, stepSettle = 0;
    int stepFlips = 0;
    double sineRms = 0, sineMax = 0;
    double acc120 = 0;
    std::vector<double> zeroTrace, noiseTrace;   // 便于导出“改前/改后”曲线
};

static KfMetrics runKf(const KfCfg &cfg)
{
    KfMetrics m;
    auto prep = [&cfg](CSKalmanFilter &f) {
        if (cfg.legacyQ) setLegacyQ(f);
        if (cfg.sigmaScale != 1.0)
            f.SetSigmaA(SIGMA_A * cfg.sigmaScale);
    };

    g_rng.seed(20260918u);  // 每个配置喂同一串噪声，保证可比

    // (1) 常值 0 输入，从 10° 起步
    {
        CSKalmanFilter f;
        prep(f);
        f.Init(10.0);

        std::vector<double> ys;
        double prev = 0;
        for (int k = 0; k < 1500; ++k)
        {
            f.Predict();
            f.Update(0.0);
            const double e = f.GetAngle();
            if (!ys.empty() && ((e > 0 && prev < 0) || (e < 0 && prev > 0)))
                m.zeroFlips++;
            prev = e;
            ys.push_back(e);
        }
        const size_t st = ys.size() * 3 / 4;
        m.zeroMin = *std::min_element(ys.begin() + st, ys.end());
        m.zeroMax = *std::max_element(ys.begin() + st, ys.end());
        m.zeroEnd = ys.back();
        m.zeroTrace = ys;
    }

    // (2) 噪声 σ=0.05°，目标真值 0
    {
        CSKalmanFilter f;
        prep(f);
        f.Init(0.0);
        std::vector<double> ys;
        for (int k = 0; k < 1500; ++k)
        {
            f.Predict();
            f.Update(gauss(0.05));
            ys.push_back(f.GetAngle());
            m.noiseMaxAbs = std::max(m.noiseMaxAbs, std::fabs(f.GetAngle()));
            m.noiseMaxAcc = std::max(m.noiseMaxAcc, std::fabs(f.GetAcc()));
        }
        const size_t st = ys.size() * 3 / 4;
        double s1 = 0, s2 = 0;
        for (size_t i = st; i < ys.size(); ++i)
        {
            s1 += ys[i];
            s2 += ys[i] * ys[i];
        }
        const double n = static_cast<double>(ys.size() - st);
        const double mean = s1 / n;
        m.noiseTailStd = std::sqrt(std::max(0.0, s2 / n - mean * mean));
        m.noiseTrace = ys;
    }

    // (3) 阶跃 0 → 5°
    {
        CSKalmanFilter f;
        prep(f);
        f.Init(0.0);
        double best = 0;
        int lastOut = -1;
        double prevE = 0;
        for (int k = 0; k < 500; ++k)
        {
            f.Predict();
            f.Update(5.0);
            const double e = f.GetAngle() - 5.0;
            if (k > 0 && ((e > 0 && prevE < 0) || (e < 0 && prevE > 0)))
                m.stepFlips++;
            prevE = e;
            m.stepOver = std::max(m.stepOver, e);
            if (std::fabs(e) > 0.05)
                lastOut = k;
        }
        best = (lastOut + 1) < 500 ? (lastOut + 1) * DT : -1.0;
        m.stepSettle = best;
    }

    // (4) 正弦跟踪 0.5Hz、幅度 8°
    {
        CSKalmanFilter f;
        prep(f);
        f.Init(0.0);
        double t = 0, sumSq = 0;
        for (int k = 0; k < 1500; ++k)
        {
            t += DT;
            f.Predict();
            f.Update(8.0 * std::sin(2.0 * PI * 0.5 * t));
            const double e = f.GetAngle() - 8.0 * std::sin(2.0 * PI * 0.5 * t);
            sumSq += e * e;
            m.sineMax = std::max(m.sineMax, std::fabs(e));
        }
        m.sineRms = std::sqrt(sumSq / 1500.0);
    }

    // (5) 120s 长时运行（目标静止 + 噪声），看加速度状态漂移
    {
        CSKalmanFilter f;
        prep(f);
        f.Init(0.0);
        for (int k = 0; k < 6000; ++k)
        {
            f.Predict();
            f.Update(gauss(0.05));
        }
        m.acc120 = f.GetAcc();
    }
    return m;
}

static void kf_config_compare()
{
    rule("2.6  卡尔曼配置对照：旧 Q → 修正 Q → 修正 U → 关闭均值前馈");

    const KfCfg cfgs[] = {
        {"旧Q (修正前)",   true,  1.0},
        {"现状 σa=0.08",   false, 1.0},
        {"σa x10  (0.8)",  false, 10.0},
        {"σa x50  (4.0)",  false, 50.0},
        {"σa x100 (8.0)",  false, 100.0},
        {"σa x250 (20.0)", false, 250.0},
    };

    std::printf("%-24s %-21s %-11s %-10s %-10s %-10s %s\n",
                "配置", "常值0输出范围", "常值0末值", "噪声max|θ|", "正弦RMS", "阶跃超调", "120s末x2");
    std::printf("%-24s %-21s %-11s %-10s %-10s %-10s %s\n",
                "", "", "", "(σ=0.05°)", "(°)", "(°)", "(°/s²)");
    for (size_t ci = 0; ci < sizeof(cfgs) / sizeof(cfgs[0]); ++ci)
    {
        const KfCfg &c = cfgs[ci];
        const KfMetrics m = runKf(c);
        char rng[64];
        std::snprintf(rng, sizeof(rng), "[%+.3f,%+.3f]", m.zeroMin, m.zeroMax);
        std::printf("%-24s %-21s %-11.4f %-10.3f %-10.4f %-10.4f %+.4e\n",
                    c.name, rng, m.zeroEnd, m.noiseMaxAbs, m.sineRms, m.stepOver, m.acc120);
        std::printf("%-24s 符号翻转: 常值0=%d 阶跃=%d   噪声尾段std=%.4f°   阶跃±0.05°稳定=%s%.3fs\n",
                    "", m.zeroFlips, m.stepFlips, m.noiseTailStd,
                    m.stepSettle < 0 ? "未" : "", m.stepSettle < 0 ? 0.0 : m.stepSettle);

        // 导出旧/新两条曲线，供画图对比
        if (ci == 0 || ci == 1)
        {
            const char *tag = (ci == 0) ? "old" : "new";
            char file[64];
            std::snprintf(file, sizeof(file), "out/kf_cmp_zero_%s.csv", tag);
            FILE *fp = std::fopen(file, "w");
            if (fp)
            {
                std::fprintf(fp, "t,angle\n");
                for (size_t k = 0; k < m.zeroTrace.size(); ++k)
                    std::fprintf(fp, "%.6f,%.9f\n", (k + 1) * DT, m.zeroTrace[k]);
                std::fclose(fp);
            }
            std::snprintf(file, sizeof(file), "out/kf_cmp_noise_%s.csv", tag);
            fp = std::fopen(file, "w");
            if (fp)
            {
                std::fprintf(fp, "t,angle\n");
                for (size_t k = 0; k < m.noiseTrace.size(); ++k)
                    std::fprintf(fp, "%.6f,%.9f\n", (k + 1) * DT, m.noiseTrace[k]);
                std::fclose(fp);
            }
        }
    }
}

// 2.7 外推点限幅：目标大机动时，预测角会不会被推出视场/机械范围
static void kf_clamp_test()
{
    rule("2.7  卡尔曼：外推点限幅（±18° 视场半角）——大机动下的越界对比");

    struct R
    {
        std::vector<double> packet;   // t=3s 时的一组 4 点
        double maxAbs = 0;            // 全程 |预测角| 最大值
        int clipHits = 0;             // 触碰限幅的次数（限幅开启时才有意义）
        double truthAtPacket = 0;
    };

    auto run = [](bool clampOn) {
        R r;
        CSKalmanFilter f;
        f.SetSigmaA(SIGMA_A * 100.0);           // 能跟机动的配置
        if (clampOn)
            f.SetAngleLimit(-18.0, 18.0);
        f.Init(0.0);

        const double taus[4] = {0.25, 0.50, 0.75, 1.00};   // 1s 视野
        double t = 0;
        for (int k = 0; k < 500; ++k)  // 10 s：前 1.5s 以 30°/s 扫到 45°，之后停住
        {
            t += DT;
            const double truth = (t <= 1.5) ? 30.0 * t : 45.0;
            f.Predict();
            f.Update(truth);

            for (double tau : taus)
            {
                const double p = f.Extrapolate(tau);
                r.maxAbs = std::max(r.maxAbs, std::fabs(p));
                if (std::fabs(p) >= 18.0 - 1e-9)
                    r.clipHits++;
            }
            if (k == 149)  // t = 3.0s 的数据包（此时真值 45°）
            {
                for (double tau : taus)
                    r.packet.push_back(f.Extrapolate(tau));
                r.truthAtPacket = truth;
            }
        }
        return r;
    };

    const R off = run(false);
    const R on  = run(true);

    auto dump = [](const char *tag, const R &r) {
        std::printf("%-28s 4 点包 = %+8.3f %+8.3f %+8.3f %+8.3f   全程max|预测|=%.3f°\n",
                    tag, r.packet[0], r.packet[1], r.packet[2], r.packet[3], r.maxAbs);
    };
    dump("限幅关闭 (旧行为)", off);
    dump("限幅开启 (±18°)", on);
    std::printf("真值：t=3s 时目标在 %.1f°（已在视场外）；转台机械范围 方位 -100~100°、俯仰 -10~70°\n",
                off.truthAtPacket);
    std::printf("限幅开启时触碰 ±18° 的预测点次数 = %d / 2000（目标确实长时间在视场外）\n", on.clipHits);
}

// 2.8 σ_a / α 标定扫描
struct KfPerf
{
    double sineRms = 0, sineMax = 0;
    double sinePktRms = 0;            // 机动目标下 1s 数据包误差（对真值）
    double sinePkt1Rms = 0;           // 同上，但外推只用 θ+ωτ（去掉 τ² 项）
    double noiseMax = 0, noiseStd = 0;
    double pktMax = 0, pktRms = 0;    // 静止目标下 1s 数据包(4点)误差，真值 0
    double stepOver = 0, acc120 = 0;
};

static KfPerf measureKf(double alpha, double sigmaA)
{
    KfPerf p;
    g_rng.seed(20260918u);   // 每组参数喂同一串噪声

    // 正弦 0.5Hz、±8°
    {
        CSKalmanFilter f;
        f.SetAlpha(alpha);
        f.SetSigmaA(sigmaA);
        f.Init(0.0);
        const double taus[4] = {0.25, 0.50, 0.75, 1.00};
        double t = 0, sumSq = 0, pktSq = 0, pkt1Sq = 0;
        int pktN = 0;
        for (int k = 0; k < 1500; ++k)
        {
            t += DT;
            f.Predict();
            f.Update(8.0 * std::sin(2.0 * PI * 0.5 * t));
            const double e = f.GetAngle() - 8.0 * std::sin(2.0 * PI * 0.5 * t);
            sumSq += e * e;
            p.sineMax = std::max(p.sineMax, std::fabs(e));

            // 每 1s 取一包（跳过前 5s 过渡），与“真值在 +0.25/0.5/0.75/1s 的位置”比
            if (k >= 250 && (k % 50) == 49)
            {
                for (double tau : taus)
                {
                    const double truth = 8.0 * std::sin(2.0 * PI * 0.5 * (t + tau));
                    const double ee = f.Extrapolate(tau) - truth;
                    pktSq += ee * ee;

                    // 对照：仅一阶外推（不用角加速度）
                    const double e1 = (f.GetAngle() + f.GetRate() * tau) - truth;
                    pkt1Sq += e1 * e1;
                    pktN++;
                }
            }
        }
        p.sineRms = std::sqrt(sumSq / 1500.0);
        p.sinePktRms = pktN ? std::sqrt(pktSq / pktN) : 0.0;
        p.sinePkt1Rms = pktN ? std::sqrt(pkt1Sq / pktN) : 0.0;
    }

    // 静止目标 + 测量噪声 σ=0.05°
    {
        CSKalmanFilter f;
        f.SetAlpha(alpha);
        f.SetSigmaA(sigmaA);
        f.Init(0.0);
        std::vector<double> ys;
        double e3sq = 0;
        for (int k = 0; k < 1500; ++k)
        {
            f.Predict();
            f.Update(gauss(0.05));
            ys.push_back(f.GetAngle());
            p.noiseMax = std::max(p.noiseMax, std::fabs(f.GetAngle()));

            // 目标静止在 0°，理想下发值应为 0；这里量实际 4 点数据包的偏差
            const double pk[4] = {f.Extrapolate(0.25), f.Extrapolate(0.50),
                                  f.Extrapolate(0.75), f.Extrapolate(1.00)};
            for (double v : pk)
            {
                p.pktMax = std::max(p.pktMax, std::fabs(v));
                e3sq += v * v;
            }
        }
        p.pktRms = std::sqrt(e3sq / (1500.0 * 4.0));
        const size_t st = ys.size() * 3 / 4;
        double s1 = 0, s2 = 0;
        for (size_t i = st; i < ys.size(); ++i)
        {
            s1 += ys[i];
            s2 += ys[i] * ys[i];
        }
        const double n = static_cast<double>(ys.size() - st);
        const double mean = s1 / n;
        p.noiseStd = std::sqrt(std::max(0.0, s2 / n - mean * mean));
    }

    // 阶跃 0 → 5°
    {
        CSKalmanFilter f;
        f.SetAlpha(alpha);
        f.SetSigmaA(sigmaA);
        f.Init(0.0);
        for (int k = 0; k < 500; ++k)
        {
            f.Predict();
            f.Update(5.0);
            p.stepOver = std::max(p.stepOver, f.GetAngle() - 5.0);
        }
    }

    // 120s 静止目标 + 噪声：加速度状态漂移量
    {
        CSKalmanFilter f;
        f.SetAlpha(alpha);
        f.SetSigmaA(sigmaA);
        f.Init(0.0);
        for (int k = 0; k < 6000; ++k)
        {
            f.Predict();
            f.Update(gauss(0.05));
        }
        p.acc120 = f.GetAcc();
    }
    return p;
}

static void kf_sigma_sweep()
{
    rule("2.8  卡尔曼：σ_a / α 标定扫描（1s 视野，跟踪能力 vs 数据包平稳度）");

    const double sigmas[] = {0.08, 0.8, 4.0, 8.0, 20.0, 40.0};
    const double alphas[] = {0.1, 0.5, 1.0};

    std::printf("%-6s %-8s %-11s %-11s %-12s %-11s %-11s %-9s\n",
                "α", "σ_a", "机动跟踪rms", "机动包rms", "机动包rms", "静止包rms",
                "噪声max|θ|", "阶跃超调");
    std::printf("%-6s %-8s %-11s %-11s %-12s %-11s %-11s %-9s\n",
                "(1/s)", "(°/s²)", "(°)", "(二次)", "(一阶对照)", "(°)", "(σ=0.05°)", "(°)");

    for (double alpha : alphas)
    {
        for (double sg : sigmas)
        {
            const KfPerf p = measureKf(alpha, sg);
            std::printf("%-6.1f %-8.2f %-11.4f %-11.4f %-12.4f %-11.4f %-11.4f %-9.4f\n",
                        alpha, sg, p.sineRms, p.sinePktRms, p.sinePkt1Rms, p.pktRms,
                        p.noiseMax, p.stepOver);
            if (FILE *fp = std::fopen("out/kf_sigma_sweep.csv", alpha == alphas[0] && sg == sigmas[0] ? "w" : "a"))
            {
                if (alpha == alphas[0] && sg == sigmas[0])
                    std::fprintf(fp, "alpha,sigma,sine_rms,sine_pkt_rms,sine_pkt1_rms,noise_max,pkt_rms,pkt_max,step_over\n");
                std::fprintf(fp, "%.2f,%.2f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                             alpha, sg, p.sineRms, p.sinePktRms, p.sinePkt1Rms, p.noiseMax,
                             p.pktRms, p.pktMax, p.stepOver);
                std::fclose(fp);
            }
        }
        std::printf("\n");
    }

    // ── α-β 基线（当前生产路径），同两个场景，便于直接比较 ──
    double abStillSq = 0, abStillMax = 0, abSineSq = 0, abTrackSq = 0;
    int abStillN = 0, abSineN = 0, abTrackN = 0;
    const double taus[4] = {0.25, 0.50, 0.75, 1.00};

    {   // 静止目标 + 0.05° 噪声
        ABTrackManager mgr(SeekerType::Image);
        mgr.Init(0.0, 0.0, 0);
        g_rng.seed(20260918u);
        qint64 t = 0;
        for (int k = 0; k < 1500; ++k)
        {
            t += 20;
            mgr.FeedData(t, gauss(0.05), 0.0);
        }
        for (int pk = 0; pk < 10; ++pk)
        {
            for (int j = 0; j < 50; ++j)
            {
                t += 20;
                mgr.FeedData(t, gauss(0.05), 0.0);
            }
            const sendExGuideData p = mgr.GenAxisPacket(true, pk);
            const double v[4] = {p.angle1, p.angle2, p.angle3, p.angle4};
            for (double x : v)
            {
                abStillSq += x * x;
                abStillN++;
                abStillMax = std::max(abStillMax, std::fabs(x));
            }
        }
    }

    {   // 机动目标 0.5Hz ±8°
        ABTrackManager mgr(SeekerType::Image);
        mgr.Init(0.0, 0.0, 0);
        qint64 t = 0;
        for (int k = 0; k < 1500; ++k)
        {
            t += 20;
            const double sec = t / 1000.0;
            const double z = 8.0 * std::sin(2.0 * PI * 0.5 * sec);
            mgr.FeedData(t, z, 0.0);

            if (k >= 250)
            {
                const double e = mgr.AzTracker().angle() - z;
                abTrackSq += e * e;
                abTrackN++;
            }
            if (k >= 250 && (k % 50) == 49)
            {
                const sendExGuideData p = mgr.GenAxisPacket(true, k);
                const double v[4] = {p.angle1, p.angle2, p.angle3, p.angle4};
                for (int i = 0; i < 4; ++i)
                {
                    const double ee = v[i] - 8.0 * std::sin(2.0 * PI * 0.5 * (sec + taus[i]));
                    abSineSq += ee * ee;
                    abSineN++;
                }
            }
        }
    }

    std::printf("α-β 基线(现役路径)：机动跟踪rms=%.4f  机动包rms=%.4f  静止包rms=%.4f  静止包max=%.4f\n",
                std::sqrt(abTrackSq / abTrackN), std::sqrt(abSineSq / abSineN),
                std::sqrt(abStillSq / abStillN), abStillMax);
}

// ═══════════════════════════════════════════════════════════════════════════
// 三、下发链路：角度 → 转台指令帧（复刻 serialport_turntable_HEX.cpp 的编码）
// ═══════════════════════════════════════════════════════════════════════════

// 复刻 serialport_turntable_HEX.cpp:65 encode3BytesSigned（3 字节有符号，bit7 为符号位）
static void encode3BytesSigned(uint8_t dest[3], double value, double unit)
{
    int32_t raw = static_cast<int32_t>(std::round(value * unit));
    if (raw > 8388607) raw = 8388607;
    if (raw < -8388608) raw = -8388608;

    const bool negative = (raw < 0);
    const uint32_t absVal = static_cast<uint32_t>(negative ? -raw : raw);
    dest[0] = static_cast<uint8_t>((absVal >> 16) & 0x7F);
    if (negative) dest[0] |= 0x80;
    dest[1] = static_cast<uint8_t>((absVal >> 8) & 0xFF);
    dest[2] = static_cast<uint8_t>(absVal & 0xFF);
}

static std::string hex3(const uint8_t b[3])
{
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%02X %02X %02X", b[0], b[1], b[2]);
    return std::string(buf);
}

static void encode_probe()
{
    rule("3.1  编码：外引导角度 → 3 字节(0.0001°) 原始值，检查 0 能不能被发出");

    const double vals[] = {0.0, -0.0, 0.00003, -0.00003, 0.00005, -0.00005,
                           0.0001, -0.0001, 18.0, -18.0, 838.8607, 838.8608, 1000.0};
    std::printf("%14s %14s  %-11s %s\n", "角度(°)", "原始值(raw)", "3字节", "说明");
    for (double v : vals)
    {
        uint8_t b[3];
        encode3BytesSigned(b, v, 10000.0);
        const int32_t raw = static_cast<int32_t>(std::round(v * 10000.0));
        const bool satHi = raw > 8388607;
        const bool satLo = raw < -8388608;
        std::printf("%14.7f %14d  %-11s %s\n", v, raw, hex3(b).c_str(),
                    satHi || satLo ? "饱和(超量程)" : (raw == 0 ? "编码为 0" : ""));
    }
    std::printf("结论：raw=0 时 3 字节为 00 00 00，0 可以被正常下发；\n"
                "      |角度| < 0.00005° 也会四舍五入成 0（即零点附近存在 1e-4° 的量化死区）。\n");
}

// 3.2 完整链路：α-β 输出 4 点预测 + 转台当前角 → 编码帧
static void full_chain()
{
    rule("3.2  完整链路：α-β 预测 → 叠加转台当前角 → 编码，检查下发的 4 点角度");

    ABTrackManager mgr(SeekerType::Image);
    mgr.Init(0.0, 0.0, 0);

    auto show = [&](const char *tag, double curOuter, const sendExGuideData &pkt) {
        std::printf("%s（转台当前外框角=%.4f°）\n", tag, curOuter);
        const double raw4[4] = {pkt.angle1, pkt.angle2, pkt.angle3, pkt.angle4};
        double cmd[4];
        int32_t enc[4];
        for (int i = 0; i < 4; ++i)
        {
            cmd[i] = raw4[i] + curOuter;
            enc[i] = static_cast<int32_t>(std::round(cmd[i] * 10000.0));
            uint8_t b[3];
            encode3BytesSigned(b, cmd[i], 10000.0);
            std::printf("   预测%d=%+10.6f°  指令角=%+10.6f°  raw=%9d  bytes=%s\n",
                        i + 1, raw4[i], cmd[i], enc[i], hex3(b).c_str());
        }
        std::printf("   time字段=%u (秒)  \n", pkt.time);
    };

    // (a) 未初始化 / 还没收到第一帧
    {
        ABTrackManager fresh(SeekerType::Image);
        fresh.Init(0.0, 0.0, 0);
        const sendExGuideData pkt = fresh.GenAxisPacket(true, 1);
        show("(a) 尚无测量", 45.0, pkt);
    }

    // (b) 目标居中（测量恒为 0）
    {
        qint64 t = 0;
        for (int k = 0; k < 500; ++k)
        {
            t += 20;
            mgr.FeedData(t, 0.0, 0.0);
        }
        const sendExGuideData pkt = mgr.GenAxisPacket(true, 30);
        show("(b) 目标居中 10s 后", 45.0, pkt);
    }

    // (c) 目标有小角度偏移
    {
        qint64 t = 500 * 20;
        for (int k = 0; k < 200; ++k)
        {
            t += 20;
            mgr.FeedData(t, 0.5, 0.0);
        }
        const sendExGuideData pkt = mgr.GenAxisPacket(true, 40);
        show("(c) 目标偏 0.5°", 45.0, pkt);
    }

    // (d) 转台当前角为 0 时，居中目标应下发 0
    {
        const sendExGuideData pkt = mgr.GenAxisPacket(true, 41);
        show("(d) 同 (c) 但转台当前角=0°", 0.0, pkt);
    }
}

// ═══════════════════════════════════════════════════════════════════════════

int main()
{
    std::printf("滤波器预测实测（源码：kalman/AlphaBetaTracker.cpp、kalman/seekKalman.cpp）\n");

    // 一、α-β
    ab_uninitialized();
    ab_step_to_zero();
    ab_step_overshoot();
    ab_noise_at_zero();
    ab_clamp_windup();
    ab_guard_tests();
    ab_packet_monotonic();

    // 二、卡尔曼
    kf_dump_Q();
    kf_zero_input();
    kf_noise_and_extrapolation();
    kf_dynamics();
    kf_long_run();
    kf_config_compare();
    kf_clamp_test();
    kf_sigma_sweep();
    packet_compare();

    // 三、下发编码
    encode_probe();
    full_chain();

    std::printf("\n完成，明细数据已写入 out/*.csv\n");
    return 0;
}
