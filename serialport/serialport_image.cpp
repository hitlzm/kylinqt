#include "serialport_image.h"

uint16_t SerialPortImage::crc16_table[256]={0};
SerialPortImage::SerialPortImage(QObject *parent)
    : SerialPort(parent)
{
    init_crc16_table();
}

SerialPortImage::~SerialPortImage() = default;

QByteArray SerialPortImage::parseData(const QByteArray &rawData)
{
    if (rawData.size() < static_cast<int>(sizeof(image_recv_frame))) {
        return QByteArray();
    }

    const image_recv_frame* pFrame = reinterpret_cast<const image_recv_frame*>(rawData.data());

    if (pFrame->frame_header1 != 0x77 || pFrame->frame_header2 != 0xAB) {
        return QByteArray();
    }

    size_t dataLen = sizeof(image_recv_frame) - sizeof(uint16_t);
    uint16_t calculatedCrc = crc16_ccitt_fast(
        reinterpret_cast<const uint8_t*>(rawData.data()), dataLen);

    if (calculatedCrc != pFrame->crc16) {
        return QByteArray();
    }

    return rawData;
}

void SerialPortImage::init_crc16_table(uint16_t poly)
{
    for (int i = 0; i < 256; ++i) {
        uint16_t crc = static_cast<uint16_t>(i << 8);
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }
        crc16_table[i] = crc;
    }

}

uint16_t SerialPortImage::crc16_ccitt_fast(const uint8_t *data, size_t len, uint16_t init)
{
    uint16_t crc = init;
    for (size_t i = 0; i < len; ++i) {
        crc = (crc << 8) ^ crc16_table[((crc >> 8) ^ data[i]) & 0xFF];
    }
    return crc;
}