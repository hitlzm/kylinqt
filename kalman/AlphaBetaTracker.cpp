#include "AlphaBetaTracker.h"

#include <QtMath>
#include <algorithm>
#include <cmath>
namespace
{
    constexpr double MIN_DT = 0.001;     // 1ms
}

AlphaBetaTracker::AlphaBetaTracker(double alpha,
                                   double beta,
                                   AngleMode mode,
                                   double minAngle,
                                   double maxAngle)
{
    m_alpha = alpha;
    m_beta = beta;

    m_mode = mode;

    m_minAngle = minAngle;
    m_maxAngle = maxAngle;

    // 默认参数
    m_speedLimit = 180.0;      // °/s
    m_missThreshold = 20;      // 连续20次未收到测量认为Lost

    reset();
}

void AlphaBetaTracker::reset()
{
    m_initialized = false;
    m_lost = false;

    m_missCount = 0;

    m_lastTimestamp = 0;

    m_angle = 0.0;
    m_velocity = 0.0;
}

void AlphaBetaTracker::init(double angle,
                            qint64 timestampMs)
{
    m_initialized = true;

    m_lost = false;

    m_missCount = 0;

    m_lastTimestamp = timestampMs;

    m_angle = normalizeAngle(angle);

    m_velocity = 0.0;
}

bool AlphaBetaTracker::initialized() const
{
    return m_initialized;
}

bool AlphaBetaTracker::lost() const
{
    return m_lost;
}

double AlphaBetaTracker::angle() const
{
    return m_angle;
}

double AlphaBetaTracker::velocity() const
{
    return m_velocity;
}

qint64 AlphaBetaTracker::lastTimestamp() const
{
    return m_lastTimestamp;
}

int AlphaBetaTracker::missCount() const
{
    return m_missCount;
}

void AlphaBetaTracker::setAlpha(double alpha)
{
    if (alpha < 0.0) alpha = 0.0;
    if (alpha > 1.0) alpha = 1.0;

    m_alpha = alpha;
}

void AlphaBetaTracker::setBeta(double beta)
{
    if(beta < 0.0)
        beta = 0.0;

    m_beta = beta;
}

void AlphaBetaTracker::setSpeedLimit(double speed)
{
    if(speed < 0.0)
        speed = 0.0;

    m_speedLimit = speed;
}

void AlphaBetaTracker::setMissThreshold(int count)
{
    if(count < 1)
        count = 1;

    m_missThreshold = count;
}

void AlphaBetaTracker::setClampRange(double minAngle,
                                     double maxAngle)
{
    if(minAngle > maxAngle)
        std::swap(minAngle, maxAngle);

    m_minAngle = minAngle;
    m_maxAngle = maxAngle;
}

void AlphaBetaTracker::setAngleMode(AngleMode mode)
{
    m_mode = mode;
}

double AlphaBetaTracker::alpha() const
{
    return m_alpha;
}

double AlphaBetaTracker::beta() const
{
    return m_beta;
}

double AlphaBetaTracker::speedLimit() const
{
    return m_speedLimit;
}

int AlphaBetaTracker::missThreshold() const
{
    return m_missThreshold;
}

AngleMode AlphaBetaTracker::angleMode() const
{
    return m_mode;
}

bool AlphaBetaTracker::update(bool hasMeasurement,
                              double measurement,
                              qint64 timestampMs)
{
    //--------------------------------------
    // 第一次收到数据
    //--------------------------------------
    if (!m_initialized)
    {
        if (!hasMeasurement)
            return false;

        init(measurement, timestampMs);
        return true;
    }

    //--------------------------------------
    // 计算dt
    //--------------------------------------
    double dt =
            (timestampMs - m_lastTimestamp) / 1000.0;

    if (dt < MIN_DT)
        dt = MIN_DT;

    m_lastTimestamp = timestampMs;

    //--------------------------------------
    // 有测量
    //--------------------------------------
    if (hasMeasurement)
    {
        correct(measurement, dt);

        m_missCount = 0;

        m_lost = false;

        return true;
    }

    //--------------------------------------
    // 无测量
    //--------------------------------------
    predictOnly(dt);

    ++m_missCount;

    if (m_missCount >= m_missThreshold)
    {
        m_lost = true;
    }

    return false;
}

void AlphaBetaTracker::correct(double measurement,
                               double dt)
{
    //--------------------------------------
    // Prediction
    //--------------------------------------

    double predictAngle =
            normalizeAngle(
                m_angle +
                m_velocity * dt);

    double predictVelocity =
            m_velocity;

    //--------------------------------------
    // Innovation
    //--------------------------------------

    double residual =
            angleResidual(
                measurement,
                predictAngle);

    //--------------------------------------
    // Correction
    //--------------------------------------

    m_angle =
            normalizeAngle(
                predictAngle +
                m_alpha * residual);

    m_velocity =
            predictVelocity +
            m_beta * residual / dt;

    //--------------------------------------
    // Speed Limit
    //--------------------------------------

    if (m_velocity > m_speedLimit)
        m_velocity = m_speedLimit;

    if (m_velocity < -m_speedLimit)
        m_velocity = -m_speedLimit;
}

void AlphaBetaTracker::predictOnly(double dt)
{
    //--------------------------------------
    // 仅预测，不修正
    //--------------------------------------

    m_angle =
            normalizeAngle(
                m_angle +
                m_velocity * dt);
}

double AlphaBetaTracker::predict(double futureSec) const
{
    if (!m_initialized)
        return 0.0;

    if (futureSec < 0.0)
        futureSec = 0.0;

    return normalizeAngle(
                m_angle +
                m_velocity * futureSec);
}


double AlphaBetaTracker::normalizeAngle(double angle) const
{
    switch (m_mode)
    {
    case AngleMode::None:
        return angle;

    case AngleMode::Wrap180:
    {
        angle = std::fmod(angle + 180.0, 360.0);

        if (angle < 0.0)
            angle += 360.0;

        return angle - 180.0;
    }

    case AngleMode::Wrap360:
    {
        angle = std::fmod(angle, 360.0);

        if (angle < 0.0)
            angle += 360.0;

        return angle;
    }

    case AngleMode::Clamp:
    {
        if (angle < m_minAngle)
            return m_minAngle;

        if (angle > m_maxAngle)
            return m_maxAngle;

        return angle;
    }
    }

    return angle;
}

double AlphaBetaTracker::angleResidual(
        double measurement,
        double prediction) const
{
    switch (m_mode)
    {
    case AngleMode::Wrap180:
    {
        double residual = measurement - prediction;

        residual = std::fmod(residual + 180.0, 360.0);

        if (residual < 0.0)
            residual += 360.0;

        return residual - 180.0;
    }

    case AngleMode::Wrap360:
    {
        double residual = measurement - prediction;

        residual = std::fmod(residual + 180.0, 360.0);

        if (residual < 0.0)
            residual += 360.0;

        return residual - 180.0;
    }

    case AngleMode::Clamp:

        return measurement - prediction;

    case AngleMode::None:

        return measurement - prediction;
    }

    return measurement - prediction;
}

// ===================== Alpha-Beta 跟踪管理器实现 =====================
ABTrackManager::ABTrackManager(SeekerType type)
    : sys_time(0)
{
    double az_limit, el_limit;

    switch (type)
    {
    case SeekerType::Laser:
        az_limit = 20.0;
        el_limit = 20.0;
        break;
    case SeekerType::Image:
        az_limit = 18.0;
        el_limit = 18.0;
        break;
    case SeekerType::CCD_Wide:
        az_limit = 27.0;
        el_limit = 16.0;
        break;
    case SeekerType::CCD_Tele:
        az_limit = 1.33;
        el_limit = 0.75;
        break;
    }

    az_tracker.setAngleMode(AngleMode::Clamp);
    az_tracker.setClampRange(-az_limit, az_limit);

    el_tracker.setAngleMode(AngleMode::Clamp);
    el_tracker.setClampRange(-el_limit, el_limit);
}

void ABTrackManager::Init(double az0, double el0, qint64 timestampMs)
{
    az_tracker.init(az0, timestampMs);
    el_tracker.init(el0, timestampMs);
    sys_time = timestampMs;
}

void ABTrackManager::FeedData(qint64 t, double az_meas, double el_meas)
{
    sys_time = t;

    // 方位轴：Predict + Correct
    az_tracker.update(true, az_meas, t);

    // 俯仰轴：Predict + Correct
    el_tracker.update(true, el_meas, t);
}

sendExGuideData ABTrackManager::GenAxisPacket(bool is_az, qint64 packetTime)
{
    sendExGuideData pkt;
    pkt.time = static_cast<uint32_t>(packetTime);

    AlphaBetaTracker& tracker = is_az ? az_tracker : el_tracker;

    // 4个点：t+0.25, t+0.5, t+0.75, t+1.0 秒预测
    pkt.angle1 = tracker.predict(0.25);
    pkt.angle2 = tracker.predict(0.50);
    pkt.angle3 = tracker.predict(0.75);
    pkt.angle4 = tracker.predict(1.00);

    return pkt;
}

