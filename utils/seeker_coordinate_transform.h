#ifndef SEEKER_COORDINATE_TRANSFORM_H
#define SEEKER_COORDINATE_TRANSFORM_H

/* ============================================================================
 * seeker_coordinate_transform.h  —  导引头/转台坐标变换工具（Eigen 实现）
 *
 * 用途
 *   导引头装在转台内框上。内框滚转 gamma 时，导引头自身的俯仰轴、方位轴会
 *   跟着一起旋转，导引头报出的 (A', E') 是"滚转后坐标系"下的角度；而控制与
 *   显示需要的是"原坐标系"（滚转前，即内框未滚转的坐标系）下的 (A, E)。
 *   本文件给出该映射及其反变换，另外附带通用滚转轴、装配矩阵、完整转台链等。
 *
 * 坐标约定（与参考实现一致）
 *   S  : 导引头基座系（= 转台内框系，滚转前）
 *        x = 视轴（零位视线），y = 右，z = 下（右手系）
 *        n_S = [cosE*cosA, cosE*sinA, -sinE]^T
 *        A 方位角，绕 z 轴正向（视轴偏右为正）；E 俯仰角，正向抬头
 *   S' : 内框绕 x 轴滚转 gamma 后的导引头基座系
 *   同一物理视线：n_S = Rx(-gamma) * n_S'
 *
 * 转台轴约定（本项目）
 *   轴1 = 内框 = 滚转 gamma；轴2 = 中框 = 俯仰 theta；轴3 = 外框 = 方位 psi
 *   地面系 -> 内框系：R = Rz(psi) * Ry(theta) * Rx(gamma)
 *
 * 角度单位
 *   不带后缀的函数用「弧度」；带 Deg 后缀的函数、以及 AzElDeg 结果用「度」，
 *   方便直接对接导引头 / 转台协议里的角度值。
 *
 * 用法示例
 *   // 内框（滚转轴）当前 30°，导引头报 A'=2°、E'=1°，求原坐标系下的角度
 *   const seeker::AzElDeg ang =
 *       seeker::deRollAboutBoresightDeg(2.0, 1.0, 30.0);
 *   // ang.az -> 原坐标系方位角；ang.el -> 原坐标系俯仰角
 *
 *   // 反向：原坐标系角度 -> 滚转后导引头应报出的角度
 *   const seeker::AzElDeg rolled =
 *       seeker::toRolledFrameDeg(ang.az, ang.el, 30.0);
 *
 * 说明
 *   若实际滚转方向与上述约定相反，把 gamma（内框角）取负号传入即可。
 * ==========================================================================*/

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <cmath>

namespace seeker {

// ═══════════════════════════ 基础类型 ═══════════════════════════

using Vec3 = Eigen::Vector3d;      // 视线单位矢量
using Vec2 = Eigen::Vector2d;      // (方位角 A, 俯仰角 E)，弧度
using Mat3 = Eigen::Matrix3d;
using Quat = Eigen::Quaterniond;

// 角度对，单位：度
struct AzElDeg
{
    double az;   // 方位角
    double el;   // 俯仰角

    AzElDeg() : az(0.0), el(0.0) {}
    AzElDeg(double azimuthDeg, double elevationDeg) : az(azimuthDeg), el(elevationDeg) {}
};

constexpr double kPi = 3.14159265358979323846;

// ═══════════════════════════ 小工具 ═══════════════════════════

inline double deg2rad(double deg) { return deg * (kPi / 180.0); }
inline double rad2deg(double rad) { return rad * (180.0 / kPi); }

inline double clampValue(double v, double lo, double hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// 绕 x / y / z 轴转 angle（弧度）的旋转矩阵（右手正向）
inline Mat3 rotationX(double angle)
{
    return Eigen::AngleAxisd(angle, Vec3::UnitX()).toRotationMatrix();
}

inline Mat3 rotationY(double angle)
{
    return Eigen::AngleAxisd(angle, Vec3::UnitY()).toRotationMatrix();
}

inline Mat3 rotationZ(double angle)
{
    return Eigen::AngleAxisd(angle, Vec3::UnitZ()).toRotationMatrix();
}

// 绕任意单位轴 a 转 angle（弧度）的旋转矩阵
inline Mat3 rotationAxis(const Vec3& a, double angle)
{
    const double n = a.norm();
    if (n < 1e-12)
        return Mat3::Identity();
    return Eigen::AngleAxisd(angle, a / n).toRotationMatrix();
}

// ═════════════════════ 角度(弧度) <-> 视线单位矢量 ═════════════════════

// (A,E) -> 视线单位矢量，等价于参考实现的 Rz(A) * Ry(E) * ex
inline Vec3 losFromAngles(const Vec2& ang)
{
    return rotationZ(ang.x()) * rotationY(ang.y()) * Vec3::UnitX();
}

// 视线单位矢量 -> (A,E)
inline Vec2 anglesFromLos(const Vec3& n)
{
    const double norm = n.norm();
    if (norm < 1e-12)
        return Vec2::Zero();

    const Vec3 u = n / norm;
    return Vec2(std::atan2(u.y(), u.x()),
                std::asin(clampValue(-u.z(), -1.0, 1.0)));
}

// ══════════════════ 核心：滚转系读数 -> 原坐标系读数 ══════════════════

// 滚转轴 = 视轴 x：n_原 = Rx(-gamma) * n_滚转，再反解出 (A,E)
inline Vec2 deRollAboutBoresight(const Vec2& angRolled, double gamma)
{
    const Mat3 Rx = rotationX(gamma);
    return anglesFromLos(Rx.transpose() * losFromAngles(angRolled));   // R^T = R^{-1}
}

// 反向：原坐标系视线 -> 滚转后导引头应报出的读数（gamma 反号即可）
inline Vec2 toRolledFrame(const Vec2& angOriginal, double gamma)
{
    return deRollAboutBoresight(angOriginal, -gamma);
}

// 通用滚转轴：滚转轴在导引头基座系里是任意单位矢量 a（处理装配偏斜）
inline Vec2 deRollAboutAxis(const Vec2& angRolled, double gamma, const Vec3& a)
{
    return anglesFromLos(rotationAxis(a, -gamma) * losFromAngles(angRolled));
}

// 装配矩阵 M：把导引头基座系坐标映射到内框系，滚转轴在内框系是 x
//   n_原系 = Rx(-gamma) * M * n_基座系
inline Vec2 deRollWithMounting(const Vec2& angRolled, double gamma, const Mat3& M)
{
    const Mat3 Rx = rotationX(gamma);
    return anglesFromLos(Rx.transpose() * M * losFromAngles(angRolled));
}

// ═══════════════════════ 显式三角函数公式（校核用） ═══════════════════════
// 与矩阵法完全等价，可用来交叉验证；输入输出均为弧度
//   sinE = cosg*sinE' + sing*cosE'*sinA'
//   A    = atan2(cosg*cosE'*sinA' - sing*sinE', cosE'*cosA')
inline Vec2 deRollClosedForm(const Vec2& angRolled, double gamma)
{
    const double A = angRolled.x();
    const double E = angRolled.y();
    const double cg = std::cos(gamma);
    const double sg = std::sin(gamma);

    const double sinE = cg * std::sin(E) + sg * std::cos(E) * std::sin(A);
    const double num = cg * std::cos(E) * std::sin(A) - sg * std::sin(E);
    const double den = std::cos(E) * std::cos(A);

    return Vec2(std::atan2(num, den), std::asin(clampValue(sinE, -1.0, 1.0)));
}

// ══════════════════ 小角度线性化（A'、E' 都很小时可用） ══════════════════
//   [A;E] = [[cosg, -sing],[sing, cosg]] * [A';E']    （弧度）
inline Vec2 linearDeRoll(const Vec2& angRolled, double gamma)
{
    return Eigen::Rotation2Dd(gamma) * angRolled;
}

// ═══════════════════════ 完整转台链（psi/theta/gamma） ═══════════════════════

// 内框系 -> 地面系
inline Mat3 turntableToGround(double psi, double theta, double gamma)
{
    return rotationZ(psi) * rotationY(theta) * rotationX(gamma);
}

// 地面系视线 -> 滚转前内框系读数（即原坐标系下的 A、E）
inline Vec2 groundToPreRoll(const Vec3& nGround, double psi, double theta)
{
    return anglesFromLos((rotationZ(psi) * rotationY(theta)).transpose() * nGround);
}

// 地面系视线 -> 当前（已滚转）内框系读数，即导引头自身系应看到的角度
inline Vec2 groundToInnerRolled(const Vec3& nGround, double psi, double theta, double gamma)
{
    return anglesFromLos(turntableToGround(psi, theta, gamma).transpose() * nGround);
}

// 滚转前内框系读数 -> 地面系视线
inline Vec3 preRollToGround(const Vec2& angPreRoll, double psi, double theta)
{
    return (rotationZ(psi) * rotationY(theta)) * losFromAngles(angPreRoll);
}

// ═══════════════════════ 四元数版（便于插值/滤波） ═══════════════════════

// 内框滚转角 gamma 对应的旋转四元数（绕内框滚转轴，默认视轴 x）
inline Quat rollQuaternion(double gamma, const Vec3& axis = Vec3::UnitX())
{
    const double n = axis.norm();
    if (n < 1e-12)
        return Quat::Identity();
    return Quat(Eigen::AngleAxisd(gamma, axis / n));
}

// 滚转系读数 -> 原坐标系读数（四元数形式）
inline Vec2 deRollByQuaternion(const Vec2& angRolled, const Quat& qRoll)
{
    return anglesFromLos(qRoll.conjugate() * losFromAngles(angRolled));
}

// ═══════════════════════════ 度版封装 ═══════════════════════════
// 转台 / 导引头协议里的角度都是「度」，直接调用下面这些函数即可

inline Vec3 losFromAnglesDeg(double azDeg, double elDeg)
{
    return losFromAngles(Vec2(deg2rad(azDeg), deg2rad(elDeg)));
}

inline AzElDeg anglesFromLosDeg(const Vec3& n)
{
    const Vec2 ang = anglesFromLos(n);
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// 核心接口：滚转系读数(度) + 内框滚转角(度) -> 原坐标系角度(度)
inline AzElDeg deRollAboutBoresightDeg(double azRolledDeg, double elRolledDeg, double gammaDeg)
{
    const Vec2 ang = deRollAboutBoresight(Vec2(deg2rad(azRolledDeg), deg2rad(elRolledDeg)),
                                          deg2rad(gammaDeg));
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// 反向接口：原坐标系角度(度) -> 滚转后导引头应报出的角度(度)
inline AzElDeg toRolledFrameDeg(double azDeg, double elDeg, double gammaDeg)
{
    const Vec2 ang = toRolledFrame(Vec2(deg2rad(azDeg), deg2rad(elDeg)), deg2rad(gammaDeg));
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// 通用滚转轴版（度）：滚转轴不必是视轴，而是在导引头基座系里任取一个方向 axis。
// 适用场合：内框滚转轴与导引头视轴因装配偏差不重合，此时不能再按 Rx 直接反旋。
// 参数：azRolledDeg / elRolledDeg —— 滚转系(导引头自身系)读数，单位度
//       gammaDeg                —— 内框滚转角，单位度
//       axis                    —— 滚转轴方向（导引头基座系，内部自动归一化）
// 返回：原坐标系下的方位角、俯仰角，单位度（等价于弧度版 deRollAboutAxis）
inline AzElDeg deRollAboutAxisDeg(double azRolledDeg, double elRolledDeg, double gammaDeg,
                                  const Vec3& axis)
{
    const Vec2 ang = deRollAboutAxis(Vec2(deg2rad(azRolledDeg), deg2rad(elRolledDeg)),
                                     deg2rad(gammaDeg), axis);
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// 带装配矩阵版（度）：导引头基座系与内框系存在安装角偏差时使用。
// 计算链路：n_原 = Rx(-gamma) * M * n_基座，其中 M 把导引头基座系坐标映射到内框系，
//          并约定滚转轴在内框系中为 x 轴；M = I 时结果与 deRollAboutBoresightDeg 相同。
// 参数：azRolledDeg / elRolledDeg —— 滚转系(导引头自身系)读数，单位度
//       gammaDeg                —— 内框滚转角，单位度
//       mounting                —— 3x3 装配/安装矩阵（基座系 -> 内框系）
// 返回：原坐标系下的方位角、俯仰角，单位度（等价于弧度版 deRollWithMounting）
inline AzElDeg deRollWithMountingDeg(double azRolledDeg, double elRolledDeg, double gammaDeg,
                                     const Mat3& mounting)
{
    const Vec2 ang = deRollWithMounting(Vec2(deg2rad(azRolledDeg), deg2rad(elRolledDeg)),
                                        deg2rad(gammaDeg), mounting);
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// 显式三角函数公式版（度）：与矩阵法 deRollAboutBoresightDeg 数学上完全等价，
// 不构造旋转矩阵，直接代入公式计算：
//     sinE = cosg*sinE' + sing*cosE'*sinA'
//     A    = atan2(cosg*cosE'*sinA' - sing*sinE', cosE'*cosA')
// 用途：① 与矩阵法交叉校核，验证实现无误；② 便于在不适合做矩阵运算的场合直接使用。
// 参数：azRolledDeg / elRolledDeg —— 滚转系读数；gammaDeg —— 内框滚转角，单位均为度
// 返回：原坐标系下的方位角、俯仰角，单位度（等价于弧度版 deRollClosedForm）
inline AzElDeg deRollClosedFormDeg(double azRolledDeg, double elRolledDeg, double gammaDeg)
{
    const Vec2 ang = deRollClosedForm(Vec2(deg2rad(azRolledDeg), deg2rad(elRolledDeg)),
                                      deg2rad(gammaDeg));
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// 小角度线性化版（度）：把变换近似成二维旋转矩阵
//     [A;E] = [[cosg, -sing],[sing, cosg]] * [A';E']
// 适用场合：滚转角任意，但导引头读数 A'、E' 都较小（几度以内）时的快速近似；
//          角度较大时必须改用 deRollAboutBoresightDeg，否则误差不可忽略。
// 参数：azRolledDeg / elRolledDeg —— 滚转系读数；gammaDeg —— 内框滚转角，单位均为度
// 返回：原坐标系下的方位角、俯仰角，单位度（等价于弧度版 linearDeRoll）
inline AzElDeg linearDeRollDeg(double azRolledDeg, double elRolledDeg, double gammaDeg)
{
    const Vec2 ang = linearDeRoll(Vec2(deg2rad(azRolledDeg), deg2rad(elRolledDeg)),
                                  deg2rad(gammaDeg));
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// 地面系视线 -> 滚转前内框系角度（度）：完整转台链的反解。
// 即已知外框方位 psi、中框俯仰 theta，求"原坐标系"（内框未滚转）下的方位角与俯仰角；
// 因为目标就是滚转前的坐标系，所以不需要内框滚转角 gamma。
// 参数：nGround  —— 地面系下的视线单位矢量（只关心方向，内部会归一化）
//       psiDeg   —— 转台外框方位角；thetaDeg —— 转台中框俯仰角，单位均为度
// 返回：滚转前内框系（原坐标系）下的方位角、俯仰角，单位度
inline AzElDeg groundToPreRollDeg(const Vec3& nGround, double psiDeg, double thetaDeg)
{
    const Vec2 ang = groundToPreRoll(nGround, deg2rad(psiDeg), deg2rad(thetaDeg));
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// 地面系视线 -> 当前已滚转内框系角度（度）：与上一个函数的区别是目标系不同。
// 这里求的是"导引头此刻自身应该看到的角度"，即含滚转 gamma 的当前内框系读数，
// 可用来与导引头实际输出的 A'、E' 对比（残差大说明滚转角或标定有问题）。
// 参数：nGround  —— 地面系下的视线单位矢量（内部归一化）
//       psiDeg   —— 外框方位角；thetaDeg —— 中框俯仰角；gammaDeg —— 内框滚转角，单位度
// 返回：当前（滚转后）内框系下的方位角、俯仰角，单位度
inline AzElDeg groundToInnerRolledDeg(const Vec3& nGround, double psiDeg, double thetaDeg,
                                      double gammaDeg)
{
    const Vec2 ang = groundToInnerRolled(nGround, deg2rad(psiDeg), deg2rad(thetaDeg),
                                         deg2rad(gammaDeg));
    return AzElDeg(rad2deg(ang.x()), rad2deg(ang.y()));
}

// ═══════════════════════════ 辅助量 ═══════════════════════════

// 离轴角（视线与视轴夹角）。内框滚转不改变它，可作为变换正确性的校核量。
inline double offBoresightDeg(const AzElDeg& ang)
{
    const double c = std::cos(deg2rad(ang.el)) * std::cos(deg2rad(ang.az));
    return rad2deg(std::acos(clampValue(c, -1.0, 1.0)));
}

}   // namespace seeker

#endif   // SEEKER_COORDINATE_TRANSFORM_H
