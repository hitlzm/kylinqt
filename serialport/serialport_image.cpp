#include "serialport_image.h"

// ─────────────────────────────────────────────
// ImageData
// ─────────────────────────────────────────────

uint16_t SerialPortImage::crc16_table[256] = {0};

ImageData::ImageData(QObject *parent)
    : QObject(parent)
{
    
}

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

    if (m_pitchLosAngVel != fromRawValue_a(pFrame->pitch_line_of_sight_ang_vel) ){
        m_pitchLosAngVel = fromRawValue_a(pFrame->pitch_line_of_sight_ang_vel);
        emit pitchLosAngVelChanged();
    }
    if (m_yawLosAngVel != fromRawValue_b(pFrame->yaw_line_of_sight_ang_vel) ) {
        m_yawLosAngVel = fromRawValue_b(pFrame->yaw_line_of_sight_ang_vel);
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

    if (m_pitchFrameAngle != fromRawValue_b(pFrame->pitch_frame_angle)) {
        m_pitchFrameAngle = fromRawValue_b(pFrame->pitch_frame_angle);
        emit pitchFrameAngleChanged();
    }
    if (m_yawFrameAngle != fromRawValue_b(pFrame->yaw_frame_angle)) {
        m_yawFrameAngle = fromRawValue_b(pFrame->yaw_frame_angle);
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

    if (m_pitchGyro != fromRawValue_a(pFrame->pitch_gyro)) {
        m_pitchGyro = fromRawValue_a(pFrame->pitch_gyro);
        emit pitchGyroChanged();
    }
    if (m_yawGyro != fromRawValue_a(pFrame->yaw_gyro)) {
        m_yawGyro = fromRawValue_a(pFrame->yaw_gyro);
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

    if (m_azimuthGyroOutput != pFrame->azimuth_gyro_output) {
        m_azimuthGyroOutput = pFrame->azimuth_gyro_output;
        emit azimuthGyroOutputChanged();
    }
    if (m_pitchGyroOutput != pFrame->pitch_gyro_output) {
        m_pitchGyroOutput = pFrame->pitch_gyro_output;
        emit pitchGyroOutputChanged();
    }

    if (m_azimuthMasterCmd != pFrame->azimuth_master_cmd) {
        m_azimuthMasterCmd = pFrame->azimuth_master_cmd;
        emit azimuthMasterCmdChanged();
    }
    if (m_pitchMasterCmd != pFrame->pitch_master_cmd) {
        m_pitchMasterCmd = pFrame->pitch_master_cmd;
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

    if (m_infraredFrameNum != static_cast<int>(pFrame->infrared_frame_num)) {
        m_infraredFrameNum = pFrame->infrared_frame_num;
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

QByteArray ImageSendData::buildFrame() const
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
    frame.tracking_correction_cmd = static_cast<quint8>(m_trackingCorrectionCmd);
    frame.correction_frame_num[0] = static_cast<quint8>(m_correctionFrameNum & 0xFF);
    frame.correction_frame_num[1] = static_cast<quint8>((m_correctionFrameNum >> 8) & 0xFF);
    frame.correction_frame_num[2] = static_cast<quint8>((m_correctionFrameNum >> 16) & 0xFF);
    frame.corrected_pitch_pos = static_cast<quint16>(m_correctedPitchPos);
    frame.corrected_yaw_pos = static_cast<quint16>(m_correctedYawPos);
    frame.search_pitch_rate = static_cast<qint16>(toRawValue_b(m_searchPitchRate));
    frame.search_yaw_rate = static_cast<qint16>(toRawValue_b(m_searchYawRate));
    frame.gate_size = static_cast<quint8>(m_gateSize);
    frame.osd_switch = static_cast<quint8>(m_osdSwitch);
    frame.capture_ref_img_cmd = static_cast<quint8>(m_captureRefImgCmd);
    frame.target_altitude = static_cast<qint16>(m_targetAltitude);
    frame.aircraft_pitch = static_cast<qint16>(toRawValue_b(m_aircraftPitch));
    frame.aircraft_yaw = static_cast<qint16>(toRawValue_b(m_aircraftYaw));
    frame.aircraft_roll = static_cast<qint16>(toRawValue_b(m_aircraftRoll));
    frame.focal_length = static_cast<quint16>(m_focalLength);
    frame.pod_pitch_angle = static_cast<qint16>(toRawValue_b(m_podPitchAngle));
    //预留位置
    frame.pod_yaw_angle = static_cast<qint16>(toRawValue_b(m_podYawAngle));
    frame.satellite_map_scale = static_cast<quint8>(toRawValue_a(m_satelliteMapScale));
    frame.pod_type = static_cast<quint8>(m_podType);
    frame.target_longitude = static_cast<qint32>(m_targetLongitude);
    frame.target_latitude = static_cast<qint32>(m_targetLatitude);
    frame.aircraft_longitude = static_cast<qint32>(m_aircraftLongitude);
    frame.aircraft_latitude = static_cast<qint32>(m_aircraftLatitude);
    frame.aircraft_altitude = static_cast<qint16>(m_aircraftAltitude);
    frame.pixel_size = static_cast<quint8>(m_pixelSize);

    uint16_t crc = SerialPortImage::crc16_ccitt_fast(
        reinterpret_cast<const uint8_t*>(&frame), sizeof(frame) - sizeof(uint16_t));
    frame.crc16 = crc;

    return QByteArray(reinterpret_cast<const char*>(&frame), sizeof(frame));
}

// ─────────────────────────────────────────────
// SerialPortImage
// ─────────────────────────────────────────────

SerialPortImage::SerialPortImage(QObject *parent)
    : SerialPort(parent)
    , m_imageData(new ImageData(nullptr))        // 留在主线程，不随 moveToThread 迁移
    , m_imageSendData(new ImageSendData(nullptr))
{
    init_crc16_table();
}

SerialPortImage::~SerialPortImage() {
    delete m_imageData;
    delete m_imageSendData;
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
        return;
    }

    size_t dataLen = sizeof(image_recv_frame) - sizeof(uint16_t);
    uint16_t calculatedCrc = crc16_ccitt_fast(
        reinterpret_cast<const uint8_t*>(rawData.data()), dataLen);
    if (calculatedCrc != pFrame->crc16) {
        // return QByteArray();
        return;
    }

    m_imageData->updateFromFrame(rawData);
    // return rawData;
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
