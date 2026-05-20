#ifndef SERIALPORT_TURNTABLE_H
#define SERIALPORT_TURNTABLE_H

#include "serialport.h"

class SerialPortTurntable : public SerialPort
{
    Q_OBJECT

public:
    explicit SerialPortTurntable(QObject *parent = nullptr);
    ~SerialPortTurntable() override;

protected:
    QByteArray parseData(const QByteArray &rawData) override;
};

#endif // SERIALPORT_TURNTABLE_H
