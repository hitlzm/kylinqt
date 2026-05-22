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

uint8_t SerialPortLaser::xorChecksumcore(const uint8_t* data, size_t len)
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < len; ++i) {
        checksum ^= data[i];
    }
    return checksum;
}

uint8_t SerialPortLaser::xorChecksum(const QByteArray& data) 
{
    return xorChecksumcore(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}