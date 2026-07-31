#include "./seekKalman.h"

CSKalmanFilter::CSKalmanFilter()
{
    a_mean = 0.0;
    // 初始化P为对角大协方差
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            P[i][j] = (i==j) ? 1.0 : 0.0;
    CalcModelMatrix();
}

void CSKalmanFilter::Init(double init_theta)
{
    x[0] = init_theta;
    x[1] = 0.0;
    x[2] = 0.0;
    a_mean = 0.0;
    // 重置协方差
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            P[i][j] = (i==j) ? 1.0 : 0.0;
}

void CSKalmanFilter::CalcModelMatrix()
{
    double a = ALPHA;
    double T = DT;
    double exp_aT = exp(-a * T);
    double a2 = a * a;
    double a3 = a2 * a;

    // ========== 计算状态转移矩阵 Φ(T) 论文式(5) ==========
    Phi[0][0] = 1.0;
    Phi[0][1] = T;
    Phi[0][2] = (-1 + a*T + exp_aT) / a2;

    Phi[1][0] = 0.0;
    Phi[1][1] = 1.0;
    Phi[1][2] = (1 - exp_aT) / a;

    Phi[2][0] = 0.0;
    Phi[2][1] = 0.0;
    Phi[2][2] = exp_aT;

    // ========== 计算输入矩阵 U(k) 论文式(6) ==========
    U[0] = (-T + 0.5*a*T*T + (1 - exp_aT)/a) / a;
    U[1] = T - (1 - exp_aT)/a;
    U[2] = 1 - exp_aT;

    // ========== 计算过程噪声Q矩阵 论文式(8) q11~q33 ==========
    double q11 = (1 - exp(-2*a*T) + 2*a*T + 2*a3*T*T*T/3 - 2*a2*T*T - 4*a*T*exp_aT) / (2*a3);
    double q12 = (exp(-2*a*T) + 1 - 2*exp_aT + 2*a*T*exp_aT - 2*a*T + a2*T*T) / (2*a2*a);
    double q13 = (1 - exp(-2*a*T) - 2*a*T*exp_aT) / (2*a3);
    double q22 = (4*exp(-2*a*T) - 3 - exp(-2*a*T) + 2*a*T) / (2*a3);
    double q23 = (1 - exp(-2*a*T)) / (2*a);
    double q33 = (1 - exp(-2*a*T)) / (2*a);

    double q_coeff = 2 * SIGMA_A * SIGMA_A;
    Q[0][0] = q_coeff * q11; Q[0][1] = q_coeff * q12; Q[0][2] = q_coeff * q13;
    Q[1][0] = q_coeff * q12; Q[1][1] = q_coeff * q22; Q[1][2] = q_coeff * q23;
    Q[2][0] = q_coeff * q13; Q[2][1] = q_coeff * q23; Q[2][2] = q_coeff * q33;
}

void CSKalmanFilter::Predict()
{
    // 1. 状态一步预测 X(k|k-1) = Φ*X(k-1|k-1) + U*ā(k)
    double x_pred[3] = {0,0,0};
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
            x_pred[i] += Phi[i][j] * x[j];
        x_pred[i] += U[i] * a_mean;
    }

    // 2. 协方差预测 P(k|k-1) = Φ*P*Φ^T + Q
    double P_pred[3][3] = {0};
    double Phi_P[3][3] = {0};
    // Phi * P
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                Phi_P[i][j] += Phi[i][k] * P[k][j];
    // Phi_P * Phi^T
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                P_pred[i][j] += Phi_P[i][k] * Phi[j][k];
    // + Q
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            P_pred[i][j] += Q[i][j];

    // 覆盖预测值
    for(int i=0; i<3; i++) x[i] = x_pred[i];
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++) P[i][j] = P_pred[i][j];

    // 自适应：当前预测加速度作为下一帧ā(k) 论文式(11)
    a_mean = x[2];
}

void CSKalmanFilter::Update(double z)
{
    // 残差 y = z - H*X_pred
    double y = z - H[0]*x[0] - H[1]*x[1] - H[2]*x[2];

    // S = H*P*H^T + R
    double HP[3] = {0};
    for(int i=0; i<3; i++)
        HP[i] = H[0]*P[0][i] + H[1]*P[1][i] + H[2]*P[2][i];
    double S = HP[0]*H[0] + HP[1]*H[1] + HP[2]*H[2] + R_MEAS;

    // 卡尔曼增益 K = P*H^T / S
    double K[3];
    for(int i=0; i<3; i++)
        K[i] = (P[i][0]*H[0] + P[i][1]*H[1] + P[i][2]*H[2]) / S;

    // 状态更新 X = X + K*y
    for(int i=0; i<3; i++)
        x[i] += K[i] * y;

    // 协方差更新 P = (I-KH)P
    double KH[3][3] = {0};
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            KH[i][j] = K[i] * H[j];
    double I_KH[3][3];
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            I_KH[i][j] = (i==j ? 1.0 : 0.0) - KH[i][j];
    double P_new[3][3] = {0};
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                P_new[i][j] += I_KH[i][k] * P[k][j];
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++) P[i][j] = P_new[i][j];
}

// 二次外推公式 θ(τ) = θ + dθ*τ + 0.5*ddθ*τ²
double CSKalmanFilter::Extrapolate(double tau)
{
    return x[0] + x[1] * tau + 0.5 * x[2] * tau * tau;
}

// ===================== 跟踪管理器实现 =====================
SeekerTrackManager::SeekerTrackManager()
{
    sys_time = 0.0;
    // is_blocked = false;
    // miss_dist = 0.0;
}

void SeekerTrackManager::Init(double az0, double el0)
{
    az_filter.Init(az0);
    el_filter.Init(el0);
    sys_time = 0.0;
}

void SeekerTrackManager::FeedSeekerData(double t, double az_meas, double el_meas)
{
    sys_time = t;
    // miss_dist = miss;
    // // 遮挡判定：脱靶量>5像元
    // is_blocked = (fabs(miss) > BLOCK_THRESH);

    // 方位轴滤波
    az_filter.Predict();
    // if(!is_blocked) az_filter.Update(az_meas);
    az_filter.Update(az_meas);

    // 俯仰轴滤波
    el_filter.Predict();
    // if(!is_blocked) el_filter.Update(el_meas);
    el_filter.Update(el_meas);
}

sendExGuideData SeekerTrackManager::GenAxisPacket(bool is_az)
{
    sendExGuideData pkt;
    pkt.time = sys_time;
    CSKalmanFilter& filt = is_az ? az_filter : el_filter;
    // 4个点：t0, t0+1, t0+2, t0+3
    pkt.angle1 = filt.Extrapolate(0.0);
    pkt.angle2 = filt.Extrapolate(1.0);
    pkt.angle3 = filt.Extrapolate(2.0);
    pkt.angle4 = filt.Extrapolate(3.0);
    return pkt;
}
