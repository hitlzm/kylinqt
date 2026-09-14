#include "serialport_tiltmeter.h"

#include <QDebug>
#include <QThread>

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

    const QByteArray request = buildReadFrame(m_slaveAddress, kRollAddr, kReadWordCount);
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
            if (frameCrc(m_rxBuffer, 3) != modbusCrc16(m_rxBuffer.left(3))) {
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

quint16 SerialPortTiltmeter::modbusCrc16(const QByteArray &data)
{
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data.at(i));
        for (int bit = 0; bit < 8; ++bit) {
            if (crc & 0x0001)
                crc = static_cast<quint16>((crc >> 1) ^ 0xA001);
            else
                crc = static_cast<quint16>(crc >> 1);
        }
    }
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
    m_rxBuffer.clear();        // 站号变了，旧残包作废
    m_awaitingReply = false;
}
