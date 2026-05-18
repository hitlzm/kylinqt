#include "serialport_laser.h"

SerialPortLaser::SerialPortLaser(QObject *parent)
    : SerialPort(parent)
{
}

SerialPortLaser::~SerialPortLaser() = default;

QByteArray SerialPortLaser::parseData(const QByteArray &rawData)
{
    // TODO: implement laser-specific data parsing
    return rawData;
}
