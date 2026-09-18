// pixelkalmantracker.h — 像素坐标卡尔曼跟踪器
//
// 对 YOLO 检测输出的目标中心像素坐标 (m_centerX, m_centerY) 进行卡尔曼滤波，
// 消除检测框帧间抖动和短暂遮挡/误检引起的跳变。
//
// 状态机：
//   UNINIT   → 首次检测到来，init() 后直接输出原始测量值（首帧不滤波）
//   TRACKING → 每帧 predict() + 有检测时 correct()
//   LOST     → 短时丢失，仅 predict() 外推；超过阈值 → 重置为 UNINIT
//
// 集成方式（StreamProcessor 侧）：
//   float dt = m_kalmanTimer.restart() / 1000.0f;   // 帧间隔（秒）
//   m_tracker.feed(rawCenterX, rawCenterY, dt);      // 喂入原始坐标
//   m_centerX = m_tracker.filteredX();                // 读回滤波坐标
//   m_centerY = m_tracker.filteredY();
//
// 内部封装：
//   - cv::KalmanFilter（4 状态 CV 模型：[x, y, vx, vy]）
//   - 连续丢失计数 + 超阈值自动重置
//   - 首帧不滤波（直接输出测量值）

#ifndef PIXELKALMANTRACKER_H
#define PIXELKALMANTRACKER_H

#include <opencv2/video/tracking.hpp>

class PixelKalmanTracker
{
public:
    enum class State {
        UNINIT,    // 未初始化：初始状态 / 长时间丢失后重置
        TRACKING,  // 正常跟踪：每帧 predict + correct
        LOST       // 短时丢失：仅 predict 外推
    };

    PixelKalmanTracker();

    // ═══════════════════════════════════════════════════════════════
    // 核心接口
    // ═══════════════════════════════════════════════════════════════

    /// 每帧喂入 YOLO 检测的目标中心坐标
    /// @param measX  原始测量 X 坐标（-1 表示本帧无检测）
    /// @param measY  原始测量 Y 坐标（-1 表示本帧无检测）
    /// @param dt     距上一帧的时间间隔（秒），首帧传入 0 即可
    void feed(float measX, float measY, float dt);

    // ═══════════════════════════════════════════════════════════════
    // 查询
    // ═══════════════════════════════════════════════════════════════

    float  filteredX() const { return m_filteredX; }
    float  filteredY() const { return m_filteredY; }
    State  state()     const { return m_state; }
    bool   isValid()   const { return m_state != State::UNINIT; }
    int    missCount() const { return m_missCount; }
    float  missTime()  const { return m_missTimer; }

    // ═══════════════════════════════════════════════════════════════
    // 参数调节
    // ═══════════════════════════════════════════════════════════════

    /// 连续丢失帧数阈值（超过后重置为 UNINIT），默认 15
    void setMissThreshold(int frames)  { m_missThreshold = frames; }

    /// 速度过程噪声（控制平滑程度，越大越信任测量），默认 1e-2
    void setProcessNoiseVel(float q)   { m_processNoiseVel = q; }

    /// 测量噪声方差（px²，YOLO bbox 抖动程度），默认 25
    void setMeasureNoise(float r);

    /// 重置为 UNINIT 状态
    void reset();

    // ═══════════════════════════════════════════════════════════════
    // 测量门限（用于挡掉高置信度误检，只读判断，不改变滤波器状态）
    //
    //   ν = 测量 - 预测位置
    //   S = 位置方向预测方差(ΦPΦᵀ) + R
    //   判据: |ν|² / S ≤ γ²      γ 默认 4
    //
    // 调用方应先用本接口筛选候选检测框，再从中取置信度最高者；若全部被拒，
    // 按“无检测”喂入 (-1,-1)，由 tracker 自身的丢失/复位逻辑恢复。
    // 实测（tests/filter_sim/pixel_risk.cpp）：γ=3 起不会误拒真目标，
    // γ=2 对 200px/s 的快速目标误拒率 99%，故不建议小于 3。
    // ═══════════════════════════════════════════════════════════════

    /// 一步预测位置（不改变滤波器状态）：px = x + dt·vx
    void  predictPosition(float dt, float &px, float &py) const;

    /// 位置方向上的创新方差 S = P_pred(pos) + R ，单位 px²
    float innovationVar(float dt) const;

    /// 测量门限判断：true = 与当前航迹统计相容，可接受
    bool  gateMeasurement(float mx, float my, float dt, float gamma) const;
    bool  gateMeasurement(float mx, float my, float dt) const
    {
        return gateMeasurement(mx, my, dt, m_gateGamma);
    }

    /// 门限倍数 γ（单位：σ），默认 4.0；小于 3 会误拒真目标
    void  setGateGamma(float gamma) { m_gateGamma = (gamma > 0.0f) ? gamma : 4.0f; }
    float gateGamma() const { return m_gateGamma; }

private:
    void initKalman(float x0, float y0);
    void predict(float dt);
    void correct(float mx, float my);

    cv::KalmanFilter m_kf;            // 4 状态 [x, y, vx, vy], 2 观测 [x, y]

    State  m_state    = State::UNINIT;
    int    m_missCount = 0;           // 连续丢失帧数
    float  m_missTimer = 0.0f;        // 累计丢失时间（秒）
    int    m_missThreshold = 15;      // 丢失帧数阈值

    float  m_filteredX = -1.0f;       // 滤波后 X
    float  m_filteredY = -1.0f;       // 滤波后 Y

    float  m_processNoiseVel = 1e-2f; // 速度过程噪声 q
    float  m_measureNoise    = 25.0f; // 测量噪声方差 R
    float  m_gateGamma       = 4.0f;  // 测量门限倍数 γ（σ）
};

#endif // PIXELKALMANTRACKER_H
