#ifndef SEEK_CS_KALMAN_H
#define SEEK_CS_KALMAN_H

#include <cmath>
#include <vector>
#include <iostream>

// 系统参数（论文固定配置）
const double DT = 0.02;        // 导引头采样周期20ms
const double ALPHA = 0.1;      // 加速度时间常数倒数α
const double SIGMA_A = 0.08;   // 目标加速度方差σ_a
const double R_MEAS = 0.001;   // 编码器观测噪声方差R
// const double BLOCK_THRESH = 5; // 遮挡判定阈值(像元)

// 下发转台数据包：单轴4点角度
struct AxisTrackPacket
{
    double time_start;   // 数据包基准起始时间戳 s
    double angle[4];     // t0, t0+1, t0+2, t0+3 预测角度
};

// 当前统计模型CS自适应卡尔曼（单轴角度专用：状态[θ, dθ, ddθ]）
class CSKalmanFilter
{
public:
    // 状态向量 X = [角度θ, 角速度dθ, 角加速度ddθ]^T
    double x[3];
    // 误差协方差 P(3×3)
    double P[3][3];
    // 状态转移矩阵 Φ(3×3)
    double Phi[3][3];
    // 机动输入矩阵 U(3×1)
    double U[3];
    // 过程噪声协方差 Q(3×3)
    double Q[3][3];
    // 观测矩阵 H = [1, 0, 0]
    const double H[3] = {1.0, 0.0, 0.0};
    // 当前机动加速度均值 ā(k)
    double a_mean;

public:
    CSKalmanFilter();
    // 初始化滤波器，初始角度init_theta
    void Init(double init_theta);
    // 预计算Φ、U、Q（仅需初始化调用一次）
    void CalcModelMatrix();
    // 预测步：时间更新（无观测也可执行，遮挡纯预测）
    void Predict();
    // 更新步：量测修正，输入导引头实测角度z
    void Update(double z);
    // 多步外推：预测τ秒后的角度
    double Extrapolate(double tau);
    // 获取当前最优滤波角度
    double GetAngle() const { return x[0]; }
    // 获取角速度、角加速度
    double GetRate() const { return x[1]; }
    double GetAcc() const { return x[2]; }
};

// 导引头跟踪管理器：同时管理方位+俯仰双轴，生成3s下发数据包
class SeekerTrackManager
{
private:
    CSKalmanFilter az_filter;  // 方位轴滤波器
    CSKalmanFilter el_filter;  // 俯仰轴滤波器
    double sys_time;           // 系统全局时间戳 s
    // bool is_blocked;           // 是否遮挡标志
    double miss_dist;          // 当前脱靶量(像元)

public:
    SeekerTrackManager();
    // 初始化双轴初始角度
    void Init(double az0, double el0);
    // 每20ms调用：接收导引头实测角度+脱靶量，执行滤波
    void FeedSeekerData(double t, double az_meas, double el_meas, double miss);
    // 生成单轴3s跟踪数据包：4个预测角度间隔1s
    AxisTrackPacket GenAxisPacket(bool is_az);
    // // 获取遮挡状态
    // bool IsBlocked() const { return is_blocked; }
};

#endif


// #include "SeekCSKalman.h"
// #include <chrono>
// #include <thread>

// int main()
// {
//     // 1. 初始化管理器：初始方位0°，俯仰10°
//     SeekerTrackManager track_mgr;
//     track_mgr.Init(0.0, 10.0);

//     double global_t = 0.0;
//     double send_timer = 0.0; // 3s下发计时器

//     std::cout << "===== 导引头角度预测跟踪仿真启动 =====" << std::endl;
//     std::cout << "采样周期20ms，每3s生成转台数据包" << std::endl;

//     while(global_t < 10.0) // 仿真10s
//     {
//         // 模拟导引头实测角度+脱靶量
//         double az_real = 0.8 * global_t + 0.05 * sin(global_t); // 方位匀速小幅机动
//         double el_real = 10.0 + 0.3 * sin(0.5*global_t);        // 俯仰正弦机动
//         double miss = (global_t > 4.0 && global_t < 6.0) ? 8.0 : 1.0; // 4~6s遮挡(脱靶8像元)

//         // 加入微小测量噪声
//         double az_meas = az_real + 0.02 * (rand()%100 / 100.0 - 0.5);
//         double el_meas = el_real + 0.02 * (rand()%100 / 100.0 - 0.5);

//         // 送入滤波（每20ms一次）
//         track_mgr.FeedSeekerData(global_t, az_meas, el_meas, miss);

//         // 每3s生成下发数据包
//         send_timer += DT;
//         if(send_timer >= 3.0)
//         {
//             send_timer = 0.0;
//             AxisTrackPacket az_pkt = track_mgr.GenAxisPacket(true);
//             AxisTrackPacket el_pkt = track_mgr.GenAxisPacket(false);

//             std::cout << "\n【" << az_pkt.time_start << "s 下发转台数据包】" << std::endl;
//             std::cout << "方位轴 [t0, t0+1, t0+2, t0+3]："
//                       << az_pkt.angle[0] << " " << az_pkt.angle[1] << " "
//                       << az_pkt.angle[2] << " " << az_pkt.angle[3] << std::endl;
//             std::cout << "俯仰轴 [t0, t0+1, t0+2, t0+3]："
//                       << el_pkt.angle[0] << " " << el_pkt.angle[1] << " "
//                       << el_pkt.angle[2] << " " << el_pkt.angle[3] << std::endl;
//             if(track_mgr.IsBlocked())
//                 std::cout << "当前状态：目标遮挡，纯预测跟踪" << std::endl;
//             else
//                 std::cout << "当前状态：正常观测跟踪" << std::endl;
//         }

//         // 打印实时滤波角度
//         std::cout << "t=" << global_t
//                   << " Az=" << track_mgr.az_filter.GetAngle()
//                   << " El=" << track_mgr.el_filter.GetAngle() << "\r";
//         std::cout.flush();

//         global_t += DT;
//         std::this_thread::sleep_for(std::chrono::milliseconds(20));
//     }

//     std::cout << "\n仿真结束" << std::endl;
//     return 0;
// }

