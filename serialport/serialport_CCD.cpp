#include "serialport_CCD.h"
#include <QDebug>
#include <QTimeZone>

namespace {
constexpr int kMotionSpeed = 4;  // 变焦/调焦连续运动速度 0-7

const quint8 kResolutionCodes[] = {
    0x07, 0x08, 0x0A, 0x0C, 0x0F, 0x11, 0x10, 0x12, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D
};
}

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
    connect(m_ccdData, &CCDData::reqResolutionchange, this, [this](int index) { sendResolutionchange(index); });
}

void SerialPortCCD::send30XFocus()
{
    writeCommand(cmd_30X);
}
void SerialPortCCD::send1XFocus()
{
    writeCommand(cmd_1X);
}
void SerialPortCCD::senddigZoomopen()
{
    writeCommand(cmd_digZoomopen);
}
void SerialPortCCD::senddigZoomclose()
{
    writeCommand(cmd_digZoomclose);
}
void SerialPortCCD::sendBacklightopen()
{
    writeCommand(cmd_Backlightopen);
}
void SerialPortCCD::sendBacklightclose()
{
    writeCommand(cmd_Backlightclose);
}
void SerialPortCCD::sendResolutionchange(int index)
{
    const int count = static_cast<int>(sizeof(kResolutionCodes)
                                       / sizeof(kResolutionCodes[0]));
    if (index < 0 || index >= count)
        return;

    QByteArray command;
    command.reserve(8);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(0x24)).append(char(0x73)).append(char(0x00))
            .append(char(kResolutionCodes[index])).append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendZoomMove(int direction)
{
    if (direction == 0) {
        sendZoomStop();
        return;
    }

    QByteArray command;
    command.reserve(6);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(0x07))
            .append(char((direction > 0 ? 0x20 : 0x30) | kMotionSpeed))
            .append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendZoomStop()
{
    writeCommand(QByteArray::fromHex("8101040700FF"));
}

void SerialPortCCD::sendFocusMode(int mode)
{
    writeCommand(mode == 0
                 ? QByteArray::fromHex("8101043802FF")
                 : QByteArray::fromHex("8101043803FF"));
}

void SerialPortCCD::sendFocusMove(int direction)
{
    if (direction == 0) {
        sendFocusStop();
        return;
    }

    QByteArray command;
    command.reserve(6);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(0x08))
            .append(char((direction > 0 ? 0x20 : 0x30) | kMotionSpeed))
            .append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendFocusStop()
{
    writeCommand(QByteArray::fromHex("8101040800FF"));
}

void SerialPortCCD::sendDefogSetting(bool enabled, int level)
{
    QByteArray command;
    command.reserve(7);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(0x37));
    if (enabled) {
        command.append(char(0x02))
                .append(char(qBound(1, level, 3)))
                .append(char(0xFF));
    } else {
        command.append(char(0x03)).append(char(0x00)).append(char(0xFF));
    }
    writeCommand(command);
}

void SerialPortCCD::sendHeatHazeLevel(int level)
{
    QByteArray command;
    command.reserve(7);
    command.append(char(0x81)).append(char(0x01)).append(char(0x09))
            .append(char(0x83)).append(char(0x08))
            .append(char(qBound(0, level, 10))).append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendNoiseReductionSetting(bool enabled, int level)
{
    QByteArray command;
    command.reserve(11);
    command.append(char(0x81)).append(char(0x01)).append(char(0x09))
            .append(char(0x83)).append(char(0x0B));
    if (enabled) {
        const int strength = qBound(0, level, 100);
        command.append(char(0x01))
                .append(char((strength >> 8) & 0xFF))
                .append(char(strength & 0xFF))
                .append(char(0x00)).append(char(0x00));
    } else {
        command.append(QByteArray(5, '\0'));
    }
    command.append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendStabilizationSetting(bool enabled, int level)
{
    QByteArray command;
    command.reserve(7);
    command.append(char(0x81)).append(char(0x01)).append(char(0x09))
            .append(char(0x83)).append(char(0x07))
            .append(char(enabled ? qBound(1, level, 3) : 0))
            .append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendWhiteBalanceMode(int mode)
{
    writeCommand(mode == 1
                 ? QByteArray::fromHex("8101043505FF")
                 : QByteArray::fromHex("8101043500FF"));
}

void SerialPortCCD::sendWhiteBalanceGain(bool red, int gain)
{
    const int value = qBound(0, gain, 255);
    QByteArray command;
    command.reserve(9);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(red ? 0x43 : 0x44))
            .append(char(0x00)).append(char(0x00))
            .append(char((value >> 8) & 0xFF)).append(char(value & 0xFF))
            .append(char(0xFF));
    writeCommand(command);
}

// 串口操作槽函数
void SerialPortCCD::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}
void SerialPortCCD::onClosePort()
{
    if (isOpen()) {
        sendZoomStop();
        sendFocusStop();
        if (m_serialPort)
            m_serialPort->waitForBytesWritten(50);
    }
    SerialPort::close();
    emit portClosed();
}
void SerialPortCCD::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }

void SerialPortCCD::writeCommand(const QByteArray &command)
{
    if (!m_serialPort || !m_serialPort->isOpen())
        return;
    m_serialPort->write(command);
}

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

void CCDData::setFocusMode(int mode) {
    mode = qBound(0, mode, 1);
    if (m_focusMode != mode) {
        m_focusMode = mode;
        emit focusModeChanged();
        if (mode == 0)
            emit reqFocusStop();
        emit reqFocusMode(mode);
    }
}

void CCDData::startZoom(int direction)
{
    if (direction != 0)
        emit reqZoomMove(direction > 0 ? 1 : -1);
}

void CCDData::stopZoom()
{
    emit reqZoomStop();
}

void CCDData::startFocus(int direction)
{
    if (m_focusMode != 1 || direction == 0)
        return;
    emit reqFocusMove(direction > 0 ? 1 : -1);
}

void CCDData::stopFocus()
{
    emit reqFocusStop();
}

void CCDData::setDefog(bool enabled)
{
    if (m_defogEnabled == enabled)
        return;
    m_defogEnabled = enabled;
    emit defogEnabledChanged();
    emit reqDefogSetting(m_defogEnabled, m_defogLevel);
}

void CCDData::setDefogLevel(int level)
{
    level = qBound(1, level, 3);
    if (m_defogLevel == level)
        return;
    m_defogLevel = level;
    emit defogLevelChanged();
    if (m_defogEnabled)
        emit reqDefogSetting(true, m_defogLevel);
}

void CCDData::setHeatHazeLevel(int level)
{
    level = qBound(0, level, 10);
    if (m_heatHazeLevel == level)
        return;
    m_heatHazeLevel = level;
    emit heatHazeLevelChanged();
    emit reqHeatHazeLevel(m_heatHazeLevel);
}

void CCDData::setNoiseReduction(bool enabled)
{
    if (m_noiseReductionEnabled == enabled)
        return;
    m_noiseReductionEnabled = enabled;
    emit noiseReductionEnabledChanged();
    emit reqNoiseReductionSetting(enabled, m_noiseReductionLevel);
}

void CCDData::setNoiseReductionLevel(int level)
{
    level = qBound(0, level, 100);
    if (m_noiseReductionLevel == level)
        return;
    m_noiseReductionLevel = level;
    emit noiseReductionLevelChanged();
    if (m_noiseReductionEnabled)
        emit reqNoiseReductionSetting(true, level);
}

void CCDData::setStabilization(bool enabled)
{
    if (m_stabilizationEnabled == enabled)
        return;
    m_stabilizationEnabled = enabled;
    emit stabilizationEnabledChanged();
    emit reqStabilizationSetting(enabled, m_stabilizationLevel);
}

void CCDData::setStabilizationLevel(int level)
{
    level = qBound(1, level, 3);
    if (m_stabilizationLevel == level)
        return;
    m_stabilizationLevel = level;
    emit stabilizationLevelChanged();
    if (m_stabilizationEnabled)
        emit reqStabilizationSetting(true, level);
}

void CCDData::setBacklight(bool on) {
    if (m_backlightOn != on) {
        m_backlightOn = on;
        emit backlightOnChanged();
        if (on) emit reqBacklightopen();
        else emit reqBacklightclose();
    }
}

void CCDData::setResolution(int index) {
    if (index < 0 || index > 17) return;
    if (m_resolutionIndex != index) {
        m_resolutionIndex = index;
        emit resolutionIndexChanged();
        emit reqResolutionchange(index);
    }
}

void CCDData::setWhiteBalanceMode(int mode)
{
    mode = qBound(0, mode, 1);
    if (m_whiteBalanceMode == mode)
        return;
    m_whiteBalanceMode = mode;
    emit whiteBalanceModeChanged();
    emit reqWhiteBalanceMode(mode);
}

void CCDData::setRedGain(int gain)
{
    gain = qBound(0, gain, 255);
    if (m_redGain == gain)
        return;
    m_redGain = gain;
    emit redGainChanged();
    if (m_whiteBalanceMode == 1)
        emit reqWhiteBalanceGain(true, gain);
}

void CCDData::setBlueGain(int gain)
{
    gain = qBound(0, gain, 255);
    if (m_blueGain == gain)
        return;
    m_blueGain = gain;
    emit blueGainChanged();
    if (m_whiteBalanceMode == 1)
        emit reqWhiteBalanceGain(false, gain);
}
