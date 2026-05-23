#include "serialport_laser.h"

// ─────────────────────────────────────────────
// LaserData
// ─────────────────────────────────────────────

LaserData::LaserData(QObject *parent)
    : QObject(parent)
{
}

int LaserData::frameStatus() const { return m_frameStatus; }
int LaserData::frameId() const { return m_frameId; }
int LaserData::dytStatus() const { return m_dytStatus; }
int LaserData::detectorStatus() const { return m_detectorStatus; }
int LaserData::faultInfo() const { return m_faultInfo; }
float LaserData::opticalAzimuth() const { return m_opticalAzimuth; }
float LaserData::opticalPitch() const { return m_opticalPitch; }
float LaserData::gyroAzimuthRate() const { return m_gyroAzimuthRate; }
float LaserData::gyroPitchRate() const { return m_gyroPitchRate; }
float LaserData::losAzimuthRate() const { return m_losAzimuthRate; }
float LaserData::losPitchRate() const { return m_losPitchRate; }
float LaserData::deviationAzimuth() const { return m_deviationAzimuth; }
float LaserData::deviationPitch() const { return m_deviationPitch; }
int LaserData::laserPeriod() const { return m_laserPeriod; }
int LaserData::gainStatus() const { return m_gainStatus; }
float LaserData::quadrant1Energy() const { return m_quadrant1Energy; }
float LaserData::quadrant2Energy() const { return m_quadrant2Energy; }
float LaserData::quadrant3Energy() const { return m_quadrant3Energy; }
float LaserData::quadrant4Energy() const { return m_quadrant4Energy; }
float LaserData::softwareVersion1() const { return m_softwareVersion1; }
float LaserData::softwareVersion2() const { return m_softwareVersion2; }

void LaserData::updateFromFrame(const QByteArray &frame)
{
    if (frame.size() < static_cast<int>(sizeof(laser_recv_frame))) {
        return;
    }

    const laser_recv_frame* pFrame = reinterpret_cast<const laser_recv_frame*>(frame.data());

    if (m_frameStatus != pFrame->frame_status) {
        m_frameStatus = pFrame->frame_status;
        emit frameStatusChanged();
    }
    if (m_frameId != pFrame->frame_ID) {
        m_frameId = pFrame->frame_ID;
        emit frameIdChanged();
    }

    if (m_dytStatus != pFrame->dyt_status) {
        m_dytStatus = pFrame->dyt_status;
        emit dytStatusChanged();
    }
    if (m_detectorStatus != pFrame->detector_status) {
        m_detectorStatus = pFrame->detector_status;
        emit detectorStatusChanged();
    }
    if (m_faultInfo != pFrame->fault_info) {
        m_faultInfo = pFrame->fault_info;
        emit faultInfoChanged();
    }

    if (m_opticalAzimuth != fromRawValue_a( pFrame->optical_azimuth)) {
        m_opticalAzimuth = fromRawValue_a(pFrame->optical_azimuth);
        emit opticalAzimuthChanged();
    }
    if (m_opticalPitch != fromRawValue_a(pFrame->optical_pitch)) {
        m_opticalPitch = fromRawValue_a(pFrame->optical_pitch);
        emit opticalPitchChanged();
    }

    if (m_gyroAzimuthRate != fromRawValue_a(pFrame->gyro_azimuth_rate)) {
        m_gyroAzimuthRate = fromRawValue_a(pFrame->gyro_azimuth_rate);
        emit gyroAzimuthRateChanged();
    }
    if (m_gyroPitchRate != fromRawValue_a(pFrame->gyro_pitch_rate)) {
        m_gyroPitchRate = fromRawValue_a(pFrame->gyro_pitch_rate);
        emit gyroPitchRateChanged();
    }

    if (m_losAzimuthRate != fromRawValue_a(pFrame->los_azimuth_rate)) {
        m_losAzimuthRate = fromRawValue_a(pFrame->los_azimuth_rate);
        emit losAzimuthRateChanged();
    }
    if (m_losPitchRate != fromRawValue_a(pFrame->los_pitch_rate)) {
        m_losPitchRate = fromRawValue_a(pFrame->los_pitch_rate);
        emit losPitchRateChanged();
    }

    if (m_deviationAzimuth != fromRawValue_b(pFrame->deviation_azimuth)) {
        m_deviationAzimuth = fromRawValue_b(pFrame->deviation_azimuth);
        emit deviationAzimuthChanged();
    }
    if (m_deviationPitch != fromRawValue_b(pFrame->deviation_pitch)) {
        m_deviationPitch = fromRawValue_b(pFrame->deviation_pitch);
        emit deviationPitchChanged();
    }

    if (m_laserPeriod != pFrame->laser_period) {
        m_laserPeriod = pFrame->laser_period;
        emit laserPeriodChanged();
    }

    if (m_gainStatus != pFrame->gain_status) {
        m_gainStatus = pFrame->gain_status;
        emit gainStatusChanged();
    }

    if (m_quadrant1Energy != fromRawValue_a(pFrame->quadrant1_energy)) {
        m_quadrant1Energy = fromRawValue_a(pFrame->quadrant1_energy);
        emit quadrant1EnergyChanged();
    }
    if (m_quadrant2Energy != fromRawValue_a(pFrame->quadrant2_energy)) {
        m_quadrant2Energy = fromRawValue_a(pFrame->quadrant2_energy);
        emit quadrant2EnergyChanged();
    }
    if (m_quadrant3Energy != fromRawValue_a(pFrame->quadrant3_energy)) {
        m_quadrant3Energy = fromRawValue_a(pFrame->quadrant3_energy);
        emit quadrant3EnergyChanged();
    }
    if (m_quadrant4Energy != fromRawValue_a(pFrame->quadrant4_energy)) {
        m_quadrant4Energy = fromRawValue_a(pFrame->quadrant4_energy);
        emit quadrant4EnergyChanged();
    }

    if (m_softwareVersion1 != fromRawValue_a(pFrame->software_version1)) {
        m_softwareVersion1 = fromRawValue_a(pFrame->software_version1);
        emit softwareVersion1Changed();
    }
    if (m_softwareVersion2 != fromRawValue_a(pFrame->software_version2)) {
        m_softwareVersion2 = fromRawValue_a(pFrame->software_version2);
        emit softwareVersion2Changed();
    }
}

// ─────────────────────────────────────────────
// LaserSendData
// ─────────────────────────────────────────────

LaserSendData::LaserSendData(QObject *parent)
    : QObject(parent)
{
}

int LaserSendData::frameStatus() const { return m_frameStatus; }
void LaserSendData::setFrameStatus(int value) {
    if (m_frameStatus != value) {
        m_frameStatus = value;
        emit frameStatusChanged();
    }
}
int LaserSendData::frameId() const { return m_frameId; }
void LaserSendData::setFrameId(int value) {
    if (m_frameId != value) {
        m_frameId = value;
        emit frameIdChanged();
    }
}
int LaserSendData::cmd() const { return m_cmd; }
void LaserSendData::setCmd(int value) {
    if (m_cmd != value) {
        m_cmd = value;
        emit cmdChanged();
    }
}
int LaserSendData::laserPeriod() const { return m_laserPeriod; }
void LaserSendData::setLaserPeriod(int value) {
    if (m_laserPeriod != value) {
        m_laserPeriod = value;
        emit laserPeriodChanged();
    }
}
float LaserSendData::param2() const { return m_param2; }
void LaserSendData::setParam2(float value) {
    if (m_param2 != value) {
        m_param2 = value;
        emit param2Changed();
    }
}
float LaserSendData::param3() const { return m_param3; }
void LaserSendData::setParam3(float value) {
    if (m_param3 != value) {
        m_param3 = value;
        emit param3Changed();
    }
}
float LaserSendData::param4() const { return m_param4; }
void LaserSendData::setParam4(float value) {
    if (m_param4 != value) {
        m_param4 = value;
        emit param4Changed();
    }
}
float LaserSendData::param5() const { return m_param5; }
void LaserSendData::setParam5(float value) {
    if (m_param5 != value) {
        m_param5 = value;
        emit param5Changed();
    }
}

QByteArray LaserSendData::buildFrame() const
{
    laser_send_frame frame = {};

    frame.frame_header1 = 0x55;
    frame.frame_header2 = 0xAA;
    frame.frame_header3 = 0xDC;
    frame.frame_status = static_cast<quint8>(m_frameStatus);
    frame.frame_ID = static_cast<quint8>(m_frameId);
    frame.cmd = static_cast<quint8>(m_cmd);
    frame.laser_period = static_cast<quint16>(m_laserPeriod);
    frame.param2 = static_cast<qint16>(toRawValue(m_param2));
    frame.param3 = static_cast<qint16>(toRawValue(m_param3));
    frame.param4 = static_cast<qint16>(toRawValue(m_param4));
    frame.param5 = static_cast<qint16>(toRawValue(m_param5));

    const uint8_t* data = reinterpret_cast<const uint8_t*>(&frame);
    uint8_t checksum = 0;
    for (size_t i = 0; i < sizeof(frame) - 1; ++i) {
        checksum ^= data[i];
    }
    frame.XOR_result = checksum;

    return QByteArray(reinterpret_cast<const char*>(&frame), sizeof(frame));
}

// ─────────────────────────────────────────────
// SerialPortLaser
// ─────────────────────────────────────────────

SerialPortLaser::SerialPortLaser(QObject *parent)
    : SerialPort(parent)
    , m_laserData(new LaserData(this))
    , m_laserSendData(new LaserSendData(this))
{
}

SerialPortLaser::~SerialPortLaser() = default;

LaserData* SerialPortLaser::laserData() const
{
    return m_laserData;
}

LaserSendData* SerialPortLaser::laserSendData() const
{
    return m_laserSendData;
}

QByteArray SerialPortLaser::parseData(const QByteArray &rawData)
{
    QByteArray checkdata;
    if (rawData.size() < static_cast<int>(sizeof(laser_recv_frame))) {
        return QByteArray();
    }

    const laser_recv_frame* pFrame = reinterpret_cast<const laser_recv_frame*>(rawData.data());
    if (pFrame->frame_header1 != 0x55 || pFrame->frame_header2 != 0xAA || pFrame->frame_header3 != 0xDC) {
        return QByteArray();
    }

    if (rawData.size() >= 4) {
        checkdata = rawData.mid(3, rawData.size() - 4);
    }

    uint8_t calculatedChecksum = xorChecksum(checkdata);
    if (calculatedChecksum != pFrame->XOR_result) {
        return QByteArray();
    }

    m_laserData->updateFromFrame(rawData);
    return rawData;
}

uint8_t SerialPortLaser::xorChecksumcore(const uint8_t* data, size_t len)
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < len; ++i) {
        checksum ^= data[i];
    }
    return checksum;
}

uint8_t SerialPortLaser::xorChecksum(const QByteArray& data)
{
    return xorChecksumcore(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}
