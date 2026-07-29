#include "serialport_CCD.h"
#include <QDebug>
#include <QTimeZone>

SerialPortCCD::SerialPortCCD(QObject *parent)
    : SerialPort(parent)
    , m_ccdData(new CCDData(nullptr))        // 留在主线程，不随 moveToThread 迁移
{   
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::errorOccurred,
            this, &SerialPort::handleError);
    onScanPorts();
    // CCDData 请求信号 → SerialPortCCD 执行槽
    connect(m_ccdData, &CCDData::req30XFocus,       this, &SerialPortCCD::send30XFocus);
    connect(m_ccdData, &CCDData::req1XFocus,        this, &SerialPortCCD::send1XFocus);
    connect(m_ccdData, &CCDData::reqdigZoomopen,    this, &SerialPortCCD::senddigZoomopen);
    connect(m_ccdData, &CCDData::reqdigZoomclose,   this, &SerialPortCCD::senddigZoomclose);
    connect(m_ccdData, &CCDData::reqBacklightopen,  this, &SerialPortCCD::sendBacklightopen);
    connect(m_ccdData, &CCDData::reqBacklightclose, this, &SerialPortCCD::sendBacklightclose);
    connect(m_ccdData, &CCDData::reqResolutionchange, this, &SerialPortCCD::sendResolutionchange);
}

void SerialPortCCD::send30XFocus()
{
    qint64 bytesWritten = m_serialPort->write(cmd_30X);
    // if (bytesWritten == -1) {
    //     qCritical() << "30倍焦距指令表发送失败：" << m_serialPort->errorString();
    // } 
}
void SerialPortCCD::send1XFocus()
{
    qint64 bytesWritten = m_serialPort->write(cmd_1X);
}
void SerialPortCCD::senddigZoomopen()
{
    qint64 bytesWritten = m_serialPort->write(cmd_digZoomopen);
}
void SerialPortCCD::senddigZoomclose()
{
    qint64 bytesWritten = m_serialPort->write(cmd_digZoomclose);
}
void SerialPortCCD::sendBacklightopen()
{
    qint64 bytesWritten = m_serialPort->write(cmd_Backlightopen);
}
void SerialPortCCD::sendBacklightclose()
{
    qint64 bytesWritten = m_serialPort->write(cmd_Backlightclose);
}
void SerialPortCCD::sendResolutionchange()
{
    qint64 bytesWritten = m_serialPort->write(cmd_Resolutionchange1);
}

void SerialPortCCD::ExmodeChanged(int mode)
{
    // CCD 模式切换处理
    // 判断当前模式：
    //   0 — 外引导模式
    //   1 — 程控模式
    //   2 — 遥控模式
    // TODO: 根据模式执行对应的 CCD 操作（如切换分辨率、变倍等）
    Q_UNUSED(mode);
}

// 串口操作槽函数
void SerialPortCCD::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}
void SerialPortCCD::onClosePort()  { SerialPort::close(); emit portClosed(); }
void SerialPortCCD::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }

CCDData::CCDData(QObject *parent)
    : QObject(parent)
{

}

void CCDData::openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void CCDData::closePort()                                     { emit requestClosePort(); }
void CCDData::scanPorts()                                     { emit requestScanPorts(); }

void CCDData::setPortOpen(bool open) { if (m_portOpen != open) { m_portOpen = open; emit portOpenChanged(); } }
void CCDData::setPortList(const QStringList &ports) { if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); } }
void CCDData::setError(const QString &msg) { if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); } }
