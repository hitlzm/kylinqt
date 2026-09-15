#ifndef SERIALPORT_TILTMETER_H
#define SERIALPORT_TILTMETER_H

#include "serialport.h"

#include <QDateTime>
#include <QHostAddress>
#include <QMetaType>
#include <QTimer>
#include <QUdpSocket>

/**
 * 倾角仪（VALUER 动态倾角传感器，Modbus RTU）定间隔采集。
 *
 * 采集对象：输入寄存器 0000H 横滚角 Roll（绕 X 轴旋转角度）、
 *           0001H 俯仰角 Pitch（X 轴与水平面夹角），单位 0.01°。
 *           两个地址连续，一次 04H 请求即可同时取回。
 *
 * 请求帧（功能码 04H 读输入寄存器，从站默认 5，读 2 个 word）：
 *      05 04 00 00 00 02 70 4F
 * 应答帧（9 字节）：
 *      05 04 04 <RollH RollL> <PitchH PitchL> <CRC_L CRC_H>
 *      数据高字节在前，CRC 低字节在前。
 *
 * Modbus 为一问一答式，传感器不会主动上报，因此数据更新率等于轮询频率；
 * 默认每 200ms 请求一次，可通过 setPollInterval() 调整。
 *
 * 性能说明：请求帧内容固定，构造一次后缓存复用；CRC16 采用 256 项查找表，
 * 表在首次使用时生成，之后的请求/校验只做移位与异或。
 *
 * 同时提供向六自由度平台发送倾角的能力（协议 V8：UDP 端口 7408，
 * 56 字节定长帧，小端 float，cmd=0x06）：由上层按钮调用 sendToPlatform()
 * 手动触发一次发送，不会随每帧倾角数据自动发送，也不需要额外的线程。
 */

// ── 一帧解析结果 ──────────────────────────────────────────────
struct TiltFrame
{
    double    roll      = 0.0;   // 横滚角 Roll，绕 X 轴，单位 °
    double    pitch     = 0.0;   // 俯仰角 Pitch，X 轴与水平面夹角，单位 °
    bool      valid     = false; // 本帧是否解析成功（false 表示本周期应答超时）
    QDateTime timestamp;         // 收到本帧的本机时刻
};

Q_DECLARE_METATYPE(TiltFrame)


// ── 倾角仪数据类（主线程，QML 直接读取）────────────────────────
class TiltData : public QObject
{
    Q_OBJECT

    // ── 角度数据属性 ──
    Q_PROPERTY(double    roll      READ roll      NOTIFY rollChanged)
    Q_PROPERTY(double    pitch     READ pitch     NOTIFY pitchChanged)
    Q_PROPERTY(bool      valid     READ valid     NOTIFY validChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY timestampChanged)

    // ── 采集参数属性 ──
    Q_PROPERTY(int pollInterval READ pollInterval NOTIFY pollIntervalChanged)  // 轮询间隔，单位 ms
    Q_PROPERTY(int slaveAddress READ slaveAddress NOTIFY slaveAddressChanged)  // 传感器 Modbus 从站号

    // ── 六自由度平台发送属性 ──
    Q_PROPERTY(QString targetAddress READ targetAddress NOTIFY targetChanged)   // 平台地址（默认广播）
    Q_PROPERTY(int     targetPort    READ targetPort    NOTIFY targetChanged)   // 平台端口（默认 7408）

    // ── 串口状态属性 ──
    Q_PROPERTY(bool        portOpen       READ portOpen       NOTIFY portOpenChanged)
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString     errorString    READ errorString    NOTIFY errorStringChanged)

public:
    explicit TiltData(QObject *parent = nullptr);

    // 数据属性访问器
    double    roll() const      { return m_roll; }
    double    pitch() const     { return m_pitch; }
    bool      valid() const     { return m_valid; }
    QDateTime timestamp() const { return m_timestamp; }

    // 采集参数访问器
    int pollInterval() const { return m_pollInterval; }
    int slaveAddress() const { return m_slaveAddress; }

    // 平台发送访问器
    QString targetAddress() const { return m_targetAddress; }
    int     targetPort() const    { return m_targetPort; }

    // 串口状态访问器
    bool        portOpen() const       { return m_portOpen; }
    QStringList availablePorts() const { return m_availablePorts; }
    QString     errorString() const    { return m_errorString; }

    // ── QML 可调用方法 ──
    Q_INVOKABLE void openPort(const QString &portName, int baudRate);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE void scanPorts();
    Q_INVOKABLE void setPollInterval(int msec);   // 轮询间隔（ms），默认 200
    Q_INVOKABLE void setSlaveAddress(int addr);   // 从站号，1~247，默认 5
    Q_INVOKABLE void sendToPlatform();                            // 手动发送一次当前倾角
    Q_INVOKABLE void setTarget(const QString &address, int port); // 平台地址与端口

signals:
    // 数据变化信号
    void rollChanged();
    void pitchChanged();
    void validChanged();
    void timestampChanged();

    // 采集参数变化信号
    void pollIntervalChanged();
    void slaveAddressChanged();

    // 平台发送参数变化信号
    void targetChanged();

    // 串口状态变化信号
    void portOpenChanged();
    void availablePortsChanged();
    void errorStringChanged();

    // ── 请求信号（→ 排队到工作线程）──
    void requestOpenPort(const QString &portName, int baudRate);
    void requestClosePort();
    void requestScanPorts();
    void requestSetPollInterval(int msec);
    void requestSetSlaveAddress(int addr);
    void requestSendToPlatform(double rollDeg, double pitchDeg);
    void requestSetTarget(const QString &address, int port);

public slots:
    // ── 工作线程回推数据/状态（QueuedConnection）──
    void updateFromFrame(const TiltFrame &frame);
    void setPortOpen(bool open);
    void setPortList(const QStringList &ports);
    void setError(const QString &msg);

private:
    // 角度数据成员
    double    m_roll  = 0.0;
    double    m_pitch = 0.0;
    bool      m_valid = false;
    bool      m_hasValidFrame = false;   // 是否收到过至少一帧有效数据（手动发送的准入条件）
    QDateTime m_timestamp;

    // 采集参数成员
    int m_pollInterval = 200;   // 每 200ms 请求一次
    int m_slaveAddress = 5;     // 出厂默认从站号

    // 平台发送参数成员（默认与工作线程保持一致）
    QString m_targetAddress = QStringLiteral("255.255.255.255");  // 协议要求广播
    int     m_targetPort    = 7408;

    // 串口状态成员
    bool        m_portOpen = false;
    QStringList m_availablePorts;
    QString     m_errorString;
};


// ── 倾角仪串口工作类（运行在独立线程）───────────────────────────
class SerialPortTiltmeter : public SerialPort
{
    Q_OBJECT
public:
    explicit SerialPortTiltmeter(QObject *parent = nullptr);
    ~SerialPortTiltmeter() override;

    TiltData *m_tiltData = nullptr;   // 主线程 Data 对象，main.cpp 中挂入

    // Modbus 常量
    static constexpr quint8  kFuncReadInput = 0x04;   // 读输入寄存器
    static constexpr quint16 kRollAddr      = 0x0000; // 横滚角 Roll
    static constexpr quint16 kPitchAddr     = 0x0001; // 俯仰角 Pitch
    static constexpr quint16 kReadWordCount = 2;      // 一次读 Roll + Pitch
    static constexpr double  kAngleScale    = 0.01;   // 原始值 × 0.01 = 角度(°)

    static constexpr int kDefaultPollIntervalMs = 200; // 默认轮询周期
    static constexpr int kMinPollIntervalMs     = 10;  // 最小轮询周期，防止打满串口

    // ── 六自由度平台 UDP 协议（通讯协议 V8）──
    static constexpr quint16 kPlatformPort      = 7408;  // 平台接收端口
    static constexpr quint8  kPlatformCmd       = 0x06;  // 固定命令位（循环运行数）
    static constexpr int     kPlatformCycleMs   = 5000;  // 循环更新周期字段，单位 ms
    static constexpr int     kPlatformFrameSize = 56;    // 3 起始 + cmd + 12 float + int

public slots:
    // 初始化串口对象（在工作线程中创建 QSerialPort 与轮询定时器），并扫描可用串口
    void dowork() { SerialPort::dowork(); initPollTimer(); initForwarding(); onScanPorts(); }

signals:
    void portOpened(bool success);
    void portClosed();
    void portError(const QString &msg);
    void portsChanged(const QStringList &ports);
    void tiltFrameReceived(const TiltFrame &frame);

public slots:
    void onOpenPort(const QString &portName, int baudRate);
    void onClosePort();
    void onScanPorts();
    void onSetPollInterval(int msec);
    void onSetSlaveAddress(int addr);
    void onSendToPlatform(double rollDeg, double pitchDeg);   // 手动触发：发一帧给平台
    void onSetTarget(const QString &address, int port);

private slots:
    void onPollTimeout();   // 定时到点：发一帧 04H 请求

protected:
    void onReadyRead() override;                        // 按帧切分后交给 parseData
    void parseData(const QByteArray &rawData) override; // 解析 04H 应答

private:
    void       initPollTimer();
    void       startPolling();
    void       stopPolling();
    void       initForwarding();
    void       sendPlatformAngles(double rollDeg, double pitchDeg);   // 组包并发出一次

    // 组包：3 个起始标志 + cmd + tx/ty/tz + rx/ry/rz + 6 个保留 float + int 周期
    static QByteArray buildPlatformFrame(float rxRad, float ryRad, qint32 cycleMs);

    // 请求帧内容固定（站号不变时永远相同），缓存下来避免每次轮询都重新拼装
    const QByteArray &requestFrame();
    QByteArray buildReadFrame(quint8 slave, quint16 startAddr, quint16 wordCount) const;

    static const quint16 *crcTable();                             // 256 项 CRC16 查找表，首次使用时生成
    static quint16 modbusCrc16(const QByteArray &data);           // CRC16（查表法），多项式 A001H
    static quint16 frameCrc(const QByteArray &frame, int len);    // 取帧尾 CRC（低字节在前）
    static qint16  readInt16BE(const QByteArray &frame, int pos); // 大端有符号 16 位

    QTimer  *m_pollTimer = nullptr;                 // 轮询定时器（工作线程内创建）
    QByteArray m_requestFrame;                      // 缓存的 04H 请求帧
    QByteArray m_rxBuffer;                          // 接收缓冲：readyRead 可能只到达半帧
    bool     m_awaitingReply = false;               // 上一帧请求是否还未应答
    int      m_pollIntervalMs = kDefaultPollIntervalMs;
    quint8   m_slaveAddress   = 5;

    QUdpSocket  *m_udp = nullptr;                                        // 向平台发包的 UDP 套接字
    QHostAddress m_targetAddress = QHostAddress(QHostAddress::Broadcast); // 平台地址（默认广播）
    quint16      m_targetPort    = kPlatformPort;
    qint32       m_cycleMs       = kPlatformCycleMs;
};


#endif // SERIALPORT_TILTMETER_H
