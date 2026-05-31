#ifndef SERIALPORT_TURNTABLE_H
#define SERIALPORT_TURNTABLE_H

#include "serialport.h"

class SerialPortTurntable : public SerialPort
{
    Q_OBJECT

public:
    explicit SerialPortTurntable(QObject *parent = nullptr);
    ~SerialPortTurntable() override;

protected:
    void parseData(const QByteArray &rawData) override;
};

#pragma pack(push,1)

// ------------------------------ 位置模式 ------------------------------
typedef struct {
    uint16_t axis;              // 轴号 (2字节)
    uint32_t acceleration;      // 加速度 (4字节)
    uint8_t  velocity[10];      // 速度 (10字节)
    uint8_t  anglePos[9];       // 角度位置 (9字节)
} PositionModeCmd;

// ------------------------------ 速度模式 ------------------------------
typedef struct {
    uint16_t axis;              // 轴号 (2字节)
    uint32_t acceleration;      // 加速度 (4字节)
    uint8_t  velocity[10];      // 速度 (10字节)
} SpeedModeCmd;

// ------------------------------ 摇摆模式 ------------------------------
typedef struct {
    uint16_t axis;              // 轴号 (2字节)
    uint8_t  amplitude[8];      // 摇摆幅度 (8字节)
    uint8_t  frequency[10];     // 摇摆频率 (10字节)
} SwingModeCmd;

// ------------------------------ 跟踪模式1 发送 ------------------------------
typedef struct {
    uint16_t axis;          // 轴号 (2字节)
    uint32_t trackTime;     // 跟踪时间 (4字节)
    uint8_t  angle11[9];    // 角度11 (9字节)
    uint8_t  angle12[9];    // 角度12 (9字节)
    uint8_t  angle13[9];    // 角度13 (9字节)
    uint8_t  angle14[9];    // 角度14 (9字节)
    uint8_t  angle21[9];    // 角度21 (9字节)
    uint8_t  angle22[9];    // 角度22 (9字节)
    uint8_t  angle23[9];    // 角度23 (9字节)
    uint8_t  angle24[9];    // 角度24 (9字节)
    uint8_t  angle31[9];    // 角度31 (9字节)
    uint8_t  angle32[9];    // 角度32 (9字节)
    uint8_t  angle33[9];    // 角度33 (9字节)
    uint8_t  angle34[9];    // 角度34 (9字节)
} Tracking1SendCmd;

// ------------------------------ 跟踪模式1 接收 ------------------------------
typedef struct {
    uint16_t axis;      // 轴号 (2字节)
    uint32_t time;      // 时间 (4字节)
} Tracking1ReceiveResp;

// ------------------------------ 跟踪模式1 修正指令发送 ------------------------------
typedef struct {
    uint8_t  axis[3];         // 轴号 (3字节)
    uint8_t  correctAngle[9]; // 修正角度 (9字节)
} Tracking1CorrectionCmd;

// ------------------------------ 跟踪模式2 发送 ------------------------------
typedef struct {
    uint16_t axis;          // 轴号 (2字节)
    uint8_t  trackTime[6];  // 跟踪时间 (6字节)
    uint8_t  angle1[9];     // 跟踪角度1 (9字节)
    uint8_t  angle2[9];     // 跟踪角度2 (9字节)
    uint8_t  angle3[9];     // 跟踪角度3 (9字节)
} Tracking2SendCmd;

// ------------------------------ 速度环模式发送 ------------------------------
typedef struct {
    uint16_t axis;              // 轴号 (2字节)
    uint8_t  innerSpeed[10];    // 内框速度 (10字节)
    uint8_t  middleSpeed[10];   // 中框速度 (10字节)
    uint8_t  outerSpeed[10];    // 外框速度 (10字节)
} SpeedLoopCmd;

// ------------------------------ 时间同步指令 ------------------------------
typedef struct {
    uint8_t axis[3];        // 轴号 (3字节)
} TimeSyncCmd;

// ------------------------------ 秒脉冲查询发送 ------------------------------
typedef struct {
    uint8_t axis[3];        // 轴号 (3字节)
} PulseQuerySend;

// ------------------------------ 秒脉冲查询接收 ------------------------------
typedef struct {
    uint16_t axis;          // 轴号 (2字节)
} PulseQueryReceive;

// ------------------------------ 复位发送 ------------------------------
typedef struct {
    uint8_t reset[3];       // 复位 (3字节)
} ResetSend;

// ------------------------------ 状态反馈接收 ------------------------------
typedef struct {
    uint32_t time;          // 时间 (4字节)
    uint16_t seq;           // 序号 (2字节)
    uint16_t innerState;    // 内框状态 (2字节)
    uint8_t  innerAngle[9]; // 内框角度 (9字节)
    uint8_t  innerErr[9];   // 内框偏差 (9字节)
    uint16_t middleState;   // 中框状态 (2字节)
    uint8_t  middleAngle[9];// 中框角度 (9字节)
    uint8_t  middleErr[9];  // 中框偏差 (9字节)
    uint16_t outerState;    // 外框状态 (2字节)
    uint8_t  outerAngle[9]; // 外框角度 (9字节)
    uint8_t  outerErr[9];   // 外框偏差 (9字节)
} StatusFeedback;

#pragma pack(pop)
#endif // SERIALPORT_TURNTABLE_H
