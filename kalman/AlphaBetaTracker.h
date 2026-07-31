#ifndef ALPHABETATRACKER_H
#define ALPHABETATRACKER_H

#include <QtGlobal>

/**
 * @brief 角度处理模式
 */
enum class AngleMode
{
    None,       ///< 普通变量，不做任何处理
    Wrap180,    ///< [-180,180)
    Wrap360,    ///< [0,360)
    Clamp       ///< 限幅
};

/**
 * @brief Alpha-Beta 跟踪器
 *
 * 状态：
 *      angle      当前角度
 *      velocity   当前角速度(°/s)
 *
 * 适用于：
 *      导引头
 *      云台
 *      转台
 *      雷达角度跟踪
 *
 * 更新流程：
 *
 *      update(true , measurement , time)
 *          -> Predict
 *          -> Correct
 *
 *      update(false, 0 , time)
 *          -> PredictOnly
 *
 */
class AlphaBetaTracker
{
public:

    /**
     * @brief 构造函数
     *
     * @param alpha Alpha系数(0~1)
     * @param beta  Beta系数
     * @param mode  角度模式
     * @param minAngle Clamp模式下最小角度
     * @param maxAngle Clamp模式下最大角度
     */
    explicit AlphaBetaTracker(
            double alpha = 0.85,
            double beta = 0.05,
            AngleMode mode = AngleMode::Wrap180,
            double minAngle = -90.0,
            double maxAngle = 90.0);

    ~AlphaBetaTracker() = default;

    //------------------------------
    // 生命周期
    //------------------------------

    /**
     * @brief 初始化
     */
    void init(double angle,
              qint64 timestampMs);

    /**
     * @brief 重置
     */
    void reset();

    //------------------------------
    // 更新
    //------------------------------

    /**
     * @brief 更新Tracker
     *
     * @param hasMeasurement 是否有测量
     * @param measurement    当前测量
     * @param timestampMs    当前时间(ms)
     *
     * @return 是否成功更新
     */
    bool update(bool hasMeasurement,
                double measurement,
                qint64 timestampMs);

    //------------------------------
    // 查询
    //------------------------------

    bool initialized() const;

    bool lost() const;

    double angle() const;

    double velocity() const;

    qint64 lastTimestamp() const;

    int missCount() const;

    //------------------------------
    // 预测
    //------------------------------

    /**
     * @brief 预测futureSec秒后的角度
     */
    double predict(double futureSec) const;

    //------------------------------
    // 参数
    //------------------------------

    void setAlpha(double alpha);

    void setBeta(double beta);

    void setSpeedLimit(double speed);

    void setMissThreshold(int count);

    void setClampRange(double minAngle,
                       double maxAngle);

    void setAngleMode(AngleMode mode);

    double alpha() const;

    double beta() const;

    double speedLimit() const;

    int missThreshold() const;

    AngleMode angleMode() const;

private:

    //-----------------------------------
    // 内部算法
    //-----------------------------------

    /**
     * @brief 有测量更新
     */
    void correct(double measurement,
                 double dt);

    /**
     * @brief 无测量预测
     */
    void predictOnly(double dt);

    /**
     * @brief 根据模式规范化角度
     */
    double normalizeAngle(double angle) const;

    /**
     * @brief 计算角度残差
     *
     * Wrap180/Wrap360时自动处理跨360°
     */
    double angleResidual(double measurement,
                         double prediction) const;

private:

    //-----------------------------------
    // Tracker状态
    //-----------------------------------

    bool m_initialized;

    bool m_lost;

    int m_missCount;

    qint64 m_lastTimestamp;

    //-----------------------------------
    // Alpha-Beta状态
    //-----------------------------------

    double m_angle;

    double m_velocity;

    //-----------------------------------
    // 参数
    //-----------------------------------

    double m_alpha;

    double m_beta;

    double m_speedLimit;

    int m_missThreshold;

    //-----------------------------------
    // Angle
    //-----------------------------------

    AngleMode m_mode;

    double m_minAngle;

    double m_maxAngle;
};

#include "../serialport/serialport.h"  // sendExGuideData

// Alpha-Beta 跟踪管理器：同时管理方位+俯仰双轴，生成1s下发数据包

// 导引头类型（决定视场角限制范围）
enum class SeekerType
{
    Laser,      // 激光导引头：方位 ±20°,  俯仰 ±20°
    Image,      // 图像导引头：方位 ±18°,  俯仰 ±18°
    CCD_Wide,   // CCD 广角模式：方位 ±27°,  俯仰 ±16°
    CCD_Tele    // CCD 远焦模式：方位 ±1.33°, 俯仰 ±0.75°
};

class ABTrackManager
{
private:
    AlphaBetaTracker az_tracker;  // 方位轴跟踪器
    AlphaBetaTracker el_tracker;  // 俯仰轴跟踪器
    qint64 sys_time;              // 系统全局时间戳 (ms)

public:
    explicit ABTrackManager(SeekerType type = SeekerType::Image);

    // 初始化双轴初始角度和时间戳
    void Init(double az0, double el0, qint64 timestampMs);

    // 每次采样周期调用：接收实测角度并执行 Predict + Correct
    void FeedData(qint64 t, double az_meas, double el_meas);

    // 生成单轴1s跟踪数据包：4个预测角度，间隔0.25s；packetTime 直接写入 time
    sendExGuideData GenAxisPacket(bool is_az, qint64 packetTime = 0);

    // 参数访问（允许外部读写跟踪器参数）
    AlphaBetaTracker& AzTracker() { return az_tracker; }
    const AlphaBetaTracker& AzTracker() const { return az_tracker; }
    AlphaBetaTracker& ElTracker() { return el_tracker; }
    const AlphaBetaTracker& ElTracker() const { return el_tracker; }

    // 直接读写 sys_time（用于每轮预测时重置数据包时间戳）
    void SetSysTime(qint64 t) { sys_time = t; }
    qint64 SysTime() const { return sys_time; }
};

#endif // ALPHABETATRACKER_H