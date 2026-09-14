#ifndef SERIALPORT_TILTMETER_H
#define SERIALPORT_TILTMETER_H

#include "serialport.h"

#include <QDateTime>
#include <QMetaType>
#include <QTimer>

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

signals:
    // 数据变化信号
    void rollChanged();
    void pitchChanged();
    void validChanged();
    void timestampChanged();

    // 采集参数变化信号
    void pollIntervalChanged();
    void slaveAddressChanged();

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
    QDateTime m_timestamp;

    // 采集参数成员
    int m_pollInterval = 200;   // 每 200ms 请求一次
    int m_slaveAddress = 5;     // 出厂默认从站号

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

public slots:
    // 初始化串口对象（在工作线程中创建 QSerialPort 与轮询定时器），并扫描可用串口
    void dowork() { SerialPort::dowork(); initPollTimer(); onScanPorts(); }

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

private slots:
    void onPollTimeout();   // 定时到点：发一帧 04H 请求

protected:
    void onReadyRead() override;                        // 按帧切分后交给 parseData
    void parseData(const QByteArray &rawData) override; // 解析 04H 应答

private:
    void       initPollTimer();
    void       startPolling();
    void       stopPolling();
    QByteArray buildReadFrame(quint8 slave, quint16 startAddr, quint16 wordCount) const;

    static quint16 modbusCrc16(const QByteArray &data);           // CRC16，多项式 A001H
    static quint16 frameCrc(const QByteArray &frame, int len);    // 取帧尾 CRC（低字节在前）
    static qint16  readInt16BE(const QByteArray &frame, int pos); // 大端有符号 16 位

    QTimer  *m_pollTimer = nullptr;                 // 轮询定时器（工作线程内创建）
    QByteArray m_rxBuffer;                          // 接收缓冲：readyRead 可能只到达半帧
    bool     m_awaitingReply = false;               // 上一帧请求是否还未应答
    int      m_pollIntervalMs = kDefaultPollIntervalMs;
    quint8   m_slaveAddress   = 5;
};


#endif // SERIALPORT_TILTMETER_H
