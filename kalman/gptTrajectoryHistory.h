#pragma once
#include <deque>   // 必须，因为使用了 std::deque
#include <cstddef>

class TrajectoryHistory
{
public:

    explicit TrajectoryHistory(double historyTime = 1.0);

    void push(double time, double angle);

    void clear();

    size_t size() const;

    bool empty() const;

    const std::deque<TrackSample>& samples() const;

private:

    std::deque<TrackSample> m_samples;

    double m_historyTime;
};