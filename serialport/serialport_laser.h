#ifndef SERIALPORT_LASER_H
#define SERIALPORT_LASER_H

#include "serialport.h"

class SerialPortLaser : public SerialPort
{
    Q_OBJECT

public:
    explicit SerialPortLaser(QObject *parent = nullptr);
    ~SerialPortLaser() override;

protected:
    QByteArray parseData(const QByteArray &rawData) override;
};

#endif // SERIALPORT_LASER_H
