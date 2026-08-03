#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QStringList>
#include <QTimer>

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

public slots:
    // virtual void dowork();
    void dowork();
    bool open(const QString &portName, qint32 baudRate = QSerialPort::Baud115200);
    void close();
    bool isOpen() const;
    
    qint64 send(const QByteArray &data);

protected:
    virtual void onReadyRead();
    virtual void parseData(const QByteArray &rawData)=0 ;  //重构这部分代码

    QSerialPort *m_serialPort;
    QStringList m_availablePorts;
    QTimer *timer;              //用于两类导引头发送指令时，多次发送时间间隔的计时

signals:
    // void dataReceived(const QByteArray &parsedData);
    void errorOccurred(const QString &errorMsg);
    void connected();
    void disconnected();
    void portOpenChanged();
    void availablePortsChanged();

public slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
};

//外引导模式下，单轴数据
struct sendExGuideData
{
    double angle1;  
    double angle2;  
    double angle3;
    double angle4;
    uint32_t time;
};

#endif // SERIALPORT_H
