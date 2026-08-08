#include "serialport_image.h"
#include <QDebug>
#include <QString>
#include "../log/LogManager.h"
#include <QThread>
#include <QTimer>
// ─────────────────────────────────────────────
// ImageData
// ─────────────────────────────────────────────
#define ExguideMode 0
#define ExguideSrcImg 3
#define Exguide_5ms 6
#define Exguide_1s 7
#define Maxsendcount 600 //每10分钟进行一次时间同步

uint16_t SerialPortImage::crc16_table[256] = {0};

ImageData::ImageData(QObject *parent)
    : QObject(parent)
{
}

// ── QML 调用方法 ──
void ImageData::openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void ImageData::closePort()                                     { emit requestClosePort(); }
void ImageData::scanPorts()                                     { emit requestScanPorts(); }
// void ImageData::sendData(const QByteArray &data)                { emit requestSendData(data); }

// ── 工作线程回推 ──
void ImageData::setPortOpen(bool open) { if (m_portOpen != open) { m_portOpen = open; emit portOpenChanged(); } }
void ImageData::setPortList(const QStringList &ports) { if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); } }
void ImageData::setError(const QString &msg) { if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); } }

int ImageData::frameLength() const { return m_frameLength; }
int ImageData::bFrameSequence() const { return m_bFrameSequence; }
int ImageData::aFrameSequenceReply() const { return m_aFrameSequenceReply; }
int ImageData::aFrameValidFlag() const { return m_aFrameValidFlag; }
int ImageData::seekerCtrlReply() const { return m_seekerCtrlReply; }
int ImageData::opticalParamReply() const { return m_opticalParamReply; }
int ImageData::currentWorkChannel() const { return m_currentWorkChannel; }
int ImageData::selfCheckFlag() const { return m_selfCheckFlag; }
int ImageData::selfCheckFlag1() const { return m_selfCheckFlag1; }
int ImageData::selfCheckFlag2() const { return m_selfCheckFlag2; }
int ImageData::selfCheckFlag3() const { return m_selfCheckFlag3; }
int ImageData::selfCheckFlag4() const { return m_selfCheckFlag4; }
int ImageData::selfCheckFlag5() const { return m_selfCheckFlag5; }
int ImageData::selfCheckFlag6() const { return m_selfCheckFlag6; }
float ImageData::pitchLosAngVel() const { return m_pitchLosAngVel; }
float ImageData::yawLosAngVel() const { return m_yawLosAngVel; }
int ImageData::targetBackgroundType() const { return m_targetBackgroundType; }
int ImageData::targetBackgroundType1() const { return m_targetBackgroundType1; }
int ImageData::targetBackgroundType2() const { return m_targetBackgroundType2; }
int ImageData::targetBackgroundType3() const { return m_targetBackgroundType3; }
int ImageData::targetBackgroundType4() const { return m_targetBackgroundType4; }
int ImageData::opticalWorkState() const { return m_opticalWorkState; }
float ImageData::pitchFrameAngle() const { return m_pitchFrameAngle; }
float ImageData::yawFrameAngle() const { return m_yawFrameAngle; }
int ImageData::correctionCmdStatus() const { return m_correctionCmdStatus; }
int ImageData::correctionCmdCount() const { return m_correctionCmdCount; }
float ImageData::pitchGyro() const { return m_pitchGyro; }
float ImageData::yawGyro() const { return m_yawGyro; }
int ImageData::trackingState() const { return m_trackingState; }
int ImageData::trackerState() const { return m_trackerState; }
int ImageData::azimuthDeviationPixel() const { return m_azimuthDeviationPixel; }
int ImageData::pitchDeviationPixel() const { return m_pitchDeviationPixel; }
float ImageData::azimuthGyroOutput() const { return m_azimuthGyroOutput; }
float ImageData::pitchGyroOutput() const { return m_pitchGyroOutput; }
float ImageData::azimuthMasterCmd() const { return m_azimuthMasterCmd; }
float ImageData::pitchMasterCmd() const { return m_pitchMasterCmd; }
int ImageData::platformSelfCheck() const { return m_platformSelfCheck; }
int ImageData::servoRunningTime() const { return m_servoRunningTime; }
int ImageData::servoStep() const { return m_servoStep; }
int ImageData::infraredFrameNum() const { return m_infraredFrameNum; }
int ImageData::cbhTv4405() const { return m_cbhTv4405; }
int ImageData::infraredFrameRate() const { return m_infraredFrameRate; }
int ImageData::tvFrameRate() const { return m_tvFrameRate; }
int ImageData::gateSize() const { return m_gateSize; }
int ImageData::softwareVersion1() const { return m_softwareVersion1; }
int ImageData::softwareVersion2() const { return m_softwareVersion2; }
int ImageData::softwareVersion3() const { return m_softwareVersion3; }

void ImageData::updateFromFrame(const QByteArray &frame)
{
    if (frame.size() < static_cast<int>(sizeof(image_recv_frame))) {
        return;
    }

    const image_recv_frame* pFrame = reinterpret_cast<const image_recv_frame*>(frame.data());

    if (m_frameLength != pFrame->frame_length) {
        m_frameLength = pFrame->frame_length;
        emit frameLengthChanged();
    }
    if (m_bFrameSequence != pFrame->b_frame_sequence) {
        m_bFrameSequence = pFrame->b_frame_sequence;
        emit bFrameSequenceChanged();
    }
    if (m_aFrameSequenceReply != pFrame->a_frame_sequence_reply) {
        m_aFrameSequenceReply = pFrame->a_frame_sequence_reply;
        emit aFrameSequenceReplyChanged();
    }
    if (m_aFrameValidFlag != pFrame->a_frame_valid_flag) {
        m_aFrameValidFlag = pFrame->a_frame_valid_flag;
        emit aFrameValidFlagChanged();
    }
    if (m_seekerCtrlReply != pFrame->seeker_ctrl_reply) {
        m_seekerCtrlReply = pFrame->seeker_ctrl_reply;
        emit seekerCtrlReplyChanged();
    }
    if (m_opticalParamReply != pFrame->optical_param_reply) {
        m_opticalParamReply = pFrame->optical_param_reply;
        emit opticalParamReplyChanged();
    }
    if (m_currentWorkChannel != pFrame->current_work_channel) {
        m_currentWorkChannel = pFrame->current_work_channel;
        emit currentWorkChannelChanged();
    }
    if (m_selfCheckFlag != pFrame->self_check_flag) {
        m_selfCheckFlag = pFrame->self_check_flag;
        m_selfCheckFlag1 = convertAndGetBit(m_selfCheckFlag, 0,0);
        m_selfCheckFlag2 = convertAndGetBit(m_selfCheckFlag, 1,1);
        m_selfCheckFlag3 = convertAndGetBit(m_selfCheckFlag, 2,2);
        m_selfCheckFlag4 = convertAndGetBit(m_selfCheckFlag, 3,3);
        m_selfCheckFlag5 = convertAndGetBit(m_selfCheckFlag, 4,4);
        m_selfCheckFlag6 = convertAndGetBit(m_selfCheckFlag, 5,5);
        emit selfCheckFlagChanged();
        emit selfCheckFlag1Changed();
        emit selfCheckFlag2Changed();
        emit selfCheckFlag3Changed();
        emit selfCheckFlag4Changed();
        emit selfCheckFlag5Changed();
        emit selfCheckFlag6Changed();
    }

    auto pitch_los_ang_vel = fromRawValue_b(pFrame->pitch_line_of_sight_ang_vel);
    if (m_pitchLosAngVel != pitch_los_ang_vel) {
        m_pitchLosAngVel = pitch_los_ang_vel;
        emit pitchLosAngVelChanged();
    }
    auto yaw_los_ang_vel = fromRawValue_b(pFrame->yaw_line_of_sight_ang_vel);
    if (m_yawLosAngVel != yaw_los_ang_vel) {
        m_yawLosAngVel = yaw_los_ang_vel;
        emit yawLosAngVelChanged();
    }

    if (m_targetBackgroundType != pFrame->target_background_type) {
        m_targetBackgroundType = pFrame->target_background_type;
        m_targetBackgroundType1=convertAndGetBit(m_targetBackgroundType, 0,2);
        m_targetBackgroundType2=convertAndGetBit(m_targetBackgroundType, 3,3);
        m_targetBackgroundType3=convertAndGetBit(m_targetBackgroundType, 4,4);
        m_targetBackgroundType4=convertAndGetBit(m_targetBackgroundType, 5,7);
        emit targetBackgroundTypeChanged();
        emit targetBackgroundType1Changed();
        emit targetBackgroundType2Changed();
        emit targetBackgroundType3Changed();
        emit targetBackgroundType4Changed();
    }
    if (m_opticalWorkState != pFrame->optical_work_state) {
        m_opticalWorkState = pFrame->optical_work_state;
        emit opticalWorkStateChanged();
    }
    auto pitch_frame_angle = fromRawValue_b(pFrame->pitch_frame_angle);
    if (m_pitchFrameAngle != pitch_frame_angle) {
        m_pitchFrameAngle = pitch_frame_angle;
        emit pitchFrameAngleChanged();
    }
    auto yaw_frame_angle = fromRawValue_b(pFrame->yaw_frame_angle);
    if (m_yawFrameAngle != yaw_frame_angle) {
        m_yawFrameAngle = yaw_frame_angle;
        emit yawFrameAngleChanged();
    }

    if (m_correctionCmdStatus != pFrame->correction_cmd_status) {
        m_correctionCmdStatus = pFrame->correction_cmd_status;
        emit correctionCmdStatusChanged();
    }
    if (m_correctionCmdCount != pFrame->correction_cmd_count) {
        m_correctionCmdCount = pFrame->correction_cmd_count;
        emit correctionCmdCountChanged();
    }

    auto pitch_gyro = fromRawValue_a(pFrame->pitch_gyro);
    if (m_pitchGyro != pitch_gyro) {
        m_pitchGyro = pitch_gyro;
        emit pitchGyroChanged();
    }
    auto yaw_gyro = fromRawValue_a(pFrame->yaw_gyro);
    if (m_yawGyro != yaw_gyro) {
        m_yawGyro = yaw_gyro;
        emit yawGyroChanged();
    }

    if (m_trackingState != pFrame->tracking_state) {
        m_trackingState = pFrame->tracking_state;
        emit trackingStateChanged();
    }
    if (m_trackerState != pFrame->tracker_state) {
        m_trackerState = pFrame->tracker_state;
        emit trackerStateChanged();
    }

    if (m_azimuthDeviationPixel != pFrame->azimuth_deviation_pixel) {
        m_azimuthDeviationPixel = pFrame->azimuth_deviation_pixel;
        emit azimuthDeviationPixelChanged();
    }
    if (m_pitchDeviationPixel != pFrame->pitch_deviation_pixel) {
        m_pitchDeviationPixel = pFrame->pitch_deviation_pixel;
        emit pitchDeviationPixelChanged();
    }

    auto azimuth_gyro_output = fromRawValue_a(pFrame->azimuth_gyro_output);
    if (m_azimuthGyroOutput != azimuth_gyro_output) {
        m_azimuthGyroOutput = azimuth_gyro_output;
        emit azimuthGyroOutputChanged();
    }
    auto pitch_gyro_output = fromRawValue_a(pFrame->pitch_gyro_output);
    if (m_pitchGyroOutput != pitch_gyro_output) {
        m_pitchGyroOutput = pitch_gyro_output;
        emit pitchGyroOutputChanged();
    }

    auto azimuth_master_cmd = fromRawValue_a(pFrame->azimuth_master_cmd);
    if (m_azimuthMasterCmd != azimuth_master_cmd) {
        m_azimuthMasterCmd = azimuth_master_cmd;
        emit azimuthMasterCmdChanged();
    }
    auto pitch_master_cmd = fromRawValue_a(pFrame->pitch_master_cmd);
    if (m_pitchMasterCmd != pitch_master_cmd) {
        m_pitchMasterCmd = pitch_master_cmd;
        emit pitchMasterCmdChanged();
    }

    if (m_platformSelfCheck != pFrame->platform_self_check) {
        m_platformSelfCheck = pFrame->platform_self_check;
        emit platformSelfCheckChanged();
    }
    if (m_servoRunningTime != pFrame->servo_running_time) {
        m_servoRunningTime = pFrame->servo_running_time;
        emit servoRunningTimeChanged();
    }
    if (m_servoStep != pFrame->servo_step) {
        m_servoStep = pFrame->servo_step;
        emit servoStepChanged();
    }

    auto infrared_frame_num = static_cast<int>(pFrame->infrared_frame_num);
    if (m_infraredFrameNum != infrared_frame_num) {
        m_infraredFrameNum = infrared_frame_num;
        emit infraredFrameNumChanged();
    }

    if (m_cbhTv4405 != pFrame->cbh_tv4405) {
        m_cbhTv4405 = pFrame->cbh_tv4405;
        emit cbhTv4405Changed();
    }
    if (m_infraredFrameRate != pFrame->infrared_frame_rate) {
        m_infraredFrameRate = pFrame->infrared_frame_rate;
        emit infraredFrameRateChanged();
    }
    if (m_tvFrameRate != pFrame->tv_frame_rate) {
        m_tvFrameRate = pFrame->tv_frame_rate;
        emit tvFrameRateChanged();
    }

    if (m_gateSize != pFrame->gate_size) {
        m_gateSize = pFrame->gate_size;
        emit gateSizeChanged();
    }

    if (m_softwareVersion1 != pFrame->software_version1) {
        m_softwareVersion1 = pFrame->software_version1;
        emit softwareVersion1Changed();
    }
    if (m_softwareVersion2 != pFrame->software_version2) {
        m_softwareVersion2 = pFrame->software_version2;
        emit softwareVersion2Changed();
    }
    if (m_softwareVersion3 != pFrame->software_version3) {
        m_softwareVersion3 = pFrame->software_version3;
        emit softwareVersion3Changed();
    }
}
int ImageData::getBitsFromQint8(qint8 value, int startBit, int endBit)
{
    // 检查位索引范围（0~7）
    if (startBit < 0 || startBit > 7 || endBit < 0 || endBit > 7) {
        return -1;
    }
    // 确保 startBit <= endBit（低位小，高位大）
    if (startBit > endBit) {
        std::swap(startBit, endBit);  // 允许用户乱序，自动交换
    }

    int width = endBit - startBit + 1;          // 位段的宽度
    int mask = (1 << width) - 1;                // 低 width 位全1
    int shifted = value >> startBit;            // 右移，使位段对齐到最低位
    return shifted & mask;                      // 提取位段
}

// ─────────────────────────────────────────────
// ImageSendData
// ─────────────────────────────────────────────

ImageSendData::ImageSendData(QObject *parent)
    : QObject(parent)
{
}

void ImageSendData::buildFrame() 
{
    image_send_frame frame = {};

    frame.frame_header1 = 0x77;
    frame.frame_header2 = 0xAA;
    frame.frame_length = static_cast<quint8>(m_frameLength);
    frame.a_frame_sequence = static_cast<quint16>(m_aFrameSequence);
    frame.seeker_ctrl_word = static_cast<quint8>(m_seekerCtrlWord);
    frame.optical_param_ctrl = static_cast<quint8>(m_opticalParamCtrl);
    frame.template_index = static_cast<quint8>(m_templateIndex);
    // frame.target_background_type = static_cast<quint8>(m_targetBackgroundType);
    // 后面不发送信息可以置0
    frame.target_background_type = static_cast<quint8>(
        (m_targetBackgroundType4 << 5) | (m_targetBackgroundType3 << 4) |
        (m_targetBackgroundType2 << 3) | m_targetBackgroundType1);
    frame.missile_target_distance = static_cast<quint16>(m_missileTargetDistance);
    frame.missile_speed = static_cast<quint16>(toRawValue_a(m_missileSpeed));
    frame.body_pitch_angle = static_cast<qint16>(toRawValue_b(m_bodyPitchAngle));
    frame.body_yaw_angle = static_cast<qint16>(toRawValue_b(m_bodyYawAngle));
    frame.body_roll_angle = static_cast<qint16>(toRawValue_b(m_bodyRollAngle));
    frame.body_pitch_rate = static_cast<qint16>(toRawValue_b(m_bodyPitchRate));
    frame.body_yaw_rate = static_cast<qint16>(toRawValue_b(m_bodyYawRate));
    frame.body_roll_rate = static_cast<qint16>(toRawValue_b(m_bodyRollRate));
    frame.body_vel_x = static_cast<qint16>(toRawValue_a(m_bodyVelX));
    frame.body_vel_y = static_cast<qint16>(toRawValue_a(m_bodyVelY));
    frame.body_vel_z = static_cast<qint16>(toRawValue_a(m_bodyVelZ));
    frame.body_pos_x = static_cast<qint16>(m_bodyPosX);
    frame.body_pos_y = static_cast<qint16>(m_bodyPosY);
    frame.body_pos_z = static_cast<qint16>(m_bodyPosZ);
    frame.pitch_gimbal_preset = static_cast<qint16>(toRawValue_c(m_pitchGimbalPreset));
    frame.yaw_gimbal_preset = static_cast<qint16>(toRawValue_c(m_yawGimbalPreset));
    frame.ir_integration_time = static_cast<quint8>(toRawValue_a(m_irIntegrationTime));
    memset(frame.reserved1, 0, sizeof(frame.reserved1));
    frame.tracking_correction_cmd = static_cast<quint8>(m_trackingCorrectionCmd);
    frame.correction_frame_num[0] = static_cast<quint8>(m_correctionFrameNum & 0xFF);
    frame.correction_frame_num[1] = static_cast<quint8>((m_correctionFrameNum >> 8) & 0xFF);
    frame.correction_frame_num[2] = static_cast<quint8>((m_correctionFrameNum >> 16) & 0xFF);
    frame.corrected_pitch_pos = static_cast<quint16>(m_correctedPitchPos);
    frame.corrected_yaw_pos = static_cast<quint16>(m_correctedYawPos);
    frame.search_pitch_rate = static_cast<qint16>(toRawValue_b(m_searchPitchRate));
    frame.search_yaw_rate = static_cast<qint16>(toRawValue_b(m_searchYawRate));
    frame.reserved2 = 0;
    frame.gate_size = static_cast<quint8>(m_gateSize);
    frame.osd_switch = static_cast<quint8>(m_osdSwitch);
    frame.capture_ref_img_cmd = static_cast<quint8>(m_captureRefImgCmd);
    memset(frame.reserved3, 0, sizeof(frame.reserved3));
    frame.target_altitude = static_cast<qint16>(m_targetAltitude);
    frame.aircraft_pitch = static_cast<qint16>(toRawValue_b(m_aircraftPitch));
    frame.aircraft_yaw = static_cast<qint16>(toRawValue_b(m_aircraftYaw));
    frame.aircraft_roll = static_cast<qint16>(toRawValue_b(m_aircraftRoll));
    frame.focal_length = static_cast<quint16>(m_focalLength);
    frame.pod_pitch_angle = static_cast<qint16>(toRawValue_b(m_podPitchAngle));
    //预留位置
    frame.pod_yaw_angle = static_cast<qint16>(toRawValue_b(m_podYawAngle));
    memset(frame.reserved4, 0, sizeof(frame.reserved4));
    frame.satellite_map_scale = static_cast<quint8>(toRawValue_a(m_satelliteMapScale));
    frame.pod_type = static_cast<quint8>(m_podType);
    frame.target_longitude = static_cast<qint32>(m_targetLongitude);
    frame.target_latitude = static_cast<qint32>(m_targetLatitude);
    frame.aircraft_longitude = static_cast<qint32>(m_aircraftLongitude);
    frame.aircraft_latitude = static_cast<qint32>(m_aircraftLatitude);
    frame.aircraft_altitude = static_cast<qint16>(m_aircraftAltitude);
    frame.pixel_size = static_cast<quint8>(m_pixelSize);
    // uint16_t crc = SerialPortImage::crc16_ccitt_fast(
    //     reinterpret_cast<const uint8_t*>(&frame), sizeof(frame) - sizeof(uint16_t));
    // frame.crc16 = crc;
    //将校验位计算移到工作线程
    emit requestSendData(frame);
}

void ImageSendData::buildDeviation(int num ,int x ,int y)
{
    image_send_frame frame = {};

    frame.frame_header1 = 0x77;
    frame.frame_header2 = 0xAA;
    frame.frame_length = static_cast<quint8>(m_frameLength);
    frame.a_frame_sequence = static_cast<quint16>(m_aFrameSequence);
    frame.seeker_ctrl_word = static_cast<quint8>(m_seekerCtrlWord);
    frame.optical_param_ctrl = static_cast<quint8>(m_opticalParamCtrl);
    frame.template_index = static_cast<quint8>(m_templateIndex);
    // frame.target_background_type = static_cast<quint8>(m_targetBackgroundType);
    // 后面不发送信息可以置0
    frame.target_background_type = static_cast<quint8>(
        (m_targetBackgroundType4 << 5) | (m_targetBackgroundType3 << 4) |
        (m_targetBackgroundType2 << 3) | m_targetBackgroundType1);
    frame.missile_target_distance = static_cast<quint16>(m_missileTargetDistance);
    frame.missile_speed = static_cast<quint16>(toRawValue_a(m_missileSpeed));
    frame.body_pitch_angle = static_cast<qint16>(toRawValue_b(m_bodyPitchAngle));
    frame.body_yaw_angle = static_cast<qint16>(toRawValue_b(m_bodyYawAngle));
    frame.body_roll_angle = static_cast<qint16>(toRawValue_b(m_bodyRollAngle));
    frame.body_pitch_rate = static_cast<qint16>(toRawValue_b(m_bodyPitchRate));
    frame.body_yaw_rate = static_cast<qint16>(toRawValue_b(m_bodyYawRate));
    frame.body_roll_rate = static_cast<qint16>(toRawValue_b(m_bodyRollRate));
    frame.body_vel_x = static_cast<qint16>(toRawValue_a(m_bodyVelX));
    frame.body_vel_y = static_cast<qint16>(toRawValue_a(m_bodyVelY));
    frame.body_vel_z = static_cast<qint16>(toRawValue_a(m_bodyVelZ));
    frame.body_pos_x = static_cast<qint16>(m_bodyPosX);
    frame.body_pos_y = static_cast<qint16>(m_bodyPosY);
    frame.body_pos_z = static_cast<qint16>(m_bodyPosZ);
    frame.pitch_gimbal_preset = static_cast<qint16>(toRawValue_c(m_pitchGimbalPreset));
    frame.yaw_gimbal_preset = static_cast<qint16>(toRawValue_c(m_yawGimbalPreset));
    frame.ir_integration_time = static_cast<quint8>(toRawValue_a(m_irIntegrationTime));
    memset(frame.reserved1, 0, sizeof(frame.reserved1));
    frame.tracking_correction_cmd = 1;  //修正帧序号置1 ,只发送一拍
    frame.correction_frame_num[0] = static_cast<quint8>(num & 0xFF);
    frame.correction_frame_num[1] = static_cast<quint8>((num >> 8) & 0xFF);
    frame.correction_frame_num[2] = static_cast<quint8>((num >> 16) & 0xFF);
    frame.corrected_pitch_pos = static_cast<quint16>(y);
    frame.corrected_yaw_pos = static_cast<quint16>(x); 
    frame.search_pitch_rate = static_cast<qint16>(toRawValue_b(m_searchPitchRate));
    frame.search_yaw_rate = static_cast<qint16>(toRawValue_b(m_searchYawRate));
    frame.reserved2 = 0;
    frame.gate_size = static_cast<quint8>(m_gateSize);
    frame.osd_switch = static_cast<quint8>(m_osdSwitch);
    frame.capture_ref_img_cmd = static_cast<quint8>(m_captureRefImgCmd);
    memset(frame.reserved3, 0, sizeof(frame.reserved3));
    frame.target_altitude = static_cast<qint16>(m_targetAltitude);
    frame.aircraft_pitch = static_cast<qint16>(toRawValue_b(m_aircraftPitch));
    frame.aircraft_yaw = static_cast<qint16>(toRawValue_b(m_aircraftYaw));
    frame.aircraft_roll = static_cast<qint16>(toRawValue_b(m_aircraftRoll));
    frame.focal_length = static_cast<quint16>(m_focalLength);
    frame.pod_pitch_angle = static_cast<qint16>(toRawValue_b(m_podPitchAngle));
    //预留位置
    frame.pod_yaw_angle = static_cast<qint16>(toRawValue_b(m_podYawAngle));
    memset(frame.reserved4, 0, sizeof(frame.reserved4));
    frame.satellite_map_scale = static_cast<quint8>(toRawValue_a(m_satelliteMapScale));
    frame.pod_type = static_cast<quint8>(m_podType);
    frame.target_longitude = static_cast<qint32>(m_targetLongitude);
    frame.target_latitude = static_cast<qint32>(m_targetLatitude);
    frame.aircraft_longitude = static_cast<qint32>(m_aircraftLongitude);
    frame.aircraft_latitude = static_cast<qint32>(m_aircraftLatitude);
    frame.aircraft_altitude = static_cast<qint16>(m_aircraftAltitude);
    frame.pixel_size = static_cast<quint8>(m_pixelSize);
    emit requestSendData(frame);
}


// ─────────────────────────────────────────────
// SerialPortImage
// ─────────────────────────────────────────────

SerialPortImage::SerialPortImage(QObject *parent)
    : SerialPort(parent)
    , m_imageData(new ImageData(nullptr))        // 留在主线程，不随 moveToThread 迁移
    , m_imageSendData(new ImageSendData(nullptr))
    , m_circularbuf(200)
{
    init_crc16_table();
}

SerialPortImage::~SerialPortImage() {
    if (m_exGuideTimer) {
        if (QThread::currentThread() != m_exGuideTimer->thread())
            m_exGuideTimer->moveToThread(QThread::currentThread());
        m_exGuideTimer->stop();
        delete m_exGuideTimer;
        m_exGuideTimer = nullptr;
    }
    // delete m_imageData;
    // delete m_imageSendData;
}

// ── Worker slots ──
void SerialPortImage::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}
void SerialPortImage::onClosePort()  { SerialPort::close(); emit portClosed(); }
void SerialPortImage::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }
void SerialPortImage::onSendData(image_send_frame frame) { 

    uint16_t crc = SerialPortImage::crc16_ccitt_fast(
        reinterpret_cast<const uint8_t*>(&frame), sizeof(frame) - sizeof(uint16_t));
    frame.crc16 = crc;
    auto data= QByteArray(reinterpret_cast<const char*>(&frame), sizeof(frame));  

    //引入定时器，每20ms发送一次
    int sendCount = 0;
    static quint16 num=0;
    
    timer->setInterval(20); // 20ms
    // 先断开旧连接，避免重复绑定导致 lambda 被多次触发
    disconnect(timer, &QTimer::timeout, this, nullptr);
    // 连接定时器的超时信号
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        // 发送数据
        qint64 count=SerialPort::send(data);
        //增加帧流水号改变,如果发送字节数没问题
        if(count >= 224){
        //计算流水号      
            num += 1;
        //  将结果拆分回两个字节
            data[3] = static_cast<char>(num & 0xFF);        // 低字节
            data[4] = static_cast<char>((num >> 8) & 0xFF); // 高字节
        }
        sendCount++;
        
        //发一拍处理
        if(sendCount >= 1){
            data[46]=0x00; //跟踪修正指令
        }
        //加入发三拍处理
        if(sendCount >= 3){
            data[5]=0x00;    //导引头控制字
            data[6]=0x00;    //光学参数装订控制字
            data[61]=0x00;   //拍摄参考图
        }
        //更新数据后重新计算并填入校验位
        crc= crc16_ccitt_fast(reinterpret_cast<const uint8_t*>(data.constData()), data.size() - sizeof(uint16_t));
        data[222] = static_cast<char>(crc & 0xFF);        // 低字节
        data[223] = static_cast<char>((crc >> 8) & 0xFF); // 高字节
        // 发送10次后停止并销毁定时器
        if (sendCount >= 10) {
            timer->stop();
            // timer->deleteLater();
        }
    });

    // 启动定时器（立即触发第一次发送，若想先等20ms再发，可改为 timer->start(20) 但默认立即触发）
    timer->start();
}
void SerialPortImage::onReadyRead()
{
    // 仿照 laser 串口的处理：串口驱动一次 readyRead 到达的数据不一定是一整帧，
    // 先存入接收缓冲，找到帧头 0x77 0xAB 后再按固定帧长切出完整一帧交给 parseData
    m_rxBuffer.append(m_serialPort->readAll());

    const int frameLen = static_cast<int>(sizeof(image_recv_frame));

    while (m_rxBuffer.size() >= 2) {
        // 1. 逐字节查找帧头：找到 0x77 后判断下一个是否为 0xAB
        int headIdx = -1;
        for (int i = 0; i <= m_rxBuffer.size() - 2; ++i) {
            if (static_cast<quint8>(m_rxBuffer[i])     == 0x77
                    && static_cast<quint8>(m_rxBuffer[i + 1]) == 0xAB) {
                headIdx = i;
                break;
            }
        }
        if (headIdx < 0) {
            // 没找到完整帧头，保留最后1字节（可能是下一帧帧头的第一个字节）
            if (m_rxBuffer.size() > 1)
                m_rxBuffer = m_rxBuffer.right(1);
            break;
        }
        if (headIdx > 0)
            m_rxBuffer.remove(0, headIdx);
        if (m_rxBuffer.size() < frameLen)
            break;
        QByteArray frame = m_rxBuffer.left(frameLen);
        m_rxBuffer.remove(0, frameLen);
        parseData(frame);
    }
}


void SerialPortImage::ExmodeChanged(int mode)
{   
    //1:外引导  2：程控模式 3：遥控模式  //还需要判断外引导源  //判断跟踪模式
    //判断使用哪个导引头的数据，来决定是否定期向转台串口线程同步数据
    //判断index与外引导模式数据选择提供位，如果被选中，就启动一个定时器，每1秒或5ms发送一次跟踪数据信息
    //先发送时间同步指令信号，再发送Kalman预测的跟踪角度数据
    if(mode < 3)
    exindex = mode;  //模式索引赋值
    if(mode > 2 && mode < 6)
    exsrcindex = mode;  //外引导源索引赋值 索引分别为3，4，5
    if(mode >5)
    exguidesetting = mode; //外引导发送时间间隔选择  索引为6，7

    if (exindex == ExguideMode) //判断是否为外引导模式
    {
        // 图像导引头被选为外引导源：启动定时器，每1秒或5ms发送一次跟踪数据
        if(exsrcindex == ExguideSrcImg )
        {
            // 判断图像导引头串口是否开启，未开启时触发弹窗提示
            if (!isOpen()) {
                emit exguideSerialNotOpen("图像导引头串口未打开");
            }

            // 只有时间间隔设置改变时才重新绑定
            if(m_lastexguidesetting != exguidesetting)
            {
                // 先解绑旧连接并停止定时器
                if(m_lastexguidesetting > 0)
                {
                    m_exGuideTimer->stop();
                    disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
                    //清空发送计数
                    m_sendCount_1s = 0;
                }
                // 延迟创建定时器
                if (!m_exGuideTimer) {
                    m_exGuideTimer = new QTimer(this);
                    m_exGuideTimer->setTimerType(Qt::PreciseTimer);
                }
                // 判断跟踪模式（5ms模式或者1秒跟踪模式）
                if(exguidesetting == Exguide_1s)
                {
                    connect(m_exGuideTimer, &QTimer::timeout, this, [this]() {
                        // 每发送完固定次数后，重新发送时间同步信号并重新计时
                        if(m_sendCount_1s == 0)
                        {
                            emit reqTimesync(0);
                        }
                        // 生成预测数据包：4点外推（0.25s, 0.5s, 0.75s, 1.0s），time = 发包计数
                        sendExGuideData az_pkt = m_abMgr.GenAxisPacket(true,  m_sendCount_1s + 1);  // 方位轴
                        sendExGuideData el_pkt = m_abMgr.GenAxisPacket(false, m_sendCount_1s +1);  // 俯仰轴
                        // 发送预测角度给转台
                        reqExsend_1s(az_pkt, el_pkt);
                        LogManager::instance()->logImageTracking(az_pkt.angle1, el_pkt.angle1);
                        if(++m_sendCount_1s >= Maxsendcount) {  // 1小时重同步
                            m_sendCount_1s = 0;
                        }
                    });
                    m_exGuideTimer->start(1000); // 每1s触发一次
                    m_lastexguidesetting = Exguide_1s;
                }
                else if(exguidesetting == Exguide_5ms)
                {
                    connect(m_exGuideTimer, &QTimer::timeout, this, [this]() {
                        // 5ms模式只发送方位角与俯仰角即可
                        reqExsend_5ms(m_azimuth,m_pitch);
                        LogManager::instance()->logImageTracking(m_azimuth, m_pitch);    
                    });
                    m_exGuideTimer->start(5); // 每5ms触发一次
                    m_lastexguidesetting = Exguide_5ms;
                }
            } 
        }
        else
        {
            if (m_exGuideTimer) {
                m_exGuideTimer->stop(); //切换到其他外引导源时，暂停图像导引头外引导定时器，停止继续发送
            }
        }
    }
    else
    {
        // 非外引导模式：停止定时器
        if (m_exGuideTimer) {
            m_exGuideTimer->stop();
        }
    }
}

ImageData* SerialPortImage::imageData() const
{
    return m_imageData;
}

ImageSendData* SerialPortImage::imageSendData() const
{
    return m_imageSendData;
}

//数据解析
void SerialPortImage::parseData(const QByteArray &rawData)
{
    if (rawData.size() < static_cast<int>(sizeof(image_recv_frame))) {
        // return QByteArray();
        return;
    }

    const image_recv_frame* pFrame = reinterpret_cast<const image_recv_frame*>(rawData.data());

    if (pFrame->frame_header1 != 0x77 || pFrame->frame_header2 != 0xAB) {
        // return QByteArray();
        qDebug() << "Invalid frame header:" << QString::number(pFrame->frame_header1, 16) << QString::number(pFrame->frame_header2, 16);
        return;
    }

    size_t dataLen = sizeof(image_recv_frame) - sizeof(uint16_t);
    uint16_t calculatedCrc = crc16_ccitt_fast(
        reinterpret_cast<const uint8_t*>(rawData.data()), dataLen);
    if (calculatedCrc != pFrame->crc16) {
        // return QByteArray();
        qDebug() << "Invalid frame CRC:" << QString::number(calculatedCrc, 16) << QString::number(pFrame->crc16, 16);
        return;
    }

    //校验无误后将方位角与俯仰角数据存入环形缓冲区
    // imageExGuideData m_data;
    // m_data.azimuth = pFrame->yaw_frame_angle * 0.002;
    // m_data.pitch = pFrame->pitch_frame_angle * 0.002;
    // m_circularbuf.push(m_data);
    m_azimuth = pFrame->yaw_frame_angle * 0.002;
    m_pitch = pFrame->pitch_frame_angle * 0.002;
    emit imageFrameReceived(rawData);

    //判断图像导引头是否被选中为外引导源，是的话更新数据（1s跟踪模式）
    if(exindex == 0)
    {
        m_filterTime += 20;
        m_abMgr.FeedData(m_filterTime, m_azimuth, m_pitch);
    }
}

void SerialPortImage::init_crc16_table(uint16_t poly)
{
    for (int i = 0; i < 256; ++i) {
        uint16_t crc = static_cast<uint16_t>(i << 8);
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
        }
        crc16_table[i] = crc;
    }
}

uint16_t SerialPortImage::crc16_ccitt_fast(const uint8_t *data, size_t len, uint16_t init)
{
    uint16_t crc = init;
    for (size_t i = 0; i < len; ++i) {
        crc = (crc << 8) ^ crc16_table[((crc >> 8) ^ data[i]) & 0xFF];
    }
    return crc;
}

// void SerialPortImage::ExmodeChanged(int mode)
// {
//     //判断使用哪个导引头的数据，来决定是否定期向转台串口线程同步数据
//     //判断index与外引导模式数据选择提供位，如果被选中，就启动一个定时器，每3S发送一次跟踪数据信息
//     //先发送时间同步指令信号，再发送Kalman预测的跟踪角度数据
//     exindex = mode;
//     //以下操作可把一小时转化为0-3599的数值，给发送的数据提供时间戳
//     //每3秒发送一次数据
//     // QDateTime current = QDateTime::currentDateTime();
//     // QTime time = current.time();
//     // int value = time.minute() * 60 + time.second();
//     if (exindex == 1)
//     {
//         // 图像导引头被选为外引导源：启动定时器，每3秒发送一次跟踪数据
//         if (!m_exGuideTimer) {
//             m_exGuideTimer = new QTimer(this);
//             connect(m_exGuideTimer, &QTimer::timeout, this, [this]() {
//                 // 用当前导引头反馈角度重新初始化Kalman滤波器
//                 m_kalman.Init(m_azimuth, m_pitch);
//                 // 生成方位轴和俯仰轴的3s预测数据包
//                 AxisTrackPacket m_tacpkt1 = m_kalman.GenAxisPacket(true);   // 方位轴
//                 AxisTrackPacket m_tacpkt2 = m_kalman.GenAxisPacket(false);  // 俯仰轴
//                 // 发送时间同步指令（0时刻）
//                 // emit reqTimesync();
//                 // 发送Kalman预测的目标角度给转台串口线程
//                 // emit reqExsend_1s(m_tacpkt1, m_tacpkt2);
//             });
//         }
//         m_exGuideTimer->start(1000); // 每1秒触发一次
//     }
//     else
//     {
//         // 非图像导引头外引导源：停止定时器
//         if (m_exGuideTimer) {
//             m_exGuideTimer->stop();
//         }
//     }
// }