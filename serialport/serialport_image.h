#ifndef SERIALPORT_IMAGE_H
#define SERIALPORT_IMAGE_H

#include "serialport.h"

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
    Q_PROPERTY(float pitchLosAngVel READ pitchLosAngVel NOTIFY pitchLosAngVelChanged)
    Q_PROPERTY(float yawLosAngVel READ yawLosAngVel NOTIFY yawLosAngVelChanged)
    Q_PROPERTY(int targetBackgroundType READ targetBackgroundType NOTIFY targetBackgroundTypeChanged)
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
    Q_PROPERTY(int cbhTv4405 READ cbhTv4405 NOTIFY cbhTv4405Changed)
    Q_PROPERTY(int infraredFrameRate READ infraredFrameRate NOTIFY infraredFrameRateChanged)
    Q_PROPERTY(int tvFrameRate READ tvFrameRate NOTIFY tvFrameRateChanged)
    Q_PROPERTY(int gateSize READ gateSize NOTIFY gateSizeChanged)
    Q_PROPERTY(int softwareVersion1 READ softwareVersion1 NOTIFY softwareVersion1Changed)
    Q_PROPERTY(int softwareVersion2 READ softwareVersion2 NOTIFY softwareVersion2Changed)
    Q_PROPERTY(int softwareVersion3 READ softwareVersion3 NOTIFY softwareVersion3Changed)

public:
    explicit ImageData(QObject *parent = nullptr);

    int frameLength() const;
    int bFrameSequence() const;
    int aFrameSequenceReply() const;
    int aFrameValidFlag() const;
    int seekerCtrlReply() const;
    int opticalParamReply() const;
    int currentWorkChannel() const;
    int selfCheckFlag() const;
    float pitchLosAngVel() const;
    float yawLosAngVel() const;
    int targetBackgroundType() const;
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
    int cbhTv4405() const;
    int infraredFrameRate() const;
    int tvFrameRate() const;
    int gateSize() const;
    int softwareVersion1() const;
    int softwareVersion2() const;
    int softwareVersion3() const;

    void updateFromFrame(const QByteArray &frame);

signals:
    void frameLengthChanged();
    void bFrameSequenceChanged();
    void aFrameSequenceReplyChanged();
    void aFrameValidFlagChanged();
    void seekerCtrlReplyChanged();
    void opticalParamReplyChanged();
    void currentWorkChannelChanged();
    void selfCheckFlagChanged();
    void pitchLosAngVelChanged();
    void yawLosAngVelChanged();
    void targetBackgroundTypeChanged();
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
    void cbhTv4405Changed();
    void infraredFrameRateChanged();
    void tvFrameRateChanged();
    void gateSizeChanged();
    void softwareVersion1Changed();
    void softwareVersion2Changed();
    void softwareVersion3Changed();

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
    int m_frameLength = 0;
    int m_bFrameSequence = 0;
    int m_aFrameSequenceReply = 0;
    int m_aFrameValidFlag = 0;
    int m_seekerCtrlReply = 0;
    int m_opticalParamReply = 0;
    int m_currentWorkChannel = 0;
    int m_selfCheckFlag = 0;
    float m_pitchLosAngVel = 0;
    float m_yawLosAngVel = 0;
    int m_targetBackgroundType = 0;
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
    int m_infraredFrameNum = 0;
    int m_cbhTv4405 = 0;
    int m_infraredFrameRate = 0;
    int m_tvFrameRate = 0;
    int m_gateSize = 0;
    int m_softwareVersion1 = 0;
    int m_softwareVersion2 = 0;
    int m_softwareVersion3 = 0;
};

class ImageSendData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int frameLength READ frameLength WRITE setFrameLength NOTIFY frameLengthChanged)
    Q_PROPERTY(int aFrameSequence READ aFrameSequence WRITE setAFrameSequence NOTIFY aFrameSequenceChanged)
    Q_PROPERTY(int seekerCtrlWord READ seekerCtrlWord WRITE setSeekerCtrlWord NOTIFY seekerCtrlWordChanged)
    Q_PROPERTY(int opticalParamCtrl READ opticalParamCtrl WRITE setOpticalParamCtrl NOTIFY opticalParamCtrlChanged)
    Q_PROPERTY(int templateIndex READ templateIndex WRITE setTemplateIndex NOTIFY templateIndexChanged)
    Q_PROPERTY(int targetBackgroundType READ targetBackgroundType WRITE setTargetBackgroundType NOTIFY targetBackgroundTypeChanged)
    Q_PROPERTY(int missileTargetDistance READ missileTargetDistance WRITE setMissileTargetDistance NOTIFY missileTargetDistanceChanged)
    Q_PROPERTY(float missileSpeed READ missileSpeed WRITE setMissileSpeed NOTIFY missileSpeedChanged)
    Q_PROPERTY(float bodyPitchAngle READ bodyPitchAngle WRITE setBodyPitchAngle NOTIFY bodyPitchAngleChanged)
    Q_PROPERTY(float bodyYawAngle READ bodyYawAngle WRITE setBodyYawAngle NOTIFY bodyYawAngleChanged)
    Q_PROPERTY(float bodyRollAngle READ bodyRollAngle WRITE setBodyRollAngle NOTIFY bodyRollAngleChanged)
    Q_PROPERTY(float bodyPitchRate READ bodyPitchRate WRITE setBodyPitchRate NOTIFY bodyPitchRateChanged)
    Q_PROPERTY(float bodyYawRate READ bodyYawRate WRITE setBodyYawRate NOTIFY bodyYawRateChanged)
    Q_PROPERTY(float bodyRollRate READ bodyRollRate WRITE setBodyRollRate NOTIFY bodyRollRateChanged)
    Q_PROPERTY(float bodyVelX READ bodyVelX WRITE setBodyVelX NOTIFY bodyVelXChanged)
    Q_PROPERTY(float bodyVelY READ bodyVelY WRITE setBodyVelY NOTIFY bodyVelYChanged)
    Q_PROPERTY(float bodyVelZ READ bodyVelZ WRITE setBodyVelZ NOTIFY bodyVelZChanged)
    Q_PROPERTY(int bodyPosX READ bodyPosX WRITE setBodyPosX NOTIFY bodyPosXChanged)
    Q_PROPERTY(int bodyPosY READ bodyPosY WRITE setBodyPosY NOTIFY bodyPosYChanged)
    Q_PROPERTY(int bodyPosZ READ bodyPosZ WRITE setBodyPosZ NOTIFY bodyPosZChanged)
    Q_PROPERTY(float pitchGimbalPreset READ pitchGimbalPreset WRITE setPitchGimbalPreset NOTIFY pitchGimbalPresetChanged)
    Q_PROPERTY(float yawGimbalPreset READ yawGimbalPreset WRITE setYawGimbalPreset NOTIFY yawGimbalPresetChanged)
    Q_PROPERTY(int irIntegrationTime READ irIntegrationTime WRITE setIrIntegrationTime NOTIFY irIntegrationTimeChanged)
    Q_PROPERTY(int trackingCorrectionCmd READ trackingCorrectionCmd WRITE setTrackingCorrectionCmd NOTIFY trackingCorrectionCmdChanged)
    Q_PROPERTY(int correctionFrameNum READ correctionFrameNum WRITE setCorrectionFrameNum NOTIFY correctionFrameNumChanged)
    Q_PROPERTY(int correctedPitchPos READ correctedPitchPos WRITE setCorrectedPitchPos NOTIFY correctedPitchPosChanged)
    Q_PROPERTY(int correctedYawPos READ correctedYawPos WRITE setCorrectedYawPos NOTIFY correctedYawPosChanged)
    Q_PROPERTY(float searchPitchRate READ searchPitchRate WRITE setSearchPitchRate NOTIFY searchPitchRateChanged)
    Q_PROPERTY(float searchYawRate READ searchYawRate WRITE setSearchYawRate NOTIFY searchYawRateChanged)
    Q_PROPERTY(int gateSize READ gateSize WRITE setGateSize NOTIFY gateSizeChanged)
    Q_PROPERTY(int osdSwitch READ osdSwitch WRITE setOsdSwitch NOTIFY osdSwitchChanged)
    Q_PROPERTY(int captureRefImgCmd READ captureRefImgCmd WRITE setCaptureRefImgCmd NOTIFY captureRefImgCmdChanged)
    Q_PROPERTY(int targetAltitude READ targetAltitude WRITE setTargetAltitude NOTIFY targetAltitudeChanged)
    Q_PROPERTY(float aircraftPitch READ aircraftPitch WRITE setAircraftPitch NOTIFY aircraftPitchChanged)
    Q_PROPERTY(float aircraftYaw READ aircraftYaw WRITE setAircraftYaw NOTIFY aircraftYawChanged)
    Q_PROPERTY(float aircraftRoll READ aircraftRoll WRITE setAircraftRoll NOTIFY aircraftRollChanged)
    Q_PROPERTY(int focalLength READ focalLength WRITE setFocalLength NOTIFY focalLengthChanged)
    Q_PROPERTY(float podPitchAngle READ podPitchAngle WRITE setPodPitchAngle NOTIFY podPitchAngleChanged)
    Q_PROPERTY(float podYawAngle READ podYawAngle WRITE setPodYawAngle NOTIFY podYawAngleChanged)
    Q_PROPERTY(int satelliteMapScale READ satelliteMapScale WRITE setSatelliteMapScale NOTIFY satelliteMapScaleChanged)
    Q_PROPERTY(int podType READ podType WRITE setPodType NOTIFY podTypeChanged)
    Q_PROPERTY(double targetLongitude READ targetLongitude WRITE setTargetLongitude NOTIFY targetLongitudeChanged)
    Q_PROPERTY(double targetLatitude READ targetLatitude WRITE setTargetLatitude NOTIFY targetLatitudeChanged)
    Q_PROPERTY(double aircraftLongitude READ aircraftLongitude WRITE setAircraftLongitude NOTIFY aircraftLongitudeChanged)
    Q_PROPERTY(double aircraftLatitude READ aircraftLatitude WRITE setAircraftLatitude NOTIFY aircraftLatitudeChanged)
    Q_PROPERTY(int aircraftAltitude READ aircraftAltitude WRITE setAircraftAltitude NOTIFY aircraftAltitudeChanged)
    Q_PROPERTY(int pixelSize READ pixelSize WRITE setPixelSize NOTIFY pixelSizeChanged)

public:
    explicit ImageSendData(QObject *parent = nullptr);

    int frameLength() const;
    void setFrameLength(int value);
    int aFrameSequence() const;
    void setAFrameSequence(int value);
    int seekerCtrlWord() const;
    void setSeekerCtrlWord(int value);
    int opticalParamCtrl() const;
    void setOpticalParamCtrl(int value);
    int templateIndex() const;
    void setTemplateIndex(int value);
    int targetBackgroundType() const;
    void setTargetBackgroundType(int value);
    int missileTargetDistance() const;
    void setMissileTargetDistance(int value);
    float missileSpeed() const;
    void setMissileSpeed(float value);
    float bodyPitchAngle() const;
    void setBodyPitchAngle(float value);
    float bodyYawAngle() const;
    void setBodyYawAngle(float value);
    float bodyRollAngle() const;
    void setBodyRollAngle(float value);
    float bodyPitchRate() const;
    void setBodyPitchRate(float value);
    float bodyYawRate() const;
    void setBodyYawRate(float value);
    float bodyRollRate() const;
    void setBodyRollRate(float value);
    float bodyVelX() const;
    void setBodyVelX(float value);
    float bodyVelY() const;
    void setBodyVelY(float value);
    float bodyVelZ() const;
    void setBodyVelZ(float value);
    int bodyPosX() const;
    void setBodyPosX(int value);
    int bodyPosY() const;
    void setBodyPosY(int value);
    int bodyPosZ() const;
    void setBodyPosZ(int value);
    float pitchGimbalPreset() const;
    void setPitchGimbalPreset(float value);
    float yawGimbalPreset() const;
    void setYawGimbalPreset(float value);
    int irIntegrationTime() const;
    void setIrIntegrationTime(int value);
    int trackingCorrectionCmd() const;
    void setTrackingCorrectionCmd(int value);
    int correctionFrameNum() const;
    void setCorrectionFrameNum(int value);
    int correctedPitchPos() const;
    void setCorrectedPitchPos(int value);
    int correctedYawPos() const;
    void setCorrectedYawPos(int value);
    float searchPitchRate() const;
    void setSearchPitchRate(float value);
    float searchYawRate() const;
    void setSearchYawRate(float value);
    int gateSize() const;
    void setGateSize(int value);
    int osdSwitch() const;
    void setOsdSwitch(int value);
    int captureRefImgCmd() const;
    void setCaptureRefImgCmd(int value);
    int targetAltitude() const;
    void setTargetAltitude(int value);
    float aircraftPitch() const;
    void setAircraftPitch(float value);
    float aircraftYaw() const;
    void setAircraftYaw(float value);
    float aircraftRoll() const;
    void setAircraftRoll(float value);
    int focalLength() const;
    void setFocalLength(int value);
    float podPitchAngle() const;
    void setPodPitchAngle(float value);
    float podYawAngle() const;
    void setPodYawAngle(float value);
    int satelliteMapScale() const;
    void setSatelliteMapScale(int value);
    int podType() const;
    void setPodType(int value);
    double targetLongitude() const;
    void setTargetLongitude(double value);
    double targetLatitude() const;
    void setTargetLatitude(double value);
    double aircraftLongitude() const;
    void setAircraftLongitude(double value);
    double aircraftLatitude() const;
    void setAircraftLatitude(double value);
    int aircraftAltitude() const;
    void setAircraftAltitude(int value);
    int pixelSize() const;
    void setPixelSize(int value);

    QByteArray buildFrame() const;

signals:
    void frameLengthChanged();
    void aFrameSequenceChanged();
    void seekerCtrlWordChanged();
    void opticalParamCtrlChanged();
    void templateIndexChanged();
    void targetBackgroundTypeChanged();
    void missileTargetDistanceChanged();
    void missileSpeedChanged();
    void bodyPitchAngleChanged();
    void bodyYawAngleChanged();
    void bodyRollAngleChanged();
    void bodyPitchRateChanged();
    void bodyYawRateChanged();
    void bodyRollRateChanged();
    void bodyVelXChanged();
    void bodyVelYChanged();
    void bodyVelZChanged();
    void bodyPosXChanged();
    void bodyPosYChanged();
    void bodyPosZChanged();
    void pitchGimbalPresetChanged();
    void yawGimbalPresetChanged();
    void irIntegrationTimeChanged();
    void trackingCorrectionCmdChanged();
    void correctionFrameNumChanged();
    void correctedPitchPosChanged();
    void correctedYawPosChanged();
    void searchPitchRateChanged();
    void searchYawRateChanged();
    void gateSizeChanged();
    void osdSwitchChanged();
    void captureRefImgCmdChanged();
    void targetAltitudeChanged();
    void aircraftPitchChanged();
    void aircraftYawChanged();
    void aircraftRollChanged();
    void focalLengthChanged();
    void podPitchAngleChanged();
    void podYawAngleChanged();
    void satelliteMapScaleChanged();
    void podTypeChanged();
    void targetLongitudeChanged();
    void targetLatitudeChanged();
    void aircraftLongitudeChanged();
    void aircraftLatitudeChanged();
    void aircraftAltitudeChanged();
    void pixelSizeChanged();

private:
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
    int m_frameLength = 0;
    int m_aFrameSequence = 0;
    int m_seekerCtrlWord = 0;
    int m_opticalParamCtrl = 0;
    int m_templateIndex = 0;
    int m_targetBackgroundType = 0;
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
    int m_trackingCorrectionCmd = 0;
    int m_correctionFrameNum = 0;
    int m_correctedPitchPos = 0;
    int m_correctedYawPos = 0;
    float m_searchPitchRate = 0;
    float m_searchYawRate = 0;
    int m_gateSize = 0;
    int m_osdSwitch = 0;
    int m_captureRefImgCmd = 0;
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

    static void init_crc16_table(uint16_t poly = 0x1021);
    static uint16_t crc16_ccitt_fast(const uint8_t *data, size_t len, uint16_t init = 0xFFFF);

protected:
    QByteArray parseData(const QByteArray &rawData) override;

private:
    static uint16_t crc16_table[256];
    ImageData *m_imageData;
    ImageSendData *m_imageSendData;
    

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

    // 字节11: 当前工作通道 (0x02=红外, 0x03=电视)
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

    // 字节23: 修正指令状态回告 (bit0)
    quint8 correction_cmd_status;

    // 字节24: 修正指令次数回告
    quint8 correction_cmd_count;

    // 字节25-26: 俯仰陀螺
    qint16 pitch_gyro;

    // 字节27-28: 偏航陀螺
    qint16 yaw_gyro;

    // 字节29-31: 预留3字节
    quint8 reserved3[3];

    // 字节32: 跟踪状态
    quint8 tracking_state;

    // 字节33: 跟踪器状态
    quint8 tracker_state;

    // 字节34-52: 预留19字节
    quint8 reserved4[19];

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

    // 字节74-81: 预留8字节
    quint8 reserved6[8];

    // 字节82: 伺服运行时间
    quint8 servo_running_time;

    // 字节83-89: 预留7字节
    quint8 reserved7[7];

    // 字节90: 伺服阶跃
    quint8 servo_step;

    // 字节91-94: 红外帧编号
    quint32 infrared_frame_num;

    // 字节95-100: 预留6字节
    quint8 reserved8[6];

    // 字节101-102: Cbh_tv4405 (上位机新增)
    quint16 cbh_tv4405;

    // 字节103-104: 未定义，作为预留
    quint8 reserved9[2];

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
typedef struct {
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

    // 字节58: 预留1字节
    quint8 reserved2;

    // 字节59: 波门 (0xAA=大, 0x55=小)
    quint8 gate_size;

    // 字节60: 字符叠加 (0xAA=开, 0x55=关)
    quint8 osd_switch;

    // 字节61: 拍摄参考图指令 (0xAA=拍摄, 发三拍后恢复默认)
    quint8 capture_ref_img_cmd;

    // 字节62-152: 预留91字节
    quint8 reserved3[91];       // 62~152 inclusive count = 152-62+1=91

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

} image_send_frame;
#pragma pack(pop)
#endif // SERIALPORT_IMAGE_H
