#ifndef SERIALPORT_CCD_H
#define SERIALPORT_CCD_H

#include "serialport.h"
#include <QDateTime>

// ── CCD 接收数据类（主线程，QML 直接读取）────────────────────────────
class CCDData : public QObject
{
    Q_OBJECT


    // ── 串口状态属性 ──
    Q_PROPERTY(bool portOpen READ portOpen NOTIFY portOpenChanged)
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

    // ── CCD 控制状态属性 ──
    Q_PROPERTY(int focusMode READ focusMode NOTIFY focusModeChanged)           // 0=自动, 1=手动
    Q_PROPERTY(bool defogEnabled READ defogEnabled NOTIFY defogEnabledChanged)
    Q_PROPERTY(int defogLevel READ defogLevel NOTIFY defogLevelChanged)        // 1=低, 2=中, 3=高
    Q_PROPERTY(int heatHazeLevel READ heatHazeLevel NOTIFY heatHazeLevelChanged) // 0-10
    Q_PROPERTY(bool noiseReductionEnabled READ noiseReductionEnabled NOTIFY noiseReductionEnabledChanged)
    Q_PROPERTY(int noiseReductionLevel READ noiseReductionLevel NOTIFY noiseReductionLevelChanged) // 0-100
    Q_PROPERTY(bool stabilizationEnabled READ stabilizationEnabled NOTIFY stabilizationEnabledChanged)
    Q_PROPERTY(int stabilizationLevel READ stabilizationLevel NOTIFY stabilizationLevelChanged) // 1=低, 2=中, 3=高
    Q_PROPERTY(int resolutionIndex READ resolutionIndex NOTIFY resolutionIndexChanged)
    Q_PROPERTY(int whiteBalanceMode READ whiteBalanceMode NOTIFY whiteBalanceModeChanged) // 0=自动, 1=手动
    Q_PROPERTY(int redGain READ redGain NOTIFY redGainChanged)                 // 0-255
    Q_PROPERTY(int blueGain READ blueGain NOTIFY blueGainChanged)              // 0-255
    Q_PROPERTY(bool backlightOn READ backlightOn NOTIFY backlightOnChanged)

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
    bool defogEnabled() const { return m_defogEnabled; }
    int defogLevel() const { return m_defogLevel; }
    int heatHazeLevel() const { return m_heatHazeLevel; }
    bool noiseReductionEnabled() const { return m_noiseReductionEnabled; }
    int noiseReductionLevel() const { return m_noiseReductionLevel; }
    bool stabilizationEnabled() const { return m_stabilizationEnabled; }
    int stabilizationLevel() const { return m_stabilizationLevel; }
    int resolutionIndex() const { return m_resolutionIndex; }
    int whiteBalanceMode() const { return m_whiteBalanceMode; }
    int redGain() const { return m_redGain; }
    int blueGain() const { return m_blueGain; }
    bool backlightOn() const { return m_backlightOn; }

    // ── QML 可调用方法 ──
    Q_INVOKABLE void openPort(const QString &portName, int baudRate);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE void scanPorts();

    // CCD 指令触发方法（QML 可调用，发射请求信号到 CCD 串口）
    Q_INVOKABLE void startZoom(int direction);      // direction > 0 放大, < 0 缩小
    Q_INVOKABLE void stopZoom();
    Q_INVOKABLE void setFocusMode(int mode);        // 0=自动, 1=手动
    Q_INVOKABLE void startFocus(int direction);     // direction > 0 正向调节, < 0 反向调节
    Q_INVOKABLE void stopFocus();
    Q_INVOKABLE void setDefog(bool enabled);
    Q_INVOKABLE void setDefogLevel(int level);      // 1-3
    Q_INVOKABLE void setHeatHazeLevel(int level);   // 0-10
    Q_INVOKABLE void setNoiseReduction(bool enabled);
    Q_INVOKABLE void setNoiseReductionLevel(int level); // 0-100
    Q_INVOKABLE void setStabilization(bool enabled);
    Q_INVOKABLE void setStabilizationLevel(int level);  // 1-3
    Q_INVOKABLE void setResolution(int index);
    Q_INVOKABLE void setWhiteBalanceMode(int mode); // 0=自动, 1=手动
    Q_INVOKABLE void setRedGain(int gain);          // 0-255
    Q_INVOKABLE void setBlueGain(int gain);         // 0-255
    Q_INVOKABLE void setBacklight(bool on);

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
    void reqZoomMove(int direction);
    void reqZoomStop();
    void reqFocusMode(int mode);
    void reqFocusMove(int direction);
    void reqFocusStop();
    void reqDefogSetting(bool enabled, int level);
    void reqHeatHazeLevel(int level);
    void reqNoiseReductionSetting(bool enabled, int level);
    void reqStabilizationSetting(bool enabled, int level);
    void reqWhiteBalanceMode(int mode);
    void reqWhiteBalanceGain(bool red, int gain);

    // CCD 控制状态变化信号
    void focusModeChanged();
    void defogEnabledChanged();
    void defogLevelChanged();
    void heatHazeLevelChanged();
    void noiseReductionEnabledChanged();
    void noiseReductionLevelChanged();
    void stabilizationEnabledChanged();
    void stabilizationLevelChanged();
    void resolutionIndexChanged();
    void whiteBalanceModeChanged();
    void redGainChanged();
    void blueGainChanged();
    void backlightOnChanged();

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
    int m_focusMode = 0;         // 0=自动, 1=手动
    bool m_defogEnabled = false;
    int m_defogLevel = 1;
    int m_heatHazeLevel = 0;
    bool m_noiseReductionEnabled = false;
    int m_noiseReductionLevel = 50;
    bool m_stabilizationEnabled = false;
    int m_stabilizationLevel = 1;
    int m_resolutionIndex = 0;
    int m_whiteBalanceMode = 0;  // 0=自动, 1=手动
    int m_redGain = 128;
    int m_blueGain = 128;
    bool m_backlightOn = false;

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
    void setCcdData(CCDData *data) { m_ccdData = data; }
    // 初始化串口对象，并扫描可用串口，CCD串口类留在主线程
signals:
    void portOpened(bool success);
    void portClosed();
    void portError(const QString &msg);
    void portsChanged(const QStringList &ports);

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
    void sendZoomMove(int direction);
    void sendZoomStop();
    void sendFocusMode(int mode);
    void sendFocusMove(int direction);
    void sendFocusStop();
    void sendDefogSetting(bool enabled, int level);
    void sendHeatHazeLevel(int level);
    void sendNoiseReductionSetting(bool enabled, int level);
    void sendStabilizationSetting(bool enabled, int level);
    void sendWhiteBalanceMode(int mode);
    void sendWhiteBalanceGain(bool red, int gain);

protected:
    void parseData(const QByteArray &rawData) override{};
    void onReadyRead() override{};
private:
    void writeCommand(const QByteArray &command);
    const QByteArray cmd_30X = QByteArray::fromHex("81010983140001020CFF");
    const QByteArray cmd_1X = QByteArray::fromHex("81010983140000000AFF");
    const QByteArray cmd_digZoomopen = QByteArray::fromHex("8101040602FF");
    const QByteArray cmd_digZoomclose = QByteArray::fromHex("8101040603FF");
    const QByteArray cmd_Backlightopen = QByteArray::fromHex("8101043302FF");
    const QByteArray cmd_Backlightclose = QByteArray::fromHex("8101043303FF");
    const QByteArray cmd_Resolutionchange1 = QByteArray::fromHex("81010424730007FF");  //1080P 30帧
    const QByteArray cmd_Resolutionchange2 = QByteArray::fromHex("8101042473000FFF");  //720P 30帧
    const QByteArray cmd_Resolutionchange_1080p25 = QByteArray::fromHex("81010424730003FF");  //1080P 25帧 (TODO: 确认协议)
    const QByteArray cmd_Resolutionchange_720p25 = QByteArray::fromHex("8101042473000BFF");   //720P 25帧 (TODO: 确认协议)

};



#endif // SERIALPORT_CCD_H
