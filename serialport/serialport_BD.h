#ifndef SERIALPORT_BD_H
#define SERIALPORT_BD_H

#include "serialport.h"
#include <QDateTime>

//北斗接收结构体
struct RMCData
{
    bool isValid;           // true=定位有效 (A), false=无效 (V)
    bool isnorth;           //判断是否为北纬
    bool iseast;
    QDateTime BJDateTime;  // 解析出的完整 UTC 时间（日期+时间）
    double latitude;        // 纬度（十进制度数，正=北纬，负=南纬）
    double longitude;       // 经度（十进制度数，正=东经，负=西经）
    QString mode;           // 定位模式 (A=单点定位, N=未定位)
};

// ── BD 接收数据类（主线程，QML 直接读取）────────────────────────────
class BDData : public QObject
{
    Q_OBJECT

    // BD 数据属性（暴露给 QML）
    Q_PROPERTY(QDateTime m_bjDateTime READ bjDateTime NOTIFY bjDateTimeChanged)
    Q_PROPERTY(bool m_isPosValid READ isPosValid NOTIFY isPosValidChanged)
    Q_PROPERTY(double m_latitude READ latitude NOTIFY latitudeChanged)
    Q_PROPERTY(bool m_isnorth READ isnorth NOTIFY isnorthChanged)
    Q_PROPERTY(double m_longitude READ longitude NOTIFY longitudeChanged)
    Q_PROPERTY(bool m_iseast READ iseast NOTIFY iseastChanged)
    Q_PROPERTY(QString m_mode READ mode NOTIFY modeChanged)

    // ── 串口状态属性 ──
    Q_PROPERTY(bool portOpen READ portOpen NOTIFY portOpenChanged)
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

public:
    explicit BDData(QObject *parent = nullptr);

    // 数据属性访问器
    QDateTime bjDateTime() const;
    bool isPosValid() const;
    double latitude() const;
    bool isnorth() const;
    double longitude() const;
    bool iseast() const;
    QString mode() const;

    // 串口状态访问器（内联）
    bool portOpen() const { return m_portOpen; }
    QStringList availablePorts() const { return m_availablePorts; }
    QString errorString() const { return m_errorString; }

    // ── QML 可调用方法 ──
    Q_INVOKABLE void openPort(const QString &portName, int baudRate);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE void scanPorts();

signals:
    // 数据变化信号
    void bjDateTimeChanged();
    void isPosValidChanged();
    void latitudeChanged();
    void isnorthChanged();
    void longitudeChanged();
    void iseastChanged();
    void modeChanged();

    // 串口状态变化信号
    void portOpenChanged();
    void availablePortsChanged();
    void errorStringChanged();

    // ── 请求信号（→ 排队到工作线程）──
    void requestOpenPort(const QString &portName, int baudRate);
    void requestClosePort();
    void requestScanPorts();

public slots:
    // ── 工作线程回推状态（QueuedConnection）──
    void setPortOpen(bool open);
    void setPortList(const QStringList &ports);
    void setError(const QString &msg);
    void updateFromFrame(const RMCData &frame);

private:
    // TODO: 根据 BD 协议补充转换辅助函数

    // 串口状态成员
    bool m_portOpen = false;
    QStringList m_availablePorts;
    QString m_errorString;

    QDateTime m_BJDateTime;
    bool m_isPosValid = false;        //位置有效标识
    double m_latitude = 0.0;        // 纬度（十进制度数，正=北纬，负=南纬）
    bool m_isnorth = true;           //判断是否为北纬
    double m_longitude = 0.0;       // 经度（十进制度数，正=东经，负=西经）
    bool m_iseast = true;            //判断是否为东经
    QString m_mode = "N";           // 定位模式 (A=单点定位, N=未定位)
};


// ── BD 串口工作线程类 ────────────────────────────────────────────
class SerialPortBD : public SerialPort
{
    Q_OBJECT
public:
    explicit SerialPortBD(QObject *parent = nullptr);
    ~SerialPortBD() override{ }

    BDData* bdData() const;
    BDData *m_bdData;

public slots:
    //初始化串口对象，并扫描可用串口
    void dowork() { SerialPort::dowork(); onScanPorts(); }

signals:
    void portOpened(bool success);
    void portClosed();
    void portError(const QString &msg);
    void portsChanged(const QStringList &ports);
    void bdFrameReceived(const RMCData &frame);

public slots:
    void onOpenPort(const QString &portName, int baudRate);
    void onClosePort();
    void onScanPorts();
protected:
    void parseData(const QByteArray &rawData) override;
    void onReadyRead() override;
private:
    double nmeaToDecimal(const QString& coord, const QString& dir);  //将 NMEA 纬度/经度（ddmm.mmmmm）转换为十进制度数
    QByteArray m_rxBuffer;   // 串口接收缓冲：readyRead 到达的数据可能只是一条 NMEA 语句的一部分，攒到 '\n' 再解析
};



#endif // SERIALPORT_BD_H
