#include "serialport.h"
#include <QDebug>

SerialPort::SerialPort(QObject *parent)
    : QObject(parent)
    , m_serialPort(nullptr)   // 延迟到 dowork() 在目标线程中创建
{
    scanPorts();
}

SerialPort::~SerialPort()
{
    close();
}

void SerialPort::dowork()
{
    // QSerialPort 在工作线程中创建，避免主线程创建后被 moveToThread 迁移
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead,
            this, &SerialPort::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred,
            this, &SerialPort::handleError);
}

QStringList SerialPort::availablePorts() const
{
    return m_availablePorts;
}

void SerialPort::scanPorts()
{
    m_availablePorts.clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const auto &info : ports) {
        m_availablePorts.append(info.portName());
    }
    emit availablePortsChanged();
}

bool SerialPort::openPort(const QString &portName, int baudRate)
{
    return open(portName, static_cast<qint32>(baudRate));
}

void SerialPort::closePort()
{
    close();
}

bool SerialPort::sendData(const QByteArray &data)
{
    return send(data) > 0;
}

bool SerialPort::open(const QString &portName, qint32 baudRate)
{
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        emit connected();
        emit portOpenChanged();
        return true;
    }

    emit errorOccurred(m_serialPort->errorString());
    return false;
}

void SerialPort::close()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        emit disconnected();
        emit portOpenChanged();
    }
}

bool SerialPort::isOpen() const
{
    return m_serialPort->isOpen();
}

qint64 SerialPort::send(const QByteArray &data)
{
    return m_serialPort->write(data);
}

void SerialPort::onReadyRead()
{
    QByteArray rawData = m_serialPort->readAll();
    // QByteArray parsed = parseData(rawData);
    parseData(rawData);
    // if (!parsed.isEmpty()) {
    //     emit dataReceived(parsed);
    // }
}

void SerialPort::handleReadyRead()
{
    onReadyRead();
}

void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError)
        return;

    emit errorOccurred(m_serialPort->errorString());
}
