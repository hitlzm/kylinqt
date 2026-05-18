#include "serialport.h"
#include <QDebug>

SerialPort::SerialPort(QObject *parent)
    : QObject(parent)
    , m_serialPort(new QSerialPort(this))
{
    connect(m_serialPort, &QSerialPort::readyRead,
            this, &SerialPort::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred,
            this, &SerialPort::handleError);
}

SerialPort::~SerialPort()
{
    close();
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
    QByteArray parsed = parseData(rawData);
    if (!parsed.isEmpty()) {
        emit dataReceived(parsed);
    }
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
