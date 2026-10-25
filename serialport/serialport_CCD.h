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
    Q_PROPERTY(bool videoStreamConnected READ videoStreamConnected NOTIFY videoStreamConnectedChanged)

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
    bool videoStreamConnected() const { return m_videoStreamConnected; }

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
    Q_INVOKABLE void setVideoStreamConnected(bool connected);

signals:

    // 串口状态变化信号
    void portOpenChanged();
    void availablePortsChanged();
    void errorStringChanged();

    // ── 弹窗请求信号（工作线程 → QML 弹窗提示）──
    void popupMessage(const QString &msg);

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
    void videoStreamConnectedChanged();

    // ── 请求信号（→ 排队到工作线程）──
    void requestOpenPort(const QString &portName, int baudRate);
    void requestClosePort();
    void requestScanPorts();

    // 兼容原有保留下来的指令信号，以及本次新增的 CCD 配置请求信号
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
    bool m_videoStreamConnected = false;

    // bool m_isValid = false;             // 数据有效标识
    // TODO: 补充 CCD 数据成员
};


// ── CCD 串口类（固定运行在主线程）─────────────────────────────────
class SerialPortCCD : public SerialPort
{
    Q_OBJECT
public:
    explicit SerialPortCCD(QObject *parent = nullptr);
    ~SerialPortCCD() override { }

    CCDData* ccdData() const;
    // 该指针由 main() 注入（对象归 main 的 QML 侧所有，本类只引用不负责释放），
    // 外部只需要“注入”，不需要直接读写成员，因此移到 private，仅通过 setter 赋值。
    void setCcdData(CCDData *data) { m_ccdData = data; }
signals:
    void portOpened(bool success);
    void portClosed();
    void portError(const QString &msg);
    void portsChanged(const QStringList &ports);
    // 外引导源串口未打开时，通知 QML 弹窗提示
    void exguideSerialNotOpen(const QString &msg);
    // 外引导源不可用：通知界面只取消对应外引导源
    void exguideSourceUnavailable();

    // 外引导模式请求信号（连接转台串口线程）
    void reqTimesync(int seconds = 0);
    void reqExsend_1s(sendExGuideData frame1, sendExGuideData frame2);
    void reqExsend_5ms(double angle1, double angle2);

public slots:
    void onOpenPort(const QString &portName, int baudRate);
    void onClosePort();
    void onScanPorts();

    // 原有焦距/数字变倍/背光指令，以及新增 CCD 配置指令
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

    void ExmodeChanged(int mode);
    void onVideoStreamConnectedChanged();

    void recvTargetCenter(int centerX, int centerY);  // 接收目标中心像素坐标,并进行俯仰角与方位角的计算，喂入卡尔曼滤波器

protected:
    void parseData(const QByteArray &rawData) override;
    void onReadyRead() override{};
private:
    // 主线程（QML 侧）对象指针：只存引用，不拥有，析构时不能 delete
    CCDData *m_ccdData = nullptr;
    void updateFovLimits();  // 焦距切换时同步更新滤波器限幅
    void requestZoomRatio();
    void updateOpticalZoom(double zoomRatio);
    void writeCommand(const QByteArray &command);

    const QByteArray cmd_30X = QByteArray::fromHex("81010983140001020CFF");
    const QByteArray cmd_1X = QByteArray::fromHex("81010983140000000AFF");
    const QByteArray cmd_digZoomopen = QByteArray::fromHex("8101040602FF");
    const QByteArray cmd_digZoomclose = QByteArray::fromHex("8101040603FF");
    const QByteArray cmd_Backlightopen = QByteArray::fromHex("8101043302FF");
    const QByteArray cmd_Backlightclose = QByteArray::fromHex("8101043303FF");
    // 协议格式：8x 01 04 24 73 0p 0q FF，其中 0xpq 为 VO 分辨率编号。
    const QByteArray cmd_resolution1080p30 = QByteArray::fromHex("81010424730007FF");
    const QByteArray cmd_resolution1080p25 = QByteArray::fromHex("81010424730008FF");
    const QByteArray cmd_resolution720p30  = QByteArray::fromHex("8101042473000FFF");
    const QByteArray cmd_resolution720p25  = QByteArray::fromHex("81010424730011FF");

    // Alpha-Beta 跟踪管理器：每帧图像更新滤波，1s定时器外推预测角度
    ABTrackManager m_abMgr{SeekerType::CCD_Wide};
    bool m_abNeedReset = true;       // 进入外引导后首次喂数据时复位滤波器（清陈旧角速度）

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
    double m_opticalZoom = 1.0;       // 光学倍率，1.0-30.0
    QByteArray m_zoomReplyBuffer;     // 倍率查询应答缓存

    float m_rotation_angle = 22.5f;        // CCD安装旋转角度（绕光轴旋转，单位：度）
};



#endif // SERIALPORT_CCD_H
