// ═══════════════════════════════════════════════════════════════════════════
// opencv/PixelKalmanTracker（cv::KalmanFilter，像素域 4 状态 CV 模型）风险实测
//
// 直接编译工程里的真实实现 opencv/pixelkalmantracker.cpp，不改动任何源码。
// 场景全部按调用方的真实用法构造：30fps、dt 来自 QElapsedTimer、
// 无检测时喂 (-1,-1)、取最高置信度检测框中心作为测量。
//
// 编译运行：见同目录 build_pixel.ps1
// ═══════════════════════════════════════════════════════════════════════════

#include <cstdio>
#include <cmath>
#include <random>
#include <vector>
#include <string>
#include <algorithm>

#include <opencv2/core.hpp>
#include "opencv/pixelkalmantracker.h"

static std::mt19937 g_rng(20260918u);

static double gauss(double sigma)
{
    std::normal_distribution<double> d(0.0, sigma);
    return d(g_rng);
}

static const float DT30 = 1.0f / 30.0f;   // 30fps 帧间隔
static const double PI_ = 3.14159265358979323846;

static void rule(const char *t)
{
    std::printf("\n══════════════════════════════════════════════════════════════════════\n");
    std::printf("%s\n", t);
    std::printf("══════════════════════════════════════════════════════════════════════\n");
}

// 目标：从 (300,240) 出发、以 40px/s 横向匀速运动（图像 1280×720 内的典型跟踪）
static double trueXAt(double t) { return 300.0 + 40.0 * t; }
static double trueYAt(double t) { return 240.0 + 15.0 * t; }

// ─────────────────────────────────────────────────────────────────────────
// 1. 基线：匀速目标 + 像素噪声
// ─────────────────────────────────────────────────────────────────────────
static void baseline()
{
    rule("1  基线：匀速目标(40px/s) + 像素噪声 σ=3px，30fps，20s");

    PixelKalmanTracker tk;
    double t = 0, sumSq = 0, sum = 0;
    int n = 0;
    double maxErr = 0;

    for (int k = 0; k < 600; ++k)
    {
        const float dt = (k == 0) ? 0.0f : DT30;
        t += DT30;
        const double zx = trueXAt(t) + gauss(3.0);
        const double zy = trueYAt(t) + gauss(3.0);
        tk.feed(static_cast<float>(zx), static_cast<float>(zy), dt);

        if (k > 60)   // 跳过收敛段
        {
            const double e = tk.filteredX() - trueXAt(t);
            sumSq += e * e;
            sum += e;
            n++;
            maxErr = std::max(maxErr, std::fabs(e));
        }
    }
    std::printf("稳态跟踪误差: rms=%.3f px  均值(滞后)=%+.3f px  max=%.3f px\n",
                std::sqrt(sumSq / n), sum / n, maxErr);
    std::printf("→ 噪声 σ=3px 经滤波后剩 %.3f px，即抖动压缩到 %.0f%%\n",
                std::sqrt(sumSq / n), 100.0 * std::sqrt(sumSq / n) / 3.0);
}

// ─────────────────────────────────────────────────────────────────────────
// 2. 单帧误检（YOLO 偶发误检 / 短暂选错目标）
// ─────────────────────────────────────────────────────────────────────────
static void single_glitch()
{
    rule("2  单帧误检：跟踪稳定后某一帧检测框突然偏 400px，下一帧恢复正常");

    PixelKalmanTracker tk;
    double t = 0;
    double peak = 0, recoverT = -1, glitchT = 0;
    bool glitched = false;

    for (int k = 0; k < 600; ++k)
    {
        const float dt = (k == 0) ? 0.0f : DT30;
        t += DT30;
        double zx = trueXAt(t) + gauss(3.0);

        if (k == 90)                    // 3s 处来一帧误检
        {
            zx += 400.0;
            glitched = true;
            glitchT = t;
        }
        tk.feed(static_cast<float>(zx), static_cast<float>(trueYAt(t) + gauss(3.0)), dt);

        const double e = tk.filteredX() - trueXAt(t);
        if (glitched)
        {
            peak = std::max(peak, std::fabs(e));
            if (recoverT < 0 && k > 90 && std::fabs(e) < 5.0)
                recoverT = t - glitchT;
        }
    }
    std::printf("输出被拉偏的最大值 = %.2f px（误检 400px）\n", peak);
    std::printf("恢复到 ±5px 以内用时 = %s%.3f s\n",
                recoverT < 0 ? "未恢复 " : "", recoverT < 0 ? 0.0 : recoverT);
    std::printf("→ 无任何门限/连续性校验，误检被当成真测量直接吸收\n");
}

// ─────────────────────────────────────────────────────────────────────────
// 3. 连续误检：短时间锁到错误目标
// ─────────────────────────────────────────────────────────────────────────
static void false_target()
{
    rule("3  连续误检：连续 10 帧（0.33s）检测到偏 300px 的错误目标");

    PixelKalmanTracker tk;
    double t = 0;
    double peakDuring = 0, peakAfter = 0, recoverT = -1, offT = 0;
    bool off = false;

    for (int k = 0; k < 900; ++k)
    {
        const float dt = (k == 0) ? 0.0f : DT30;
        t += DT30;
        double zx = trueXAt(t) + gauss(3.0);
        if (k >= 90 && k < 100)
        {
            zx += 300.0;
            if (!off) { off = true; offT = t; }
        }
        tk.feed(static_cast<float>(zx), static_cast<float>(trueYAt(t) + gauss(3.0)), dt);

        const double e = tk.filteredX() - trueXAt(t);
        if (off && k < 100) peakDuring = std::max(peakDuring, std::fabs(e));
        if (off && k >= 100)
        {
            peakAfter = std::max(peakAfter, std::fabs(e));
            if (recoverT < 0 && std::fabs(e) < 5.0)
                recoverT = t - offT;
        }
    }
    std::printf("误检期间的输出偏差 max = %.2f px\n", peakDuring);
    std::printf("误检结束后(含反向超调) max = %.2f px，回到 ±5px 用时 = %s%.3f s\n",
                peakAfter, recoverT < 0 ? "未恢复 " : "", recoverT < 0 ? 0.0 : recoverT);
    std::printf("→ 误检持续 0.33s 就足以把速度和位置都带偏，且会产生反向超调\n");
}

// ─────────────────────────────────────────────────────────────────────────
// 4. 遮挡/丢失：只剩外推
// ─────────────────────────────────────────────────────────────────────────
static void occlusion()
{
    rule("4  遮挡丢失：稳定跟踪后连续 25 帧无检测（喂 -1）");

    PixelKalmanTracker tk;
    double t = 0;
    std::printf("丢失帧数  输出有效性  滤波输出X    真值X     偏差(px)   已丢失时间(s)\n");
    for (int k = 0; k < 130; ++k)
    {
        const float dt = (k == 0) ? 0.0f : DT30;
        t += DT30;
        const bool lost = (k >= 90);
        const float zx = lost ? -1.0f : static_cast<float>(trueXAt(t) + gauss(3.0));
        const float zy = lost ? -1.0f : static_cast<float>(trueYAt(t) + gauss(3.0));
        tk.feed(zx, zy, dt);

        if (k >= 90 && (k - 90) % 4 == 0)
        {
            std::printf("%6d    %-10s %9.2f  %9.2f  %+9.2f   %.3f\n",
                        k - 90, tk.isValid() ? "有效" : "无效(-1)",
                        tk.filteredX(), trueXAt(t),
                        tk.filteredX() - trueXAt(t), tk.missTime());
        }
    }
    std::printf("→ 丢失 %d 帧后自动 reset，输出 -1（下游按“无目标”处理）\n", 16);
}

// ─────────────────────────────────────────────────────────────────────────
// 5. dt 异常：同一毫秒两帧 / 卡顿后的大 dt
// ─────────────────────────────────────────────────────────────────────────
static void dt_anomaly()
{
    rule("5  dt 异常：dt=0（同一毫秒两帧）与 dt=2s（卡顿）");

    {   // dt=0
        PixelKalmanTracker tk;
        double t = 0;
        tk.feed(static_cast<float>(trueXAt(0.0)), static_cast<float>(trueYAt(0.0)), 0.0f);
        float prev = tk.filteredX();
        float stepNormal = 0, stepZero = 0;
        for (int k = 1; k <= 60; ++k)
        {
            t += DT30;
            tk.feed(static_cast<float>(trueXAt(t)), static_cast<float>(trueYAt(t)), DT30);
            if (k == 30) stepNormal = tk.filteredX() - prev;
            prev = tk.filteredX();
        }
        // 再喂一帧 dt=0（真实中同一毫秒内到达两帧）
        const float before = tk.filteredX();
        tk.feed(static_cast<float>(trueXAt(t)), static_cast<float>(trueYAt(t)), 0.0f);
        stepZero = tk.filteredX() - before;
        std::printf("正常帧位置增量 = %+.4f px，dt=0 帧位置增量 = %+.4f px（按 33ms 兜底推进）\n",
                    stepNormal, stepZero);
    }

    {   // 大 dt
        PixelKalmanTracker tk;
        double t = 0;
        tk.feed(static_cast<float>(trueXAt(0.0)), static_cast<float>(trueYAt(0.0)), 0.0f);
        for (int k = 1; k <= 90; ++k)
        {
            t += DT30;
            tk.feed(static_cast<float>(trueXAt(t)), static_cast<float>(trueYAt(t)), DT30);
        }
        const float before = tk.filteredX();
        t += 2.0;   // 卡顿 2 秒
        tk.feed(static_cast<float>(trueXAt(t)), static_cast<float>(trueYAt(t)), 2.0f);
        std::printf("卡顿 2s 后单帧位置跳变 = %+.2f px（一次预测把 2s 的速度外推全吃进来）\n",
                    tk.filteredX() - before);
    }
}

// ─────────────────────────────────────────────────────────────────────────
// 6. 长时运行：数值健康（float32 协方差）
// ─────────────────────────────────────────────────────────────────────────
// 复刻 pixelkalmantracker.cpp 的 predict()/correct() 取值，仅用于观察内部协方差
// （原类的 m_kf 是私有成员，外部读不到）。参数与源码完全一致。
struct Replica
{
    cv::KalmanFilter kf;

    explicit Replica(int type)
        : kf(4, 2, 0, type)
    {
        cv::setIdentity(kf.measurementMatrix);
        kf.measurementNoiseCov = cv::Mat::eye(2, 2, type) * 25.0;
        kf.transitionMatrix = cv::Mat::eye(4, 4, type);
        kf.processNoiseCov = cv::Mat::eye(4, 4, type) * 1e-4;
    }

    void init(float x0, float y0)
    {
        const int t = kf.transitionMatrix.type();
        cv::Mat st = (cv::Mat_<double>(4, 1) << x0, y0, 0.0, 0.0);
        cv::Mat cv0 = (cv::Mat_<double>(4, 4) <<
            100.0, 0.0,   0.0,    0.0,
            0.0,   100.0, 0.0,    0.0,
            0.0,   0.0,   1000.0, 0.0,
            0.0,   0.0,   0.0,    1000.0);
        st.convertTo(kf.statePost, t);
        kf.statePre = kf.statePost.clone();
        cv0.convertTo(kf.errorCovPost, t);
    }

    void predict(float dt)
    {
        if (dt <= 0.0f) dt = 0.033f;
        const int t = kf.transitionMatrix.type();

        const double dt2 = dt * dt, dt3 = dt2 * dt, dt4 = dt3 * dt;
        const double q = 1e-2;
        cv::Mat Phi = (cv::Mat_<double>(4, 4) <<
            1.0, 0.0, dt,  0.0,
            0.0, 1.0, 0.0, dt,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);
        cv::Mat Q = (cv::Mat_<double>(4, 4) <<
            dt4 / 4.0 * q, 0.0,           dt3 / 2.0 * q, 0.0,
            0.0,           dt4 / 4.0 * q, 0.0,           dt3 / 2.0 * q,
            dt3 / 2.0 * q, 0.0,           dt2 * q,       0.0,
            0.0,           dt3 / 2.0 * q, 0.0,           dt2 * q);
        Phi.convertTo(kf.transitionMatrix, t);
        Q.convertTo(kf.processNoiseCov, t);
        kf.predict();
    }

    void correct(float mx, float my)
    {
        cv::Mat meas = (cv::Mat_<float>(2, 1) << mx, my);
        meas.convertTo(meas, kf.statePost.type());
        kf.correct(meas);
    }

    double posX() const
    {
        return (kf.statePost.type() == CV_32F) ? kf.statePost.at<float>(0)
                                               : kf.statePost.at<double>(0);
    }
    double cov(int i, int j) const
    {
        return (kf.errorCovPost.type() == CV_32F) ? kf.errorCovPost.at<float>(i, j)
                                                  : kf.errorCovPost.at<double>(i, j);
    }
};

static void long_run()
{
    rule("6  长时运行数值健康：float32 协方差（20 万帧 ≈ 1.85 小时 @30fps，慢速目标）");

    // 慢速目标（速度变化 <1px/s），把机动因素排除，只看数值精度的影响
    auto sx = [](double t) { return 640.0 + 60.0 * std::sin(2.0 * PI_ * t / 300.0); };
    auto sy = [](double t) { return 360.0 + 40.0 * std::sin(2.0 * PI_ * t / 400.0); };

    struct R { double rmsEarly = 0, rmsLate = 0; double minEig = 0, k00 = 0, k11 = 0; bool finite = true; };

    auto run = [&](int type, int frames) {
        R r;
        g_rng.seed(20260918u);
        Replica rp(type);
        double t = 0;
        rp.init(static_cast<float>(sx(0.0)), static_cast<float>(sy(0.0)));

        double sumSqE = 0, sumSqL = 0;
        int nE = 0, nL = 0;
        for (int k = 1; k <= frames; ++k)
        {
            t += DT30;
            rp.predict(DT30);
            rp.correct(static_cast<float>(sx(t) + gauss(3.0)),
                       static_cast<float>(sy(t) + gauss(3.0)));

            const double x = rp.posX();
            const double e = x - sx(t);
            if (!std::isfinite(x) || !std::isfinite(rp.cov(0, 0)))
                r.finite = false;

            if (k <= 3000) { sumSqE += e * e; nE++; }
            if (k > frames - 3000) { sumSqL += e * e; nL++; }
        }
        r.rmsEarly = std::sqrt(sumSqE / nE);
        r.rmsLate  = std::sqrt(sumSqL / nL);

        // 协方差健康度：最小特征值（负值 = 不再是半正定）
        cv::Mat cov, ev;
        rp.kf.errorCovPost.convertTo(cov, CV_64F);
        cv::eigen(cov, ev);
        r.minEig = ev.at<double>(3);
        const double p00 = rp.cov(0, 0);
        r.k00 = p00 / (p00 + 25.0);          // 位置增益
        const double p11 = rp.cov(2, 2);
        r.k11 = p11 / (p11 + 25.0);          // 速度增益（近似）
        return r;
    };

    const R f32 = run(CV_32F, 200000);
    const R f64 = run(CV_64F, 200000);

    std::printf("%-12s %-14s %-14s %-16s %-10s %s\n",
                "精度", "前3000帧rms", "末3000帧rms", "协方差最小特征值", "位置增益K0", "数值有限?");
    std::printf("%-12s %-14.4f %-14.4f %-16.3e %-10.5f %s\n",
                "CV_32F(工程)", f32.rmsEarly, f32.rmsLate, f32.minEig, f32.k00,
                f32.finite ? "是" : "出现 NaN/Inf *");
    std::printf("%-12s %-14.4f %-14.4f %-16.3e %-10.5f %s\n",
                "CV_64F(对照)", f64.rmsEarly, f64.rmsLate, f64.minEig, f64.k00,
                f64.finite ? "是" : "出现 NaN/Inf *");
    std::printf("→ 同一条轨迹、同一串噪声，只差矩阵精度\n");
}

// ─────────────────────────────────────────────────────────────────────────
// 6b. 变速跟随能力：速度反向阶跃（真实类，比较默认 q 与放大后的 q）
// ─────────────────────────────────────────────────────────────────────────
static void velocity_step()
{
    rule("6b 变速跟随：目标速度从 +33px/s 反转为 -33px/s（真实类，q = 过程噪声）");

    // 10s 正向 +25px/s，随后反向 -25px/s（全程留在 1280 宽图像内）
    const double V1 = 25.0, V2 = -25.0, T0 = 10.0;
    auto truth = [&](double t) {
        return (t <= T0) ? (300.0 + V1 * t) : (300.0 + V1 * T0 + V2 * (t - T0));
    };

    const float qs[] = {1e-2f, 1.0f, 100.0f};
    std::printf("%-12s %-11s %-11s %-11s %-11s %-11s %-11s\n",
                "q(过程噪声)", "t=+1s", "t=+2s", "t=+5s", "t=+10s", "t=+20s", "最大偏差");
    for (float q : qs)
    {
        PixelKalmanTracker tk;
        tk.setProcessNoiseVel(q);
        g_rng.seed(20260918u);

        double t = 0, e1 = 0, e2 = 0, e5 = 0, e10 = 0, e20 = 0, peak = 0;
        tk.feed(static_cast<float>(truth(0.0)), 240.0f, 0.0f);
        for (int k = 1; k <= 900; ++k)    // 30s
        {
            t += DT30;
            tk.feed(static_cast<float>(truth(t) + gauss(3.0)), 240.0f, DT30);
            const double e = std::fabs(tk.filteredX() - truth(t));
            if (t > T0) peak = std::max(peak, e);
            const double d = t - T0;
            if (std::fabs(d - 1.0) < DT30 / 2)  e1 = e;
            if (std::fabs(d - 2.0) < DT30 / 2)  e2 = e;
            if (std::fabs(d - 5.0) < DT30 / 2)  e5 = e;
            if (std::fabs(d - 10.0) < DT30 / 2) e10 = e;
            if (std::fabs(d - 20.0) < DT30 / 2) e20 = e;
        }
        std::printf("%-12.3g %-11.2f %-11.2f %-11.2f %-11.2f %-11.2f %-11.2f\n",
                    q, e1, e2, e5, e10, e20, peak);
    }
    std::printf("→ 表内为 |滤波输出 - 真值| (px)。默认 q=0.01 时目标反向 20s 后仍有明显偏差\n");
    std::printf("  （q 的物理含义：每秒允许速度改变 sqrt(q·1s) px/s；0.01 即 0.1px/s，远小于真机动）\n");
}

// ─────────────────────────────────────────────────────────────────────────
// 7. 参数灵敏度：R（测量噪声）设错会怎样
// ─────────────────────────────────────────────────────────────────────────
static void noise_param()
{
    rule("7  参数灵敏度：R 设错时的表现（真实像素噪声 σ=3px → R 应为 9）");
    const float rs[] = {0.0f, 1.0f, 9.0f, 25.0f, 400.0f};
    for (float r : rs)
    {
        PixelKalmanTracker tk;
        tk.setMeasureNoise(r);
        g_rng.seed(20260918u);
        double t = 0, sumSq = 0, sum = 0;
        int n = 0;
        for (int k = 0; k < 600; ++k)
        {
            const float dt = (k == 0) ? 0.0f : DT30;
            t += DT30;
            tk.feed(static_cast<float>(trueXAt(t) + gauss(3.0)),
                    static_cast<float>(trueYAt(t) + gauss(3.0)), dt);
            if (k > 60)
            {
                const double e = tk.filteredX() - trueXAt(t);
                sumSq += e * e;
                sum += e;
                n++;
            }
        }
        const double rms = std::sqrt(sumSq / n);
        std::printf("R=%-6.1f → 稳态 rms=%s  滞后=%+.3f px   末值=%s\n",
                    r,
                    std::isfinite(rms) ? (std::to_string(rms).substr(0, 7)).c_str() : "NaN/Inf *",
                    sum / n,
                    std::isfinite(tk.filteredX()) ? (std::to_string(tk.filteredX()).substr(0, 8)).c_str()
                                                  : "NaN/Inf *");
    }
    std::printf("→ R 偏大：更信模型、更平滑但滞后更大；R 偏小：跟得紧但噪声穿透（默认 25 对应 σ=5px）\n");
}

// ═══════════════════════════════════════════════════════════════════════════
// 8. 门限方案验证：在复刻滤波器上实现“预测位置 + 马氏门限 + 重捕”三种策略对比
//    （复刻参数与 pixelkalmantracker.cpp 完全一致，仅用于验证方案与选阈值）
// ═══════════════════════════════════════════════════════════════════════════
struct GateCfg
{
    const char *name;
    bool  useGate;          // 是否启用门限
    float gamma;            // 门限倍数（3σ → 9）
    int   reacquireFrames;  // 连续被拒多少帧后强制重捕（0 = 不重捕）
};

struct Cand { float x = 0, y = 0, conf = 0; bool valid = false; };

// 与 PixelKalmanTracker 内部一致的“一步预测位置 + 位置方向创新方差”
static void predictAndVar(const Replica &rp, float dt, float &px, float &py, float &varXY)
{
    const cv::Mat &P = rp.kf.errorCovPost;
    const float p00 = P.at<float>(0, 0), p02 = P.at<float>(0, 2), p22 = P.at<float>(2, 2);
    const float p11 = P.at<float>(1, 1), p13 = P.at<float>(1, 3), p33 = P.at<float>(3, 3);
    const float q = 1e-2f;

    px = rp.kf.statePost.at<float>(0) + dt * rp.kf.statePost.at<float>(2);
    py = rp.kf.statePost.at<float>(1) + dt * rp.kf.statePost.at<float>(3);

    const float varX = p00 + 2.0f * dt * p02 + dt * dt * p22 + q * dt * dt * dt * dt / 4.0f;
    const float varY = p11 + 2.0f * dt * p13 + dt * dt * p33 + q * dt * dt * dt * dt / 4.0f;
    varXY = 0.5f * (varX + varY) + 25.0f;      // + R（两轴同方差，取均值）
}

struct GateRes
{
    double maxErr = 0;        // 全程最大 |输出-真值|
    double errAfter = 0;      // 干扰结束后 3s 内最大偏差
    double recoverT = -1;     // 回到 ±5px 用时
    int    rejFrames = 0;     // 所有候选都被拒 → 本帧按丢失处理
    int    trueRejects = 0;   // 真目标落在门限外（误拒）
    int    falseAccepts = 0;  // 选中了假目标（误纳）
    int    frames = 0;
};

// scenario: 0=单帧误检400px  1=连续10帧假目标(更高置信度)  2=正常跟踪  3=快速目标200px/s
static GateRes runGate(int scenario, const GateCfg &cfg,
                       double *outRejectRate = nullptr,
                       const char *csvPath = nullptr)
{
    GateRes r;
    FILE *fp = csvPath ? std::fopen(csvPath, "w") : nullptr;
    if (fp)
        std::fprintf(fp, "t,truth,cand_true,cand_false,pred,gate_half,out,has_false\n");

    g_rng.seed(20260918u);
    Replica rp(CV_32F);
    rp.init(static_cast<float>(trueXAt(0.0)), static_cast<float>(trueYAt(0.0)));

    double t = 0;
    int rejects = 0;
    int trueRejected = 0, truePresent = 0;
    double glitchT = -1;

        const int N = (scenario == 3) ? 300 : 900;
    for (int k = 1; k <= N; ++k)
    {
        t += DT30;
        const double tx = (scenario == 3) ? (300.0 + 200.0 * t) : trueXAt(t);
        const double ty = trueYAt(t);

        // ── 生成本帧候选检测 ──
        Cand cands[2];
        int n = 0;
        const bool trueDetected = !(scenario == 2 && false) ;   // 真目标一直能被检出
        if (trueDetected)
        {
            cands[n].x = static_cast<float>(tx + gauss(3.0));
            cands[n].y = static_cast<float>(ty + gauss(3.0));
            cands[n].conf = 0.80f;
            cands[n].valid = true;
            n++;
        }
        bool glitchNow = false;
        if (scenario == 0 && k == 90) glitchNow = true;
        if (scenario == 1 && k >= 90 && k < 100) glitchNow = true;
        if (glitchNow)
        {
            cands[n].x = static_cast<float>(tx + (scenario == 0 ? 400.0 : 300.0) + gauss(3.0));
            cands[n].y = static_cast<float>(ty + gauss(3.0));
            cands[n].conf = 0.90f;      // 误检置信度更高
            cands[n].valid = true;
            n++;
            if (glitchT < 0) glitchT = t;
        }

        // ── 选目标（现状：只看置信度；方案：门限内取最高置信度）──
        int pick = -1;
        float predX = 0, gateHalf = 0;
        bool falsePresent = false;
        if (n > 1) falsePresent = true;
        if (n > 0)
        {
            int best = 0;
            for (int i = 1; i < n; ++i)
                if (cands[i].conf > cands[best].conf) best = i;

            if (!cfg.useGate)
            {
                pick = best;
            }
            else
            {
                float px, py, var;
                predictAndVar(rp, DT30, px, py, var);
                const float lim = cfg.gamma * cfg.gamma * var;
                predX = px;
                gateHalf = cfg.gamma * std::sqrt(var);
                bool inGate[2] = {false, false};
                int bestIn = -1;
                for (int i = 0; i < n; ++i)
                {
                    const float dx = cands[i].x - px, dy = cands[i].y - py;
                    inGate[i] = (dx * dx + dy * dy <= lim);
                    if (inGate[i] && (bestIn < 0 || cands[i].conf > cands[bestIn].conf))
                        bestIn = i;
                }
                // 真目标(cands[0])是否被误拒
                if (!inGate[0])
                    r.trueRejects++;

                if (bestIn >= 0)
                {
                    pick = bestIn;
                    rejects = 0;
                }
                else
                {
                    r.rejFrames++;
                    rejects++;
                    if (cfg.reacquireFrames > 0 && rejects >= cfg.reacquireFrames)
                    {
                        pick = best;            // 强制重捕：用最高置信度框重建滤波器
                        rp.init(cands[best].x, cands[best].y);
                        rejects = 0;
                    }
                }
            }
        }

        // ── 喂入 ──
        if (pick >= 0)
        {
            // 选中了假目标？（与真值差 > 50px）
            if (std::hypot(cands[pick].x - tx, cands[pick].y - ty) > 50.0)
                r.falseAccepts++;
            rp.predict(DT30);
            rp.correct(cands[pick].x, cands[pick].y);
        }
        else
        {
            rp.predict(DT30);       // 无有效测量 → 只预测（对应 feed(-1,-1)）
        }

        const double e = std::fabs(rp.posX() - tx);
        r.maxErr = std::max(r.maxErr, e);

        if (fp)
        {
            // 记录门限侧的量（现状策略没有门限，这里用同参数的预测值仅作参考）
            float px2 = 0, py2 = 0, var2 = 0;
            predictAndVar(rp, DT30, px2, py2, var2);
            (void)py2;
            std::fprintf(fp, "%.4f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%d\n",
                         t, tx,
                         static_cast<double>(cands[0].x),
                         falsePresent ? static_cast<double>(cands[1].x) : 0.0,
                         static_cast<double>(px2),
                         static_cast<double>(cfg.gamma * std::sqrt(var2)),
                         rp.posX(), falsePresent ? 1 : 0);
        }

        if (glitchT > 0 && t > glitchT)
        {
            r.errAfter = std::max(r.errAfter, e);
            if (r.recoverT < 0 && t > glitchT + 0.2 && e < 5.0)
                r.recoverT = t - glitchT;
        }
    }
    r.frames = N;
    if (outRejectRate) *outRejectRate = 100.0 * r.trueRejects / N;
    if (fp) std::fclose(fp);
    return r;
}

static void gate_sim()
{
    rule("8 门限方案验证（复刻参数：q=0.01, R=25；三种策略对比）");

    const GateCfg cfgs[] = {
        {"现状: 只看置信度",      false, 3.0f, 0},
        {"门限3σ, 拒绝=丢失",     true,  3.0f, 0},
        {"门限3σ + 3帧重捕",      true,  3.0f, 3},
    };
    const char *scen[] = {"单帧误检400px", "连续10帧假目标(+300px,更高置信度)", "正常跟踪", "快速目标200px/s"};

    for (int s = 0; s < 4; ++s)
    {
        std::printf("\n场景: %s\n", scen[s]);
        std::printf("%-22s %-10s %-12s %-12s %-10s %-10s %s\n",
                    "策略", "最大偏差", "干扰后最大", "恢复±5px",
                    "误拒率", "误纳帧", "拒帧数");
        for (const GateCfg &c : cfgs)
        {
            double rej = 0;
            const GateRes r = runGate(s, c, &rej);
            char rec[24];
            if (r.recoverT < 0) std::snprintf(rec, sizeof(rec), "%s", "未恢复");
            else std::snprintf(rec, sizeof(rec), "%.3f s", r.recoverT);
            std::printf("%-22s %-10.2f %-12.2f %-12s %-10.2f %-10d %d\n",
                        c.name, r.maxErr, r.errAfter, rec, rej, r.falseAccepts, r.rejFrames);
        }
    }

    // 阈值扫描：门限倍数对“误拒率”和“误检拦截”的影响（正常跟踪场景）
    // 另外导出“连续 10 帧假目标”场景的逐帧数据，用于画图说明机理
    runGate(1, GateCfg{"现状", false, 3.0f, 0}, nullptr, "out/pixel_gate_nogate.csv");
    runGate(1, GateCfg{"门限", true,  4.0f, 0}, nullptr, "out/pixel_gate_on.csv");

    std::printf("\n门限倍数扫描（正常跟踪 30s，只有真目标）:\n");
    std::printf("%-10s %-14s %-14s %s\n", "γ(σ)", "正常跟踪误拒率", "快速目标误拒率", "说明");
    const float gammas[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    for (float gm : gammas)
    {
        GateCfg c{"", true, gm, 0};
        double rej = 0, rejFast = 0;
        runGate(2, c, &rej);
        runGate(3, c, &rejFast);
        std::printf("%-10.1f %-14.2f %-14.2f %s\n", gm, rej, rejFast,
                    rej < 0.5 ? "可用" : (rej < 2.0 ? "偏紧" : "过紧，会频繁断跟踪"));
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// 9. 用工程真实类的新接口复测（验证 pixelkalmantracker 的实现，而非复刻）
//    复刻 StreamProcessor 改动后的选框逻辑：门限内取最高置信度，全拒则喂 -1
// ═══════════════════════════════════════════════════════════════════════════
static void real_gate_check()
{
    rule("9  真实 PixelKalmanTracker::gateMeasurement 复测（工程实现，γ=4）");

    struct Res { double maxErr = 0, recoverT = -1; int falseAccept = 0, frames = 0; };

    auto run = [](bool useGate, int scenario) {
        Res r;
        g_rng.seed(20260918u);
        PixelKalmanTracker tk;
        tk.setGateGamma(4.0f);

        double t = 0;
        const int N = 900;
        for (int k = 1; k <= N; ++k)
        {
            t += DT30;
            const float dt = (k == 1) ? 0.0f : DT30;
            const double tx = trueXAt(t), ty = trueYAt(t);

            // 候选：真目标(conf 0.8) 常在；假目标(conf 0.9) 只在干扰窗口出现
            Cand c[2];
            int n = 0;
            c[n++] = {static_cast<float>(tx + gauss(3.0)), static_cast<float>(ty + gauss(3.0)), 0.80f, true};
            bool glitch = (scenario == 0) ? (k == 90)
                                          : (k >= 90 && k < 100);
            if (glitch)
                c[n++] = {static_cast<float>(tx + (scenario == 0 ? 400.0 : 300.0) + gauss(3.0)),
                          static_cast<float>(ty + gauss(3.0)), 0.90f, true};

            // 选目标
            int pick = -1;
            int best = 0;
            for (int i = 1; i < n; ++i)
                if (c[i].conf > c[best].conf) best = i;
            if (!useGate || !tk.isValid())
            {
                pick = best;
            }
            else
            {
                int bestIn = -1;
                for (int i = 0; i < n; ++i)
                    if (tk.gateMeasurement(c[i].x, c[i].y, dt) &&
                        (bestIn < 0 || c[i].conf > c[bestIn].conf))
                        bestIn = i;
                pick = bestIn;                 // -1 → 本帧无有效测量
            }

            if (pick >= 0)
                tk.feed(c[pick].x, c[pick].y, dt);
            else
                tk.feed(-1.0f, -1.0f, dt);

            const double e = std::fabs(tk.filteredX() - tx);
            r.maxErr = std::max(r.maxErr, e);
            if (pick >= 0 && std::hypot(c[pick].x - tx, c[pick].y - ty) > 50.0)
                r.falseAccept++;
            if (k >= 90 && k > 90 && r.recoverT < 0 && e < 5.0)
                r.recoverT = t - 90 * DT30;
        }
        r.frames = N;
        return r;
    };

    std::printf("%-26s %-12s %-12s %s\n", "策略", "最大偏差", "恢复±5px", "误纳帧数");
    for (int s = 0; s < 2; ++s)
    {
        const char *name = (s == 0) ? "单帧误检 400px" : "连续10帧假目标 +300px";
        const Res a = run(false, s);
        const Res b = run(true, s);
        std::printf("场景 %s\n", name);
        std::printf("  %-24s %-12.2f %-12s %d\n", "现状(只看置信度)", a.maxErr,
                    a.recoverT < 0 ? "未恢复" : (std::to_string(a.recoverT).substr(0, 6) + " s").c_str(),
                    a.falseAccept);
        std::printf("  %-24s %-12.2f %-12s %d\n", "门限γ=4(本工程实现)", b.maxErr,
                    b.recoverT < 0 ? "未恢复" : (std::to_string(b.recoverT).substr(0, 6) + " s").c_str(),
                    b.falseAccept);
        const float gx = 0, gy = 0; (void)gx; (void)gy;
        // 顺带打印门限半径，便于现场核对
        PixelKalmanTracker probe;
        probe.feed(640.0f, 360.0f, 0.0f);
        for (int i = 0; i < 30; ++i) probe.feed(640.0f + i * 1.3f, 360.0f, DT30);
        std::printf("  (稳态门限半径 ≈ %.1f px, 创新方差 ≈ %.1f px², γ=%.1f)\n",
                    probe.gateGamma() * std::sqrt(probe.innovationVar(DT30)),
                    probe.innovationVar(DT30), probe.gateGamma());
    }
}

int main()
{
    std::printf("PixelKalmanTracker / cv::KalmanFilter 风险实测（真实源码 + OpenCV 4.2）\n");
    baseline();
    single_glitch();
    false_target();
    occlusion();
    dt_anomaly();
    long_run();
    velocity_step();
    noise_param();
    gate_sim();
    real_gate_check();
    std::printf("\n完成\n");
    return 0;
}
