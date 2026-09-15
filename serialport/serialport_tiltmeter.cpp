#include "serialport_tiltmeter.h"

#include <QDataStream>
#include <QDebug>
#include <QThread>
#include <QVector>

namespace {
constexpr double kPi = 3.14159265358979323846;   // 角度转弧度用
}

// ─────────────────────────────────────────────────────────────
// TiltData：主线程数据对象，QML 直接读取
// ─────────────────────────────────────────────────────────────

TiltData::TiltData(QObject *parent)
    : QObject(parent)
{
}

void TiltData::openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void TiltData::closePort()                                     { emit requestClosePort(); }
void TiltData::scanPorts()                                     { emit requestScanPorts(); }

void TiltData::setPollInterval(int msec)
{
    if (msec < 1)
        msec = 1;
    if (m_pollInterval == msec)
        return;

    m_pollInterval = msec;
    emit pollIntervalChanged();
    emit requestSetPollInterval(msec);
}

void TiltData::setSlaveAddress(int addr)
{
    if (addr < 1 || addr > 247)
        return;
    if (m_slaveAddress == addr)
        return;

    m_slaveAddress = addr;
    emit slaveAddressChanged();
    emit requestSetSlaveAddress(addr);
}

// 手动触发：把界面上当前显示的角度发给六自由度平台一次。
// 准入条件是"曾经收到过有效数据"：偶发一次丢包（当前 valid=false）不影响手动发送，
// 但一帧都没收到时直接拦下，避免把初始的 0°/0° 当真实角度发出去。
void TiltData::sendToPlatform()
{
    if (!m_hasValidFrame) {
        setError(QStringLiteral("尚未收到有效倾角数据，未向六自由度平台发送"));
        return;
    }
    emit requestSendToPlatform(m_roll, m_pitch);
}

void TiltData::setTarget(const QString &address, int port)
{
    if (port < 1 || port > 65535)
        return;
    if (m_targetAddress == address && m_targetPort == port)
        return;

    m_targetAddress = address;
    m_targetPort    = port;
    emit targetChanged();
    emit requestSetTarget(address, port);
}

void TiltData::updateFromFrame(const TiltFrame &frame)
{
    // 先同步有效性：应答超时帧会带着 valid=false 进来，此时只标记数据失效，
    // 保留上一次的角度值，避免界面在偶发丢包时跳回 0°。
    if (m_valid != frame.valid) {
        m_valid = frame.valid;
        emit validChanged();
    }

    m_timestamp = frame.timestamp;
    emit timestampChanged();

    if (!frame.valid)
        return;

    m_hasValidFrame = true;

    if (m_roll != frame.roll) {
        m_roll = frame.roll;
        emit rollChanged();
    }
    if (m_pitch != frame.pitch) {
        m_pitch = frame.pitch;
        emit pitchChanged();
    }
}

void TiltData::setPortOpen(bool open)
{
    if (m_portOpen != open) {
        m_portOpen = open;
        emit portOpenChanged();
    }
}

void TiltData::setPortList(const QStringList &ports)
{
    if (m_availablePorts != ports) {
        m_availablePorts = ports;
        emit availablePortsChanged();
    }
}

void TiltData::setError(const QString &msg)
{
    if (m_errorString != msg) {
        m_errorString = msg;
        emit errorStringChanged();
    }
}


// ─────────────────────────────────────────────────────────────
// SerialPortTiltmeter：工作线程串口对象
// ─────────────────────────────────────────────────────────────

SerialPortTiltmeter::SerialPortTiltmeter(QObject *parent)
    : SerialPort(parent)
    , m_tiltData(new TiltData(nullptr))   // 留在主线程，不随 moveToThread 迁移
{
    // QSerialPort 与轮询定时器都在 dowork() 中创建（工作线程内）
}

SerialPortTiltmeter::~SerialPortTiltmeter()
{
    // 定时器在工作线程创建，析构可能发生在主线程；
    // 需先迁回当前线程再 stop，否则 Qt 会报“不能跨线程操作定时器”。
    if (m_pollTimer && QThread::currentThread() != m_pollTimer->thread())
        m_pollTimer->moveToThread(QThread::currentThread());
    if (m_pollTimer)
        m_pollTimer->stop();
}

void SerialPortTiltmeter::initPollTimer()
{
    if (m_pollTimer)
        return;

    m_pollTimer = new QTimer(this);
    m_pollTimer->setTimerType(Qt::PreciseTimer);
    connect(m_pollTimer, &QTimer::timeout, this, &SerialPortTiltmeter::onPollTimeout);
}

// UDP 套接字同样在工作线程里创建，发送与倾角解析在同一个线程，不需要加锁
void SerialPortTiltmeter::initForwarding()
{
    if (m_udp)
        return;

    m_udp = new QUdpSocket(this);
}

// 把 Roll/Pitch（度）换算为弧度后组包发出，由上层按钮手动调用，每次只发一帧。
// X/Y 一一对应：平台 rx（绕 X 轴角）← 倾角仪 Roll，平台 ry（绕 Y 轴角）← 倾角仪 Pitch。
void SerialPortTiltmeter::sendPlatformAngles(double rollDeg, double pitchDeg)
{
    if (!m_udp)
        return;

    const float rx = static_cast<float>(rollDeg  * kPi / 180.0);
    const float ry = static_cast<float>(pitchDeg * kPi / 180.0);
    const QByteArray frame = buildPlatformFrame(rx, ry, m_cycleMs);

    const qint64 sent = m_udp->writeDatagram(frame, m_targetAddress, m_targetPort);
    if (sent != frame.size()) {
        // 手动发送，失败就报一次，不需要抑制重复
        qWarning() << "倾角仪→六自由度平台发送失败:" << m_udp->errorString();
        emit portError(QStringLiteral("向六自由度平台发送倾角失败：%1").arg(m_udp->errorString()));
        return;
    }
    qDebug() << "已向六自由度平台发送倾角: Roll" << rollDeg << "° Pitch" << pitchDeg
             << "° →" << m_targetAddress.toString() << m_targetPort;
}

// 平台帧（56 字节，小端，协议 V8）：
//   0    55 AA BB 起始标志
//   3    cmd = 0x06（固定）
//   4    tx, ty, tz   单精度位移（米）——只控制倾角，这里恒为 0
//   16   rx, ry, rz   单精度角度（弧度），rx=Roll、ry=Pitch、rz=0
//   28   6 个保留 float，恒为 0
//   52   int32 循环更新周期（ms）
QByteArray SerialPortTiltmeter::buildPlatformFrame(float rxRad, float ryRad, qint32 cycleMs)
{
    QByteArray frame;
    frame.reserve(kPlatformFrameSize);

    QDataStream ds(&frame, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);                        // C# 结构体直发，小端
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);        // float 占 4 字节

    ds << static_cast<quint8>(0x55)
       << static_cast<quint8>(0xAA)
       << static_cast<quint8>(0xBB)
       << static_cast<quint8>(kPlatformCmd);

    ds << float(0) << float(0) << float(0);        // tx/ty/tz：只发倾角，位移给 0
    ds << rxRad << ryRad << float(0);              // rx/ry/rz

    for (int i = 0; i < 6; ++i)                    // 6 个保留 float
        ds << float(0);

    ds << cycleMs;                                 // 循环更新周期，单位 ms
    return frame;
}

void SerialPortTiltmeter::startPolling()
{
    if (!m_pollTimer)
        return;

    m_pollTimer->start(m_pollIntervalMs);
    onPollTimeout();   // 立即取一次，首帧数据不用等到第一个周期
}

void SerialPortTiltmeter::stopPolling()
{
    if (m_pollTimer)
        m_pollTimer->stop();
    m_awaitingReply = false;
    m_rxBuffer.clear();
}

void SerialPortTiltmeter::onPollTimeout()
{
    if (!isOpen())
        return;

    if (m_awaitingReply) {
        // 上一帧请求没等到应答：大概率是从站号/波特率不匹配或线缆问题，
        // 丢弃残包重新对齐帧头，并按超时上报（角度值保持不变）。
        m_rxBuffer.clear();
        m_awaitingReply = false;

        TiltFrame timeoutFrame;
        timeoutFrame.valid     = false;
        timeoutFrame.timestamp = QDateTime::currentDateTime();
        emit tiltFrameReceived(timeoutFrame);
    }

    const QByteArray &request = requestFrame();   // 复用缓存帧，不再每次重新拼装
    if (send(request) != request.size()) {
        emit portError(m_serialPort ? m_serialPort->errorString() : QStringLiteral("倾角仪指令发送失败"));
        return;
    }
    m_awaitingReply = true;
}

void SerialPortTiltmeter::onReadyRead()
{
    m_rxBuffer.append(m_serialPort->readAll());

    while (!m_rxBuffer.isEmpty()) {
        // ── 帧头对齐：不合法的首字节直接丢弃，逐字节重新同步 ──
        if (static_cast<quint8>(m_rxBuffer.at(0)) != m_slaveAddress) {
            m_rxBuffer.remove(0, 1);
            continue;
        }
        if (m_rxBuffer.size() < 2)
            break;   // 只到了一半，等下次 readyRead

        const quint8 func = static_cast<quint8>(m_rxBuffer.at(1));

        // ── 异常应答：功能码最高位置 1，固定 5 字节 ──
        if (func == static_cast<quint8>(kFuncReadInput | 0x80)) {
            if (m_rxBuffer.size() < 5)
                break;
            if (frameCrc(m_rxBuffer, 3) != modbusCrc16(m_rxBuffer.left(3))) {   // CRC 走 256 项查找表，见 crcTable()
                m_rxBuffer.remove(0, 1);
                continue;
            }
            const quint8 exCode = static_cast<quint8>(m_rxBuffer.at(2));
            m_rxBuffer.remove(0, 5);
            m_awaitingReply = false;
            qWarning() << "倾角仪返回 Modbus 异常码: 0x"
                       << QString::number(exCode, 16).rightJustified(2, QChar('0')).toUpper();
            emit portError(QStringLiteral("倾角仪返回 Modbus 异常码 0x%1")
                               .arg(exCode, 2, 16, QChar('0')));
            continue;
        }

        if (func != kFuncReadInput) {
            m_rxBuffer.remove(0, 1);
            continue;
        }
        if (m_rxBuffer.size() < 3)
            break;

        // ── 正常应答：地址 + 功能码 + 字节数 + 数据 + CRC ──
        const int byteCount = static_cast<quint8>(m_rxBuffer.at(2));
        const int frameLen  = 3 + byteCount + 2;
        if (m_rxBuffer.size() < frameLen)
            break;   // 剩余数据还没到齐

        if (frameCrc(m_rxBuffer, frameLen - 2) != modbusCrc16(m_rxBuffer.left(frameLen - 2))) {
            qWarning() << "倾角仪应答 CRC 校验失败，丢弃首字节重新同步";
            m_rxBuffer.remove(0, 1);
            continue;
        }

        const QByteArray frame = m_rxBuffer.left(frameLen);
        m_rxBuffer.remove(0, frameLen);
        m_awaitingReply = false;
        parseData(frame);
    }
}

// 应答帧格式：<从站地址> 04 <字节数> <RollH RollL> <PitchH PitchL> <CRC_L CRC_H>
void SerialPortTiltmeter::parseData(const QByteArray &rawData)
{
    if (rawData.size() < 7) {
        qWarning() << "倾角仪应答长度不足:" << rawData.size();
        return;
    }
    if (static_cast<quint8>(rawData.at(1)) != kFuncReadInput) {
        qWarning() << "倾角仪应答功能码异常";
        return;
    }

    const int byteCount = static_cast<quint8>(rawData.at(2));
    if (byteCount < kReadWordCount * 2 || rawData.size() < 3 + byteCount + 2) {
        qWarning() << "倾角仪应答数据长度异常:" << byteCount;
        return;
    }

    const qint16 rawRoll  = readInt16BE(rawData, 3);   // 0000H 横滚角 Roll
    const qint16 rawPitch = readInt16BE(rawData, 5);   // 0001H 俯仰角 Pitch

    TiltFrame frame;
    frame.roll      = rawRoll  * kAngleScale;   // 原始值 0.01° → °
    frame.pitch     = rawPitch * kAngleScale;
    frame.valid     = true;
    frame.timestamp = QDateTime::currentDateTime();

    emit tiltFrameReceived(frame);
}

// 请求帧内容固定（站号不变时永远相同），首次调用构造后缓存；站号变化时清空重建。
// 这样 200ms 一次的轮询只做一次 send()，不再重复拼装字节与算 CRC。
const QByteArray &SerialPortTiltmeter::requestFrame()
{
    if (m_requestFrame.isEmpty())
        m_requestFrame = buildReadFrame(m_slaveAddress, kRollAddr, kReadWordCount);
    return m_requestFrame;
}

// 请求帧：<从站地址> 04 <起始地址H L> <寄存器数H L> <CRC_L CRC_H>
QByteArray SerialPortTiltmeter::buildReadFrame(quint8 slave, quint16 startAddr, quint16 wordCount) const
{
    QByteArray frame;
    frame.append(static_cast<char>(slave));
    frame.append(static_cast<char>(kFuncReadInput));
    frame.append(static_cast<char>(startAddr >> 8));
    frame.append(static_cast<char>(startAddr & 0xFF));
    frame.append(static_cast<char>(wordCount >> 8));
    frame.append(static_cast<char>(wordCount & 0xFF));

    const quint16 crc = modbusCrc16(frame);
    frame.append(static_cast<char>(crc & 0xFF));   // CRC 低字节在前
    frame.append(static_cast<char>(crc >> 8));     // CRC 高字节在后
    return frame;
}

// 256 项 CRC16 查找表（多项式 0xA001，反射式），只在首次调用时生成一次，
// 之后每个字节只需一次查表 + 移位异或，省掉内层 8 次循环。
// C++11 起函数内静态变量初始化是线程安全的，工作线程可放心直接取用。
const quint16 *SerialPortTiltmeter::crcTable()
{
    static const QVector<quint16> table = []() {
        QVector<quint16> t(256);
        for (int i = 0; i < 256; ++i) {
            quint16 crc = static_cast<quint16>(i);
            for (int bit = 0; bit < 8; ++bit) {
                if (crc & 0x0001)
                    crc = static_cast<quint16>((crc >> 1) ^ 0xA001);
                else
                    crc = static_cast<quint16>(crc >> 1);
            }
            t[i] = crc;
        }
        return t;
    }();
    return table.constData();
}

// 查表法计算 CRC16：初值 0xFFFF，低字节在前
quint16 SerialPortTiltmeter::modbusCrc16(const QByteArray &data)
{
    const quint16 *table = crcTable();
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i)
        crc = static_cast<quint16>((crc >> 8)
                                   ^ table[(crc ^ static_cast<quint8>(data.at(i))) & 0xFF]);
    return crc;
}

quint16 SerialPortTiltmeter::frameCrc(const QByteArray &frame, int len)
{
    const quint8 lo = static_cast<quint8>(frame.at(len));
    const quint8 hi = static_cast<quint8>(frame.at(len + 1));
    return static_cast<quint16>((hi << 8) | lo);   // 线上先低字节后高字节
}

qint16 SerialPortTiltmeter::readInt16BE(const QByteArray &frame, int pos)
{
    const quint16 v = static_cast<quint16>((static_cast<quint8>(frame.at(pos)) << 8)
                                           | static_cast<quint8>(frame.at(pos + 1)));
    return static_cast<qint16>(v);   // 按补码解释为有符号角度值
}

// ── 串口操作槽函数 ──
void SerialPortTiltmeter::onOpenPort(const QString &name, int baud)
{
    if (!SerialPort::open(name, baud)) {
        emit portError(m_serialPort ? m_serialPort->errorString() : QStringLiteral("QSerialPort not created"));
        return;
    }

    m_awaitingReply = false;
    m_rxBuffer.clear();
    m_serialPort->readAll();   // 丢掉打开瞬间的残留字节

    startPolling();            // 打开成功后按周期开始采集
    emit portOpened(true);
}

void SerialPortTiltmeter::onClosePort()
{
    stopPolling();
    SerialPort::close();
    emit portClosed();
}

void SerialPortTiltmeter::onScanPorts()
{
    SerialPort::scanPorts();
    emit portsChanged(m_availablePorts);
}

void SerialPortTiltmeter::onSetPollInterval(int msec)
{
    if (msec < kMinPollIntervalMs)
        msec = kMinPollIntervalMs;
    m_pollIntervalMs = msec;

    if (m_pollTimer && m_pollTimer->isActive())
        m_pollTimer->start(m_pollIntervalMs);

    qDebug() << "倾角仪轮询周期已设置为" << m_pollIntervalMs << "ms";
}

void SerialPortTiltmeter::onSetSlaveAddress(int addr)
{
    if (addr < 1 || addr > 247)
        return;
    m_slaveAddress = static_cast<quint8>(addr);
    m_requestFrame.clear();    // 站号变了，缓存帧作废，下次轮询重建
    m_rxBuffer.clear();        // 旧残包同样作废
    m_awaitingReply = false;
}

// 手动发送一次：角度由主线程的 TiltData 传进来（即界面上当前显示的值）
void SerialPortTiltmeter::onSendToPlatform(double rollDeg, double pitchDeg)
{
    sendPlatformAngles(rollDeg, pitchDeg);
}

void SerialPortTiltmeter::onSetTarget(const QString &address, int port)
{
    QHostAddress addr;
    if (address == QStringLiteral("255.255.255.255") || address.isEmpty()) {
        addr = QHostAddress(QHostAddress::Broadcast);
    } else if (!addr.setAddress(address)) {
        qWarning() << "六自由度平台地址非法，忽略:" << address;
        return;
    }
    if (port < 1 || port > 65535)
        return;

    m_targetAddress = addr;
    m_targetPort    = static_cast<quint16>(port);
}
