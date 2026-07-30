#ifndef SERIALPORT_TURNTABLE_HEX_H
#define SERIALPORT_TURNTABLE_HEX_H

/*
brief: 转台串口类（HEX十六进制协议版本）
       协议版本: 三轴跟踪转台通讯协议 V2.0
       硬件: RS422, 230400bps, 1停止位, 1起始位, 无校验

帧格式:
  发送帧(控制指令): 19字节 = 0x55 0xAA + axis_cmd(1B) + params(14B) + checksum(2B)
  接收帧(状态反馈): 29字节 = 0x55 0xAA + time(3B) + ... + checksum(2B)

轴号: 1=内框, 2=中框, 3=外框 (占用byte2的高2位)
指令代号: 占用byte2的低6位
多字节数据: 大端序(高位在前)
校验和: 不含帧头的累加和 (发送:bytes2-16累加, 接收:bytes2-26累加)
*/

#include "serialport.h"

// ═════════════════════ 指令代号定义（byte2的低6位）═════════════════════
#define CMD_ENABLE      0x01    // 使能/释放电机
#define CMD_STOP        0x02    // 停车
#define CMD_ZERO        0x03    // 回零
#define CMD_POSITION    0x04    // 位置模式
#define CMD_SPEED       0x05    // 速率模式
#define CMD_SWING       0x06    // 摇摆模式
#define CMD_TRACK_250   0x0A    // 250ms跟踪模式
#define CMD_TRACK_5MS   0x0C    // 5ms跟踪模式
#define CMD_TIME_SET    0x10    // 秒时间设置
#define CMD_CORRECTION  0x11    // 跟踪修正
#define CMD_RESET       0x1F    // 复位

// ═════════════════════ 编码辅助宏 ═════════════════════
// 将轴号(1/2/3)和指令代号编码到byte2
// axis: 1=内框(0b01), 2=中框(0b10), 3=外框(0b11)
// cmd:  指令代号(低6位)
#define ENCODE_AXIS_CMD(axis, cmd)  ((((uint8_t)(axis) & 0x03) << 6) | ((uint8_t)(cmd) & 0x3F))

// 角度单位: 0.0001度
#define ANGLE_UNIT      10000.0f
// 速度单位: 0.001度/秒
#define SPEED_UNIT      1000.0f
// 加速度单位: 0.01度/秒²
#define ACCEL_UNIT      100.0f
// 频率单位: 0.001Hz
#define FREQ_UNIT       1000.0f

// ════════════════════════ 应用层结构体 ════════════════════════

// 转台信息反馈结构体（应用层）
typedef struct StatusFeedbackHex {
    int     m_time;                 // 毫秒时间 (0-14399999)
    int     m_ctlnumber;            // 序号 (从指令提示中提取)
    int     m_inner_statusnumber;   // 内框状态 (低4位: 状态, 高4位: 报警)
    float   m_inner_angle;          // 内框角度 (度)
    float   m_inner_ctlDeviation;   // 内框控制偏差 (度)
    int     m_middle_statusnumber;  // 中框状态
    float   m_middle_angle;         // 中框角度
    float   m_middle_ctlDeviation;  // 中框控制偏差
    int     m_outter_statusnumber;  // 外框状态
    float   m_outter_angle;         // 外框角度
    float   m_outter_ctlDeviation;  // 外框控制偏差
    int     m_hasSecPulse;          // 是否接收到秒脉冲 (0/1)
    int     m_cmdHint;              // 指令提示 (接收到有效指令时返回)
} StatusFeedbackHex;

// 程控模式帧
typedef struct programSend_frameHex {
    int     runtime;
    int     index;
    float   current_inner_angle;
    float   inner_endangle;
    float   current_middle_angle;
    float   middle_endangle;
    float   current_outter_angle;
    float   outter_endangle;
} programSend_frameHex;

// 位置模式命令（应用层）
typedef struct PositionModeCmd1Hex {
    int     axis;           // 轴号 1/2/3
    float   acceleration;   // 加速度 (°/s², 发送时×100转为0.01°/s²单位)
    float   velocity;       // 速度 (°/s)
    float   anglePos;       // 目标角度 (°)
} PositionModeCmd1Hex;

// 速度模式命令（应用层）
typedef struct SpeedModeCmd1Hex {
    int     axis;
    float   acceleration;   // 加速度 (°/s²)
    float   velocity;       // 速度 (°/s)
} SpeedModeCmd1Hex;

// 跟踪模式1命令（应用层, 250ms周期 = 1秒4个点）
typedef struct {
    int     axis;           // 轴号
    int     trackTime;      // 秒时间 (0-14399)
    float   angle1;         // 第0.25秒角度
    float   angle2;         // 第0.50秒角度
    float   angle3;         // 第0.75秒角度
    float   angle4;         // 第1.00秒角度
} TrackingSendCmd1Hex;

// 跟踪模式2命令（应用层, 5ms周期 = 200Hz）
typedef struct {
    int     trackTime;      // 时间(仅用于记录, 协议不使用)
    float   angle1;         // 内框角度 (度)
    float   angle2;         // 中框角度 (度)
    float   angle3;         // 外框角度 (度)
} TrackingSendCmd2Hex;


// ════════════════════════ HEX协议打包结构体 (packed) ════════════════════════
// #pragma pack(push,1)

// // 控制指令发送帧 (19字节)
// typedef struct ControlCmdHex {
//     uint8_t  header1;           // 0x55
//     uint8_t  header2;           // 0xAA
//     uint8_t  axis_cmd;          // 高2位=轴号, 低6位=指令代号
//     uint8_t  params[14];        // 参数1-14
//     uint16_t checksum;          // bytes2-16累加和 (大端)
// } ControlCmdHex;

// // 状态反馈接收帧 (29字节)
// typedef struct StatusFeedbackRawHex {
//     uint8_t  header1;               // 0x55
//     uint8_t  header2;               // 0xAA
//     uint8_t  time[3];               // 毫秒时间 高-中-低
//     uint8_t  innerAlarmStatus;      // 内框: 高4位报警 + 低4位状态
//     uint8_t  innerAngle[3];         // 内框角度 高-中-低 (有符号, 单位0.0001°)
//     uint8_t  innerDeviation[3];     // 内框偏差
//     uint8_t  middleAlarmStatus;     // 中框
//     uint8_t  middleAngle[3];
//     uint8_t  middleDeviation[3];
//     uint8_t  outerAlarmStatus;      // 外框
//     uint8_t  outerAngle[3];
//     uint8_t  outerDeviation[3];
//     uint8_t  pulseAndHint;          // bit7:秒脉冲, bit5-0:指令提示
//     uint16_t checksum;              // bytes2-26累加和
// } StatusFeedbackRawHex;

// #pragma pack(pop)


// ════════════════════════ 数据类（主线程, QML可用）═══════════════════════════

class TurntableDataHex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int time MEMBER m_time NOTIFY timeChanged)
    Q_PROPERTY(int ctlnumber MEMBER m_ctlnumber NOTIFY ctlnumberChanged)
    Q_PROPERTY(int inner_statusnumber MEMBER m_inner_statusnumber NOTIFY inner_statusnumberChanged)
    Q_PROPERTY(float inner_angle MEMBER m_inner_angle NOTIFY inner_angleChanged)
    Q_PROPERTY(float inner_ctlDeviation MEMBER m_inner_ctlDeviation NOTIFY inner_ctlDeviationChanged)
    Q_PROPERTY(int middle_statusnumber MEMBER m_middle_statusnumber NOTIFY middle_statusnumberChanged)
    Q_PROPERTY(float middle_angle MEMBER m_middle_angle NOTIFY middle_angleChanged)
    Q_PROPERTY(float middle_ctlDeviation MEMBER m_middle_ctlDeviation NOTIFY middle_ctlDeviationChanged)
    Q_PROPERTY(int outter_statusnumber MEMBER m_outter_statusnumber NOTIFY outter_statusnumberChanged)
    Q_PROPERTY(float outter_angle MEMBER m_outter_angle NOTIFY outter_angleChanged)
    Q_PROPERTY(float outter_ctlDeviation MEMBER m_outter_ctlDeviation NOTIFY outter_ctlDeviationChanged)
    Q_PROPERTY(int hasSecPulse MEMBER m_hasSecPulse NOTIFY hasSecPulseChanged)
    Q_PROPERTY(int cmdHint MEMBER m_cmdHint NOTIFY cmdHintChanged)

    Q_PROPERTY(bool portOpen READ portOpen NOTIFY portOpenChanged)
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

public:
    explicit TurntableDataHex(QObject *parent = nullptr);
    ~TurntableDataHex() override {};

    bool portOpen() const { return m_portOpen; }
    QStringList availablePorts() const { return m_availablePorts; }
    QString errorString() const { return m_errorString; }

    Q_INVOKABLE void openPort(const QString &portName, int baudRate);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE void scanPorts();

signals:
    void timeChanged();
    void ctlnumberChanged();
    void inner_statusnumberChanged();
    void inner_angleChanged();
    void inner_ctlDeviationChanged();
    void middle_statusnumberChanged();
    void middle_angleChanged();
    void middle_ctlDeviationChanged();
    void outter_statusnumberChanged();
    void outter_angleChanged();
    void outter_ctlDeviationChanged();
    void hasSecPulseChanged();
    void cmdHintChanged();

    void myinner_angleChanged(float inner_angle);
    void mymiddle_angleChanged(float middle_angle);
    void myoutter_angleChanged(float outter_angle);

    void portOpenChanged();
    void availablePortsChanged();
    void errorStringChanged();

    void requestOpenPort(const QString &portName, int baudRate);
    void requestClosePort();
    void requestScanPorts();

public slots:
    void updateframe(StatusFeedbackHex recvdata);
    void setPortOpen(bool open);
    void setPortList(const QStringList &ports);
    void setError(const QString &msg);

private:
    int m_time = 0;
    int m_ctlnumber = 0;
    int m_inner_statusnumber = 0;
    float m_inner_angle = 0;
    float m_inner_ctlDeviation = 0;
    int m_middle_statusnumber = 0;
    float m_middle_angle = 0;
    float m_middle_ctlDeviation = 0;
    int m_outter_statusnumber = 0;
    float m_outter_angle = 0;
    float m_outter_ctlDeviation = 0;
    int m_hasSecPulse = 0;
    int m_cmdHint = 0;

    bool m_portOpen = false;
    QStringList m_availablePorts;
    QString m_errorString;
};


// ════════════════════════ 发送数据类（主线程）═══════════════════════════

class TurntableSendDataHex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int runtime MEMBER m_runtime NOTIFY runtimeChanged)
    Q_PROPERTY(float inner_startangle MEMBER m_inner_startangle NOTIFY inner_startangleChanged)
    Q_PROPERTY(float inner_endangle MEMBER m_inner_endangle NOTIFY inner_endangleChanged)
    Q_PROPERTY(float middle_startangle MEMBER m_middle_startangle NOTIFY middle_startangleChanged)
    Q_PROPERTY(float middle_endangle MEMBER m_middle_endangle NOTIFY middle_endangleChanged)
    Q_PROPERTY(float outter_startangle MEMBER m_outter_startangle NOTIFY outter_startangleChanged)
    Q_PROPERTY(float outter_endangle MEMBER m_outter_endangle NOTIFY outter_endangleChanged)

public:
    explicit TurntableSendDataHex(QObject *parent = nullptr);
    ~TurntableSendDataHex() override {};

    Q_INVOKABLE void buildFrame(int m_index);

public slots:
    void recvinner_angle(float angle)  { m_current_inner_angle = angle; }
    void recvmiddle_angle(float angle) { m_current_middle_angle = angle; }
    void recvoutter_angle(float angle) { m_current_outter_angle = angle; }

signals:
    void requestOpenTurntable();
    void requestCloseTurntable();
    void requestResetTurntable();
    void requestZeroTurntable();
    void requestSendProgramMode(programSend_frameHex frame);
    void runtimeChanged();
    void inner_startangleChanged();
    void inner_endangleChanged();
    void middle_startangleChanged();
    void middle_endangleChanged();
    void outter_startangleChanged();
    void outter_endangleChanged();
    void indexChanged();

    void reqopenTurntable();
    void reqcloseTurntable();
    void reqzeroTurntable();
    void reqresetTurntable();

private:
    int m_runtime = 1;
    float m_inner_startangle = 0.0f;
    float m_current_inner_angle = 0.0f;
    float m_inner_endangle = 0.0f;
    float m_middle_startangle = 0.0f;
    float m_current_middle_angle = 0.0f;
    float m_middle_endangle = 0.0f;
    float m_outter_startangle = 0.0f;
    float m_current_outter_angle = 0.0f;
    float m_outter_endangle = 0.0f;
};


// ════════════════════════ 串口工作类（工作线程）═══════════════════════════

class SerialPortTurntableHex : public SerialPort
{
    Q_OBJECT

public:
    explicit SerialPortTurntableHex(QObject *parent = nullptr);
    ~SerialPortTurntableHex() override;

    TurntableDataHex * m_turntableDataHex;
    TurntableSendDataHex * m_turntableSendDataHex;

signals:
    void requpdateframe(StatusFeedbackHex recvdata);
    void portOpened(bool success);
    void portClosed();
    void portError(const QString &msg);
    void portsChanged(const QStringList &ports);

public slots:
    // ── 基础指令 ──
    void openTurntable();
    void closeTurntable();
    void resetTurntable();
    void zeroTurntable();

    // ── 程控模式 ──
    void sendProgramMode(programSend_frameHex frame);

    // ── 遥控模式（手柄）──
    void sendHandleMode(float axisLeftX, float axisLeftY, float axisRightX,
                        float buttonL2, float buttonR2,
                        bool buttonA, bool buttonB, int Acount, int Bcount);

    // ── 外引导模式 ──
    void sendTrackMode_1s(const sendExGuideData &frame1, const sendExGuideData &frame2);
    void sendTrackMode_5ms(int time, int yawangle, int pitchangle);

    // ── 时间同步 ──
    void sendTimesync();

    // ── 模式切换 ──
    void ProgramModeChanged(int mode);

    // ── 初始化 ──
    void dowork() {
        SerialPort::dowork();
        onScanPorts();
    }

    // ── 角度数据接收 ──
    void recvinner_angle(float angle)  { m_current_inner_angle = angle; }
    void recvmiddle_angle(float angle) { m_current_middle_angle = angle; }
    void recvoutter_angle(float angle) { m_current_outter_angle = angle; }

    // ── 串口管理 ──
    void onOpenPort(const QString &portName, int baudRate);
    void onClosePort();
    void onScanPorts();

protected:
    void parseData(const QByteArray &rawData) override;

    // ── HEX二进制发送函数 ──
    void sendEnableCmd(int axis, bool enable);          // 使能/释放电机
    void sendStopCmd(int axis);                         // 停车
    void sendZeroCmd(int axis);                         // 回零
    void sendResetCmd();                                // 复位 (联合指令)
    void sendPositionCmd(const PositionModeCmd1Hex &cmd);   // 位置模式
    void sendVecCmd(const SpeedModeCmd1Hex &cmd);           // 速度模式
    void sendTrackCmd_1s(const TrackingSendCmd1Hex &cmd);     // 250ms跟踪(1s周期)
    void sendTrackCmd_5ms(const TrackingSendCmd2Hex &cmd);  // 5ms跟踪
    void sendTimeSetCmd(int seconds);                       // 时间设置

    // ── 协议辅助函数 ──
    void buildAndSend(uint8_t axis_cmd, const uint8_t params[14]);
    static uint16_t calcChecksum(const uint8_t* data, size_t len);

    // ── 数值编解码 ──
    static void encode3BytesSigned(uint8_t dest[3], float value, float unit);
    static float decode3BytesSigned(const uint8_t src[3], float unit);
    static void encode2BytesUint(uint8_t dest[2], uint16_t value);
    static uint16_t decode2BytesUint(const uint8_t src[2]);
    static void encode3BytesUint(uint8_t dest[3], uint32_t value);
    static uint32_t decode3BytesUint(const uint8_t src[3]);

    // ── 角度值钳位与编码 ──
    static int32_t angleToRaw(float degrees);
    static float rawToAngle(int32_t raw);
    static int32_t speedToRaw(float speed);
    static float rawToSpeed(int32_t raw);

private:
    bool m_isProgramMode;

    float m_current_inner_angle;
    float m_current_middle_angle;
    float m_current_outter_angle;
};

#endif // SERIALPORT_TURNTABLE_HEX_H
