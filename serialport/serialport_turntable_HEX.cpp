#include "serialport_turntable_HEX.h"
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <cmath>
#include <cstring>
#include <QTime>
#include "../utils/seeker_coordinate_transform.h"
#include "../utils/sendlatencyprobe.h"
#define MAX_SPEED_HEX 14.0f           //转台的最大速度可以再加大一些
#define MAX_ACCELERATION_HEX 10.0f
#define HANDLE_SPEED_CHANGE_EPSILON 0.1f  // 低于此变化的摇杆噪声不触发停车重设

// ════════════════════════ 数值编解码实现 ════════════════════════

// 3字节有符号值编码 (最高位bit7=符号, 0正1负)
void SerialPortTurntableHex::encode3BytesSigned(uint8_t dest[3], float value, float unit)
{
    // 将浮点值转换为原始整数
    int32_t raw = static_cast<int32_t>(std::round(value * unit));

    // 钳位到23位有符号范围 (-8388608 ~ +8388607)
    if (raw > 8388607)  raw = 8388607;
    if (raw < -8388608) raw = -8388608;

    bool negative = (raw < 0);
    uint32_t absVal = static_cast<uint32_t>(negative ? -raw : raw);

    dest[0] = (absVal >> 16) & 0x7F;    // 高7位数据
    if (negative) dest[0] |= 0x80;       // 符号位
    dest[1] = (absVal >> 8) & 0xFF;     // 中8位
    dest[2] = absVal & 0xFF;            // 低8位
}

// 3字节有符号值解码
float SerialPortTurntableHex::decode3BytesSigned(const uint8_t src[3], float unit)
{
    bool negative = (src[0] & 0x80) != 0;
    uint32_t absVal = ((src[0] & 0x7F) << 16) | (src[1] << 8) | src[2];
    int32_t raw = static_cast<int32_t>(absVal);
    if (negative) raw = -raw;
    return static_cast<float>(raw) / unit;
}

// 2字节无符号值编码 (大端)
void SerialPortTurntableHex::encode2BytesUint(uint8_t dest[2], uint16_t value)
{
    dest[0] = (value >> 8) & 0xFF;
    dest[1] = value & 0xFF;
}

// 2字节无符号值解码 (大端)
uint16_t SerialPortTurntableHex::decode2BytesUint(const uint8_t src[2])
{
    return (static_cast<uint16_t>(src[0]) << 8) | src[1];
}

// 3字节无符号值编码 (小端序变大端序)
void SerialPortTurntableHex::encode3BytesUint(uint8_t dest[3], uint32_t value)
{
    dest[0] = (value >> 16) & 0xFF;
    dest[1] = (value >> 8) & 0xFF;
    dest[2] = value & 0xFF;
}

// 3字节无符号值解码 (大端序变小端序)
uint32_t SerialPortTurntableHex::decode3BytesUint(const uint8_t src[3])
{
    return (static_cast<uint32_t>(src[0]) << 16)
         | (static_cast<uint32_t>(src[1]) << 8)
         | src[2];
}

// 角度值 → 原始值 (单位: 0.0001°)
int32_t SerialPortTurntableHex::angleToRaw(float degrees)
{
    return static_cast<int32_t>(std::round(degrees * ANGLE_UNIT));
}

// 原始值 → 角度值
float SerialPortTurntableHex::rawToAngle(int32_t raw)
{
    return static_cast<float>(raw) / ANGLE_UNIT;
}

// 速度值 → 原始值 (单位: 0.001°/s)
int32_t SerialPortTurntableHex::speedToRaw(float speed)
{
    return static_cast<int32_t>(std::round(speed * SPEED_UNIT));
}

// 原始值 → 速度值
float SerialPortTurntableHex::rawToSpeed(int32_t raw)
{
    return static_cast<float>(raw) / SPEED_UNIT;
}


// ════════════════════════ 校验和计算 ════════════════════════

uint16_t SerialPortTurntableHex::calcChecksum(const uint8_t* data, size_t len)
{
    uint16_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        sum += data[i];
    }
    return sum;
}


// ════════════════════════ 帧构建与发送（通用）═══════════════════════════

void SerialPortTurntableHex::buildAndSend(uint8_t axis_cmd, const uint8_t params[14])
{
    if (!m_serialPort || !m_serialPort->isOpen()) {
        //qWarning() << "[HEX] 串口未打开，无法发送指令";应该给出转台串口未打开的提示
        // 发送延迟探针：本次请求没有发出去，弹出配对令牌，避免后续样本错配
        SendLat::discardClick(SendLat::Turntable);
        return;
    }

    // 构建19字节发送帧
    uint8_t buf[19];
    buf[0] = 0x55;
    buf[1] = 0xAA;
    buf[2] = axis_cmd;
    std::memcpy(buf + 3, params, 14);

    // 计算校验和 (bytes 2-16, 共15字节)
    uint16_t sum = calcChecksum(buf + 2, 15);
    buf[17] = (sum >> 8) & 0xFF;    // 校验和高字节
    buf[18] = sum & 0xFF;           // 校验和低字节

    QByteArray data(reinterpret_cast<const char*>(buf), 19);
    // 发送延迟探针打点：本次人工请求的第一次 write
    // （开转台/停车/回零这类一次点击发多帧的，只取第一帧；外引导与手柄等
    //   自动来源没有配对令牌，只会落到探针的 unmatched 计数）
    SendLat::markSend(SendLat::Turntable);
    qint64 written = m_serialPort->write(data);
    if (written == -1) {
        qCritical() << "[HEX] 指令发送失败:" << m_serialPort->errorString();
    }
}


// ════════════════════════ 数据接收与解析（HEX二进制格式, 29字节接收帧）═══════════════════════════

void SerialPortTurntableHex::onReadyRead()
{
    // 仿照 image/laser 串口的处理：串口驱动一次 readyRead 到达的数据不一定是一整帧，
    // 先存入接收缓冲，找到帧头 0x55 0xAA 后再按固定帧长(29字节)切出完整一帧交给 parseData
    m_rxBuffer.append(m_serialPort->readAll());

    const int frameLen = 29;

    while (m_rxBuffer.size() >= 2) {
        // 1. 逐字节查找帧头：找到 0x55 后判断下一个是否为 0xAA
        int headIdx = -1;
        for (int i = 0; i <= m_rxBuffer.size() - 2; ++i) {
            if (static_cast<quint8>(m_rxBuffer[i])     == 0x55
                    && static_cast<quint8>(m_rxBuffer[i + 1]) == 0xAA) {
                headIdx = i;
                break;
            }
        }
        if (headIdx < 0) {
            // 没找到完整帧头，保留最后1字节（可能是下一帧帧头的第一个字节）
            if (m_rxBuffer.size() > 1)
                m_rxBuffer = m_rxBuffer.right(1);
            break;
        }
        if (headIdx > 0)
            m_rxBuffer.remove(0, headIdx);
        if (m_rxBuffer.size() < frameLen)
            break;
        QByteArray frame = m_rxBuffer.left(frameLen);
        m_rxBuffer.remove(0, frameLen);
        parseData(frame);
    }
}

void SerialPortTurntableHex::parseData(const QByteArray &rawData)
{
    // 接收帧固定29字节:
    // 0-1: 帧头(0x55 0xAA), 2-4: 毫秒时间, 5: 内框报警+状态,
    // 6-8: 内框角度, 9-11: 内框偏差, 12: 中框报警+状态,
    // 13-15: 中框角度, 16-18: 中框偏差, 19: 外框报警+状态,
    // 20-22: 外框角度, 23-25: 外框偏差, 26: 秒脉冲+指令提示,
    // 27-28: 校验和(bytes 2-26累加)

    if (rawData.size() < 29) {
        qDebug() << "[HEX] 接收数据不完整, 期望29字节, 实际" << rawData.size();
        return;
    }

    const uint8_t* buf = reinterpret_cast<const uint8_t*>(rawData.constData());

    // 校验帧头
    if (buf[0] != 0x55 || buf[1] != 0xAA) {
        qDebug() << "[HEX] 帧头错误:"
                 << QString("0x%1 0x%2").arg(buf[0], 2, 16, QChar('0')).arg(buf[1], 2, 16, QChar('0'));
        return;
    }

    // 校验校验和 (bytes 2-26, 共25字节)
    uint16_t expectedSum = (static_cast<uint16_t>(buf[27]) << 8) | buf[28];
    uint16_t actualSum = calcChecksum(buf + 2, 25);
    if (expectedSum != actualSum) {
        qDebug() << "[HEX] 校验和错误: 期望"
                 << QString("0x%1").arg(expectedSum, 4, 16, QChar('0'))
                 << "实际"
                 << QString("0x%1").arg(actualSum, 4, 16, QChar('0'));
        return;
    }

    StatusFeedbackHex feedback;

    // 毫秒时间 (3字节无符号, 范围 0-14399999)
    feedback.m_time = static_cast<int>(decode3BytesUint(buf + 2));

    // 跟踪时基：目标时刻一律以转台反馈时间为准。
    m_latestTurntableTimeMs = feedback.m_time;
    m_statusAgeClock.restart();
    m_turntableTimeValid = true;
    m_waitingStatusLogged = false;

    // 三轴状态字节：高4位为报警标志，低4位为运行状态标志。
    feedback.m_inner_alarm = (buf[5] >> 4) & 0x0F;
    feedback.m_inner_status = buf[5] & 0x0F;

    // 内框角度 (3字节有符号, 0.0001°)
    feedback.m_inner_angle = decode3BytesSigned(buf + 6, ANGLE_UNIT);

    // 内框偏差
    feedback.m_inner_ctlDeviation = decode3BytesSigned(buf + 9, ANGLE_UNIT);

    // 中框状态
    feedback.m_middle_alarm = (buf[12] >> 4) & 0x0F;
    feedback.m_middle_status = buf[12] & 0x0F;

    // 中框角度
    feedback.m_middle_angle = decode3BytesSigned(buf + 13, ANGLE_UNIT);

    // 中框偏差
    feedback.m_middle_ctlDeviation = decode3BytesSigned(buf + 16, ANGLE_UNIT);

    // 外框状态
    feedback.m_outter_alarm = (buf[19] >> 4) & 0x0F;
    feedback.m_outter_status = buf[19] & 0x0F;

    // 外框角度
    feedback.m_outter_angle = decode3BytesSigned(buf + 20, ANGLE_UNIT);

    // 外框偏差
    feedback.m_outter_ctlDeviation = decode3BytesSigned(buf + 23, ANGLE_UNIT);

    // 秒脉冲 + 指令提示
    feedback.m_hasSecPulse = (buf[26] & 0x80) ? 1 : 0;
    feedback.m_cmdHint     = buf[26] & 0x3F;     // 低6位

    // 停车完成后对应轴会回到伺服状态(0x01)。在这里处理反馈，可以在收到
    // 状态帧的第一时间下发等待中的新速度，而不必再等下一个 20ms 手柄周期。
    applyPendingHandleSpeed(1, feedback.m_inner_status, feedback.m_inner_alarm);
    applyPendingHandleSpeed(2, feedback.m_middle_status, feedback.m_middle_alarm);
    applyPendingHandleSpeed(3, feedback.m_outter_status, feedback.m_outter_alarm);

    emit requpdateframe(feedback);
}


// ════════════════════════ 构造函数 / 析构函数 ════════════════════════

SerialPortTurntableHex::SerialPortTurntableHex(QObject *parent)
    : SerialPort(parent)
{
}

SerialPortTurntableHex::~SerialPortTurntableHex()
{
    // m_turntableDataHex 是 main.cpp 中 new TurntableDataHex(&app) 创建、
    // 并作为 QML context property（turntableData）暴露给界面的 Data 对象，
    // 其生命周期由 app 管理：
    //   * 它带父对象（app），moveToThread() 必然失败并打印
    //     "QObject::moveToThread: Cannot move objects with a parent"；
    //   * 更不能在这里跨线程 delete，否则 QML 绑定和其它已建立的连接会持有悬垂指针。
    // 因此析构时不做任何迁移/释放动作，实际释放由 app 的析构链路完成
    //（与 laser / image / BD / CCD 的 Data 对象保持一致）。
}


// ════════════════════════ 基础指令：使能电机 ════════════════════════

void SerialPortTurntableHex::sendEnableCmd(int axis, bool enable)
{
    // 使能/释放电机 (CMD_ENABLE = 0x01)
    // params[0] = 0x00(释放) / 0x01(使能), params[1-13] = 0x00
    uint8_t params[14];
    std::memset(params, 0, 14);
    params[0] = enable ? 0x01 : 0x00;

    uint8_t axis_cmd = ENCODE_AXIS_CMD(axis, CMD_ENABLE);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 基础指令：停车 ════════════════════════

void SerialPortTurntableHex::sendStopCmd(int axis)
{
    // 停车 (CMD_STOP = 0x02)
    // 全部params为0x00
    uint8_t params[14];
    std::memset(params, 0, 14);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(axis, CMD_STOP);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 基础指令：回零 ════════════════════════

void SerialPortTurntableHex::sendZeroCmd(int axis)
{
    // 回零 (CMD_ZERO = 0x03)
    // 全部params为0x00
    uint8_t params[14];
    std::memset(params, 0, 14);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(axis, CMD_ZERO);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 基础指令：复位 ════════════════════════

void SerialPortTurntableHex::sendResetCmd()
{
    // 复位 (CMD_RESET = 0x1F)：依次向三个轴各发送一条复位指令
    uint8_t params[14];
    std::memset(params, 0, 14);

    for (int axis = 1; axis <= 3; ++axis) {
        uint8_t axis_cmd = ENCODE_AXIS_CMD(axis, CMD_RESET);
        buildAndSend(axis_cmd, params);
    }
}


// ════════════════════════ 高级指令：开机/使能 ════════════════════════

void SerialPortTurntableHex::openTurntable()
{
    resetAllHandleSpeedStates();
    // 开机流程: 先释放再使能三个轴电机
    sendEnableCmd(1, false); sendEnableCmd(2, false); sendEnableCmd(3, false);
    sendEnableCmd(1, true);  sendEnableCmd(2, true);  sendEnableCmd(3, true);
}

// ════════════════════════ 高级指令：释放 ════════════════════════
void SerialPortTurntableHex::releaseTurntable()
{
    resetAllHandleSpeedStates();
    // 释放三轴电机
    sendEnableCmd(1, false); sendEnableCmd(2, false); sendEnableCmd(3, false);
}

// ════════════════════════ 高级指令：停机 ════════════════════════

void SerialPortTurntableHex::closeTurntable()
{
    resetAllHandleSpeedStates();
    sendStopCmd(1);
    sendStopCmd(2);
    sendStopCmd(3);
}


// ════════════════════════ 高级指令：回零 ════════════════════════

void SerialPortTurntableHex::zeroTurntable()
{
    // 回零是状态切换指令。清除遥控轴状态，避免等待中的新速度覆盖回零。
    resetAllHandleSpeedStates();

    sendZeroCmd(1);
    sendZeroCmd(2);
    sendZeroCmd(3);
}


// ════════════════════════ 高级指令：复位 ════════════════════════

void SerialPortTurntableHex::resetTurntable()
{
    resetAllHandleSpeedStates();
    sendResetCmd();
}


// ════════════════════════ 位置模式 ════════════════════════

void SerialPortTurntableHex::sendPositionCmd(const PositionModeCmd1Hex &cmd)
{
    // CMD_POSITION (0x04)
    // params[0-2] = 位置值 (3B, 有符号, 0.0001°)
    // params[3-5] = 速度值 (3B, 有符号, 0.001°/s)
    // params[6-7] = 加速度 (2B, 无符号, 0.01°/s²)
    // params[8-13] = 0x00
    uint8_t params[14];
    std::memset(params, 0, 14);

    // 位置
    encode3BytesSigned(params + 0, cmd.anglePos, ANGLE_UNIT);

    // 速度
    encode3BytesSigned(params + 3, cmd.velocity, SPEED_UNIT);

    // 加速度 (°/s² → 0.01°/s²原始值, 范围 1-65535)
    uint16_t accelRaw = static_cast<uint16_t>(
        std::max(1, std::min(65535,
            static_cast<int>(std::round(cmd.acceleration * ACCEL_UNIT)))));
    encode2BytesUint(params + 6, accelRaw);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(cmd.axis, CMD_POSITION);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 速度模式 ════════════════════════

void SerialPortTurntableHex::sendVecCmd(const SpeedModeCmd1Hex &cmd)
{
    // CMD_SPEED (0x05)
    // params[0-2] = 0x00 (未使用)
    // params[3-5] = 速度值 (3B, 有符号, 0.001°/s)
    // params[6-7] = 加速度 (2B, 无符号, 0.01°/s²)
    // params[8-13] = 0x00
    uint8_t params[14];
    std::memset(params, 0, 14);

    // 速度
    encode3BytesSigned(params + 3, cmd.velocity, SPEED_UNIT);

    // 加速度 (°/s² → 0.01°/s²原始值, 范围 1-65535)
    uint16_t accelRaw = static_cast<uint16_t>(
        std::max(1, std::min(65535,
            static_cast<int>(std::round(cmd.acceleration * ACCEL_UNIT)))));
    encode2BytesUint(params + 6, accelRaw);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(cmd.axis, CMD_SPEED);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 摇摆模式 ════════════════════════

void SerialPortTurntableHex::sendSwingCmd(int axis, float freq, float amplitude)
{
    // CMD_SWING (0x06)
    // params[0-2] = 摇摆幅度 (3B, 有符号, 0.0001°)  → 幅度高位/中位/低位
    // params[3-5] = 0x00
    // params[6-7] = 摇摆频率 (2B, 无符号, 0.001Hz) → 频率高位/低位
    // params[8-13]= 0x00
    uint8_t params[14];
    std::memset(params, 0, 14);

    // 幅度 (3字节, 高-中-低, 大端, 0.0001°)
    encode3BytesSigned(params + 0, amplitude, ANGLE_UNIT);

    // 频率 (2字节, 高-低, 大端, 0.001Hz; 范围 0-65535 → 0-65.535Hz)
    uint16_t freqRaw = static_cast<uint16_t>(
        std::max(0, std::min(65535,
            static_cast<int>(std::round(freq * FREQ_UNIT)))));
    encode2BytesUint(params + 6, freqRaw);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(axis, CMD_SWING);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 250ms跟踪模式 (独立指令) ════════════════════════

void SerialPortTurntableHex::sendTrackCmd_1s(const TrackingSendCmd1Hex &cmd)
{
    // CMD_TRACK_250 (0x0A)
    // params[0-1]  = 秒时间 (2B, 无符号, 0-14399)
    // params[2-4]  = 角度1 (3B, 有符号, 0.0001°)
    // params[5-7]  = 角度2
    // params[8-10] = 角度3
    // params[11-13]= 角度4
    uint8_t params[14];
    std::memset(params, 0, 14);

    // 秒时间
    uint16_t secTime = static_cast<uint16_t>(
        std::max(0, std::min(14399, cmd.trackTime)));
    encode2BytesUint(params + 0, secTime);
    // qDebug().nospace() << "[HEX][TEST] 0x0A axis=" << cmd.axis
    //                    << " sec=" << secTime
    //                    << " angles=" << cmd.angle1 << "," << cmd.angle2
    //                    << "," << cmd.angle3 << "," << cmd.angle4;

    // 四个角度值 (每个对应一个时间点: 0.25s, 0.50s, 0.75s, 1.00s)
    encode3BytesSigned(params + 2,  cmd.angle1, ANGLE_UNIT);
    encode3BytesSigned(params + 5,  cmd.angle2, ANGLE_UNIT);
    encode3BytesSigned(params + 8,  cmd.angle3, ANGLE_UNIT);
    encode3BytesSigned(params + 11, cmd.angle4, ANGLE_UNIT);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(cmd.axis, CMD_TRACK_250);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 20ms跟踪模式 (联合指令) ════════════════════════

void SerialPortTurntableHex::sendTrackCmd_20ms(const TrackingSendCmd20msHex &cmd)
{
    // CMD_TRACK_20MS (0x0B) — 联合指令
    // params[0-2]  = 目标毫秒时间 (3B, 无符号, 大端)
    // params[3-5]  = 内框位置 (3B, 有符号, 0.0001°)
    // params[6-8]  = 中框位置
    // params[9-11] = 外框位置
    // params[12-13] = 0x00
    uint8_t params[14];
    std::memset(params, 0, 14);

    const uint32_t targetMs = static_cast<uint32_t>(
        std::max(0, std::min(14399999, cmd.timeMs)));
    encode3BytesUint(params + 0, targetMs);
    encode3BytesSigned(params + 3,  cmd.angle1, ANGLE_UNIT);  // 内框
    encode3BytesSigned(params + 6,  cmd.angle2, ANGLE_UNIT);  // 中框
    encode3BytesSigned(params + 9,  cmd.angle3, ANGLE_UNIT);  // 外框

    // 联合指令, 轴号任意
    uint8_t axis_cmd = ENCODE_AXIS_CMD(1, CMD_TRACK_20MS);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 跟踪修正 ════════════════════════

void SerialPortTurntableHex::sendTrackingCorrection(TrackingCorrectionCmdHex cmd)
{
    sendCorrectionCmd(cmd);
}

void SerialPortTurntableHex::zeroInnerAngleOffset()
{
    // 当前画面零位对应的内框偏置，使“当前内框角 + 偏置角”等于零。
    m_inner_angle_offset_deg = -m_current_inner_angle;
    qDebug() << "[HEX] 内框零位已校正，当前内框角:"
             << m_current_inner_angle
             << "°，偏置角:" << m_inner_angle_offset_deg << "°";
}


void SerialPortTurntableHex::sendCorrectionCmd(const TrackingCorrectionCmdHex &cmd)
{
    // CMD_CORRECTION (0x11)
    // 联合指令：byte2 高2位填 1/2/3 均可，低6位 = 0x11
    // params[0-2] = 内框修正角 (3B, 有符号, 0.0001°)
    // params[3-5] = 中框修正角
    // params[6-8] = 外框修正角
    // params[9-13]= 0x00
    if (cmd.target < 1 || cmd.target > 3) {
        qWarning() << "[HEX] 跟踪修正目标无效，应为1=内框、2=中框或3=外框:" << cmd.target;
        SendLat::discardClick(SendLat::Turntable);
        return;
    }
    if (!std::isfinite(cmd.innerDeviation)
            || !std::isfinite(cmd.middleDeviation)
            || !std::isfinite(cmd.outerDeviation)) {
        qWarning() << "[HEX] 跟踪修正角包含无效数值";
        SendLat::discardClick(SendLat::Turntable);
        return;
    }

    uint8_t params[14];
    std::memset(params, 0, 14);

    encode3BytesSigned(params + 0, cmd.innerDeviation, ANGLE_UNIT);
    encode3BytesSigned(params + 3, cmd.middleDeviation, ANGLE_UNIT);
    encode3BytesSigned(params + 6, cmd.outerDeviation, ANGLE_UNIT);

    const uint8_t axis_cmd = ENCODE_AXIS_CMD(cmd.target, CMD_CORRECTION);
    qDebug() << "[HEX] 跟踪修正:"
             << "目标" << cmd.target
             << "内框" << cmd.innerDeviation
             << "中框" << cmd.middleDeviation
             << "外框" << cmd.outerDeviation;
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 时间设置 ════════════════════════

void SerialPortTurntableHex::sendTimeSetCmd(int seconds)
{
    // CMD_TIME_SET (0x10) — 联合指令
    // params[0-1] = 秒时间 (2B, 无符号, 0-14399)
    // params[2-13] = 0x00
    uint8_t params[14];
    std::memset(params, 0, 14);

    uint16_t secTime = static_cast<uint16_t>(
        std::max(0, std::min(14399, seconds)));
    encode2BytesUint(params + 0, secTime);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(1, CMD_TIME_SET);  // 联合指令
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 时间同步 (对外接口) ════════════════════════

void SerialPortTurntableHex::sendTimesync(int seconds)
{
    // 秒时间设置只用于把转台的秒计时对齐到 0；跟踪目标时刻本身
    // 一律取状态帧里的转台时间，不再依赖本地 QElapsedTimer。
    sendTimeSetCmd(seconds);

    // 0x10 之后旧的时间反馈已不代表新的时基，等下一帧状态刷新后再发跟踪帧。
    m_turntableTimeValid = false;
    m_waitingStatusLogged = false;
    m_last20msTargetMs = -1;
}


// ════════════════════════ 程控模式 ════════════════════════

void SerialPortTurntableHex::ProgramModeChanged(int mode)
{
    const bool leavingExguide = (m_operationMode == 0 && mode != 0);

    m_operationMode = mode;
    m_isProgramMode = (mode == 1);
    m_trackingAllowed = (mode == 0);

    // 进入遥控模式后，首次速度控制先停车再等待伺服，确保不会继承其它模式
    // 已经建立的速度；离开遥控模式时清掉等待中的目标，避免反馈触发误下发。
    const bool remoteMode = (mode == 2);
    resetAllHandleSpeedStates(remoteMode);
    resetTrackingSmoother();

    if (leavingExguide) {
        // 外引导 1s 模式可能已经下发了最长约 1.2s 的未来轨迹。
        // 切出外引导时必须显式停车，不能只停止导引头的发送定时器。
        // m_trackingAllowed 已在上面关闭，因此排队中的旧跟踪帧即使随后到达
        // 也会被 sendTrackMode_* 丢弃，最终留在设备上的命令是停车。
        m_last20msTargetMs = -1;
        sendStopCmd(1);
        sendStopCmd(2);
        sendStopCmd(3);
        qDebug() << "[HEX] 已切出外引导模式，发送三轴停车指令";
    }
}

void SerialPortTurntableHex::resetTrackingSmoother()
{
    m_outerTrackingSmoother.reset(m_current_outter_angle);
    m_middleTrackingSmoother.reset(m_current_middle_angle);
    m_20msTrackingClock.invalidate();
}


void SerialPortTurntableHex::sendProgramMode(programSend_frameHex frame)
{
    if (!m_isProgramMode) {
        qDebug() << "[HEX] 未进入程控模式，无法发送程控指令";
        SendLat::discardClick(SendLat::Turntable);
        return;
    }

    if (frame.runtime <= 0) {
        qDebug() << "[HEX] 程控模式: runtime无效 (" << frame.runtime << ")";
        SendLat::discardClick(SendLat::Turntable);
        return;
    }

    if (frame.index == 0) {
        // ── 三轴联动 ──
        PositionModeCmd1Hex cmd1;
        cmd1.axis = 1;
        cmd1.acceleration = MAX_ACCELERATION_HEX;
        cmd1.velocity = (frame.inner_endangle - frame.current_inner_angle) / frame.runtime;
        cmd1.anglePos = frame.inner_endangle;
        sendPositionCmd(cmd1);

        PositionModeCmd1Hex cmd2;
        cmd2.axis = 2;
        cmd2.acceleration = MAX_ACCELERATION_HEX;
        cmd2.velocity = (frame.middle_endangle - frame.current_middle_angle) / frame.runtime;
        cmd2.anglePos = frame.middle_endangle;
        sendPositionCmd(cmd2);

        PositionModeCmd1Hex cmd3;
        cmd3.axis = 3;
        cmd3.acceleration = MAX_ACCELERATION_HEX;
        cmd3.velocity = (frame.outter_endangle - frame.current_outter_angle) / frame.runtime;
        cmd3.anglePos = frame.outter_endangle;
        sendPositionCmd(cmd3);
    } else if (frame.index == 1) {
        // ── 仅内框 ──
        PositionModeCmd1Hex cmd1;
        cmd1.axis = 1;
        cmd1.acceleration = MAX_ACCELERATION_HEX;
        cmd1.velocity = (frame.inner_endangle - frame.current_inner_angle) / frame.runtime;
        cmd1.anglePos = frame.inner_endangle;
        sendPositionCmd(cmd1);
    } else if (frame.index == 2) {
        // ── 仅中框 ──
        PositionModeCmd1Hex cmd2;
        cmd2.axis = 2;
        cmd2.acceleration = MAX_ACCELERATION_HEX;
        cmd2.velocity = (frame.middle_endangle - frame.current_middle_angle) / frame.runtime;
        cmd2.anglePos = frame.middle_endangle;
        sendPositionCmd(cmd2);
    } else if (frame.index == 3) {
        // ── 仅外框 ──
        PositionModeCmd1Hex cmd3;
        cmd3.axis = 3;
        cmd3.acceleration = MAX_ACCELERATION_HEX;
        cmd3.velocity = (frame.outter_endangle - frame.current_outter_angle) / frame.runtime;
        cmd3.anglePos = frame.outter_endangle;
        sendPositionCmd(cmd3);
    } else {
        qDebug() << "[HEX] 程控模式: 无效index" << frame.index;
    }
}

void SerialPortTurntableHex::sendProgramAxisCommand(ProgramAxisCommandHex command)
{
    if (!m_isProgramMode) {
        qDebug() << "[HEX] 未进入程控模式，无法发送程控轴指令";
        SendLat::discardClick(SendLat::Turntable);
        return;
    }

    if (command.axis < 1 || command.axis > 3 || command.mode < 0 || command.mode > 1) {
        qDebug() << "[HEX] 程控模式: 无效轴号或模式" << command.axis << command.mode;
        SendLat::discardClick(SendLat::Turntable);
        return;
    }

    if (command.mode == 0) {
        PositionModeCmd1Hex cmd;
        cmd.axis = command.axis;
        cmd.anglePos = command.position;
        cmd.velocity = command.velocity;
        cmd.acceleration = command.acceleration;
        sendPositionCmd(cmd);
    } else {
        SpeedModeCmd1Hex cmd;
        cmd.axis = command.axis;
        cmd.velocity = command.velocity;
        cmd.acceleration = command.acceleration;
        sendVecCmd(cmd);
    }
}

void SerialPortTurntableHex::enableAxis(int axis, bool enabled)
{
    if (axis < 1 || axis > 3)
        return;

    resetHandleSpeedState(axis);
    sendEnableCmd(axis, enabled);
}

void SerialPortTurntableHex::stopAxis(int axis)
{
    if (axis < 1 || axis > 3)
        return;

    resetHandleSpeedState(axis);
    sendStopCmd(axis);
}

void SerialPortTurntableHex::zeroAxis(int axis)
{
    if (axis < 1 || axis > 3)
        return;

    resetHandleSpeedState(axis);
    sendZeroCmd(axis);
}


// ════════════════════════ 外引导模式 ════════════════════════

qint64 SerialPortTurntableHex::estimatedTurntableTimeMs() const
{
    if (!m_turntableTimeValid || !m_statusAgeClock.isValid())
        return -1;

    const qint64 ageMs = m_statusAgeClock.elapsed();
    // 状态帧周期 5ms；超过 100ms 视为反馈中断，不再拿旧时间外推。
    if (ageMs < 0 || ageMs > 100)
        return -1;

    // 状态帧从转台发出到本机解析约有 1-2ms 的固定延迟；补上后估算值
    // 才更接近发送时刻，避免刚好落在 20ms 边界之后。
    constexpr qint64 kStatusFrameLatencyMs = 2;

    // 转台时间按 4 小时循环，估算值也要落回 0-14399999。
    constexpr qint64 kTrackLoopMs = 14400000;
    return (m_latestTurntableTimeMs + ageMs + kStatusFrameLatencyMs) % kTrackLoopMs;
}

void SerialPortTurntableHex::sendTrackMode_1s(sendExGuideData frame1, sendExGuideData frame2)
{
    if (!m_trackingAllowed)
        return;

    // 250ms跟踪模式: 上位机每隔1秒发送一组跟踪数据
    // frame1 对应外框(轴3, 方位角), frame2 对应中框(轴2, 俯仰角)
    // 内框保持当前角度不变

    // 目标秒时间取转台状态帧时间：当前秒的下一秒。
    // 不再使用导引头本地调度生成的 frame*.time，避免 0x10 被拒绝或丢失后
    // 本地时基与转台时基错位，导致整组跟踪数据被判为过期。
    const qint64 turntableNowMs = estimatedTurntableTimeMs();
    if (turntableNowMs < 0) {
        if (!m_waitingStatusLogged) {
            qDebug() << "[HEX] 1s跟踪: 转台状态帧不可用，暂不发送";
            m_waitingStatusLogged = true;
        }
        return;
    }
    const qint64 nextSecond = (turntableNowMs / 1000 + 1) % 14400;

    //可以判断一下角度是否超过转台运动范围并给出提示，俯仰范围：-10~70，方位范围 -100~100 ，滚转范围 -200~200

    // 图像头/激光头画面零位与转台内框零位存在偏置，坐标变换使用补偿后的内框角。
    // 内框正角对应逆时针旋转补偿，因此直接将补偿后的内框角传给 deRoll 接口。
    const float innerAngleForTransform = m_current_inner_angle + m_inner_angle_offset_deg;

    //对方位角与俯仰角结构体进行更新
    auto ang = seeker::deRollAboutBoresightDeg(frame1.angle1, frame2.angle1, innerAngleForTransform);
    frame1.angle1 = ang.az;
    frame2.angle1 = ang.el;
    ang = seeker::deRollAboutBoresightDeg(frame1.angle2, frame2.angle2, innerAngleForTransform);
    frame1.angle2 = ang.az;
    frame2.angle2 = ang.el;
    ang = seeker::deRollAboutBoresightDeg(frame1.angle3, frame2.angle3, innerAngleForTransform);
    frame1.angle3 = ang.az;
    frame2.angle3 = ang.el;
    ang = seeker::deRollAboutBoresightDeg(frame1.angle4, frame2.angle4, innerAngleForTransform);
    frame1.angle4 = ang.az;
    frame2.angle4 = ang.el;

    // 每个 250 ms 点都以当前转台反馈为起点生成受速度/加速度约束的轨迹。
    // 外框正方向与导引头方位角符号相反，中框与俯仰角同向。
    const std::array<double, 4> outerOffsets = {{
        -static_cast<double>(frame1.angle1),
        -static_cast<double>(frame1.angle2),
        -static_cast<double>(frame1.angle3),
        -static_cast<double>(frame1.angle4)
    }};
    const std::array<double, 4> middleOffsets = {{
        static_cast<double>(frame2.angle1),
        static_cast<double>(frame2.angle2),
        static_cast<double>(frame2.angle3),
        static_cast<double>(frame2.angle4)
    }};
    const std::array<double, 4> outerPositions =
            m_outerTrackingSmoother.preview(
                m_current_outter_angle, outerOffsets, 0.25);
    const std::array<double, 4> middlePositions =
            m_middleTrackingSmoother.preview(
                m_current_middle_angle, middleOffsets, 0.25);

    // ── 外框跟踪 (轴3) ──
    TrackingSendCmd1Hex cmdOuter;
    cmdOuter.axis = 3;  // 外框
    cmdOuter.trackTime = static_cast<int>(nextSecond);
    cmdOuter.angle1 = static_cast<float>(outerPositions[0]);
    cmdOuter.angle2 = static_cast<float>(outerPositions[1]);
    cmdOuter.angle3 = static_cast<float>(outerPositions[2]);
    cmdOuter.angle4 = static_cast<float>(outerPositions[3]);
    if (cmdOuter.angle1 < -100 || cmdOuter.angle1 > 100 || cmdOuter.angle2 < -100 || cmdOuter.angle2 > 100 ||
        cmdOuter.angle3 < -100 || cmdOuter.angle3 > 100 || cmdOuter.angle4 < -100 || cmdOuter.angle4 > 100) {
        qDebug() << "外框跟踪角度将超出范围，请及时调整转台";
    }else{
        sendTrackCmd_1s(cmdOuter);
    }
    

    // ── 中框跟踪 (轴2) ──
    TrackingSendCmd1Hex cmdMiddle;
    cmdMiddle.axis = 2;  // 中框
    cmdMiddle.trackTime = static_cast<int>(nextSecond);
    cmdMiddle.angle1 = static_cast<float>(middlePositions[0]);
    cmdMiddle.angle2 = static_cast<float>(middlePositions[1]);
    cmdMiddle.angle3 = static_cast<float>(middlePositions[2]);
    cmdMiddle.angle4 = static_cast<float>(middlePositions[3]);
    if (cmdMiddle.angle1 < -10 || cmdMiddle.angle1 > 70 || cmdMiddle.angle2 < -10 || cmdMiddle.angle2 > 70 ||
        cmdMiddle.angle3 < -10 || cmdMiddle.angle3 > 70 || cmdMiddle.angle4 < -10 || cmdMiddle.angle4 > 70) {
        qDebug() << "中框跟踪角度将超出范围，请及时调整转台";
    } else {
        sendTrackCmd_1s(cmdMiddle);
    }

    // [TEST] 同时向内框发送0x0A，验证250ms模式是否要求三轴共同进入。
    // 内框四个插值点保持当前反馈角度。
    TrackingSendCmd1Hex cmdInner;
    cmdInner.axis = 1;
    cmdInner.trackTime = static_cast<int>(nextSecond);
    cmdInner.angle1 = cmdInner.angle2 =
    cmdInner.angle3 = cmdInner.angle4 = m_current_inner_angle;
    if (cmdInner.angle1 < -200 || cmdInner.angle1 > 200 || cmdInner.angle2 < -200 || cmdInner.angle2 > 200 ||
        cmdInner.angle3 < -200 || cmdInner.angle3 > 200 || cmdInner.angle4 < -200 || cmdInner.angle4 > 200) {
        qDebug() << "内框跟踪角度超出范围";
    } else {
        sendTrackCmd_1s(cmdInner);
    }
}


void SerialPortTurntableHex::sendTrackMode_20ms(double yawangle, double pitchangle)
{
    if (!m_trackingAllowed)
        return;

    // 20ms目标时刻取转台状态帧时间，并对齐到下一个20ms边界。
    const qint64 turntableNowMs = estimatedTurntableTimeMs();
    if (turntableNowMs < 0) {
        if (!m_waitingStatusLogged) {
            qDebug() << "[HEX] 20ms跟踪: 转台状态帧不可用，暂不发送";
            m_waitingStatusLogged = true;
        }
        return;
    }

    // 对俯仰角和方位角进行变换；画面零位偏置只用于坐标变换，不改变内框保持角。
    // 内框正角对应逆时针旋转补偿，因此直接将补偿后的内框角传给 deRoll 接口。
    const float innerAngleForTransform = m_current_inner_angle + m_inner_angle_offset_deg;
    auto ang = seeker::deRollAboutBoresightDeg(yawangle, pitchangle, innerAngleForTransform);
    yawangle = ang.az;
    pitchangle = ang.el;

    // 同一 20 ms 目标时刻只生成一次斜坡，定时器抖动不会多推进限速状态。
    constexpr qint64 kTrackLoopMs = 14400000; // 4小时循环
    constexpr qint64 kTrackStepMs = 20;
    qint64 targetMs = ((turntableNowMs / kTrackStepMs) + 1) * kTrackStepMs;
    if (targetMs >= kTrackLoopMs)
        targetMs = 0;
    if (targetMs == m_last20msTargetMs)
        return;

    double dtSec = 0.02;
    if (!m_20msTrackingClock.isValid()) {
        resetTrackingSmoother();
        m_outerTrackingSmoother.reset(m_current_outter_angle);
        m_middleTrackingSmoother.reset(m_current_middle_angle);
        m_20msTrackingClock.start();
    } else {
        dtSec = static_cast<double>(m_20msTrackingClock.restart()) / 1000.0;
        // 避免串口线程短时卡顿后一次推进过大。
        dtSec = std::max(0.005, std::min(0.100, dtSec));
    }

    // 外框正方向与导引头方位角符号相反，中框与俯仰角同向。
    const TrackingTargetSmoother::StepResult outerStep =
            m_outerTrackingSmoother.update(
                m_current_outter_angle, -yawangle, dtSec);
    const TrackingTargetSmoother::StepResult middleStep =
            m_middleTrackingSmoother.update(
                m_current_middle_angle, pitchangle, dtSec);

    // 20ms跟踪模式 (联合指令, 一帧包含三轴数据)
    TrackingSendCmd20msHex m_cmd;
    m_cmd.angle1 = m_current_inner_angle;                    // 内框保持
    m_cmd.angle2 = static_cast<float>(middleStep.positionDeg); // 中框目标
    m_cmd.angle3 = static_cast<float>(outerStep.positionDeg);  // 外框目标
    if (m_cmd.angle2 < -10 || m_cmd.angle2 > 70) {
        qDebug() << "中框跟踪角度将超出范围，请及时调整转台";
    } else if (m_cmd.angle3 < -100 || m_cmd.angle3 > 100) {
        qDebug() << "外框跟踪角度将超出范围，请及时调整转台";
    } else {
        m_cmd.timeMs = static_cast<int>(targetMs);
        m_last20msTargetMs = targetMs;
        sendTrackCmd_20ms(m_cmd);
    }
}

// ════════════════════════ 摇摆模式  ═══════════════════════════
void SerialPortTurntableHex::sendSwingMode(int index, float amplitude, float freq)
{
    if (index == 0) {
        //三轴同时正弦运动
        sendSwingCmd(1, freq, amplitude);
        sendSwingCmd(2, freq, amplitude);
        sendSwingCmd(3, freq, amplitude);
    } else if (index == 1) {
        // ── 仅内框 ──
        sendSwingCmd(1, freq, amplitude);
    } else if (index == 2) {
        // ── 仅中框 ──
        sendSwingCmd(2, freq, amplitude);
    } else if (index == 3) {
        // ── 仅外框 ──
        sendSwingCmd(3, freq, amplitude);
    } else {
        qDebug() << " 程控模式: 无效index，无法正弦运动" << endl;
    }

}


// ════════════════════════ 遥控模式（手柄控制）═══════════════════════════

void SerialPortTurntableHex::sendHandleSpeedCmd(int axis, float velocity)
{
    SpeedModeCmd1Hex cmd;
    cmd.axis = axis;
    cmd.acceleration = MAX_ACCELERATION_HEX;
    cmd.velocity = velocity;
    sendVecCmd(cmd);
}


void SerialPortTurntableHex::resetHandleSpeedState(int axis, bool hardwareMayBeMoving)
{
    if (axis < 1 || axis > 3)
        return;

    HandleSpeedAxisState &state = m_handleSpeedAxes[axis];
    state.state = HandleSpeedState::Idle;
    state.activeSpeed = 0.0f;
    state.pendingSpeed = 0.0f;
    state.hardwareMayBeMoving = hardwareMayBeMoving;
    state.feedbackStatus = -1;
    state.feedbackAlarm = -1;
    state.sawNonServoAfterStop = false;
    state.feedbackFramesSinceStop = 0;
}


void SerialPortTurntableHex::resetAllHandleSpeedStates(bool hardwareMayBeMoving)
{
    for (int axis = 1; axis <= 3; ++axis)
        resetHandleSpeedState(axis, hardwareMayBeMoving);
}


void SerialPortTurntableHex::setHandleSpeedTarget(int axis, float target)
{
    if (axis < 1 || axis > 3)
        return;

    if (!std::isfinite(target))
        target = 0.0f;
    target = std::max(-MAX_SPEED_HEX, std::min(MAX_SPEED_HEX, target));

    // 与协议编码精度对齐，避免浮点尾差被误判为目标变化。
    target = static_cast<float>(speedToRaw(target)) / SPEED_UNIT;

    HandleSpeedAxisState &state = m_handleSpeedAxes[axis];
    const bool targetIsStop = speedToRaw(target) == 0;

    switch (state.state) {
    case HandleSpeedState::Idle:
        if (targetIsStop) {
            // 进入遥控模式时 hardwareMayBeMoving 会置位；此时仍需停车一次，
            // 以清除其它模式可能遗留的速度，但不能在静止时周期性重发停车。
            if (state.hardwareMayBeMoving) {
                state.pendingSpeed = 0.0f;
                state.hardwareMayBeMoving = false;
                state.state = HandleSpeedState::WaitingForServo;
                state.sawNonServoAfterStop = false;
                state.feedbackFramesSinceStop = 0;
                sendStopCmd(axis);
            }
            return;
        }

        if (state.hardwareMayBeMoving) {
            // 当前速度不确定，先停车，再等待该轴回到伺服状态后设置新速度。
            state.pendingSpeed = target;
            state.hardwareMayBeMoving = false;
            state.state = HandleSpeedState::WaitingForServo;
            state.sawNonServoAfterStop = false;
            state.feedbackFramesSinceStop = 0;
            sendStopCmd(axis);
        } else {
            sendHandleSpeedCmd(axis, target);
            state.activeSpeed = target;
            state.hardwareMayBeMoving = true;
            state.state = HandleSpeedState::Running;
        }
        break;

    case HandleSpeedState::Running:
        if (targetIsStop) {
            state.pendingSpeed = 0.0f;
            state.hardwareMayBeMoving = false;
            state.state = HandleSpeedState::WaitingForServo;
            state.sawNonServoAfterStop = false;
            state.feedbackFramesSinceStop = 0;
            sendStopCmd(axis);
        } else if (std::abs(target - state.activeSpeed) >= HANDLE_SPEED_CHANGE_EPSILON) {
            // 转台不允许在速率模式下直接改目标速度，必须先停车。
            state.pendingSpeed = target;
            state.hardwareMayBeMoving = false;
            state.state = HandleSpeedState::WaitingForServo;
            state.sawNonServoAfterStop = false;
            state.feedbackFramesSinceStop = 0;
            sendStopCmd(axis);
        }
        break;

    case HandleSpeedState::WaitingForServo:
        // 停车过程可能持续多个手柄周期，只更新最终目标，不重复发送停车。
        state.pendingSpeed = target;
        break;
    }
}


void SerialPortTurntableHex::applyPendingHandleSpeed(int axis, int status, int alarm)
{
    if (axis < 1 || axis > 3)
        return;

    HandleSpeedAxisState &state = m_handleSpeedAxes[axis];
    state.feedbackStatus = status;
    state.feedbackAlarm = alarm;

    if (state.state != HandleSpeedState::WaitingForServo || alarm != 0)
        return;

    ++state.feedbackFramesSinceStop;
    if (status != 0x01) {
        state.sawNonServoAfterStop = true;
        return;
    }

    // 刚发停车后的第一帧可能仍是停车前的旧状态。优先等待明确的非伺服状态；
    // 如果控制器不上报中间态，则至少再等一帧，避免旧反馈触发新速度。
    if (!state.sawNonServoAfterStop && state.feedbackFramesSinceStop < 2)
        return;

    const bool targetIsStop = speedToRaw(state.pendingSpeed) == 0;
    if (targetIsStop) {
        state.activeSpeed = 0.0f;
        state.pendingSpeed = 0.0f;
        state.hardwareMayBeMoving = false;
        state.state = HandleSpeedState::Idle;
        return;
    }

    const float velocity = state.pendingSpeed;
    sendHandleSpeedCmd(axis, velocity);
    state.activeSpeed = velocity;
    state.pendingSpeed = 0.0f;
    state.hardwareMayBeMoving = true;
    state.state = HandleSpeedState::Running;

    qDebug() << "[HEX] 遥控速度动态切换: 轴" << axis
             << "新速度" << velocity << "°/s";
}


void SerialPortTurntableHex::sendHandleMode(float axisLeftX, float axisLeftY, float axisRightX,
                                             float buttonL2, float buttonR2,
                                             bool buttonA, bool buttonB, int Acount, int Bcount)
{
    if (buttonL2 != 0.0f || buttonR2 != 0.0f) {
        // 进入位置模式时不继承速率模式下的旧速度。这里最多发送一次停车；
        // 后续周期状态已变为 WaitingForServo，不会重复发送。
        setHandleSpeedTarget(1, 0.0f);
        setHandleSpeedTarget(2, 0.0f);
        setHandleSpeedTarget(3, 0.0f);

        // ── 位置模式 (扳机按下 → 步进/步退) ──
        if (buttonR2 > 0.9f && buttonL2 > 0.9f) {
            // 两扳机同时按下: 内框步进/步退
            PositionModeCmd1Hex cmd;
            cmd.axis = 1;
            cmd.acceleration = MAX_ACCELERATION_HEX;
            if (buttonA && Acount >= 1) {
                cmd.velocity = MAX_SPEED_HEX ;
                cmd.anglePos = m_current_inner_angle + m_step_angle;
                if(cmd.anglePos < 199.995 && cmd.anglePos >-199.995){
                        sendPositionCmd(cmd);
                }else{
                        qDebug() << "内框接近临界角，停止内框步进";
                }
                //sendPositionCmd(cmd);
            } else if (buttonB && Bcount >= 1) {
                cmd.velocity = -MAX_SPEED_HEX ;
                cmd.anglePos = m_current_inner_angle - m_step_angle;
                if(cmd.anglePos < 199.995 && cmd.anglePos >-199.995){
                        sendPositionCmd(cmd);
                }else{
                        qDebug() << "内框接近临界角，停止内框步进";
                }
                //sendPositionCmd(cmd);
            }
        } else if (buttonL2 != 0.0f) {
            // 左扳机: 外框
            PositionModeCmd1Hex cmd;
            cmd.axis = 3;
            cmd.acceleration = MAX_ACCELERATION_HEX;
            if (buttonA && Acount >= 1) {
                cmd.velocity = MAX_SPEED_HEX ;
                cmd.anglePos = m_current_outter_angle + m_step_angle;
                if(cmd.anglePos < 99.995 && cmd.anglePos >-99.995){
                    sendPositionCmd(cmd);
                }else{
                    qDebug() << "外框接近临界角，停止外框步进";
                }
                //sendPositionCmd(cmd);
            } else if (buttonB && Bcount >= 1) {
                cmd.velocity = -MAX_SPEED_HEX ;
                cmd.anglePos = m_current_outter_angle - m_step_angle;
                if(cmd.anglePos < 99.995 && cmd.anglePos >-99.995){
                    sendPositionCmd(cmd);
                }else{
                    qDebug() << "外框接近临界角，停止外框步进";
                }
                //sendPositionCmd(cmd);
            }
        } else if (buttonR2 != 0.0f) {
            // 右扳机: 中框
            PositionModeCmd1Hex cmd;
            cmd.axis = 2;
            cmd.acceleration = MAX_ACCELERATION_HEX;
            if (buttonA && Acount >= 1) {
                cmd.velocity = MAX_SPEED_HEX ;
                cmd.anglePos = m_current_middle_angle + m_step_angle;
                if(cmd.anglePos < 69.995 && cmd.anglePos >-9.995){
                    sendPositionCmd(cmd);
                }else{
                    qDebug() << "中框接近临界角，停止中框步进控制";
                }
                //sendPositionCmd(cmd);
            } else if (buttonB && Bcount >= 1) {
                cmd.velocity = -MAX_SPEED_HEX ;
                cmd.anglePos = m_current_middle_angle - m_step_angle;
                if(cmd.anglePos < 69.995 && cmd.anglePos >-9.995){
                    sendPositionCmd(cmd);
                }else{
                    qDebug() << "中框接近临界角，停止中框步进控制";
                }
                //sendPositionCmd(cmd);
            }
        }
    } else {
        // ── 速度模式 (无扳机 → 摇杆直接控制速度) ──
        // axisLeftX → 外框, axisLeftY → 中框, axisRightX → 内框
        const float innerTarget = axisRightX * MAX_SPEED_HEX * m_speed_multiplier;
        const float middleTarget = axisLeftY * MAX_SPEED_HEX * m_speed_multiplier * (-1);   //中框运动方向调整，使手柄与转台保持一致
        const float outterTarget = axisLeftX * MAX_SPEED_HEX * m_speed_multiplier;

        if (m_current_inner_angle < 200.0f && m_current_inner_angle > -200.0f) {
            setHandleSpeedTarget(1, innerTarget);
        } else {
            setHandleSpeedTarget(1, 0.0f);
            qDebug() << "内框接近临界角，停止内框速度控制";
        }

        if (m_current_middle_angle < 80.0f && m_current_middle_angle > -10.0f) {
            setHandleSpeedTarget(2, middleTarget);
        } else {
            setHandleSpeedTarget(2, 0.0f);
            qDebug() << "中框接近临界角，停止中框速度控制";
        }

        if (m_current_outter_angle < 100.0f && m_current_outter_angle > -100.0f) {
            setHandleSpeedTarget(3, outterTarget);
        } else {
            setHandleSpeedTarget(3, 0.0f);
            qDebug() << "外框接近临界角，停止外框速度控制";
        }
    }
}


// ════════════════════════ 串口管理 ════════════════════════

void SerialPortTurntableHex::onOpenPort(const QString &name, int baud)
{
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}

void SerialPortTurntableHex::onClosePort()  { SerialPort::close(); emit portClosed(); }
void SerialPortTurntableHex::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }


// ════════════════════════ TurntableSendDataHex ════════════════════════

TurntableSendDataHex::TurntableSendDataHex(QObject *parent)
    : QObject(parent)
{
}

void TurntableSendDataHex::buildFrame(int m_index)
{
    if (m_runtime <= 0) {
        qDebug() << "[HEX] 程控模式发送失败: runtime无效 (" << m_runtime << ")";
        return;
    }

    programSend_frameHex m_frame;
    m_frame.index = m_index;
    m_frame.runtime = m_runtime;

    switch (m_index) {
    case 0:     // 三轴控制
        m_frame.current_inner_angle  = m_current_inner_angle;
        m_frame.inner_endangle       = m_inner_endangle;
        m_frame.current_middle_angle = m_current_middle_angle;
        m_frame.middle_endangle      = m_middle_endangle;
        m_frame.current_outter_angle = m_current_outter_angle;
        m_frame.outter_endangle      = m_outter_endangle;
        break;
    case 1:     // 仅内框
        m_frame.current_inner_angle  = m_current_inner_angle;
        m_frame.inner_endangle       = m_inner_endangle;
        break;
    case 2:     // 仅中框
        m_frame.current_middle_angle = m_current_middle_angle;
        m_frame.middle_endangle      = m_middle_endangle;
        break;
    case 3:     // 仅外框
        m_frame.current_outter_angle = m_current_outter_angle;
        m_frame.outter_endangle      = m_outter_endangle;
        break;
    default:
        qDebug() << "[HEX] 程控模式发送失败: 无效index";
        return;
    }

    emit requestSendProgramMode(m_frame);
}

void TurntableSendDataHex::sendProgramCommand(int axis, int mode, float position,
                                              float velocity, float acceleration)
{
    ProgramAxisCommandHex command;
    command.axis = axis;
    command.mode = mode;
    command.position = position;
    command.velocity = velocity;
    command.acceleration = acceleration;
    emit requestProgramAxisCommand(command);
}

void TurntableSendDataHex::setAxisEnabled(int axis, bool enabled)
{
    emit reqAxisEnable(axis, enabled);
}

void TurntableSendDataHex::stopAxis(int axis)
{
    emit reqAxisStop(axis);
}

void TurntableSendDataHex::zeroAxis(int axis)
{
    emit reqAxisZero(axis);
}


void TurntableSendDataHex::requestTrackingCorrection(float innerDeviation,
                                                     float middleDeviation,
                                                     float outerDeviation,
                                                     int target)
{
    TrackingCorrectionCmdHex cmd;
    cmd.target = target;
    cmd.innerDeviation = innerDeviation;
    cmd.middleDeviation = middleDeviation;
    cmd.outerDeviation = outerDeviation;
    emit reqTrackingCorrection(cmd);
}

void TurntableSendDataHex::requestInnerAngleZeroCorrection()
{
    emit reqInnerAngleZeroCorrection();
}


// ════════════════════════ TurntableDataHex ════════════════════════

TurntableDataHex::TurntableDataHex(QObject *parent)
    : QObject(parent)
{
}

void TurntableDataHex::setRemoteStepAngle(float stepAngle)
{
    if (stepAngle == m_remoteStepAngle)
        return;

    m_remoteStepAngle = stepAngle;
    emit remoteStepAngleChanged();
    emit stepAngleChanged(stepAngle);
}

void TurntableDataHex::setRemoteSpeedMultiplier(float speedMultiplier)
{
    if (speedMultiplier == m_remoteSpeedMultiplier)
        return;

    m_remoteSpeedMultiplier = speedMultiplier;
    emit remoteSpeedMultiplierChanged();
    emit speedMultiplierChanged(speedMultiplier);
}

void TurntableDataHex::setPortOpen(bool open)
{
    if (m_portOpen != open) { m_portOpen = open; emit portOpenChanged(); }
}

void TurntableDataHex::setPortList(const QStringList &ports)
{
    if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); }
}

void TurntableDataHex::setError(const QString &msg)
{
    if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); }
}

void TurntableDataHex::updateframe(StatusFeedbackHex recvdata)
{
    if (m_time != recvdata.m_time) {
        m_time = recvdata.m_time;
        emit timeChanged();
    }
    if (m_inner_alarm != recvdata.m_inner_alarm) {
        m_inner_alarm = recvdata.m_inner_alarm;
        emit inner_alarmChanged();
    }
    if (m_inner_status != recvdata.m_inner_status) {
        m_inner_status = recvdata.m_inner_status;
        emit inner_statusChanged();
    }
    if (m_inner_angle != recvdata.m_inner_angle) {
        m_inner_angle = recvdata.m_inner_angle;
        emit inner_angleChanged();
        emit myinner_angleChanged(m_inner_angle);
    }
    if (m_inner_ctlDeviation != recvdata.m_inner_ctlDeviation) {
        m_inner_ctlDeviation = recvdata.m_inner_ctlDeviation;
        emit inner_ctlDeviationChanged();
    }
    if (m_middle_alarm != recvdata.m_middle_alarm) {
        m_middle_alarm = recvdata.m_middle_alarm;
        emit middle_alarmChanged();
    }
    if (m_middle_status != recvdata.m_middle_status) {
        m_middle_status = recvdata.m_middle_status;
        emit middle_statusChanged();
    }
    if (m_middle_angle != recvdata.m_middle_angle) {
        m_middle_angle = recvdata.m_middle_angle;
        emit middle_angleChanged();
        emit mymiddle_angleChanged(m_middle_angle);
    }
    if (m_middle_ctlDeviation != recvdata.m_middle_ctlDeviation) {
        m_middle_ctlDeviation = recvdata.m_middle_ctlDeviation;
        emit middle_ctlDeviationChanged();
    }
    if (m_outter_alarm != recvdata.m_outter_alarm) {
        m_outter_alarm = recvdata.m_outter_alarm;
        emit outter_alarmChanged();
    }
    if (m_outter_status != recvdata.m_outter_status) {
        m_outter_status = recvdata.m_outter_status;
        emit outter_statusChanged();
    }
    if (m_outter_angle != recvdata.m_outter_angle) {
        m_outter_angle = recvdata.m_outter_angle;
        emit outter_angleChanged();
        emit myoutter_angleChanged(m_outter_angle);
    }
    if (m_outter_ctlDeviation != recvdata.m_outter_ctlDeviation) {
        m_outter_ctlDeviation = recvdata.m_outter_ctlDeviation;
        emit outter_ctlDeviationChanged();
    }
    if (m_hasSecPulse != recvdata.m_hasSecPulse) {
        m_hasSecPulse = recvdata.m_hasSecPulse;
        emit hasSecPulseChanged();
    }
    if (m_cmdHint != recvdata.m_cmdHint) {
        m_cmdHint = recvdata.m_cmdHint;
        emit cmdHintChanged();
    }
}

void TurntableDataHex::openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void TurntableDataHex::closePort()                                     { emit requestClosePort(); }
void TurntableDataHex::scanPorts()                                     { emit requestScanPorts(); }
