#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QStringList>

class SerialPort : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(bool portOpen READ isOpen NOTIFY portOpenChanged)

public:
    explicit SerialPort(QObject *parent = nullptr);
    virtual ~SerialPort();

    QStringList availablePorts() const;

    Q_INVOKABLE bool openPort(const QString &portName, int baudRate);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE void scanPorts();
    Q_INVOKABLE bool sendData(const QByteArray &data);

    bool open(const QString &portName, qint32 baudRate = QSerialPort::Baud115200);
    void close();
    bool isOpen() const;

    qint64 send(const QByteArray &data);

protected:
    virtual void onReadyRead();
    virtual void parseData(const QByteArray &rawData) = 0;

    QSerialPort *m_serialPort;
    QStringList m_availablePorts;

signals:
    // void dataReceived(const QByteArray &parsedData);
    void errorOccurred(const QString &errorMsg);
    void connected();
    void disconnected();
    void portOpenChanged();
    void availablePortsChanged();

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
};

#endif // SERIALPORT_H
