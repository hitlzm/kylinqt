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
float ImageData::pitchLosAngVel() const { return m_pitchLosAngVel; }
float ImageData::yawLosAngVel() const { return m_yawLosAngVel; }
int ImageData::targetBackgroundType() const { return m_targetBackgroundType; }
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
        emit selfCheckFlagChanged();
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
        emit targetBackgroundTypeChanged();
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

// ─────────────────────────────────────────────
// ImageSendData
// ─────────────────────────────────────────────

ImageSendData::ImageSendData(QObject *parent)
    : QObject(parent)
{
}

int ImageSendData::frameLength() const { return m_frameLength; }
void ImageSendData::setFrameLength(int value) { if (m_frameLength != value) { m_frameLength = value; emit frameLengthChanged(); } }
int ImageSendData::aFrameSequence() const { return m_aFrameSequence; }
void ImageSendData::setAFrameSequence(int value) { if (m_aFrameSequence != value) { m_aFrameSequence = value; emit aFrameSequenceChanged(); } }
int ImageSendData::seekerCtrlWord() const { return m_seekerCtrlWord; }
void ImageSendData::setSeekerCtrlWord(int value) { if (m_seekerCtrlWord != value) { m_seekerCtrlWord = value; emit seekerCtrlWordChanged(); } }
int ImageSendData::opticalParamCtrl() const { return m_opticalParamCtrl; }
void ImageSendData::setOpticalParamCtrl(int value) { if (m_opticalParamCtrl != value) { m_opticalParamCtrl = value; emit opticalParamCtrlChanged(); } }
int ImageSendData::templateIndex() const { return m_templateIndex; }
void ImageSendData::setTemplateIndex(int value) { if (m_templateIndex != value) { m_templateIndex = value; emit templateIndexChanged(); } }
int ImageSendData::targetBackgroundType() const { return m_targetBackgroundType; }
void ImageSendData::setTargetBackgroundType(int value) { if (m_targetBackgroundType != value) { m_targetBackgroundType = value; emit targetBackgroundTypeChanged(); } }
int ImageSendData::missileTargetDistance() const { return m_missileTargetDistance; }
void ImageSendData::setMissileTargetDistance(int value) { if (m_missileTargetDistance != value) { m_missileTargetDistance = value; emit missileTargetDistanceChanged(); } }
float ImageSendData::missileSpeed() const { return m_missileSpeed; }
void ImageSendData::setMissileSpeed(float value) { if (m_missileSpeed != value) { m_missileSpeed = value; emit missileSpeedChanged(); } }
float ImageSendData::bodyPitchAngle() const { return m_bodyPitchAngle; }
void ImageSendData::setBodyPitchAngle(float value) { if (m_bodyPitchAngle != value) { m_bodyPitchAngle = value; emit bodyPitchAngleChanged(); } }
float ImageSendData::bodyYawAngle() const { return m_bodyYawAngle; }
void ImageSendData::setBodyYawAngle(float value) { if (m_bodyYawAngle != value) { m_bodyYawAngle = value; emit bodyYawAngleChanged(); } }
float ImageSendData::bodyRollAngle() const { return m_bodyRollAngle; }
void ImageSendData::setBodyRollAngle(float value) { if (m_bodyRollAngle != value) { m_bodyRollAngle = value; emit bodyRollAngleChanged(); } }
float ImageSendData::bodyPitchRate() const { return m_bodyPitchRate; }
void ImageSendData::setBodyPitchRate(float value) { if (m_bodyPitchRate != value) { m_bodyPitchRate = value; emit bodyPitchRateChanged(); } }
float ImageSendData::bodyYawRate() const { return m_bodyYawRate; }
void ImageSendData::setBodyYawRate(float value) { if (m_bodyYawRate != value) { m_bodyYawRate = value; emit bodyYawRateChanged(); } }
float ImageSendData::bodyRollRate() const { return m_bodyRollRate; }
void ImageSendData::setBodyRollRate(float value) { if (m_bodyRollRate != value) { m_bodyRollRate = value; emit bodyRollRateChanged(); } }
float ImageSendData::bodyVelX() const { return m_bodyVelX; }
void ImageSendData::setBodyVelX(float value) { if (m_bodyVelX != value) { m_bodyVelX = value; emit bodyVelXChanged(); } }
float ImageSendData::bodyVelY() const { return m_bodyVelY; }
void ImageSendData::setBodyVelY(float value) { if (m_bodyVelY != value) { m_bodyVelY = value; emit bodyVelYChanged(); } }
float ImageSendData::bodyVelZ() const { return m_bodyVelZ; }
void ImageSendData::setBodyVelZ(float value) { if (m_bodyVelZ != value) { m_bodyVelZ = value; emit bodyVelZChanged(); } }
int ImageSendData::bodyPosX() const { return m_bodyPosX; }
void ImageSendData::setBodyPosX(int value) { if (m_bodyPosX != value) { m_bodyPosX = value; emit bodyPosXChanged(); } }
int ImageSendData::bodyPosY() const { return m_bodyPosY; }
void ImageSendData::setBodyPosY(int value) { if (m_bodyPosY != value) { m_bodyPosY = value; emit bodyPosYChanged(); } }
int ImageSendData::bodyPosZ() const { return m_bodyPosZ; }
void ImageSendData::setBodyPosZ(int value) { if (m_bodyPosZ != value) { m_bodyPosZ = value; emit bodyPosZChanged(); } }
float ImageSendData::pitchGimbalPreset() const { return m_pitchGimbalPreset; }
void ImageSendData::setPitchGimbalPreset(float value) { if (m_pitchGimbalPreset != value) { m_pitchGimbalPreset = value; emit pitchGimbalPresetChanged(); } }
float ImageSendData::yawGimbalPreset() const { return m_yawGimbalPreset; }
void ImageSendData::setYawGimbalPreset(float value) { if (m_yawGimbalPreset != value) { m_yawGimbalPreset = value; emit yawGimbalPresetChanged(); } }
int ImageSendData::irIntegrationTime() const { return m_irIntegrationTime; }
void ImageSendData::setIrIntegrationTime(int value) { if (m_irIntegrationTime != value) { m_irIntegrationTime = value; emit irIntegrationTimeChanged(); } }
int ImageSendData::trackingCorrectionCmd() const { return m_trackingCorrectionCmd; }
void ImageSendData::setTrackingCorrectionCmd(int value) { if (m_trackingCorrectionCmd != value) { m_trackingCorrectionCmd = value; emit trackingCorrectionCmdChanged(); } }
int ImageSendData::correctionFrameNum() const { return m_correctionFrameNum; }
void ImageSendData::setCorrectionFrameNum(int value) { if (m_correctionFrameNum != value) { m_correctionFrameNum = value; emit correctionFrameNumChanged(); } }
int ImageSendData::correctedPitchPos() const { return m_correctedPitchPos; }
void ImageSendData::setCorrectedPitchPos(int value) { if (m_correctedPitchPos != value) { m_correctedPitchPos = value; emit correctedPitchPosChanged(); } }
int ImageSendData::correctedYawPos() const { return m_correctedYawPos; }
void ImageSendData::setCorrectedYawPos(int value) { if (m_correctedYawPos != value) { m_correctedYawPos = value; emit correctedYawPosChanged(); } }
float ImageSendData::searchPitchRate() const { return m_searchPitchRate; }
void ImageSendData::setSearchPitchRate(float value) { if (m_searchPitchRate != value) { m_searchPitchRate = value; emit searchPitchRateChanged(); } }
float ImageSendData::searchYawRate() const { return m_searchYawRate; }
void ImageSendData::setSearchYawRate(float value) { if (m_searchYawRate != value) { m_searchYawRate = value; emit searchYawRateChanged(); } }
int ImageSendData::gateSize() const { return m_gateSize; }
void ImageSendData::setGateSize(int value) { if (m_gateSize != value) { m_gateSize = value; emit gateSizeChanged(); } }
int ImageSendData::osdSwitch() const { return m_osdSwitch; }
void ImageSendData::setOsdSwitch(int value) { if (m_osdSwitch != value) { m_osdSwitch = value; emit osdSwitchChanged(); } }
int ImageSendData::captureRefImgCmd() const { return m_captureRefImgCmd; }
void ImageSendData::setCaptureRefImgCmd(int value) { if (m_captureRefImgCmd != value) { m_captureRefImgCmd = value; emit captureRefImgCmdChanged(); } }
int ImageSendData::targetAltitude() const { return m_targetAltitude; }
void ImageSendData::setTargetAltitude(int value) { if (m_targetAltitude != value) { m_targetAltitude = value; emit targetAltitudeChanged(); } }
float ImageSendData::aircraftPitch() const { return m_aircraftPitch; }
void ImageSendData::setAircraftPitch(float value) { if (m_aircraftPitch != value) { m_aircraftPitch = value; emit aircraftPitchChanged(); } }
float ImageSendData::aircraftYaw() const { return m_aircraftYaw; }
void ImageSendData::setAircraftYaw(float value) { if (m_aircraftYaw != value) { m_aircraftYaw = value; emit aircraftYawChanged(); } }
float ImageSendData::aircraftRoll() const { return m_aircraftRoll; }
void ImageSendData::setAircraftRoll(float value) { if (m_aircraftRoll != value) { m_aircraftRoll = value; emit aircraftRollChanged(); } }
int ImageSendData::focalLength() const { return m_focalLength; }
void ImageSendData::setFocalLength(int value) { if (m_focalLength != value) { m_focalLength = value; emit focalLengthChanged(); } }
float ImageSendData::podPitchAngle() const { return m_podPitchAngle; }
void ImageSendData::setPodPitchAngle(float value) { if (m_podPitchAngle != value) { m_podPitchAngle = value; emit podPitchAngleChanged(); } }
float ImageSendData::podYawAngle() const { return m_podYawAngle; }
void ImageSendData::setPodYawAngle(float value) { if (m_podYawAngle != value) { m_podYawAngle = value; emit podYawAngleChanged(); } }
int ImageSendData::satelliteMapScale() const { return m_satelliteMapScale; }
void ImageSendData::setSatelliteMapScale(int value) { if (m_satelliteMapScale != value) { m_satelliteMapScale = value; emit satelliteMapScaleChanged(); } }
int ImageSendData::podType() const { return m_podType; }
void ImageSendData::setPodType(int value) { if (m_podType != value) { m_podType = value; emit podTypeChanged(); } }
double ImageSendData::targetLongitude() const { return m_targetLongitude; }
void ImageSendData::setTargetLongitude(double value) { if (m_targetLongitude != value) { m_targetLongitude = value; emit targetLongitudeChanged(); } }
double ImageSendData::targetLatitude() const { return m_targetLatitude; }
void ImageSendData::setTargetLatitude(double value) { if (m_targetLatitude != value) { m_targetLatitude = value; emit targetLatitudeChanged(); } }
double ImageSendData::aircraftLongitude() const { return m_aircraftLongitude; }
void ImageSendData::setAircraftLongitude(double value) { if (m_aircraftLongitude != value) { m_aircraftLongitude = value; emit aircraftLongitudeChanged(); } }
double ImageSendData::aircraftLatitude() const { return m_aircraftLatitude; }
void ImageSendData::setAircraftLatitude(double value) { if (m_aircraftLatitude != value) { m_aircraftLatitude = value; emit aircraftLatitudeChanged(); } }
int ImageSendData::aircraftAltitude() const { return m_aircraftAltitude; }
void ImageSendData::setAircraftAltitude(int value) { if (m_aircraftAltitude != value) { m_aircraftAltitude = value; emit aircraftAltitudeChanged(); } }
int ImageSendData::pixelSize() const { return m_pixelSize; }
void ImageSendData::setPixelSize(int value) { if (m_pixelSize != value) { m_pixelSize = value; emit pixelSizeChanged(); } }

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
    frame.target_background_type = static_cast<quint8>(m_targetBackgroundType);
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
    , m_imageData(new ImageData(this))
    , m_imageSendData(new ImageSendData(this))
{
    init_crc16_table();
}

SerialPortImage::~SerialPortImage() = default;

ImageData* SerialPortImage::imageData() const
{
    return m_imageData;
}

ImageSendData* SerialPortImage::imageSendData() const
{
    return m_imageSendData;
}

//数据解析
QByteArray SerialPortImage::parseData(const QByteArray &rawData)
{
    if (rawData.size() < static_cast<int>(sizeof(image_recv_frame))) {
        return QByteArray();
    }

    const image_recv_frame* pFrame = reinterpret_cast<const image_recv_frame*>(rawData.data());

    if (pFrame->frame_header1 != 0x77 || pFrame->frame_header2 != 0xAB) {
        return QByteArray();
    }

    size_t dataLen = sizeof(image_recv_frame) - sizeof(uint16_t);
    uint16_t calculatedCrc = crc16_ccitt_fast(
        reinterpret_cast<const uint8_t*>(rawData.data()), dataLen);
    if (calculatedCrc != pFrame->crc16) {
        return QByteArray();
    }

    m_imageData->updateFromFrame(rawData);
    return rawData;
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
