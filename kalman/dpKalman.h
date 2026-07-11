#pragma once
#include <cmath>
#include <array>
#include <stdexcept>

class KalmanPredictor {
public:
    // dt: 滤波采样周期 (秒)
    // q_pos, q_vel, q_acc: 过程噪声方差 (对应位置、速度、加速度)
    // r_angle: 角度测量噪声方差
    KalmanPredictor(double dt, double q_pos = 1e-6, double q_vel = 1e-4, 
                    double q_acc = 1e-2, double r_angle = 1e-4)
        : T(dt), R(r_angle) {
        // 初始化状态和协方差
        X.fill(0.0);
        P.fill({ {1e6, 0, 0}, {0, 1e6, 0}, {0, 0, 1e6} });
        
        // 构建过程噪声协方差 Q (3x3)
        // 标准恒定加速度模型的离散化噪声
        double q11 = q_pos;
        double q12 = 0.5 * q_vel * T;
        double q13 = 0.5 * q_acc * T * T * 0.5; // 略
        // 更精确的Q计算可参考公式，这里简化为对角
        Q.fill(0.0);
        Q[0][0] = q_pos;
        Q[1][1] = q_vel;
        Q[2][2] = q_acc;
    }

    // 输入测量角度 (度)，更新滤波
    void update(double z) {
        // 1. 先验预测 (一步预测)
        std::array<double,3> X_pred;
        X_pred[0] = X[0] + T * X[1] + 0.5 * T * T * X[2];
        X_pred[1] = X[1] + T * X[2];
        X_pred[2] = X[2];

        // 预测协方差 P_pred = Φ * P * Φ^T + Q
        std::array<std::array<double,3>,3> P_pred = multiply_Phi_P_PhiT(P);
        for (int i=0; i<3; ++i) for (int j=0; j<3; ++j) P_pred[i][j] += Q[i][j];

        // 2. 更新 (卡尔曼增益)
        // 观测矩阵 H = [1,0,0]
        // 创新协方差 S = H * P_pred * H^T + R = P_pred[0][0] + R
        double S = P_pred[0][0] + R;
        // 卡尔曼增益 K = P_pred * H^T / S
        std::array<double,3> K;
        K[0] = P_pred[0][0] / S;
        K[1] = P_pred[1][0] / S;
        K[2] = P_pred[2][0] / S;

        // 状态更新
        double innovation = z - X_pred[0];
        X[0] = X_pred[0] + K[0] * innovation;
        X[1] = X_pred[1] + K[1] * innovation;
        X[2] = X_pred[2] + K[2] * innovation;

        // 协方差更新 P = (I - K*H) * P_pred
        for (int i=0; i<3; ++i) {
            for (int j=0; j<3; ++j) {
                P[i][j] = P_pred[i][j] - K[i] * P_pred[0][j];
            }
        }
    }

    // 预测未来 Δt 秒后的角度 (度)
    double predictAngle(double dt) const {
        // 状态转移矩阵应用于当前状态估计
        double angle_pred = X[0] + dt * X[1] + 0.5 * dt * dt * X[2];
        return angle_pred;
    }

    // 获取当前估计的角度、角速度、角加速度 (用于调试)
    double getAngle() const { return X[0]; }
    double getVelocity() const { return X[1]; }
    double getAcceleration() const { return X[2]; }

private:
    double T;                 // 滤波采样周期
    double R;                 // 测量噪声方差
    std::array<double,3> X;   // 状态 [角度, 角速度, 角加速度]
    std::array<std::array<double,3>,3> P; // 协方差矩阵
    std::array<std::array<double,3>,3> Q; // 过程噪声协方差

    // 辅助函数：计算 Φ * P * Φ^T (仅用于3x3矩阵)
    std::array<std::array<double,3>,3> multiply_Phi_P_PhiT(
        const std::array<std::array<double,3>,3>& Pmat) {
        // 结果矩阵
        std::array<std::array<double,3>,3> res = {{{0,0,0},{0,0,0},{0,0,0}}};
        // 先计算 Φ * P 存于 temp
        double temp[3][3] = {0};
        for (int i=0; i<3; ++i) {
            for (int j=0; j<3; ++j) {
                temp[i][j] = Pmat[i][j] + T * Pmat[i][j] + ... ; // 按公式展开
                // 具体展开：
                // row0: [P00 + T*P10 + 0.5*T^2*P20, ...]
                // 为简化，采用循环方式（但效率影响不大）
            }
        }
        // 鉴于篇幅，这里给出直接循环乘法（理解清晰）
        double Phi[3][3] = {
            {1, T, 0.5*T*T},
            {0, 1, T},
            {0, 0, 1}
        };
        // 计算 Phi * P
        double PhiP[3][3] = {0};
        for (int i=0; i<3; ++i) {
            for (int j=0; j<3; ++j) {
                for (int k=0; k<3; ++k) {
                    PhiP[i][j] += Phi[i][k] * Pmat[k][j];
                }
            }
        }
        // 计算 (PhiP) * Phi^T
        for (int i=0; i<3; ++i) {
            for (int j=0; j<3; ++j) {
                res[i][j] = 0;
                for (int k=0; k<3; ++k) {
                    res[i][j] += PhiP[i][k] * Phi[j][k]; // Phi^T 即 Phi[j][k]
                }
            }
        }
        return res;
    }
};