#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class SerialPort : public QObject
{
    Q_OBJECT

public:
    explicit SerialPort(QObject *parent = nullptr);
    virtual ~SerialPort();

    bool open(const QString &portName, qint32 baudRate = QSerialPort::Baud115200);
    void close();
    bool isOpen() const;

    qint64 send(const QByteArray &data);

protected:
    virtual void onReadyRead();
    virtual QByteArray parseData(const QByteArray &rawData) = 0;

    QSerialPort *m_serialPort;

signals:
    void dataReceived(const QByteArray &parsedData);
    void errorOccurred(const QString &errorMsg);
    void connected();
    void disconnected();

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
};

#endif // SERIALPORT_H
