// pixelkalmantracker.cpp — 像素坐标卡尔曼跟踪器实现
//
// 使用 OpenCV cv::KalmanFilter，4 状态匀速模型：
//   状态向量:  [x, y, vx, vy]^T
//   转移矩阵:  x'  = x + dt*vx
//             y'  = y + dt*vy
//             vx' = vx
//             vy' = vy
//   过程噪声:  离散白噪声加速度模型 (DWNA)
//   观测矩阵:  直接观测 x, y

#include "pixelkalmantracker.h"
#include <opencv2/core.hpp>

// ══════════════════════════════════════════════════════════════════════════════
// 构造
// ══════════════════════════════════════════════════════════════════════════════

PixelKalmanTracker::PixelKalmanTracker()
    : m_kf(4, 2, 0, CV_32F)   // dynamParams=4, measureParams=2, controlParams=0
{
    // ── 观测矩阵 H：直接观测 x, y ──
    //     [1 0 0 0]
    //     [0 1 0 0]
    cv::setIdentity(m_kf.measurementMatrix);

    // ── 测量噪声 R ──
    setMeasureNoise(m_measureNoise);

    // 转移矩阵 Φ 和过程噪声 Q 含 dt，在 predict() 中每帧更新
    // 先设模板（dt 相关元素填 0，predict 时覆盖）：
    m_kf.transitionMatrix = (cv::Mat_<float>(4, 4) <<
        1.0f, 0.0f, 0.0f, 0.0f,   // x  = x + dt*vx
        0.0f, 1.0f, 0.0f, 0.0f,   // y  = y + dt*vy
        0.0f, 0.0f, 1.0f, 0.0f,   // vx = vx
        0.0f, 0.0f, 0.0f, 1.0f);  // vy = vy

    // 初始过程噪声（dt 未知时用默认值）
    m_kf.processNoiseCov = cv::Mat::eye(4, 4, CV_32F) * 1e-4f;
}

// ══════════════════════════════════════════════════════════════════════════════
// 参数设置
// ══════════════════════════════════════════════════════════════════════════════

void PixelKalmanTracker::setMeasureNoise(float r)
{
    m_measureNoise = r;
    m_kf.measurementNoiseCov = (cv::Mat_<float>(2, 2) <<
        r, 0.0f,
        0.0f, r);
}

void PixelKalmanTracker::reset()
{
    m_state      = State::UNINIT;
    m_missCount  = 0;
    m_missTimer  = 0.0f;
    m_filteredX  = -1.0f;
    m_filteredY  = -1.0f;
}

// ══════════════════════════════════════════════════════════════════════════════
// 内部：卡尔曼初始化（首次检测到来时调用）
// ══════════════════════════════════════════════════════════════════════════════

void PixelKalmanTracker::initKalman(float x0, float y0)
{
    // ── 初始状态：位置 = 测量值，速度 = 0 ──
    m_kf.statePost = (cv::Mat_<float>(4, 1) << x0, y0, 0.0f, 0.0f);

    // ── 初始误差协方差：位置不确定度小，速度不确定度大 ──
    m_kf.errorCovPost = (cv::Mat_<float>(4, 4) <<
        100.0f, 0.0f,    0.0f,    0.0f,
        0.0f,   100.0f,  0.0f,    0.0f,
        0.0f,   0.0f,    1000.0f, 0.0f,
        0.0f,   0.0f,    0.0f,    1000.0f);
}

// ══════════════════════════════════════════════════════════════════════════════
// 内部：预测步（每帧必执行）
// ══════════════════════════════════════════════════════════════════════════════

void PixelKalmanTracker::predict(float dt)
{
    if (dt <= 0.0f) {
        dt = 0.033f;    // 保护：默认 ~30fps
    }

    // ── 更新转移矩阵 Φ 中的 dt 项 ──
    //     使用 ptr<float>() 直接访问连续内存：
    //     col:  0    1    2    3
    //     row0: 1,   0,  dt,   0    → 索引 2
    //     row1: 0,   1,   0,  dt    → 索引 7
    //     row2: 0,   0,   1,   0
    //     row3: 0,   0,   0,   1
    float *tm = m_kf.transitionMatrix.ptr<float>(0);
    tm[2] = dt;    // Φ(0,2) = dt
    tm[7] = dt;    // Φ(1,3) = dt

    // ── 更新过程噪声协方差 Q（离散白噪声加速度模型）──
    //     q = 加速度功率谱密度
    //     Q = q * [dt⁴/4·I₂   dt³/2·I₂ ]
    //             [dt³/2·I₂   dt²·I₂   ]
    const float dt2 = dt * dt;
    const float dt3 = dt2 * dt;
    const float dt4 = dt3 * dt;
    const float q  = m_processNoiseVel;

    float *qptr = m_kf.processNoiseCov.ptr<float>(0);
    qptr[0]  = dt4 / 4.0f * q;   // Q(0,0)  x-pos
    qptr[1]  = 0.0f;             // Q(0,1)
    qptr[2]  = dt3 / 2.0f * q;   // Q(0,2)  x-pos ↔ vx
    qptr[3]  = 0.0f;             // Q(0,3)
    qptr[4]  = 0.0f;             // Q(1,0)
    qptr[5]  = dt4 / 4.0f * q;   // Q(1,1)  y-pos
    qptr[6]  = 0.0f;             // Q(1,2)
    qptr[7]  = dt3 / 2.0f * q;   // Q(1,3)  y-pos ↔ vy
    qptr[8]  = dt3 / 2.0f * q;   // Q(2,0)  vx ↔ x-pos
    qptr[9]  = 0.0f;             // Q(2,1)
    qptr[10] = dt2 * q;          // Q(2,2)  vx
    qptr[11] = 0.0f;             // Q(2,3)
    qptr[12] = 0.0f;             // Q(3,0)
    qptr[13] = dt3 / 2.0f * q;   // Q(3,1)  vy ↔ y-pos
    qptr[14] = 0.0f;             // Q(3,2)
    qptr[15] = dt2 * q;          // Q(3,3)  vy

    m_kf.predict();
}

// ══════════════════════════════════════════════════════════════════════════════
// 内部：量测修正（有检测时执行）
// ══════════════════════════════════════════════════════════════════════════════

void PixelKalmanTracker::correct(float mx, float my)
{
    cv::Mat meas = (cv::Mat_<float>(2, 1) << mx, my);
    m_kf.correct(meas);
}

// ══════════════════════════════════════════════════════════════════════════════
// 核心：每帧主入口
// ══════════════════════════════════════════════════════════════════════════════

void PixelKalmanTracker::feed(float measX, float measY, float dt)
{
    // ════════════════════════════════════════════════════════════
    // 情况 A：UNINIT → 首次检测到来
    //          初始化 KF，输出原始测量值，不执行 predict+correct
    // ════════════════════════════════════════════════════════════
    if (m_state == State::UNINIT) {
        if (measX >= 0.0f && measY >= 0.0f) {
            initKalman(measX, measY);
            m_filteredX  = measX;        // 首帧不滤波
            m_filteredY  = measY;
            m_state      = State::TRACKING;
            m_missCount  = 0;
            m_missTimer  = 0.0f;
        }
        // 无检测 → 保持 UNINIT，输出 (-1, -1)
        return;
    }

    // ════════════════════════════════════════════════════════════
    // 情况 B：已初始化（TRACKING 或 LOST）
    //          每帧先预测，有测量再修正
    // ════════════════════════════════════════════════════════════
    predict(dt);
    m_missTimer += dt;

    if (measX >= 0.0f && measY >= 0.0f) {
        // ── 有测量 → 量测修正 ──
        correct(measX, measY);
        m_filteredX  = m_kf.statePost.at<float>(0);
        m_filteredY  = m_kf.statePost.at<float>(1);
        m_missCount  = 0;
        m_missTimer  = 0.0f;
        m_state      = State::TRACKING;
    } else {
        // ── 无测量 → 仅预测，输出运动模型外推值 ──
        m_filteredX  = m_kf.statePost.at<float>(0);
        m_filteredY  = m_kf.statePost.at<float>(1);
        m_missCount++;

        if (m_missCount > m_missThreshold) {
            // 连续丢失超过阈值 → 重置，输出 (-1, -1)
            reset();
        } else {
            m_state = State::LOST;
        }
    }
}
