#include "serialport_turntable.h"

SerialPortTurntable::SerialPortTurntable(QObject *parent)
    : SerialPort(parent)
{
}

SerialPortTurntable::~SerialPortTurntable() = default;

void SerialPortTurntable::parseData(const QByteArray &rawData)
{
    // TODO: implement turntable-specific data parsing
    // return rawData;
    return;
}
