#include "gptTrajectoryFitter.h"

bool TrajectoryFitter::fit(
        const std::deque<TrackSample>& samples)
{
    if(samples.size()<6)
        return false;

    const int n =
        static_cast<int>(samples.size());

    Eigen::MatrixXd A(n,3);

    Eigen::VectorXd y(n);

    m_lastTime =
        samples.back().time;

    // 为了数值稳定，把最后一个采样点作为时间原点
    for(int i=0;i<n;i++)
    {
        double t =
            samples[i].time - m_lastTime;

        A(i,0)=1.0;

        A(i,1)=t;

        A(i,2)=t*t;

        y(i)=samples[i].angle;
    }

    m_coef =
        A.colPivHouseholderQr().solve(y);

    return true;
}

double TrajectoryFitter::predict(
        double futureTime) const
{
    double t = futureTime;

    return
            m_coef(0)
          + m_coef(1)*t
          + m_coef(2)*t*t;
}

double TrajectoryFitter::velocity() const
{
    return m_coef(1);
}

double TrajectoryFitter::acceleration() const
{
    return
        2.0*m_coef(2);
}

