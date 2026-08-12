#include "serialport_turntable_HEX.h"
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <cmath>
#include <cstring>
#include <QTime>

#define MAX_SPEED_HEX 12.0f

// ════════════════════════ HEX状态码 → QML兼容编码转换 ════════════════════════
//
// HEX协议: 1字节 = 高4位报警 + 低4位状态
//   状态(低4位): 0=空闲, 1=伺服, 2=回零, 3=位置, 4=速率, 5=速率稳定,
//                6=摇摆, 7=摇摆稳定, A=250ms跟踪, C=5ms跟踪, F=停车
//   报警(高4位): 0=无, 1=驱动器, 2=伺服超差, 5=时钟同步, 6=参数初始化,
//                7=限位开关, 8=传感器, A=瞬态电流, B=连续电流
//
// QML端沿用ASCII协议遗留编码:
//   0x01=伺服, 0x02=回零, 0x03=位置, 0x04=速率, 0x05=速率稳定,
//   0x06=摇摆, 0x07=摇摆稳定, 0x08=停车, 0x09=跟踪1, 0x0B=跟踪2
//   0x1F=驱动器报警, 0x20=伺服超差, 0x23=时钟同步, 0x24=初始化,
//   0x25=限位开关, 0x26=传感器, 0x29=瞬态电流, 0x2A=连续电流

static int convertHexStatusToLegacy(uint8_t alarmStatus)
{
    int alarm  = (alarmStatus >> 4) & 0x0F;
    int status = alarmStatus & 0x0F;

    if (alarm == 0) {
        // 正常状态 → QML遗留编码
        switch (status) {
            case 0x0: return 0x00;   // 空闲
            case 0x1: return 0x01;   // 伺服
            case 0x2: return 0x02;   // 回零执行中
            case 0x3: return 0x03;   // 位置执行中
            case 0x4: return 0x04;   // 速率执行中
            case 0x5: return 0x05;   // 速率稳定
            case 0x6: return 0x06;   // 摇摆执行中
            case 0x7: return 0x07;   // 摇摆稳定
            case 0xA: return 0x09;   // 250ms跟踪模式
            case 0xC: return 0x0B;   // 5ms跟踪模式
            case 0xF: return 0x08;   // 停车执行中
            default:  return status; // 未知状态原样返回
        }
    } else {
        // 报警状态 → QML遗留报警编码
        switch (alarm) {
            case 0x1: return 0x1F;   // 驱动器报警
            case 0x2: return 0x20;   // 伺服超差报警
            case 0x5: return 0x23;   // 时钟同步报警
            case 0x6: return 0x24;   // 参数初始化错误
            case 0x7: return 0x25;   // 限位开关同时导通
            case 0x8: return 0x26;   // 角度传感器数据错误
            case 0xA: return 0x29;   // 瞬态电流报警
            case 0xB: return 0x2A;   // 连续电流报警
            default:  return (alarm << 4) | 0x0F;  // 未知报警
        }
    }
}

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
        qWarning() << "[HEX] 串口未打开，无法发送指令";
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

    // 内框状态 — 转换为QML兼容编码
    feedback.m_inner_statusnumber = convertHexStatusToLegacy(buf[5]);

    // 内框角度 (3字节有符号, 0.0001°)
    feedback.m_inner_angle = decode3BytesSigned(buf + 6, ANGLE_UNIT);

    // 内框偏差
    feedback.m_inner_ctlDeviation = decode3BytesSigned(buf + 9, ANGLE_UNIT);

    // 中框状态
    feedback.m_middle_statusnumber = convertHexStatusToLegacy(buf[12]);

    // 中框角度
    feedback.m_middle_angle = decode3BytesSigned(buf + 13, ANGLE_UNIT);

    // 中框偏差
    feedback.m_middle_ctlDeviation = decode3BytesSigned(buf + 16, ANGLE_UNIT);

    // 外框状态
    feedback.m_outter_statusnumber = convertHexStatusToLegacy(buf[19]);

    // 外框角度
    feedback.m_outter_angle = decode3BytesSigned(buf + 20, ANGLE_UNIT);

    // 外框偏差
    feedback.m_outter_ctlDeviation = decode3BytesSigned(buf + 23, ANGLE_UNIT);

    // 秒脉冲 + 指令提示
    feedback.m_hasSecPulse = (buf[26] & 0x80) ? 1 : 0;
    feedback.m_cmdHint     = buf[26] & 0x3F;     // 低6位

    // 用指令提示作为序号（与ASCII版兼容）
    feedback.m_ctlnumber = feedback.m_cmdHint;

    emit requpdateframe(feedback);
}


// ════════════════════════ 构造函数 / 析构函数 ════════════════════════

SerialPortTurntableHex::SerialPortTurntableHex(QObject *parent)
    : SerialPort(parent)
{
}

SerialPortTurntableHex::~SerialPortTurntableHex()
{
    if (m_turntableDataHex) {
        if (QThread::currentThread() != m_turntableDataHex->thread())
            m_turntableDataHex->moveToThread(QThread::currentThread());
        delete m_turntableDataHex;
        m_turntableDataHex = nullptr;
    }
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
    // 复位 (CMD_RESET = 0x1F) — 联合指令, 轴号可为1/2/3
    uint8_t params[14];
    std::memset(params, 0, 14);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(1, CMD_RESET);  // 联合指令, 轴号任意
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 高级指令：开机 ════════════════════════

void SerialPortTurntableHex::openTurntable()
{
    // 开机流程: 先释放再使能三个轴电机
    sendEnableCmd(1, false); sendEnableCmd(2, false); sendEnableCmd(3, false);
    sendEnableCmd(1, true);  sendEnableCmd(2, true);  sendEnableCmd(3, true);
}


// ════════════════════════ 高级指令：停机 ════════════════════════

void SerialPortTurntableHex::closeTurntable()
{
    sendStopCmd(1);
    sendStopCmd(2);
    sendStopCmd(3);
}


// ════════════════════════ 高级指令：回零 ════════════════════════

void SerialPortTurntableHex::zeroTurntable()
{
    sendZeroCmd(1);
    sendZeroCmd(2);
    sendZeroCmd(3);
}


// ════════════════════════ 高级指令：复位 ════════════════════════

void SerialPortTurntableHex::resetTurntable()
{
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

    // 四个角度值 (每个对应一个时间点: 0.25s, 0.50s, 0.75s, 1.00s)
    encode3BytesSigned(params + 2,  cmd.angle1, ANGLE_UNIT);
    encode3BytesSigned(params + 5,  cmd.angle2, ANGLE_UNIT);
    encode3BytesSigned(params + 8,  cmd.angle3, ANGLE_UNIT);
    encode3BytesSigned(params + 11, cmd.angle4, ANGLE_UNIT);

    uint8_t axis_cmd = ENCODE_AXIS_CMD(cmd.axis, CMD_TRACK_250);
    buildAndSend(axis_cmd, params);
}


// ════════════════════════ 5ms跟踪模式 (联合指令) ════════════════════════

void SerialPortTurntableHex::sendTrackCmd_5ms(const TrackingSendCmd2Hex &cmd)
{
    // CMD_TRACK_5MS (0x0C) — 联合指令
    // params[0-2]  = 内框位置 (3B, 有符号, 0.0001°)
    // params[3-5]  = 中框位置
    // params[6-8]  = 外框位置
    // params[9-13] = 0x00
    uint8_t params[14];
    std::memset(params, 0, 14);

    encode3BytesSigned(params + 0, cmd.angle1, ANGLE_UNIT);  // 内框
    encode3BytesSigned(params + 3, cmd.angle2, ANGLE_UNIT);  // 中框
    encode3BytesSigned(params + 6, cmd.angle3, ANGLE_UNIT);  // 外框

    // 联合指令, 轴号任意
    uint8_t axis_cmd = ENCODE_AXIS_CMD(1, CMD_TRACK_5MS);
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
    // // 时间同步 = 时间设置, 使用时当前秒值
    // // 获取当前半天内的秒数 (0-14399, 即4小时)
    // QTime now = QTime::currentTime();
    // int seconds = now.hour() * 3600 + now.minute() * 60 + now.second();
    // // 限制在半天范围内
    // seconds = seconds % 14400;
    sendTimeSetCmd(seconds);
}


// ════════════════════════ 程控模式 ════════════════════════

void SerialPortTurntableHex::ProgramModeChanged(int mode)
{
    m_isProgramMode = (mode == 1);
}


void SerialPortTurntableHex::sendProgramMode(programSend_frameHex frame)
{
    if (!m_isProgramMode) {
        qDebug() << "[HEX] 未进入程控模式，无法发送程控指令";
        return;
    }

    if (frame.runtime <= 0) {
        qDebug() << "[HEX] 程控模式: runtime无效 (" << frame.runtime << ")";
        return;
    }

    // 加速度设为1000 (即 10.00 °/s²)
    const float defaultAccel = 10.0f;  // 10.00 °/s² → 发送时为1000原始值

    if (frame.index == 0) {
        // ── 三轴联动 ──
        PositionModeCmd1Hex cmd1;
        cmd1.axis = 1;
        cmd1.acceleration = defaultAccel;
        cmd1.velocity = (frame.inner_endangle - frame.current_inner_angle) / frame.runtime;
        cmd1.anglePos = frame.inner_endangle;
        sendPositionCmd(cmd1);

        PositionModeCmd1Hex cmd2;
        cmd2.axis = 2;
        cmd2.acceleration = defaultAccel;
        cmd2.velocity = (frame.middle_endangle - frame.current_middle_angle) / frame.runtime;
        cmd2.anglePos = frame.middle_endangle;
        sendPositionCmd(cmd2);

        PositionModeCmd1Hex cmd3;
        cmd3.axis = 3;
        cmd3.acceleration = defaultAccel;
        cmd3.velocity = (frame.outter_endangle - frame.current_outter_angle) / frame.runtime;
        cmd3.anglePos = frame.outter_endangle;
        sendPositionCmd(cmd3);
    } else if (frame.index == 1) {
        // ── 仅内框 ──
        PositionModeCmd1Hex cmd1;
        cmd1.axis = 1;
        cmd1.acceleration = defaultAccel;
        cmd1.velocity = (frame.inner_endangle - frame.current_inner_angle) / frame.runtime;
        cmd1.anglePos = frame.inner_endangle;
        sendPositionCmd(cmd1);
    } else if (frame.index == 2) {
        // ── 仅中框 ──
        PositionModeCmd1Hex cmd2;
        cmd2.axis = 2;
        cmd2.acceleration = defaultAccel;
        cmd2.velocity = (frame.middle_endangle - frame.current_middle_angle) / frame.runtime;
        cmd2.anglePos = frame.middle_endangle;
        sendPositionCmd(cmd2);
    } else if (frame.index == 3) {
        // ── 仅外框 ──
        PositionModeCmd1Hex cmd3;
        cmd3.axis = 3;
        cmd3.acceleration = defaultAccel;
        cmd3.velocity = (frame.outter_endangle - frame.current_outter_angle) / frame.runtime;
        cmd3.anglePos = frame.outter_endangle;
        sendPositionCmd(cmd3);
    } else {
        qDebug() << "[HEX] 程控模式: 无效index" << frame.index;
    }
}


// ════════════════════════ 外引导模式 ════════════════════════

void SerialPortTurntableHex::sendTrackMode_1s(const sendExGuideData &frame1, const sendExGuideData &frame2)
{
    // 250ms跟踪模式: 上位机每隔1秒发送一组跟踪数据 (内/外框各一帧)
    // frame1 对应外框(轴3, 方位角), frame2 对应中框(轴2, 俯仰角)
    // 内框保持当前角度不变

    // ── 外框跟踪 (轴3) ──
    TrackingSendCmd1Hex cmdOuter;
    cmdOuter.axis = 3;  // 外框
    cmdOuter.trackTime = frame1.time;
    cmdOuter.angle1 = frame1.angle1 + m_current_outter_angle;
    cmdOuter.angle2 = frame1.angle2 + m_current_outter_angle;
    cmdOuter.angle3 = frame1.angle3 + m_current_outter_angle;
    cmdOuter.angle4 = frame1.angle4 + m_current_outter_angle;
    sendTrackCmd_1s(cmdOuter);

    // ── 中框跟踪 (轴2) ──
    TrackingSendCmd1Hex cmdMiddle;
    cmdMiddle.axis = 2;  // 中框
    cmdMiddle.trackTime = frame2.time;
    cmdMiddle.angle1 = frame2.angle1 + m_current_middle_angle;
    cmdMiddle.angle2 = frame2.angle2 + m_current_middle_angle;
    cmdMiddle.angle3 = frame2.angle3 + m_current_middle_angle;
    cmdMiddle.angle4 = frame2.angle4 + m_current_middle_angle;
    sendTrackCmd_1s(cmdMiddle);

    // ── 内框跟踪 (轴1, 保持当前位置) ──
    TrackingSendCmd1Hex cmdInner;
    cmdInner.axis = 1;  // 内框
    cmdInner.trackTime = frame1.time;
    cmdInner.angle1 = m_current_inner_angle;
    cmdInner.angle2 = m_current_inner_angle;
    cmdInner.angle3 = m_current_inner_angle;
    cmdInner.angle4 = m_current_inner_angle;
    sendTrackCmd_1s(cmdInner);
}


void SerialPortTurntableHex::sendTrackMode_5ms(double yawangle, double pitchangle)
{
    // 5ms跟踪模式 (联合指令, 一帧包含三轴数据)
    TrackingSendCmd2Hex m_cmd;
    m_cmd.angle1 = m_current_inner_angle;                    // 内框保持
    m_cmd.angle2 = m_current_middle_angle + pitchangle;      // 中框 + 俯仰角
    m_cmd.angle3 = m_current_outter_angle + yawangle;        // 外框 + 方位角

    sendTrackCmd_5ms(m_cmd);
}


// ════════════════════════ 遥控模式（手柄控制）═══════════════════════════

void SerialPortTurntableHex::sendHandleMode(float axisLeftX, float axisLeftY, float axisRightX,
                                             float buttonL2, float buttonR2,
                                             bool buttonA, bool buttonB, int Acount, int Bcount)
{
    const float defaultAccel = 10.0f;  // 10.00 °/s² → 发送时为1000原始值  // 10.00 °/s²

    if (buttonL2 != 0.0f || buttonR2 != 0.0f) {
        // ── 位置模式 (扳机按下 → 步进/步退) ──
        if (buttonR2 > 0.9f && buttonL2 > 0.9f) {
            // 两扳机同时按下: 内框步进/步退
            PositionModeCmd1Hex cmd;
            cmd.axis = 1;
            cmd.acceleration = defaultAccel;
            if (buttonA && Acount >= 1) {
                cmd.velocity = MAX_SPEED_HEX ;
                cmd.anglePos = m_current_inner_angle + 3.0f;
                sendPositionCmd(cmd);
            } else if (buttonB && Bcount >= 1) {
                cmd.velocity = -MAX_SPEED_HEX ;
                cmd.anglePos = m_current_inner_angle - 3.0f;
                sendPositionCmd(cmd);
            }
        } else if (buttonL2 != 0.0f) {
            // 左扳机: 外框
            PositionModeCmd1Hex cmd;
            cmd.axis = 3;
            cmd.acceleration = defaultAccel;
            if (buttonA && Acount >= 1) {
                cmd.velocity = MAX_SPEED_HEX ;
                cmd.anglePos = m_current_outter_angle + 3.0f;
                sendPositionCmd(cmd);
            } else if (buttonB && Bcount >= 1) {
                cmd.velocity = -MAX_SPEED_HEX ;
                cmd.anglePos = m_current_outter_angle - 3.0f;
                sendPositionCmd(cmd);
            }
        } else if (buttonR2 != 0.0f) {
            // 右扳机: 中框
            PositionModeCmd1Hex cmd;
            cmd.axis = 2;
            cmd.acceleration = defaultAccel;
            if (buttonA && Acount >= 1) {
                cmd.velocity = MAX_SPEED_HEX ;
                cmd.anglePos = m_current_middle_angle + 3.0f;
                sendPositionCmd(cmd);
            } else if (buttonB && Bcount >= 1) {
                cmd.velocity = -MAX_SPEED_HEX ;
                cmd.anglePos = m_current_middle_angle - 3.0f;
                sendPositionCmd(cmd);
            }
        }
    } else {
        // ── 速度模式 (无扳机 → 摇杆直接控制速度) ──
        // axisLeftX → 外框, axisLeftY → 中框, axisRightX → 内框
        SpeedModeCmd1Hex innercmd;
        innercmd.axis = 1;
        innercmd.acceleration = defaultAccel;
        innercmd.velocity = axisRightX * MAX_SPEED_HEX;

        SpeedModeCmd1Hex middlecmd;
        middlecmd.axis = 2;
        middlecmd.acceleration = defaultAccel;
        middlecmd.velocity = axisLeftY * MAX_SPEED_HEX;

        SpeedModeCmd1Hex outtercmd;
        outtercmd.axis = 3;
        outtercmd.acceleration = defaultAccel;
        outtercmd.velocity = axisLeftX * MAX_SPEED_HEX;

        sendVecCmd(innercmd);
        sendVecCmd(middlecmd);
        sendVecCmd(outtercmd);
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


// ════════════════════════ TurntableDataHex ════════════════════════

TurntableDataHex::TurntableDataHex(QObject *parent)
    : QObject(parent)
{
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
    if (m_ctlnumber != recvdata.m_ctlnumber) {
        m_ctlnumber = recvdata.m_ctlnumber;
        emit ctlnumberChanged();
    }
    if (m_inner_statusnumber != recvdata.m_inner_statusnumber) {
        m_inner_statusnumber = recvdata.m_inner_statusnumber;
        emit inner_statusnumberChanged();
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
    if (m_middle_statusnumber != recvdata.m_middle_statusnumber) {
        m_middle_statusnumber = recvdata.m_middle_statusnumber;
        emit middle_statusnumberChanged();
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
    if (m_outter_statusnumber != recvdata.m_outter_statusnumber) {
        m_outter_statusnumber = recvdata.m_outter_statusnumber;
        emit outter_statusnumberChanged();
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
