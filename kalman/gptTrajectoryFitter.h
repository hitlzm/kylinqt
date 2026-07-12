#pragma once

#include <Eigen/Dense>
#include "TrajectoryHistory.h"

class TrajectoryFitter
{
public:

    bool fit(const std::deque<TrackSample>& samples);

    double predict(double futureTime) const;

    double angle() const;

    double velocity() const;

    double acceleration() const;

private:

    Eigen::Vector3d m_coef;
    double m_lastTime = 0.0;
};