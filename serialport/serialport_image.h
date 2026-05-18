#ifndef SERIALPORT_IMAGE_H
#define SERIALPORT_IMAGE_H

#include "serialport.h"

class SerialPortImage : public SerialPort
{
    Q_OBJECT

public:
    explicit SerialPortImage(QObject *parent = nullptr);
    ~SerialPortImage() override;

protected:
    QByteArray parseData(const QByteArray &rawData) override;
};

#endif // SERIALPORT_IMAGE_H
