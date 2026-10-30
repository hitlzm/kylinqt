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

转台最大速度为12度每秒
转台加速度最大为10
*/

#include "serialport.h"
#include "../utils/tracking_target_smoother.h"
#include "../kalman/AlphaBetaTracker.h"
#include <QElapsedTimer>

// ═════════════════════ 指令代号定义（byte2的低6位）═════════════════════
#define CMD_ENABLE      0x01    // 使能/释放电机
#define CMD_STOP        0x02    // 停车
#define CMD_ZERO        0x03    // 回零
#define CMD_POSITION    0x04    // 位置模式
#define CMD_SPEED       0x05    // 速率模式
#define CMD_SWING       0x06    // 摇摆模式
#define CMD_TRACK_3S    0x09    // 3秒跟踪模式
#define CMD_TRACK_250   0x0A    // 250ms跟踪模式
#define CMD_TRACK_20MS  0x0B    // 20ms跟踪模式
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
    int     m_inner_alarm;          // 内框报警标志 (原始状态字节高4位)
    int     m_inner_status;         // 内框状态标志 (原始状态字节低4位)
    float   m_inner_angle;          // 内框角度 (度)
    float   m_inner_ctlDeviation;   // 内框控制偏差 (度)
    int     m_middle_alarm;         // 中框报警标志 (原始状态字节高4位)
    int     m_middle_status;        // 中框状态标志 (原始状态字节低4位)
    float   m_middle_angle;         // 中框角度
    float   m_middle_ctlDeviation;  // 中框控制偏差
    int     m_outter_alarm;         // 外框报警标志 (原始状态字节高4位)
    int     m_outter_status;        // 外框状态标志 (原始状态字节低4位)
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

// 程控模式单轴指令。mode: 0=位置模式, 1=速率模式。
// 位置模式使用 position/velocity/acceleration；速率模式使用 velocity/acceleration。
typedef struct ProgramAxisCommandHex {
    int     axis;           // 轴号 1=内框, 2=中框, 3=外框
    int     mode;           // 0=位置模式, 1=速率模式
    float   position;       // 目标位置 (度)
    float   velocity;       // 位置模式限速，或速率模式目标速度 (度/秒)
    float   acceleration;   // 加速度 (度/秒²)
} ProgramAxisCommandHex;

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

// 3 秒/250ms 跟踪命令（应用层；每组4个角度点，由指令代号区分周期）
typedef struct {
    int     axis;           // 轴号
    int     trackTime;      // 秒时间 (0-14399)
    float   angle1;         // 第0.25秒角度
    float   angle2;         // 第0.50秒角度
    float   angle3;         // 第0.75秒角度
    float   angle4;         // 第1.00秒角度
} TrackingSendCmd1Hex;

// 20ms跟踪命令（联合指令，一帧同时携带三轴角度）
typedef struct {
    int     timeMs;         // 目标毫秒时间 (0-14399999，必须为偶数)
    float   angle1;         // 内框角度 (度)
    float   angle2;         // 中框角度 (度)
    float   angle3;         // 外框角度 (度)
} TrackingSendCmd20msHex;

// 跟踪修正命令（应用层）
// 联合指令：byte2 高2位填 1/2/3 均可；低6位固定为 CMD_CORRECTION (0x11)
// byte3-11: 内框、中框、外框修正角 (各3字节, 有符号, 0.0001°, 大端)
typedef struct {
    int     target;         // 1=内框, 2=中框，3=外框
    float   innerDeviation; // 内框修正角 (度)
    float   middleDeviation;// 中框修正角 (度)
    float   outerDeviation; // 外框修正角 (度)
} TrackingCorrectionCmdHex;

Q_DECLARE_METATYPE(TrackingCorrectionCmdHex)


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
    Q_PROPERTY(int inner_alarm MEMBER m_inner_alarm NOTIFY inner_alarmChanged)
    Q_PROPERTY(int inner_status MEMBER m_inner_status NOTIFY inner_statusChanged)
    Q_PROPERTY(float inner_angle MEMBER m_inner_angle NOTIFY inner_angleChanged)
    Q_PROPERTY(float inner_ctlDeviation MEMBER m_inner_ctlDeviation NOTIFY inner_ctlDeviationChanged)
    Q_PROPERTY(int middle_alarm MEMBER m_middle_alarm NOTIFY middle_alarmChanged)
    Q_PROPERTY(int middle_status MEMBER m_middle_status NOTIFY middle_statusChanged)
    Q_PROPERTY(float middle_angle MEMBER m_middle_angle NOTIFY middle_angleChanged)
    Q_PROPERTY(float middle_ctlDeviation MEMBER m_middle_ctlDeviation NOTIFY middle_ctlDeviationChanged)
    Q_PROPERTY(int outter_alarm MEMBER m_outter_alarm NOTIFY outter_alarmChanged)
    Q_PROPERTY(int outter_status MEMBER m_outter_status NOTIFY outter_statusChanged)
    Q_PROPERTY(float outter_angle MEMBER m_outter_angle NOTIFY outter_angleChanged)
    Q_PROPERTY(float outter_ctlDeviation MEMBER m_outter_ctlDeviation NOTIFY outter_ctlDeviationChanged)
    Q_PROPERTY(int hasSecPulse MEMBER m_hasSecPulse NOTIFY hasSecPulseChanged)
    Q_PROPERTY(int cmdHint MEMBER m_cmdHint NOTIFY cmdHintChanged)

    Q_PROPERTY(bool portOpen READ portOpen NOTIFY portOpenChanged)
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(float remoteStepAngle READ remoteStepAngle WRITE setRemoteStepAngle NOTIFY remoteStepAngleChanged)
    Q_PROPERTY(float remoteSpeedMultiplier READ remoteSpeedMultiplier WRITE setRemoteSpeedMultiplier NOTIFY remoteSpeedMultiplierChanged)

public:
    explicit TurntableDataHex(QObject *parent = nullptr);
    ~TurntableDataHex() override {};

    bool portOpen() const { return m_portOpen; }
    QStringList availablePorts() const { return m_availablePorts; }
    QString errorString() const { return m_errorString; }
    float remoteStepAngle() const { return m_remoteStepAngle; }
    float remoteSpeedMultiplier() const { return m_remoteSpeedMultiplier; }

    Q_INVOKABLE void openPort(const QString &portName, int baudRate);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE void scanPorts();
    void setRemoteStepAngle(float stepAngle);
    void setRemoteSpeedMultiplier(float speedMultiplier);

signals:
    void timeChanged();
    void inner_alarmChanged();
    void inner_statusChanged();
    void inner_angleChanged();
    void inner_ctlDeviationChanged();
    void middle_alarmChanged();
    void middle_statusChanged();
    void middle_angleChanged();
    void middle_ctlDeviationChanged();
    void outter_alarmChanged();
    void outter_statusChanged();
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
    void remoteStepAngleChanged();
    void remoteSpeedMultiplierChanged();

    void requestOpenPort(const QString &portName, int baudRate);
    void requestClosePort();
    void requestScanPorts();

    // 通知步进角度与速度倍率变化
    void stepAngleChanged(float step_angle);
    void speedMultiplierChanged(float speed_multiplier);

public slots:
    void updateframe(StatusFeedbackHex recvdata);
    void setPortOpen(bool open);
    void setPortList(const QStringList &ports);
    void setError(const QString &msg);

private:
    int m_time = 0;
    int m_inner_alarm = 0;
    int m_inner_status = 0;
    float m_inner_angle = 0;
    float m_inner_ctlDeviation = 0;
    int m_middle_alarm = 0;
    int m_middle_status = 0;
    float m_middle_angle = 0;
    float m_middle_ctlDeviation = 0;
    int m_outter_alarm = 0;
    int m_outter_status = 0;
    float m_outter_angle = 0;
    float m_outter_ctlDeviation = 0;
    int m_hasSecPulse = 0;
    int m_cmdHint = 0;

    bool m_portOpen = false;
    QStringList m_availablePorts;
    QString m_errorString;
    // 遥控页面会随模式切换被销毁重建，设置值保存在主线程对象中以便恢复显示
    float m_remoteStepAngle = 1.0f;
    float m_remoteSpeedMultiplier = 1.0f;
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
    Q_INVOKABLE void sendProgramCommand(int axis, int mode, float position,
                                        float velocity, float acceleration);
    Q_INVOKABLE void setAxisEnabled(int axis, bool enabled);
    Q_INVOKABLE void stopAxis(int axis);
    Q_INVOKABLE void zeroAxis(int axis);
    //Q_INVOKABLE void sinMove(int m_index, float amplitude, float frequency);
    // 跟踪修正入口：以当前三个轴的控制偏差直接生成一帧修正指令。
    Q_INVOKABLE void requestTrackingCorrection(float innerDeviation,
                                               float middleDeviation,
                                               float outerDeviation,
                                               int target = 1);
    // 将内框偏置角校正为 -当前内框角度，使当前画面零位对应内框零度。
    Q_INVOKABLE void requestInnerAngleZeroCorrection();

public slots:
    void recvinner_angle(float angle)  { m_current_inner_angle = angle; }
    void recvmiddle_angle(float angle) { m_current_middle_angle = angle; }
    void recvoutter_angle(float angle) { m_current_outter_angle = angle; }

signals:
    void requestSendProgramMode(programSend_frameHex frame);
    void requestProgramAxisCommand(ProgramAxisCommandHex command);
    void runtimeChanged();
    void inner_startangleChanged();
    void inner_endangleChanged();
    void middle_startangleChanged();
    void middle_endangleChanged();
    void outter_startangleChanged();
    void outter_endangleChanged();
    void indexChanged();

    void reqopenTurntable();
    void reqreleaseTurntable();
    void reqcloseTurntable();
    void reqzeroTurntable();
    void reqresetTurntable();
    void reqAxisEnable(int axis, bool enabled);
    void reqAxisStop(int axis);
    void reqAxisZero(int axis);

    void sinMove(int m_index, float amplitude, float frequency);//桥接信号，驱动三轴进行正弦运动
    void reqTrackingCorrection(TrackingCorrectionCmdHex cmd);
    void reqInnerAngleZeroCorrection();

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

    // 这两个指针由 main() 注入（对象归 main 的 QML 侧所有，本类只引用不负责释放），
    // 外部只需要“注入”，不需要直接读写成员，因此移到 private，仅通过 setter 赋值。
    void setTurntableDataHex(TurntableDataHex *data) { m_turntableDataHex = data; }
    void setTurntableSendDataHex(TurntableSendDataHex *data) { m_turntableSendDataHex = data; }

signals:
    void requpdateframe(StatusFeedbackHex recvdata);
    void portOpened(bool success);
    void portClosed();
    void portError(const QString &msg);
    void portsChanged(const QStringList &ports);

public slots:
    // ── 基础指令 ──
    void openTurntable();
    void releaseTurntable();
    void closeTurntable();
    void resetTurntable();
    void zeroTurntable();

    // ── 程控模式 ──
    void sendProgramMode(programSend_frameHex frame);
    void sendProgramAxisCommand(ProgramAxisCommandHex command);
    void enableAxis(int axis, bool enabled);
    void stopAxis(int axis);
    void zeroAxis(int axis);

    // ── 遥控模式（手柄）──
    void sendHandleMode(float axisLeftX, float axisLeftY, float axisRightX,
                        float buttonL2, float buttonR2,
                        bool buttonA, bool buttonB, int Acount, int Bcount);

    // ── 外引导模式 ──
    // source: 3=图像, 4=激光, 5=CCD。相对角由转台线程结合当前反馈组成
    // 绝对目标角后，再交给绝对目标 Alpha-Beta 跟踪器。
    void recvExGuideMeasurement(exGuideMeasurement sample);
    void sendAbsoluteTrackMode_1s(int sourceId, double firstPredictionDelaySec);
    // 保留旧入口供历史代码调用；新的 1 秒链路不再使用外部相对角预测包。
    void sendTrackMode_1s(sendExGuideData frame1, sendExGuideData frame2);
    void sendTrackMode_20ms(double yawangle, double pitchangle);
    void ExGuideSourceChanged(int source);
    void ExGuidePeriodChanged(int setting);

    // ── 跟踪修正 ──
    void sendTrackingCorrection(TrackingCorrectionCmdHex cmd);
    void zeroInnerAngleOffset();

    // ── 摇摆模式 ──
    void sendSwingMode(int index, float amplitude, float freq);

    // ── 时间同步 ──
    
    void sendTimesync(int seconds);

    // ── 模式切换 ──
    void ProgramModeChanged(int mode);
    // 外引导源/周期切换时清除上一轮斜坡状态，首拍从当前反馈角重新起坡。
    void resetTrackingSmoother();
    void resetAbsoluteTargetTracker();

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

    // 遥控模式的步进角度与速度倍率设置
    void setStepAngle(float step_angle) { m_step_angle = step_angle; }
    void setSpeedMultiplier(float speed_multiplier) { m_speed_multiplier = speed_multiplier; }

protected:
    void parseData(const QByteArray &rawData) override;

    void onReadyRead() override; //把数据存入缓冲区，再分包并解析数据帧

    // ── HEX二进制发送函数 ──
    void sendEnableCmd(int axis, bool enable);          // 使能/释放电机
    void sendStopCmd(int axis);                         // 停车
    void sendZeroCmd(int axis);                         // 回零
    void sendResetCmd();                                // 复位（依次发送三个轴）
    void sendPositionCmd(const PositionModeCmd1Hex &cmd);   // 位置模式
    void sendVecCmd(const SpeedModeCmd1Hex &cmd);           // 速度模式
    void sendSwingCmd(int axis, float freq, float amplitude); // 摇摆模式,可以单轴摇摆，也可以配置三轴摇摆
    void sendTrackCmd_1s(const TrackingSendCmd1Hex &cmd);     // 250ms跟踪(1s周期)
    void sendTrackCmd_20ms(const TrackingSendCmd20msHex &cmd);  // 20ms跟踪
    void sendCorrectionCmd(const TrackingCorrectionCmdHex &cmd); // 跟踪修正
    void sendTimeSetCmd(int seconds);                       // 时间设置

    // ── 遥控速度动态切换 ──
    void setHandleSpeedTarget(int axis, float target);
    void applyPendingHandleSpeed(int axis, int status, int alarm);
    void resetHandleSpeedState(int axis, bool hardwareMayBeMoving = false);
    void resetAllHandleSpeedStates(bool hardwareMayBeMoving = false);
    void sendHandleSpeedCmd(int axis, float velocity);

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
    // 用最近一帧状态时间加上该帧到达后的经过时间，估算当前转台时间。
    // 返回 -1 表示还没有状态帧，或反馈已超时。
    qint64 estimatedTurntableTimeMs() const;

    // 主线程（QML 侧）对象指针：只存引用，不拥有，析构时不能 delete
    TurntableDataHex *m_turntableDataHex = nullptr;
    TurntableSendDataHex *m_turntableSendDataHex = nullptr;
    bool m_isProgramMode = false;
    // 0=外引导，1=程控，2=遥控。-1 表示尚未收到模式通知。
    int m_operationMode = -1;
    // 只有外引导模式允许执行跟踪指令。关闭后可直接丢弃跨线程队列中
    // 尚未处理的旧 0x0A/0x0B，避免停车之后又被旧跟踪帧重新驱动。
    bool m_trackingAllowed = false;

    float m_current_inner_angle = 0.0f;
    float m_current_middle_angle = 0.0f;
    float m_current_outter_angle = 0.0f;
    // 按下“零位校正”后设为 -当前内框角度；不再使用固定 75° 默认值。
    float m_inner_angle_offset_deg = 0.0f;

    // 状态反馈中的转台毫秒时间（0-14399999，4小时循环）。
    // 跟踪目标时刻以此为准，避免本地时钟与转台时基不一致。
    qint64 m_latestTurntableTimeMs = -1;
    QElapsedTimer m_statusAgeClock;
    bool m_turntableTimeValid = false;
    bool m_waitingStatusLogged = false;
    qint64 m_last20msTargetMs = -1;

    // 大偏差时把绝对角度阶跃整形为受速度、加速度约束的斜坡。
    // 20 ms 模式使用跨帧状态；1 秒模式的四个点由 preview() 一次生成。
    TrackingTargetSmoother m_outerTrackingSmoother;
    TrackingTargetSmoother m_middleTrackingSmoother;
    QElapsedTimer m_20msTrackingClock;

    // 1 秒模式绝对目标预测。AB 的输入是“转台反馈 + 相对误差”，
    // 因此目标静止时转台自身运动不会形成虚假目标速度。
    ABTrackManager m_absoluteTargetTracker{SeekerType::Image};
    bool m_absoluteTargetReady = false;
    int m_exGuideSource = -1;
    int m_exGuidePeriod = -1;
    qint64 m_lastExGuideSampleMs = -1;
    double m_lastRelativeAzimuthDeg = 0.0;
    double m_lastRelativeElevationDeg = 0.0;

    // 转台从已有速度切换到新速度时必须先停车，并等待对应轴回到伺服状态。
    enum class HandleSpeedState {
        Idle,
        Running,
        WaitingForServo
    };

    struct HandleSpeedAxisState {
        HandleSpeedState state = HandleSpeedState::Idle;
        float activeSpeed = 0.0f;
        float pendingSpeed = 0.0f;
        bool hardwareMayBeMoving = false;
        int feedbackStatus = -1;
        int feedbackAlarm = -1;
        bool sawNonServoAfterStop = false;
        int feedbackFramesSinceStop = 0;
    };

    // 下标与轴号一致，使用 1/2/3；0 号元素不使用。
    HandleSpeedAxisState m_handleSpeedAxes[4];

    float m_step_angle = 1.0f; // 步进角度，单位度（与遥控界面默认值一致）
    float m_speed_multiplier = 1.0f; // 速度倍率，单位倍

    QByteArray m_rxBuffer;   // 串口接收缓冲：readyRead 到达的数据可能只是一帧的一部分，按帧头+固定帧长切帧
};

#endif // SERIALPORT_TURNTABLE_HEX_H
