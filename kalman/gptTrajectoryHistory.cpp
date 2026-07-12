#include "gptTrajectoryHistory.h"

TrajectoryHistory::TrajectoryHistory(double historyTime)
    : m_historyTime(historyTime)
{
}

void TrajectoryHistory::push(double time,
                             double angle)
{
    m_samples.push_back({time, angle});

    while (!m_samples.empty())
    {
        double dt =
            time -
            m_samples.front().time;

        if (dt <= m_historyTime)
            break;

        m_samples.pop_front();
    }
}

void TrajectoryHistory::clear()
{
    m_samples.clear();
}

size_t TrajectoryHistory::size() const
{
    return m_samples.size();
}

bool TrajectoryHistory::empty() const
{
    return m_samples.empty();
}

const std::deque<TrackSample>&
TrajectoryHistory::samples() const
{
    return m_samples;
}