#include "./seekKalman.h"

CSKalmanFilter::CSKalmanFilter()
{
    // 在线标定参数：默认取头文件顶部的论文固定配置
    m_alpha = ALPHA;
    m_sigmaA = SIGMA_A;

    // 外推点限幅：默认关闭，保持历史行为
    m_hasAngleLimit = false;
    m_minAngle = -180.0;
    m_maxAngle =  180.0;

    a_mean = 0.0;
    // 初始化P为对角大协方差
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            P[i][j] = (i==j) ? 1.0 : 0.0;
    CalcModelMatrix();
}

void CSKalmanFilter::SetSigmaA(double sigmaA)
{
    if(sigmaA <= 0.0)
        sigmaA = 1e-6;          // 防止 Q 退化为全零

    m_sigmaA = sigmaA;
    CalcModelMatrix();          // Q ∝ σ_a²，需要重算
}

void CSKalmanFilter::SetAlpha(double alpha)
{
    if(alpha <= 0.0)
        alpha = 1e-6;

    m_alpha = alpha;
    CalcModelMatrix();          // Φ、U、Q 都与 α 有关
}

void CSKalmanFilter::SetAngleLimit(double minAngle, double maxAngle)
{
    if(minAngle > maxAngle)
    {
        const double tmp = minAngle;
        minAngle = maxAngle;
        maxAngle = tmp;
    }

    m_minAngle = minAngle;
    m_maxAngle = maxAngle;
    m_hasAngleLimit = true;
}

double CSKalmanFilter::clampAngle(double angle) const
{
    if(!m_hasAngleLimit)
        return angle;

    if(angle < m_minAngle)
        return m_minAngle;

    if(angle > m_maxAngle)
        return m_maxAngle;

    return angle;
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
    double a = m_alpha;
    double T = DT;
    double exp_aT = exp(-a * T);
    double a2 = a * a;

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
    // 连续模型 ȧ = -α·a + α·ā + w 的输入项是 α·ā，故离散输入矩阵为
    //      U = α·∫₀^T v(s) ds ,  v(s) = e^(A·s)·[0,0,1]ᵀ
    //   U0 = (1/α)·[ -T + αT²/2 + (1-e^(-αT))/α ]
    //   U1 =         T -            (1-e^(-αT))/α
    //   U2 = 1 - e^(-αT)
    // 三式都自带 α 因子，与 tests/filter_sim 的“α·∫v”数值积分一致。
    U[0] = (-T + 0.5*a*T*T + (1 - exp_aT)/a) / a;
    U[1] = T - (1 - exp_aT)/a;
    U[2] = 1 - exp_aT;

    // ========== 计算过程噪声Q矩阵 论文式(8) ==========
    // 连续模型:  dθ/dt = v,  dv/dt = a,  da/dt = -α·a + w,
    //            E[w(t)·w(τ)] = 2ασ_a²·δ(t-τ)
    //
    // 该模型的精确离散化就是
    //      Q = 2ασ_a² · ∫₀^T v(s)·vᵀ(s) ds      v(s) = e^(A·s)·[0,0,1]ᵀ
    //      v(s) = [ (-1+αs+e^(-αs))/α² , (1-e^(-αs))/α , e^(-αs) ]ᵀ
    //
    // 注意：解析闭式中每一项都是 O(α) 量级，而结果只有 O(T⁵) 量级。
    // 本例 αT = 0.002，两者相差约 11 个数量级，直接写闭式会因大数相减失真
    // ——旧代码正是因此把 Q22 算成了负值（Q 不再半正定，滤波器必然发散）。
    // 这里改为按定义做 Simpson 数值积分：被积函数 v_i(s)·v_j(s) ≥ 0 且光滑，
    // 既不会相减失真，又保证 Q 天然半正定。
    const int    Q_N = 200;              // Simpson 区间数（偶数）
    const double qh  = T / Q_N;
    double qsum[3][3] = {{0.0}};

    for(int k = 0; k <= Q_N; k++)
    {
        const double s = k * qh;
        const double w = (k == 0 || k == Q_N) ? 1.0 : ((k % 2) ? 4.0 : 2.0);
        const double t = a * s;

        double v[3];
        // v0 在 αs→0 时是 O(s²)，用级数避免 (t + expm1(-t)) 型相减丢精度
        if(std::fabs(t) < 1e-2)
            v[0] = s * s * 0.5 * (1.0 - t / 3.0 + t * t / 12.0);
        else
            v[0] = (t + std::expm1(-t)) / a2;
        v[1] = (std::fabs(t) < 1e-8) ? s : (-std::expm1(-t) / a);
        v[2] = std::exp(-t);

        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                qsum[i][j] += w * v[i] * v[j];
    }

    // 2ασ_a² · (Simpson 步长 / 3)
    const double q_coeff = 2.0 * a * m_sigmaA * m_sigmaA * qh / 3.0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            Q[i][j] = q_coeff * qsum[i][j];
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
    if(tau < 0.0)
        tau = 0.0;

    // 限幅在最后一步做：预测值可以超出，但下发前不能越界
    return clampAngle(x[0] + x[1] * tau + 0.5 * x[2] * tau * tau);
}

// ===================== 跟踪管理器实现 =====================
SeekerTrackManager::SeekerTrackManager(double azLimitDeg, double elLimitDeg)
{
    sys_time = 0.0;
    SetAngleLimit(azLimitDeg, elLimitDeg);
    // is_blocked = false;
    // miss_dist = 0.0;
}

void SeekerTrackManager::SetAngleLimit(double azLimitDeg, double elLimitDeg)
{
    if(azLimitDeg < 0.0) azLimitDeg = -azLimitDeg;
    if(elLimitDeg < 0.0) elLimitDeg = -elLimitDeg;

    az_filter.SetAngleLimit(-azLimitDeg, azLimitDeg);
    el_filter.SetAngleLimit(-elLimitDeg, elLimitDeg);
}

void SeekerTrackManager::SetSigmaA(double sigmaA)
{
    az_filter.SetSigmaA(sigmaA);
    el_filter.SetSigmaA(sigmaA);
}

void SeekerTrackManager::SetAlpha(double alpha)
{
    az_filter.SetAlpha(alpha);
    el_filter.SetAlpha(alpha);
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
    // 4 个点：t+0.25, t+0.50, t+0.75, t+1.00 秒预测
    // 与 Alpha-Beta 路径（AlphaBetaTracker.cpp 的 GenAxisPacket）时间基一致：
    // 转台 0x0A 命令按 250ms 间隔解析这 4 个点，整包视野为 1s。
    pkt.angle1 = filt.Extrapolate(0.25);
    pkt.angle2 = filt.Extrapolate(0.50);
    pkt.angle3 = filt.Extrapolate(0.75);
    pkt.angle4 = filt.Extrapolate(1.00);
    return pkt;
}
