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

#endif // ALPHABETATRACKER_H