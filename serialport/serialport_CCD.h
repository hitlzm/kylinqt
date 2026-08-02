#ifndef SERIALPORT_CCD_H
#define SERIALPORT_CCD_H

#include "serialport.h"
#include "kalman/AlphaBetaTracker.h"
#include <QDateTime>
#include <QTimer>

// ── CCD 接收数据类（主线程，QML 直接读取）────────────────────────────
class CCDData : public QObject
{
    Q_OBJECT
    // ── 串口状态属性 ──
    Q_PROPERTY(bool portOpen READ portOpen NOTIFY portOpenChanged)
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

    // ── CCD 控制状态属性 ──
    Q_PROPERTY(int focusMode READ focusMode NOTIFY focusModeChanged)           // 0=30X, 1=1X
    Q_PROPERTY(bool backlightOn READ backlightOn NOTIFY backlightOnChanged)
    Q_PROPERTY(int resolutionIndex READ resolutionIndex NOTIFY resolutionIndexChanged)  // 0=1080p/30, 1=1080p/25, 2=720p/30, 3=720p/25

public:
    explicit CCDData(QObject *parent = nullptr);

    // 数据属性访问器
    bool isValid() const;

    // 串口状态访问器（内联）
    bool portOpen() const { return m_portOpen; }
    QStringList availablePorts() const { return m_availablePorts; }
    QString errorString() const { return m_errorString; }

    // CCD 控制状态访问器（内联）
    int focusMode() const { return m_focusMode; }
    bool backlightOn() const { return m_backlightOn; }
    int resolutionIndex() const { return m_resolutionIndex; }

    // ── QML 可调用方法 ──
    Q_INVOKABLE void openPort(const QString &portName, int baudRate);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE void scanPorts();

    // CCD 指令触发方法（QML 可调用，发射请求信号到 CCD 串口）
    Q_INVOKABLE void setFocusMode(int mode);       // 0=30X, 1=1X
    Q_INVOKABLE void setBacklight(bool on);        // true=开, false=关
    Q_INVOKABLE void setResolution(int index);     // 0=1080p/30, 1=1080p/25, 2=720p/30, 3=720p/25

signals:

    // 串口状态变化信号
    void portOpenChanged();
    void availablePortsChanged();
    void errorStringChanged();

    // ── 请求信号（→ 排队到工作线程）──
    void requestOpenPort(const QString &portName, int baudRate);
    void requestClosePort();
    void requestScanPorts();

    //负责发送信号给CCD串口类，发送的指令有30倍焦距，1倍焦距，数字变倍开/关，背光补偿开/关，分辨率切换
    void req30XFocus();
    void req1XFocus();
    void reqdigZoomopen();
    void reqdigZoomclose();
    void reqBacklightopen();
    void reqBacklightclose();
    void reqResolutionchange(int index);

public slots:
    // ── 工作线程回推状态（QueuedConnection）──
    void setPortOpen(bool open);
    void setPortList(const QStringList &ports);
    void setError(const QString &msg);
    // void updateFromFrame(const CCDFrameData &frame);

private:
    // TODO: 根据 CCD 协议补充转换辅助函数

    // 串口状态成员
    bool m_portOpen = false;
    QStringList m_availablePorts;
    QString m_errorString;

    // CCD 控制状态成员
    int m_focusMode = 0;         // 0=30X, 1=1X
    bool m_backlightOn = false;
    int m_resolutionIndex = 0;   // 0=1080p/30, 1=1080p/25, 2=720p/30, 3=720p/25

    // bool m_isValid = false;             // 数据有效标识
    // TODO: 补充 CCD 数据成员
};


// ── CCD 串口工作线程类 ────────────────────────────────────────────
class SerialPortCCD : public SerialPort
{
    Q_OBJECT
public:
    explicit SerialPortCCD(QObject *parent = nullptr);
    ~SerialPortCCD() override { }

    CCDData* ccdData() const;
    CCDData *m_ccdData;
signals:
    void portOpened(bool success);
    void portClosed();
    void portError(const QString &msg);
    void portsChanged(const QStringList &ports);

    // 外引导模式请求信号（连接转台串口线程）
    void reqTimesync(int seconds = 0);
    void reqExsend_1s(const sendExGuideData &frame1, const sendExGuideData &frame2);
    void reqExsend_5ms(double angle1, double angle2);

public slots:
    void onOpenPort(const QString &portName, int baudRate);
    void onClosePort();
    void onScanPorts();

    //实现的指令有30倍焦距，1倍焦距，数字变倍开/关，背光补偿开/关，分辨率切换
    void send30XFocus();
    void send1XFocus();
    void senddigZoomopen();
    void senddigZoomclose();
    void sendBacklightopen();
    void sendBacklightclose();
    void sendResolutionchange(int index);

    void ExmodeChanged(int mode);

    void recvTargetCenter(int centerX, int centerY);  // 接收目标中心像素坐标

protected:
    void parseData(const QByteArray &rawData) override{};
    void onReadyRead() override{};
private:
    void updateFovLimits();  // 焦距切换时同步更新滤波器限幅

    const QByteArray cmd_30X = QByteArray::fromHex("81010983140001020CFF");
    const QByteArray cmd_1X = QByteArray::fromHex("81010983140000000AFF");
    const QByteArray cmd_digZoomopen = QByteArray::fromHex("8101040602FF");
    const QByteArray cmd_digZoomclose = QByteArray::fromHex("8101040603FF");
    const QByteArray cmd_Backlightopen = QByteArray::fromHex("8101043302FF");
    const QByteArray cmd_Backlightclose = QByteArray::fromHex("8101043303FF");
    const QByteArray cmd_Resolutionchange1 = QByteArray::fromHex("81010424730007FF");  //1080P 30帧
    const QByteArray cmd_Resolutionchange2 = QByteArray::fromHex("8101042473000FFF");  //720P 30帧
    const QByteArray cmd_Resolutionchange3 = QByteArray::fromHex("81010424730008FF");  //1080P 25帧
    const QByteArray cmd_Resolutionchange4 = QByteArray::fromHex("81010424730101FF");  //720P 25帧

    // Alpha-Beta 跟踪管理器：每帧图像更新滤波，1s定时器外推预测角度
    ABTrackManager m_abMgr{SeekerType::CCD_Wide};

    // 目标中心像素坐标（由 StreamProcessor 同步）
    int m_targetCenterX = -1;
    int m_targetCenterY = -1;
    double m_azimuth = 0.0f;
    double m_pitch = 0.0f;

    // 外引导模式状态
    int exindex = -1;               // 外引导模式标志
    int exsrcindex = -1;            // 外引导源标志
    int exguidesetting = -1;        // 跟踪模式时间间隔选择
    int m_lastexguidesetting = -1;  // 记录上一次的时间间隔
    int m_sendCount_1s = 0;
    QTimer* m_exGuideTimer = nullptr;   // 外引导模式定时器

    // 图像分辨率与视场角（视场角随广角/远焦切换更新）
    // TODO:后期默认值可能需要根据CCD相机开机默认设置更改
    int   m_imageWidth  = 1920;
    int   m_imageHeight = 1080;
    float m_hFov = 55.27f;   // 横向视场角（默认广角）
    float m_vFov = 32.26f;   // 纵向视场角（默认广角）
};



#endif // SERIALPORT_CCD_H
