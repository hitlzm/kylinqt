#ifndef SERIALPORT_IMAGE_H
#define SERIALPORT_IMAGE_H

#include "serialport.h"
#include "./circularbuffer.h"
#include <QDateTime>
#include <QTimer>
#include "kalman/AlphaBetaTracker.h"

//默认视频拉流地址：rtsp://192.168.1.100:554/stream

struct image_send_frame;
//使用5ms模式进行外引导
struct imageExGuideData
{
    double pitch;  //俯仰角
    double azimuth;  //方位角
    uint32_t time;
};

class ImageData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int frameLength READ frameLength NOTIFY frameLengthChanged)
    Q_PROPERTY(int bFrameSequence READ bFrameSequence NOTIFY bFrameSequenceChanged)
    Q_PROPERTY(int aFrameSequenceReply READ aFrameSequenceReply NOTIFY aFrameSequenceReplyChanged)
    Q_PROPERTY(int aFrameValidFlag READ aFrameValidFlag NOTIFY aFrameValidFlagChanged)
    Q_PROPERTY(int seekerCtrlReply READ seekerCtrlReply NOTIFY seekerCtrlReplyChanged)
    Q_PROPERTY(int opticalParamReply READ opticalParamReply NOTIFY opticalParamReplyChanged)
    Q_PROPERTY(int currentWorkChannel READ currentWorkChannel NOTIFY currentWorkChannelChanged)
    Q_PROPERTY(int selfCheckFlag READ selfCheckFlag NOTIFY selfCheckFlagChanged)
    Q_PROPERTY(int selfCheckFlag1 READ selfCheckFlag1 NOTIFY selfCheckFlag1Changed)
    Q_PROPERTY(int selfCheckFlag2 READ selfCheckFlag2 NOTIFY selfCheckFlag2Changed)
    Q_PROPERTY(int selfCheckFlag3 READ selfCheckFlag3 NOTIFY selfCheckFlag3Changed)
    Q_PROPERTY(int selfCheckFlag4 READ selfCheckFlag4 NOTIFY selfCheckFlag4Changed)
    Q_PROPERTY(int selfCheckFlag5 READ selfCheckFlag5 NOTIFY selfCheckFlag5Changed)
    Q_PROPERTY(int selfCheckFlag6 READ selfCheckFlag6 NOTIFY selfCheckFlag6Changed)
    Q_PROPERTY(float pitchLosAngVel READ pitchLosAngVel NOTIFY pitchLosAngVelChanged)
    Q_PROPERTY(float yawLosAngVel READ yawLosAngVel NOTIFY yawLosAngVelChanged)
    Q_PROPERTY(int targetBackgroundType READ targetBackgroundType NOTIFY targetBackgroundTypeChanged)
    Q_PROPERTY(int targetBackgroundType1 READ targetBackgroundType1 NOTIFY targetBackgroundType1Changed)
    Q_PROPERTY(int targetBackgroundType2 READ targetBackgroundType2 NOTIFY targetBackgroundType2Changed)
    Q_PROPERTY(int targetBackgroundType3 READ targetBackgroundType3 NOTIFY targetBackgroundType3Changed)
    Q_PROPERTY(int targetBackgroundType4 READ targetBackgroundType4 NOTIFY targetBackgroundType4Changed)
    Q_PROPERTY(int opticalWorkState READ opticalWorkState NOTIFY opticalWorkStateChanged)
    Q_PROPERTY(float pitchFrameAngle READ pitchFrameAngle NOTIFY pitchFrameAngleChanged)
    Q_PROPERTY(float yawFrameAngle READ yawFrameAngle NOTIFY yawFrameAngleChanged)
    Q_PROPERTY(int correctionCmdStatus READ correctionCmdStatus NOTIFY correctionCmdStatusChanged)
    Q_PROPERTY(int correctionCmdCount READ correctionCmdCount NOTIFY correctionCmdCountChanged)
    Q_PROPERTY(float pitchGyro READ pitchGyro NOTIFY pitchGyroChanged)
    Q_PROPERTY(float yawGyro READ yawGyro NOTIFY yawGyroChanged)
    Q_PROPERTY(int trackingState READ trackingState NOTIFY trackingStateChanged)
    Q_PROPERTY(int trackerState READ trackerState NOTIFY trackerStateChanged)
    Q_PROPERTY(int azimuthDeviationPixel READ azimuthDeviationPixel NOTIFY azimuthDeviationPixelChanged)
    Q_PROPERTY(int pitchDeviationPixel READ pitchDeviationPixel NOTIFY pitchDeviationPixelChanged)
    Q_PROPERTY(float azimuthGyroOutput READ azimuthGyroOutput NOTIFY azimuthGyroOutputChanged)
    Q_PROPERTY(float pitchGyroOutput READ pitchGyroOutput NOTIFY pitchGyroOutputChanged)
    Q_PROPERTY(float azimuthMasterCmd READ azimuthMasterCmd NOTIFY azimuthMasterCmdChanged)
    Q_PROPERTY(float pitchMasterCmd READ pitchMasterCmd NOTIFY pitchMasterCmdChanged)
    Q_PROPERTY(int platformSelfCheck READ platformSelfCheck NOTIFY platformSelfCheckChanged)
    Q_PROPERTY(int servoRunningTime READ servoRunningTime NOTIFY servoRunningTimeChanged)
    Q_PROPERTY(int servoStep READ servoStep NOTIFY servoStepChanged)
    Q_PROPERTY(int infraredFrameNum READ infraredFrameNum NOTIFY infraredFrameNumChanged)
    Q_PROPERTY(int temperture READ temperture NOTIFY tempertureChanged)
    Q_PROPERTY(int cbhTv4405 READ cbhTv4405 NOTIFY cbhTv4405Changed)
    Q_PROPERTY(int infraredFrameRate READ infraredFrameRate NOTIFY infraredFrameRateChanged)
    Q_PROPERTY(int tvFrameRate READ tvFrameRate NOTIFY tvFrameRateChanged)
    Q_PROPERTY(int gateSize READ gateSize NOTIFY gateSizeChanged)
    Q_PROPERTY(int softwareVersion1 READ softwareVersion1 NOTIFY softwareVersion1Changed)
    Q_PROPERTY(int softwareVersion2 READ softwareVersion2 NOTIFY softwareVersion2Changed)
    Q_PROPERTY(int softwareVersion3 READ softwareVersion3 NOTIFY softwareVersion3Changed)
    // 接收帧携带的返回时间戳：34-41字节为毫秒部分，42-43字节为微秒部分
    Q_PROPERTY(qlonglong msTime READ msTime NOTIFY msTimeChanged)
    Q_PROPERTY(int usTime READ usTime NOTIFY usTimeChanged)
    Q_PROPERTY(qlonglong timeStampUs READ timeStampUs NOTIFY timeStampUsChanged)
    // 返回时间换算结果：recvDateTime 为 UTC 时刻，
    // recvTimeText 为北京时间“年月日时分秒.毫秒微秒”（秒的小数部分6位，微秒精度）
    Q_PROPERTY(QDateTime recvDateTime READ recvDateTime NOTIFY recvDateTimeChanged)
    Q_PROPERTY(QString recvTimeText READ recvTimeText NOTIFY recvTimeTextChanged)

public:
    explicit ImageData(QObject *parent = nullptr);

    // 数据属性
    int frameLength() const;
    int bFrameSequence() const;
    int aFrameSequenceReply() const;
    int aFrameValidFlag() const;
    int seekerCtrlReply() const;
    int opticalParamReply() const;
    int currentWorkChannel() const;
    int selfCheckFlag() const;
    int selfCheckFlag1() const;
    int selfCheckFlag2() const;
    int selfCheckFlag3() const;
    int selfCheckFlag4() const;
    int selfCheckFlag5() const;
    int selfCheckFlag6() const;
    float pitchLosAngVel() const;
    float yawLosAngVel() const;
    int targetBackgroundType() const;
    int targetBackgroundType1() const;
    int targetBackgroundType2() const;
    int targetBackgroundType3() const;
    int targetBackgroundType4() const;
    int opticalWorkState() const;
    float pitchFrameAngle() const;
    float yawFrameAngle() const;
    int correctionCmdStatus() const;
    int correctionCmdCount() const;
    float pitchGyro() const;
    float yawGyro() const;
    int trackingState() const;
    int trackerState() const;
    int azimuthDeviationPixel() const;
    int pitchDeviationPixel() const;
    float azimuthGyroOutput() const;
    float pitchGyroOutput() const;
    float azimuthMasterCmd() const;
    float pitchMasterCmd() const;
    int platformSelfCheck() const;
    int servoRunningTime() const;
    int servoStep() const;
    int infraredFrameNum() const;
    int temperture() const;
    int cbhTv4405() const;
    int infraredFrameRate() const;
    int tvFrameRate() const;
    int gateSize() const;
    int softwareVersion1() const;
    int softwareVersion2() const;
    int softwareVersion3() const;
    qint64 msTime() const;
    int usTime() const;
    qint64 timeStampUs() const;
    QDateTime recvDateTime() const;
    QString recvTimeText() const;


    // ── 串口状态属性（主线程，QML 直接读取）──
    Q_PROPERTY(bool portOpen READ portOpen NOTIFY portOpenChanged)
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

    bool portOpen() const { return m_portOpen; }
    QStringList availablePorts() const { return m_availablePorts; }
    QString errorString() const { return m_errorString; }

    // ── QML 可调用方法 ──
    Q_INVOKABLE void openPort(const QString &portName, int baudRate);
    Q_INVOKABLE void closePort();
    Q_INVOKABLE void scanPorts();
    // Q_INVOKABLE void sendData(const QByteArray &data);

signals:
    // 数据变化信号
    void frameLengthChanged();
    void bFrameSequenceChanged();
    void aFrameSequenceReplyChanged();
    void aFrameValidFlagChanged();
    void seekerCtrlReplyChanged();
    void opticalParamReplyChanged();
    void currentWorkChannelChanged();
    void selfCheckFlagChanged();
    void selfCheckFlag1Changed();
    void selfCheckFlag2Changed();
    void selfCheckFlag3Changed();
    void selfCheckFlag4Changed();
    void selfCheckFlag5Changed();
    void selfCheckFlag6Changed();
    void pitchLosAngVelChanged();
    void yawLosAngVelChanged();
    void targetBackgroundTypeChanged();
    void targetBackgroundType1Changed();
    void targetBackgroundType2Changed();
    void targetBackgroundType3Changed();
    void targetBackgroundType4Changed();
    void opticalWorkStateChanged();
    void pitchFrameAngleChanged();
    void yawFrameAngleChanged();
    void correctionCmdStatusChanged();
    void correctionCmdCountChanged();
    void pitchGyroChanged();
    void yawGyroChanged();
    void trackingStateChanged();
    void trackerStateChanged();
    void azimuthDeviationPixelChanged();
    void pitchDeviationPixelChanged();
    void azimuthGyroOutputChanged();
    void pitchGyroOutputChanged();
    void azimuthMasterCmdChanged();
    void pitchMasterCmdChanged();
    void platformSelfCheckChanged();
    void servoRunningTimeChanged();
    void servoStepChanged();
    void infraredFrameNumChanged();
    void tempertureChanged();
    void cbhTv4405Changed();
    void infraredFrameRateChanged();
    void tvFrameRateChanged();
    void gateSizeChanged();
    void softwareVersion1Changed();
    void softwareVersion2Changed();
    void softwareVersion3Changed();
    void msTimeChanged();
    void usTimeChanged();
    void timeStampUsChanged();
    void recvDateTimeChanged();
    void recvTimeTextChanged();

    // ── 串口状态变化信号 ──
    void portOpenChanged();
    void availablePortsChanged();
    void errorStringChanged();

    // ── 弹窗请求信号（工作线程 → QML 弹窗提示）──
    void popupMessage(const QString &msg);

    // ── 请求信号（→ 排队到工作线程）──
    void requestOpenPort(const QString &portName, int baudRate);
    void requestClosePort();
    void requestScanPorts();
    // void requestSendData(const QByteArray &data);

public slots:
    // ── 工作线程回推状态（QueuedConnection）──
    void setPortOpen(bool open);
    void setPortList(const QStringList &ports);
    void setError(const QString &msg);
    void updateFromFrame(const QByteArray &frame);

private:
    inline float fromRawValue_a(qint16 raw)
    {
    return static_cast<float>(raw)
            * 0.01f;
    }
    inline float fromRawValue_b(qint16 raw)
    {
    return static_cast<float>(raw)
            * 0.002f;
    }
    //int转qint8
    qint8 intToQint8Saturated(int value)
    {
    if (value > 127) return 127;
    if (value < -128) return -128;
    return static_cast<qint8>(value);
    }
    //取出一个字节对应位置的函数
    int getBitsFromQint8(qint8 value, int startBit, int endBit);
    //做转换并取出对应位
    int convertAndGetBit(int value, int startPos,int endPos)
    {
    qint8 converted = intToQint8Saturated(value);
    return getBitsFromQint8(converted, startPos,endPos);
    }

    int m_frameLength = 0;
    int m_bFrameSequence = 0;
    int m_aFrameSequenceReply = 0;
    int m_aFrameValidFlag = 0;
    int m_seekerCtrlReply = 0;
    int m_opticalParamReply = 0;
    int m_currentWorkChannel = 0;

    int m_selfCheckFlag = 0;  //细分为很多种，按位解析接收
    int m_selfCheckFlag1 = 0;
    int m_selfCheckFlag2 = 0;
    int m_selfCheckFlag3 = 0;
    int m_selfCheckFlag4 = 0;
    int m_selfCheckFlag5 = 0;
    int m_selfCheckFlag6 = 0;

    float m_pitchLosAngVel = 0;
    float m_yawLosAngVel = 0;

    int m_targetBackgroundType = 0;//细分为很多种，按位解析接收
    int m_targetBackgroundType1 = 0;
    int m_targetBackgroundType2 = 0;
    int m_targetBackgroundType3 = 0;
    int m_targetBackgroundType4 = 0;

    int m_opticalWorkState = 0;
    float m_pitchFrameAngle = 0;
    float m_yawFrameAngle = 0;
    int m_correctionCmdStatus = 0;
    int m_correctionCmdCount = 0;
    float m_pitchGyro = 0;
    float m_yawGyro = 0;
    int m_trackingState = 0;
    int m_trackerState = 0;
    int m_azimuthDeviationPixel = 0;
    int m_pitchDeviationPixel = 0;
    float m_azimuthGyroOutput = 0;
    float m_pitchGyroOutput = 0;
    float m_azimuthMasterCmd = 0;
    float m_pitchMasterCmd = 0;
    int m_platformSelfCheck = 0;
    int m_servoRunningTime = 0;
    int m_servoStep = 0;
    quint32 m_infraredFrameNum = 0;  //红外帧编号
    int m_temperture = 0;       //温度
    quint32 m_cbhTv4405 = 0;   //电视帧编号
    int m_infraredFrameRate = 0;
    int m_tvFrameRate = 0;
    int m_gateSize = 0;
    int m_softwareVersion1 = 0;
    int m_softwareVersion2 = 0;
    int m_softwareVersion3 = 0;
    // 接收帧时间戳：毫秒部分(qint64) + 微秒部分(0~999)，timeStampUs 为两者合并后的微秒时间戳
    qint64 m_msTime = 0;
    int m_usTime = 0;
    qint64 m_timeStampUs = 0;
    QDateTime m_recvDateTime;      // 返回时间(UTC)
    QString m_recvTimeText;        // 返回时间(北京时间，年月日时分秒.毫秒微秒，微秒精度)

    bool m_portOpen = false;
    QStringList m_availablePorts;
    QString m_errorString;
    //加入是否判断发送帧有效的标志位与计数位，只有发送数据时，才将标志位置1并检查数据是否发送成功
    bool m_check = false;
    int m_check_count = 20;
};

class ImageSendData : public QObject
{
    Q_OBJECT

    // ── QML 接口：只暴露界面上真实读写的装订参数 ──
    // 其余发送参数（弹体运动量、载机/吊舱/目标地理量、预留量等）界面上没有控件，
    // 保持普通 C++ 成员即可，由 C++ 赋值后经 buildFrame()/buildDeviation() 装帧，
    // 不需要经过元对象系统暴露给 QML。
    Q_PROPERTY(int m_seekerCtrlWord MEMBER m_seekerCtrlWord NOTIFY seekerCtrlWordChanged)
    Q_PROPERTY(int m_opticalParamCtrl MEMBER m_opticalParamCtrl NOTIFY opticalParamCtrlChanged)
    Q_PROPERTY(int m_templateIndex MEMBER m_templateIndex NOTIFY templateIndexChanged)
    Q_PROPERTY(int m_targetBackgroundType1 MEMBER m_targetBackgroundType1 NOTIFY targetBackgroundType1Changed)
    Q_PROPERTY(int m_targetBackgroundType2 MEMBER m_targetBackgroundType2 NOTIFY targetBackgroundType2Changed)
    Q_PROPERTY(int m_targetBackgroundType3 MEMBER m_targetBackgroundType3 NOTIFY targetBackgroundType3Changed)
    Q_PROPERTY(int m_targetBackgroundType4 MEMBER m_targetBackgroundType4 NOTIFY targetBackgroundType4Changed)
    Q_PROPERTY(float m_pitchGimbalPreset MEMBER m_pitchGimbalPreset NOTIFY pitchGimbalPresetChanged)
    Q_PROPERTY(float m_yawGimbalPreset MEMBER m_yawGimbalPreset NOTIFY yawGimbalPresetChanged)
    Q_PROPERTY(int m_trackingCorrectionCmd MEMBER m_trackingCorrectionCmd NOTIFY trackingCorrectionCmdChanged)
    Q_PROPERTY(float m_searchPitchRate MEMBER m_searchPitchRate NOTIFY searchPitchRateChanged)
    Q_PROPERTY(float m_searchYawRate MEMBER m_searchYawRate NOTIFY searchYawRateChanged)
    Q_PROPERTY(int m_gateSize MEMBER m_gateSize NOTIFY gateSizeChanged)
    Q_PROPERTY(int m_captureRefImgCmd MEMBER m_captureRefImgCmd NOTIFY captureRefImgCmdChanged)

public:
    explicit ImageSendData(QObject *parent = nullptr);

    Q_INVOKABLE void buildFrame() ;

    // 发送一次时间同步帧：m_timeSync 置1 → buildFrame() → 再置0
    // 由内部10分钟定时器周期调用，也可从 QML 手动触发
    Q_INVOKABLE void sendTimeSyncFrame();

    int templateIndex() const { return m_templateIndex; }

    // ── 偏差像素桥接：QML → ImageSendData → SerialPortImage ──
    Q_INVOKABLE void relayDeviationPixel(int x, int y) { emit deviationPixelRelayed(x, y); }

signals:
    // ── 只保留 QML 需要绑定/读回的参数变更信号（与上面的 Q_PROPERTY 一一对应）──
    void seekerCtrlWordChanged();
    void opticalParamCtrlChanged();
    void templateIndexChanged();
    void targetBackgroundType1Changed();
    void targetBackgroundType2Changed();
    void targetBackgroundType3Changed();
    void targetBackgroundType4Changed();
    void pitchGimbalPresetChanged();
    void yawGimbalPresetChanged();
    void trackingCorrectionCmdChanged();
    void searchPitchRateChanged();
    void searchYawRateChanged();
    void gateSizeChanged();
    void captureRefImgCmdChanged();

    void requestSendData(image_send_frame frame);
    void deviationPixelRelayed(int x, int y);

public slots:
    //用于构建偏差像素发送帧
    void buildDeviation(int num ,int x ,int y);
    
private:
    //把显示数据转化为串口原始数据
    inline qint16 toRawValue_a(float value) const
    {
    return static_cast<qint16>(
                qRound(value / 0.1f));
    }

    inline qint16 toRawValue_b(float value) const
    {
    return static_cast<qint16>(
                qRound(value / 0.01f));
    }
    inline qint16 toRawValue_c(float value) const
    {
    return static_cast<qint16>(
                qRound(value / 0.002f));
    }
    int m_frameLength = 220;
    int m_aFrameSequence = 0;
    int m_seekerCtrlWord = 0;
    int m_opticalParamCtrl = 0;
    int m_templateIndex = 0;  //模板序号变成了通道类型，选择电视通道时发送0，选择红外通道时发送1
    // int m_targetBackgroundType = 0;//细分为很多种
    int m_targetBackgroundType1 = 0;
    int m_targetBackgroundType2 = 0;
    int m_targetBackgroundType3 = 0;
    int m_targetBackgroundType4 = 0;
    int m_missileTargetDistance = 0;
    float m_missileSpeed = 0;
    float m_bodyPitchAngle = 0;
    float m_bodyYawAngle = 0;
    float m_bodyRollAngle = 0;
    float m_bodyPitchRate = 0;
    float m_bodyYawRate = 0;
    float m_bodyRollRate = 0;
    float m_bodyVelX = 0;
    float m_bodyVelY = 0;
    float m_bodyVelZ = 0;
    int m_bodyPosX = 0;
    int m_bodyPosY = 0;
    int m_bodyPosZ = 0;
    float m_pitchGimbalPreset = 0;
    float m_yawGimbalPreset = 0;
    int m_irIntegrationTime = 0;
    int m_trackingCorrectionCmd = 0;//只关注第一位即可
    int m_correctionFrameNum = 0;
    int m_correctedPitchPos = 0;
    int m_correctedYawPos = 0;
    float m_searchPitchRate = 0;
    float m_searchYawRate = 0;
    int m_gateSize = 0;
    // int m_osdSwitch = 0;    //字符叠加指令消失，后面的61-152字节为预留区
    int m_captureRefImgCmd = 0;
    int m_timeSync = 0;     //字节61：时间同步信号（0=不发时间同步，可由QML置位）
    int m_time = 0;

    QTimer *m_timeSyncTimer = nullptr;   // 每10分钟自动发一次时间同步帧
    
    int m_targetAltitude = 0;
    float m_aircraftPitch = 0;
    float m_aircraftYaw = 0;
    float m_aircraftRoll = 0;
    int m_focalLength = 0;
    float m_podPitchAngle = 0;
    float m_podYawAngle = 0;
    int m_satelliteMapScale = 0;
    int m_podType = 0;
    double m_targetLongitude = 0;
    double m_targetLatitude = 0;
    double m_aircraftLongitude = 0;
    double m_aircraftLatitude = 0;
    int m_aircraftAltitude = 0;
    int m_pixelSize = 0;
};

class SerialPortImage : public SerialPort
{
    Q_OBJECT
public:
    explicit SerialPortImage(QObject *parent = nullptr);
    ~SerialPortImage() override;

    ImageData* imageData() const;
    ImageSendData* imageSendData() const;
    ImageData *m_imageData;
    ImageSendData *m_imageSendData;

public slots:
    void dowork() { SerialPort::dowork(); onScanPorts(); }

    static void init_crc16_table(uint16_t poly = 0x1021);
    static uint16_t crc16_ccitt_fast(const uint8_t *data, size_t len, uint16_t init = 0x0000);

signals:
    void portOpened(bool success);
    void portClosed();
    void portError(const QString &msg);
    void portsChanged(const QStringList &ports);
    void imageFrameReceived(const QByteArray &rawData);
    // 外引导源串口未打开时，通知 QML 弹窗提示
    void exguideSerialNotOpen(const QString &msg);
    void reqTimesync(int seconds);
    void reqExsend_1s(sendExGuideData frame1 , sendExGuideData frame2 );
    void reqExsend_5ms(double angle1 ,double angle2 );

    void reqSendDeviationPixel(int num ,int x , int y);
public slots:
    void onOpenPort(const QString &portName, int baudRate);
    void onClosePort();
    void onScanPorts();
    void onSendData(image_send_frame frame);
    void ExmodeChanged(int mode);

    
    void recvDeviationPixel(int x ,int y){
        //后期修改：可以判断串口是否打开，未打开时弹窗提示：未打开图像串口，无法发送偏差像素
        //获取图像帧序号 or电视帧序号
        if(m_imageSendData->templateIndex() == 0)
        {
            //读取电视帧序号
            Cbh_tv = m_imageData->cbhTv4405();
            reqSendDeviationPixel(Cbh_tv,x,y);
        }else{
            //读取红外帧序号
            Infrared_num = m_imageData->infraredFrameNum();
            reqSendDeviationPixel(Infrared_num,x,y);
        }
        // x与y为偏差像素信息
    };

protected:
    void parseData(const QByteArray &rawData) override;
    void onReadyRead() override;

private:
    static uint16_t crc16_table[256];
    //图像导引头每20ms接收一次数据，1s共50组数据，预留200个位置
    CircularBuffer<imageExGuideData> m_circularbuf;
    // Alpha-Beta 跟踪管理器：每20ms更新滤波，1s定时器外推预测角度
    ABTrackManager m_abMgr{SeekerType::Image};
    qint64 m_filterTime = 0;         // 虚拟时间戳(ms)，每20ms+20
    bool   m_abNeedReset = true;     // 进入外引导后首次喂数据时复位滤波器（清陈旧角速度）

    double m_azimuth = 0.0f;
    double m_pitch = 0.0f;    //存储图像导引头的方位角与俯仰角

    int exindex = -1;         //外引模式判断
    int exsrcindex = -1;      //外引导源标志
    int exguidesetting = -1;  //跟踪模式时间间隔选择
    int m_lastexguidesetting = -1;  //记录上一次的时间间隔
    // int m_sendCount_5ms = 0;
    int m_sendCount_1s = 0;
    QTime m_startTime = {};
    QDateTime m_dateTime = {};
    int m_startvalue =0 ;
    
    QTimer* m_exGuideTimer = nullptr;   // 外引导模式定时器 ，和串口发送定时器不同
    int Cbh_tv; //电视帧编号
    int Infrared_num; //红外帧编号
    QByteArray m_rxBuffer;   // 串口接收缓冲：readyRead 到达的数据可能只是一帧的一部分，攒到完整帧头+固定帧长再切出交给 parseData
};


//图像导引头接收数据结构体
#pragma pack(push,1)
typedef struct {
    // 字节0-1: 帧头
    quint8 frame_header1;       // 0x77
    quint8 frame_header2;       // 0xAB

    // 字节2: 帧长（除去帧头和校验位的字节数）
    quint8 frame_length;

    // 字节3-4: B帧流水号
    quint16 b_frame_sequence;

    // 字节5-6: A帧流水号回告
    quint16 a_frame_sequence_reply;

    // 字节7: A帧有效标志 (0xAA=收到新帧, 0x00=未收到)
    quint8 a_frame_valid_flag;

    // 字节8: 导引头控制字回告
    quint8 seeker_ctrl_reply;

    // 字节9: 光学参数装订回告
    quint8 optical_param_reply;

    // 字节10: 预留1
    quint8 reserved1;

    // 字节11: 当前工作通道 (0x01=红外, 0x00=电视)
    quint8 current_work_channel;

    // 字节12: 自检标志 (位域)
    quint8 self_check_flag;

    // 字节13-14: 俯仰视线角速度 (单位:°/s, 精度0.002)
    qint16 pitch_line_of_sight_ang_vel;

    // 字节15-16: 偏航视线角速度
    qint16 yaw_line_of_sight_ang_vel;

    // 字节17: 目标/背景类型预装回告
    quint8 target_background_type;

    // 字节18: 光学工作状态
    quint8 optical_work_state;

    // 字节19-20: 俯仰框架角
    qint16 pitch_frame_angle;

    // 字节21-22: 偏航框架角
    qint16 yaw_frame_angle;

    //字节23-25预留三字节
    quint8 reserved3[3];
    // 字节26: 修正指令状态回告 (bit0)
    quint8 correction_cmd_status;
    // 字节27: 修正指令次数回告
    quint8 correction_cmd_count;
    // 字节28-29: 俯仰陀螺
    qint16 pitch_gyro;

    // 字节30-31: 偏航陀螺
    qint16 yaw_gyro;

    // 字节32: 跟踪状态
    quint8 tracking_state;

    // 字节33: 跟踪器状态
    quint8 tracker_state;

    // 字节34-52: 预留19字节
    // 现在34-41字节为返回时间戳的毫秒部分，42-43为返回时间戳的微秒部分。44-52为新预留的字节
    quint8 ms_time[8];
    quint16 us_time;
    //44-52 预留9个字节
    quint8 reserved4[9];

    // 字节53-54: 方位偏差像素
    qint16 azimuth_deviation_pixel;

    // 字节55-56: 俯仰偏差像素
    qint16 pitch_deviation_pixel;

    // 字节57-58: 方位轴陀螺输出 (LSB=0.01°/s)
    qint16 azimuth_gyro_output;

    // 字节59-60: 俯仰轴陀螺输出
    qint16 pitch_gyro_output;

    // 字节61-62: 方位主令
    qint16 azimuth_master_cmd;

    // 字节63-64: 俯仰主令
    qint16 pitch_master_cmd;

    // 字节65-72: 预留8字节
    quint8 reserved5[8];

    // 字节73: 平台自检结果
    quint8 platform_self_check;

    // -----------------------------------------------------

    // 字节74-81: 预留8字节
    // 字节74-82: 预留9字节
    quint8 reserved6[9];

    // 字节82: 伺服运行时间
    //现在字节83是伺服运行时间
    qint8 servo_running_time;

    // 字节83-89: 预留7字节
    // 字节84-89：预留6字节
    quint8 reserved7[6];

    // -----------------------------------------------------

    // 字节90: 伺服阶跃
    quint8 servo_step;

    // 字节91-94: 红外帧编号
    quint32 infrared_frame_num;

    // 字节95-100: 预留6字节
    // 字节95-96：温度
    qint16 temperture;
    // 97-100 4字节
    quint8 reserved8[4];

    // 字节101-104: Cbh_tv4405 (上位机新增) ,帧编号改成四位，不是两位，改成qint16
    quint32 cbh_tv4405;

    // 字节103-104: 未定义，作为预留。去掉此预留
    // quint8 reserved9[2];

    // 字节105-110: 预留6字节
    quint8 reserved10[6];

    // 字节111: 红外帧频
    quint8 infrared_frame_rate;

    // 字节112: 电视帧频
    quint8 tv_frame_rate;

    // 字节113-116: 预留4字节
    quint8 reserved11[4];

    // 字节117-118: 波门尺寸
    quint16 gate_size;

    // 字节119-130: 预留12字节
    quint8 reserved12[12];

    // 字节131: 软件1版本号
    quint8 software_version1;

    // 字节132: 软件2版本号
    quint8 software_version2;

    // 字节133: 软件3版本号
    quint8 software_version3;

    // 字节134-153: 预留20字节
    quint8 reserved13[20];

    // 字节154-155: CRC16校验
    quint16 crc16;
} image_recv_frame;
#pragma pack(pop)

//图像导引头发送数据结构体
#pragma pack(push,1)
struct image_send_frame {
    // 定义发送数据的字段
    // 字节0-1: 帧头
    quint8 frame_header1;       // 0x77
    quint8 frame_header2;       // 0xAA

    // 字节2: A帧帧长（除去帧头和校验位的字节数）
    quint8 frame_length;

    // 字节3-4: A帧流水号 (0~65535)
    quint16 a_frame_sequence;

    // 字节5: 导引头控制字 (见文档说明)
    quint8 seeker_ctrl_word;

    // 字节6: 光学参数装订控制字
    quint8 optical_param_ctrl;

    // 字节7: 模板序号 (1-10)
    quint8 template_index;

    // 字节8: 目标/背景类型预装 (位域)
    quint8 target_background_type;

    // 字节9-10: 弹目距离 (单位: m, 精度1, 范围20~20000)
    quint16 missile_target_distance;

    // 字节11-12: 导弹速度 (单位: m/s, 精度0.1, 范围0~500)
    quint16 missile_speed;      // 实际值 = 存储值 * 0.1

    // 字节13-14: 弹体俯仰角 (单位: °, 精度0.01, 范围-90~+90)
    qint16 body_pitch_angle;

    // 字节15-16: 弹体方位角 (单位: °, 精度0.01, 范围-180~+180)
    qint16 body_yaw_angle;

    // 字节17-18: 弹体滚转角 (单位: °, 精度0.01, 范围-180~+180)
    qint16 body_roll_angle;

    // 字节19-20: 弹体俯仰角速度 (单位: °/s, 精度0.01, 范围-300~+300)
    qint16 body_pitch_rate;

    // 字节21-22: 弹体方位角速度 (单位: °/s, 精度0.01, 范围-300~+300)
    qint16 body_yaw_rate;

    // 字节23-24: 弹体滚转角速度 (单位: °/s, 精度0.01, 范围-300~+300)
    qint16 body_roll_rate;

    // 字节25-26: 弹体实时X向速度 (目标系, 单位: m/s, 精度0.1, 范围-500~+500)
    qint16 body_vel_x;

    // 字节27-28: 弹体实时Y向速度 (目标系)
    qint16 body_vel_y;

    // 字节29-30: 弹体实时Z向速度 (目标系)
    qint16 body_vel_z;

    // 字节31-32: 弹体实时X坐标 (目标系, 单位: m, 精度1, 范围-10000~+10000)
    qint16 body_pos_x;

    // 字节33-34: 弹体实时Y坐标
    qint16 body_pos_y;

    // 字节35-36: 弹体实时Z坐标
    qint16 body_pos_z;

    // 字节37-38: 俯仰框架角预装 (单位: °, 精度0.002, 范围-18~+18)
    qint16 pitch_gimbal_preset;

    // 字节39-40: 偏航框架角预装 (单位: °, 精度0.002, 范围-18~+18)
    qint16 yaw_gimbal_preset;

    // 字节41: 红外积分时间 (单位: ms, 精度0.1, 范围0.5~19.5)
    quint8 ir_integration_time; // 实际值 = 存储值 * 0.1

    // 字节42-45: 预留4字节
    quint8 reserved1[4];

    // 字节46: 跟踪位置修正指令 (Bit0: 0-非修正, 1-修正)
    quint8 tracking_correction_cmd;

    // 字节47-49: 修正图像帧序号 (24位, 低字节在前)
    quint8 correction_frame_num[3]; // 小端序

    // 字节50-51: 修正后俯仰跟踪位置 (0~511像素)
    quint16 corrected_pitch_pos;

    // 字节52-53: 修正后偏航跟踪位置 (0~639像素)
    quint16 corrected_yaw_pos;

    // 字节54-55: 搜索俯仰角速度 (单位: °/s, 精度0.01)
    qint16 search_pitch_rate;

    // 字节56-57: 搜索方位角速度 (单位: °/s, 精度0.01)
    qint16 search_yaw_rate;

    // 字节58: 预留1字节,现在为红外待机指令
    quint8 reserved2;

    // 字节59: 波门 (0xAA=大, 0x55=小)
    quint8 gate_size;

    // 字节60: 字符叠加 (0xAA=开, 0x55=关)
    //quint8 osd_switch;

    // 字节60: 拍摄参考图指令 (0xAA=拍摄, 发三拍后恢复默认)
    quint8 capture_ref_img_cmd;

    // 字节61-152: 预留92字节
    // 现在61字节是时间同步信号，62~69 8个字节为北京时间转成毫秒
    qint8 timeSync_sig;
    // 62~69字节
    quint8 time[8];
    // 70~152字节
    quint8 reserved3[83];       

    // 字节153-154: 目标海拔高度 (单位: m, 精度1)
    qint16 target_altitude;

    // 字节155-156: 飞机俯仰角 (单位: °, 精度0.01)
    qint16 aircraft_pitch;

    // 字节157-158: 飞机方位角 (单位: °, 精度0.01)
    qint16 aircraft_yaw;

    // 字节159-160: 飞机滚转角 (单位: °, 精度0.01)
    qint16 aircraft_roll;

    // 字节161-162: 焦距 (单位: mm, 精度1)
    quint16 focal_length;

    // 字节163-164: 吊舱俯仰框架角 (单位: °, 精度0.01)
    qint16 pod_pitch_angle;

    // 字节165-166: 吊舱方位框架角 (单位: °, 精度0.01)
    qint16 pod_yaw_angle;

    // 字节167-200: 预留34字节 (200-167+1=34)
    quint8 reserved4[34];

    // 字节201: 卫星图比例 (单像素代表实际尺寸, 精度0.1, 默认0)
    quint8 satellite_map_scale; // 实际值 = 存储值 * 0.1

    // 字节202: 吊舱类型 (待定)
    quint8 pod_type;

    // 字节203-206: 目标经度 (单位: °, 精度1e-6, 东经正西经负)
    qint32 target_longitude;    // 实际值 = 存储值 * 1e-6

    // 字节207-210: 目标纬度 (单位: °, 精度1e-6)
    qint32 target_latitude;

    // 字节211-214: 飞机经度
    qint32 aircraft_longitude;

    // 字节215-218: 飞机纬度
    qint32 aircraft_latitude;

    // 字节219-220: 飞机海拔高度 (单位: m, 精度1)
    qint16 aircraft_altitude;

    // 字节221: 像元尺寸 (单位: um, 精度1)
    quint8 pixel_size;

    // 字节222-223: CRC16校验位 (多项式X16+X12+X5+1)
    quint16 crc16;

};
#pragma pack(pop)

#endif // SERIALPORT_IMAGE_H
