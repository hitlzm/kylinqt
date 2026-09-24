#include "serialport.h"
#include <QDebug>
#include <QThread>
#include <cstring>
//#include <sys/mman.h>

SerialPort::SerialPort(QObject *parent)
    : QObject(parent)
    , m_serialPort(nullptr)   // 延迟到 dowork() 在目标线程中创建
    , timer(nullptr)
{
    scanPorts();  //创建串口对象时提供一次扫描
}

SerialPort::~SerialPort()
{
    // m_serialPort / timer 都是 dowork() 里以 this 为父对象创建的子对象。
    // 有父对象的 QObject 不能单独 moveToThread()：Qt 只会打印
    // "QObject::moveToThread: Cannot move objects with a parent" 然后什么都不做，
    // 所以这里不再做跨线程迁移（它们本来就会随本对象一起析构）。
    // 只在本对象所属线程内调用 close()，避免跨线程去停 QSerialPort 内部的 notifier/timer；
    // 非本线程的析构场景下，串口与定时器由 QObject 的父子析构链完成释放。
    if (QThread::currentThread() == thread())
        close();

    delete m_serialPort;
    m_serialPort = nullptr;
    delete timer;
    timer = nullptr;
}

void SerialPort::dowork()
{

    // QSerialPort 在工作线程中创建，避免主线程创建后被 moveToThread 迁移
    m_serialPort = new QSerialPort(this);
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
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
    // m_serialPort 由 dowork() 在工作线程中创建；未创建时（对象刚构造、线程未启动）直接返回，
    // 避免空指针解引用
    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->close();
        emit disconnected();
        emit portOpenChanged();
    }
}

bool SerialPort::isOpen() const
{
    return m_serialPort && m_serialPort->isOpen();
}

qint64 SerialPort::send(const QByteArray &data)
{
    if (!m_serialPort)
        return -1;
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
    close(); //出错后关闭串口，并通知串口异常
}
