#include "serialport_image.h"

SerialPortImage::SerialPortImage(QObject *parent)
    : SerialPort(parent)
{
}

SerialPortImage::~SerialPortImage() = default;

QByteArray SerialPortImage::parseData(const QByteArray &rawData)
{
    // TODO: implement image-specific data parsing
    return rawData;
}
