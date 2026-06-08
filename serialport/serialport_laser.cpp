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
int LaserData::detectorStatus1() const { return m_detectorStatus1; }
int LaserData::detectorStatus2() const { return m_detectorStatus2; }
int LaserData::detectorStatus3() const { return m_detectorStatus3; }
int LaserData::faultInfo() const { return m_faultInfo; }
int LaserData::faultInfo1() const { return m_faultInfo1; }
int LaserData::faultInfo2() const { return m_faultInfo2; }
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
        m_detectorStatus = pFrame->detector_status;   //写位操作函数细分，qml端判断应该显示什么
        //单独定义几个变量并使用位操作赋值
        m_detectorStatus1 = convertAndGetBit(m_detectorStatus,4,4);
        m_detectorStatus2 = convertAndGetBit(m_detectorStatus,2,3);
        m_detectorStatus3 = convertAndGetBit(m_detectorStatus,0,1);
        emit detectorStatusChanged();
        emit detectorStatus1Changed();
        emit detectorStatus2Changed();
        emit detectorStatus3Changed();
    }
    if (m_faultInfo != pFrame->fault_info) {
        m_faultInfo = pFrame->fault_info;             //写位操作函数细分，qml端判断应该显示什么
        m_faultInfo1 = convertAndGetBit(m_faultInfo,6,7);
        m_faultInfo2 = convertAndGetBit(m_faultInfo,0,5);
        emit faultInfoChanged();
        emit faultInfo1Changed();
        emit faultInfo2Changed();
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

    if (m_laserPeriod != pFrame->laser_period*0.002) {
        m_laserPeriod = pFrame->laser_period*0.002;
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

int LaserData::getBitsFromQint8(qint8 value, int startBit, int endBit)
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
// LaserSendData
// ─────────────────────────────────────────────

LaserSendData::LaserSendData(QObject *parent)
    : QObject(parent)
{
}

QByteArray LaserSendData::buildFrame() const
{
    laser_send_frame frame = {};

    frame.frame_header1 = 0x55;
    frame.frame_header2 = 0xAA;
    frame.frame_header3 = 0xDC;
    frame.frame_status = static_cast<quint8>(m_frameStatus);   //帧长为固定值
    frame.frame_ID = static_cast<quint8>(m_frameId);
    frame.cmd = static_cast<quint8>(m_cmd);
    frame.laser_period = static_cast<quint16>(m_laserPeriod / 0.002);

    // 根据命令字选择不同的参数映射
    switch (m_cmd) {
    case 0x02: // 锁定
    case 0x06: // 定轴搜索
    case 0x09: // 定轴搜索(位置回路)
        frame.param2 = static_cast<qint16>(toRawValue(m_azimuthAngle));
        frame.param3 = static_cast<qint16>(toRawValue(m_elevationAngle));
        frame.param4 = 0;
        frame.param5 = 0;
        break;
    case 0x07: // 矩形搜索
    case 0x0A: // 矩形搜索(位置回路)
        frame.param2 = static_cast<qint16>(toRawValue(m_searchCenterAzimuth));
        frame.param3 = static_cast<qint16>(toRawValue(m_searchCenterElevation));
        frame.param4 = static_cast<qint16>(toRawValue(m_azimuthSearchRange));
        frame.param5 = static_cast<qint16>(toRawValue(m_elevationSearchRange));
        break;
    case 0x08: // 圆形搜索
    case 0x0B: // 圆形搜索(位置回路)
        frame.param2 = static_cast<qint16>(toRawValue(m_searchCenterAzimuth));
        frame.param3 = static_cast<qint16>(toRawValue(m_searchCenterElevation));
        frame.param4 = static_cast<qint16>(toRawValue(m_searchRadius));
        frame.param5 = 0;
        break;
    default:
        frame.param2 = static_cast<qint16>(toRawValue(m_azimuthAngle));
        frame.param3 = static_cast<qint16>(toRawValue(m_elevationAngle));
        frame.param4 = 0;
        frame.param5 = 0;
        break;
    }

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
    , m_laserData(new LaserData(nullptr))        // 留在主线程，不随 moveToThread 迁移
    , m_laserSendData(new LaserSendData(nullptr))
{
}

SerialPortLaser::~SerialPortLaser() {
    delete m_laserData;
    delete m_laserSendData;
}

LaserData* SerialPortLaser::laserData() const
{
    return m_laserData;
}

LaserSendData* SerialPortLaser::laserSendData() const
{
    return m_laserSendData;
}
//解析函数返回值可为void类型
void SerialPortLaser::parseData(const QByteArray &rawData)
{
    QByteArray checkdata;
    if (rawData.size() < static_cast<int>(sizeof(laser_recv_frame))) {
        //这里可以给出一个弹窗说明数据接收失败
        return;
        
    }

    const laser_recv_frame* pFrame = reinterpret_cast<const laser_recv_frame*>(rawData.data());
    if (pFrame->frame_header1 != 0x55 || pFrame->frame_header2 != 0xAA || pFrame->frame_header3 != 0xDC) {
        // return QByteArray();     //数据帧错误
        return;
    }
    //截取需要校验的数据
    if (rawData.size() >= 4) {
        checkdata = rawData.mid(3, rawData.size() - 4);
    }

    uint8_t calculatedChecksum = xorChecksum(checkdata);
    if (calculatedChecksum != pFrame->XOR_result) {
        // return QByteArray();
        return;
    }
    
     //检验无误后更新数据并刷新QML界面显示
    m_laserData->updateFromFrame(rawData);    
    // return rawData;
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
