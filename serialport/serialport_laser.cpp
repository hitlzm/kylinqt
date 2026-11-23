#include "serialport_laser.h"
#include <QDebug>
#include <QString>
#include "../log/LogManager.h"
#include <QTimer>
#include <QTimeZone>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QDir>
#include <QUrl>
#include <QCoreApplication>
#include <cstring>
#include "../utils/sendlatencyprobe.h"
// ─────────────────────────────────────────────
// 外引导模式常量
// ─────────────────────────────────────────────
#define ExguideMode     0
#define ExguideSrcLaser 4
#define Exguide_20ms    6
#define Exguide_1s      7
// ─────────────────────────────────────────────
// 大端字节序工具：只在“位模式”层面解释字节顺序，不做任何数值缩放
// ─────────────────────────────────────────────
namespace {

// 16 位无符号位模式 -> 有符号值：按二进制补码解释
// 负数用 raw - 2^16 计算，避免把取值可能超过 qint16 范围的 int 直接窄化赋值
//（C++20 之前该转换为实现定义行为，本工程为 C++14）。
inline qint16 toSigned16(quint16 raw)
{
    if (raw <= 0x7FFFu)                                             // 0x0000~0x7FFF：非负
        return static_cast<qint16>(raw);
    return static_cast<qint16>(static_cast<qint32>(raw) - 0x10000);  // 0x8000~0xFFFF：负数
}

// 大端字节流 -> 16 位无符号值
// quint16 参与运算前先整型提升为 int，移位结果最大 0xFFFF，落在 int 范围内，不存在移位溢出。
inline quint16 readBigEndianU16(const uint8_t *p)
{
    return static_cast<quint16>((static_cast<quint16>(p[0]) << 8)
                                | static_cast<quint16>(p[1]));
}

// 大端字节流 -> 16 位有符号值：先按 quint16 组装，再按补码解释
inline qint16 readBigEndianI16(const uint8_t *p)
{
    return toSigned16(readBigEndianU16(p));
}

// ─────────────────────────────────────────────
// 时间戳工具（与图像导引头保持一致的做法）
// ─────────────────────────────────────────────
// 取当前北京时间(UTC+8，本机时钟按北京时间显示)并换算为 UTC 毫秒时间戳
// (1970-01-01 00:00:00 UTC 起算)：先把本地“墙上时间”按 UTC 解释，再减去 8 小时固定偏差，
// 这样无论系统时区是否已配成 Asia/Shanghai，结果都是 UTC 绝对毫秒时间。
inline quint64 beijingNowUtcMs()
{
    const QDateTime local = QDateTime::currentDateTime();
    const QDateTime wallClockAsUtc(local.date(), local.time(), Qt::UTC);
    const qint64 ms = wallClockAsUtc.toMSecsSinceEpoch() - 8LL * 3600LL * 1000LL;
    return static_cast<quint64>(ms > 0 ? ms : 0);
}

// 8字节时间戳按大端(高字节在前)写入/读出：激光帧的其它多字节字段也是大端传输，保持一致
inline void writeBigEndianU64(uint8_t *dst, quint64 value)
{
    for (int i = 0; i < 8; ++i)
        dst[i] = static_cast<uint8_t>((value >> (8 * (7 - i))) & 0xFFu);
}

inline quint64 readBigEndianU64(const uint8_t *p)
{
    quint64 value = 0;
    for (int i = 0; i < 8; ++i)
        value = (value << 8) | static_cast<quint64>(p[i]);
    return value;
}

} // namespace

static QString protocolProfileLocalPath(const QString &profileLocation)
{
    const QString trimmed = profileLocation.trimmed();
    if (trimmed.isEmpty())
        return QString();

    const QUrl url = QUrl::fromUserInput(trimmed);
    if (url.isLocalFile())
        return QDir::cleanPath(url.toLocalFile());
    return QDir::cleanPath(trimmed);
}

// ─────────────────────────────────────────────
// LaserData
// ─────────────────────────────────────────────

LaserData::LaserData(QObject *parent)
    : QObject(parent)
{
}

// ── QML 调用方法：emit 请求信号，由 main.cpp 的 QueuedConnection 转发到工作线程 ──
void LaserData::openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void LaserData::closePort()                                     { emit requestClosePort(); }
void LaserData::scanPorts()                                     { emit requestScanPorts(); }
void LaserData::loadProtocolProfile(const QString &profileLocation)
{
    const QString path = protocolProfileLocalPath(profileLocation);
    if (path.isEmpty()) {
        applyProtocolProfileResult(false, QString(),
                                   QStringLiteral("未选择协议 JSON 文件"));
        return;
    }
    emit requestLoadProtocolProfile(path);
}
void LaserData::restoreDefaultProtocolProfile()
{
    emit requestRestoreDefaultProtocolProfile();
}
// void LaserData::sendData(const QByteArray &data)                { emit requestSendData(data); }

// ── 工作线程回推：QueuedConnection 调用，更新状态并 NOTIFY QML ──
void LaserData::setPortOpen(bool open)
{
    if (m_portOpen != open) {
        m_portOpen = open;
        emit portOpenChanged();
    }
    if (!open) {
        m_previousValidTimeStampUs = 0;
        if (m_timeScaleAccuracyValid) {
            m_timeScaleAccuracyValid = false;
            emit timeScaleAccuracyValidChanged();
        }
    }
}
void LaserData::setPortList(const QStringList &ports) { if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); } }
void LaserData::setError(const QString &msg) { if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); } }

void LaserData::applyProtocolProfileResult(bool success,
                                           const QString &profileId,
                                           const QString &errorMessage)
{
    if (success && m_protocolProfileId != profileId) {
        m_protocolProfileId = profileId;
        emit protocolProfileIdChanged();
    }
    emit protocolProfileLoadResult(success, success ? profileId : QString(),
                                   success ? QString() : errorMessage);
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
int LaserData::dytfault1() const { return m_dytfault1; }
int LaserData::dytfault2() const { return m_dytfault2; }
int LaserData::dytfault3() const { return m_dytfault3; }
int LaserData::dytfault4() const { return m_dytfault4; }
int LaserData::dytfault5() const { return m_dytfault5; }
int LaserData::dytfault6() const { return m_dytfault6; }
float LaserData::opticalAzimuth() const { return m_opticalAzimuth; }
float LaserData::opticalPitch() const { return m_opticalPitch; }
float LaserData::gyroAzimuthRate() const { return m_gyroAzimuthRate; }
float LaserData::gyroPitchRate() const { return m_gyroPitchRate; }
float LaserData::losAzimuthRate() const { return m_losAzimuthRate; }
float LaserData::losPitchRate() const { return m_losPitchRate; }
float LaserData::deviationAzimuth() const { return m_deviationAzimuth; }
float LaserData::deviationPitch() const { return m_deviationPitch; }
double LaserData::laserPeriod() const { return m_laserPeriod; }
int LaserData::gainStatus() const { return m_gainStatus; }
float LaserData::quadrant1Energy() const { return m_quadrant1Energy; }
float LaserData::quadrant2Energy() const { return m_quadrant2Energy; }
float LaserData::quadrant3Energy() const { return m_quadrant3Energy; }
float LaserData::quadrant4Energy() const { return m_quadrant4Energy; }
float LaserData::softwareVersion1() const { return m_softwareVersion1; }
float LaserData::softwareVersion2() const { return m_softwareVersion2; }
qint64 LaserData::msTime() const { return m_msTime; }
int LaserData::usTime() const { return m_usTime; }
qint64 LaserData::timeStampUs() const { return m_timeStampUs; }
qint64 LaserData::timeScaleAccuracyUs() const { return m_timeScaleAccuracyUs; }
bool LaserData::timeScaleAccuracyValid() const { return m_timeScaleAccuracyValid; }
QDateTime LaserData::recvDateTime() const { return m_recvDateTime; }
QString LaserData::recvTimeText() const { return m_recvTimeText; }
QString LaserData::protocolProfileId() const { return m_protocolProfileId; }
QString LaserData::trackingStatusName() const { return m_trackingStatusName; }
QString LaserData::trackingStatusText() const { return m_trackingStatusText; }
bool LaserData::detectorPeriodConfigured() const { return m_detectorPeriodConfigured; }
int LaserData::detectorSelfTestState() const { return m_detectorSelfTestState; }
QString LaserData::detectorSelfTestText() const { return m_detectorSelfTestText; }
int LaserData::detectorSignalState() const { return m_detectorSignalState; }
QString LaserData::detectorSignalText() const { return m_detectorSignalText; }
int LaserData::faultStatusState() const { return m_faultStatusState; }
QString LaserData::faultStatusText() const { return m_faultStatusText; }
QString LaserData::faultCodeText() const { return m_faultCodeText; }
int LaserData::faultFlags() const { return m_faultFlags; }

// 本函数由 SerialPortLaser::parseData() 完成帧头/异或校验与大端转小端后经信号触发
void LaserData::updateFromFrame(const laser_recv_frame &pFrame)
{
    
    // const laser_recv_frame* pFrame = reinterpret_cast<const laser_recv_frame*>(frame.data());
    // laser_recv_frame pFrame{};
    // memcpy(&pFrame, frame.constData(), sizeof(frame));

    if (m_frameStatus != pFrame.frame_status) {
        m_frameStatus = pFrame.frame_status;
        emit frameStatusChanged();
    }
    //帧ID不用判断
    if (m_frameId != pFrame.frame_ID) {
        m_frameId = pFrame.frame_ID;
        emit frameIdChanged();
    }

    if (m_dytStatus != pFrame.dyt_status) {
        m_dytStatus = pFrame.dyt_status;
        emit dytStatusChanged();
    }
    if (m_detectorStatus != pFrame.detector_status) {
        m_detectorStatus = pFrame.detector_status;   //写位操作函数细分，qml端判断应该显示什么
        //单独定义几个变量并使用位操作赋值
        m_detectorStatus1 = getBitsFromQint8(m_detectorStatus,4,4);
        m_detectorStatus2 = getBitsFromQint8(m_detectorStatus,2,3);
        m_detectorStatus3 = getBitsFromQint8(m_detectorStatus,0,1);
        emit detectorStatusChanged();
        emit detectorStatus1Changed();
        emit detectorStatus2Changed();
        emit detectorStatus3Changed();
    }
    if (m_faultInfo != pFrame.fault_info) {
        m_faultInfo = pFrame.fault_info;             //写位操作函数细分，qml端判断应该显示什么
        m_faultInfo1 = getBitsFromQint8(m_faultInfo,6,7);
        m_faultInfo2 = getBitsFromQint8(m_faultInfo,0,5);
        // 故障码(bit5~bit0)按位展开为6路DYT故障标志：bit0→dytfault1 ... bit5→dytfault6
        m_dytfault1 = getBitsFromQint8(m_faultInfo,0,0);
        m_dytfault2 = getBitsFromQint8(m_faultInfo,1,1);
        m_dytfault3 = getBitsFromQint8(m_faultInfo,2,2);
        m_dytfault4 = getBitsFromQint8(m_faultInfo,3,3);
        m_dytfault5 = getBitsFromQint8(m_faultInfo,4,4);
        m_dytfault6 = getBitsFromQint8(m_faultInfo,5,5);
        emit faultInfoChanged();
        emit faultInfo1Changed();
        emit faultInfo2Changed();
        emit dytfault1Changed();
        emit dytfault2Changed();
        emit dytfault3Changed();
        emit dytfault4Changed();
        emit dytfault5Changed();
        emit dytfault6Changed();
    }
    auto opticalAzimuth = fromRawValue_a(pFrame.optical_azimuth);
    if (m_opticalAzimuth != opticalAzimuth) {
        m_opticalAzimuth = opticalAzimuth;
        emit opticalAzimuthChanged();
    }
    auto opticalPitch = fromRawValue_a(pFrame.optical_pitch);
    if (m_opticalPitch != opticalPitch) {
        m_opticalPitch = opticalPitch;
        emit opticalPitchChanged();
    }
    auto gyroAzimuthRate = fromRawValue_a(pFrame.gyro_azimuth_rate);
    if (m_gyroAzimuthRate != gyroAzimuthRate) {
        m_gyroAzimuthRate = gyroAzimuthRate;
        emit gyroAzimuthRateChanged();
    }
    auto gyro_pitch_rate = fromRawValue_a(pFrame.gyro_pitch_rate);
    if (m_gyroPitchRate != gyro_pitch_rate) {
        m_gyroPitchRate = gyro_pitch_rate;
        emit gyroPitchRateChanged();
    }
    auto los_azimuth_rate = fromRawValue_a(pFrame.los_azimuth_rate);
    if (m_losAzimuthRate != los_azimuth_rate) {
        m_losAzimuthRate = los_azimuth_rate;
        emit losAzimuthRateChanged();
    }
    auto los_pitch_rate = fromRawValue_a(pFrame.los_pitch_rate);
    if (m_losPitchRate != los_pitch_rate) {
        m_losPitchRate = los_pitch_rate;
        emit losPitchRateChanged();
    }
    auto deviation_azimuth = fromRawValue_b(pFrame.deviation_azimuth);
    if (m_deviationAzimuth != deviation_azimuth) {
        m_deviationAzimuth = deviation_azimuth;
        emit deviationAzimuthChanged();
    }
    auto deviation_pitch = fromRawValue_b(pFrame.deviation_pitch);
    if (m_deviationPitch != deviation_pitch) {
        m_deviationPitch = deviation_pitch;
        emit deviationPitchChanged();
    }

    if (m_laserPeriod != pFrame.laser_period*0.002) {
        m_laserPeriod = pFrame.laser_period*0.002;
        emit laserPeriodChanged();
    }

    if (m_gainStatus != pFrame.gain_status) {
        m_gainStatus = pFrame.gain_status;
        emit gainStatusChanged();
    }
    auto quadrant1_energy = fromRawValue_a(pFrame.quadrant1_energy);
    if (m_quadrant1Energy != quadrant1_energy) {
        m_quadrant1Energy = quadrant1_energy;
        emit quadrant1EnergyChanged();
    }
    auto quadrant2_energy = fromRawValue_a(pFrame.quadrant2_energy);
    if (m_quadrant2Energy != quadrant2_energy) {
        m_quadrant2Energy = quadrant2_energy;
        emit quadrant2EnergyChanged();
    }
    auto quadrant3_energy = fromRawValue_a(pFrame.quadrant3_energy);
    if (m_quadrant3Energy != quadrant3_energy) {
        m_quadrant3Energy = quadrant3_energy;
        emit quadrant3EnergyChanged();
    }
    auto quadrant4_energy = fromRawValue_a(pFrame.quadrant4_energy);
    if (m_quadrant4Energy != quadrant4_energy) {
        m_quadrant4Energy = quadrant4_energy;
        emit quadrant4EnergyChanged();
    }
    auto software_version1 = fromRawValue_a(pFrame.software_version1);
    if (m_softwareVersion1 != software_version1) {
        m_softwareVersion1 = software_version1;
        emit softwareVersion1Changed();
    }
    auto software_version2 = fromRawValue_a(pFrame.software_version2);
    if (m_softwareVersion2 != software_version2) {
        m_softwareVersion2 = software_version2;
        emit softwareVersion2Changed();
    }

    // 导引头返回时间戳：毫秒部分(8字节大端) + 微秒部分(0~999)
    const qint64 msTime = static_cast<qint64>(
                readBigEndianU64(reinterpret_cast<const uint8_t*>(pFrame.ms_time)));
    const int usTime = static_cast<int>(pFrame.us_time);
    const qint64 timeStampUs = msTime * 1000LL + usTime;
    if (m_msTime != msTime) {
        m_msTime = msTime;
        emit msTimeChanged();
    }
    if (m_usTime != usTime) {
        m_usTime = usTime;
        emit usTimeChanged();
    }
    if (m_timeStampUs != timeStampUs) {
        m_timeStampUs = timeStampUs;
        emit timeStampUsChanged();
    }

    // 仅在连续两组非零返回时标之间计算时标精度。
    // 协议标称帧周期为20000us，因此精度 = 相邻时标差(us) - 20000us。
    bool accuracyValid = false;
    qint64 accuracyUs = 0;
    if (msTime > 0) {
        if (m_previousValidTimeStampUs > 0) {
            accuracyUs = timeStampUs - m_previousValidTimeStampUs - 20000;
            accuracyValid = true;
        }
        m_previousValidTimeStampUs = timeStampUs;
    } else {
        // 未时间同步或返回时标为0时，不能跨过该帧继续计算。
        m_previousValidTimeStampUs = 0;
    }
    if (m_timeScaleAccuracyUs != accuracyUs) {
        m_timeScaleAccuracyUs = accuracyUs;
        emit timeScaleAccuracyUsChanged();
    }
    if (m_timeScaleAccuracyValid != accuracyValid) {
        m_timeScaleAccuracyValid = accuracyValid;
        emit timeScaleAccuracyValidChanged();
    }

    // 把返回时间戳换算成时分秒保存：
    // 时间戳按 UTC 毫秒解释(与发送侧同一基准)，再折算成北京时间(UTC+8)的“时:分:秒.毫秒微秒”
    if (msTime > 0) {
        const QDateTime recvUtc = QDateTime::fromMSecsSinceEpoch(msTime, Qt::UTC);
        // 用固定偏移时区做UTC→北京时间换算，避免Qt5.12在Windows上访问时区库出问题
        const QDateTime recvBj = recvUtc.toTimeZone(QTimeZone(8 * 3600));
        if (m_recvDateTime != recvUtc) {
            m_recvDateTime = recvUtc;
            emit recvDateTimeChanged();
        }
        //秒的小数部分：毫秒(3位) + 微秒(3位)，合计微秒精度，例如 2026-09-24 11:01:57.054321
        const QString recvText = recvBj.toString("HH:mm:ss.zzz")
                               + QString("%1").arg(usTime, 3, 10, QLatin1Char('0'));
        if (m_recvTimeText != recvText) {
            m_recvTimeText = recvText;
            emit recvTimeTextChanged();
        }
    }

    // 本帧各字段已全部更新，通知消费者（数据保存等）按同一份解析结果取用
    emit frameParsed();
}

void LaserData::updateFromCanonicalFrame(const CanonicalSeekerFrame &frame)
{
    // 过渡兼容：旧 QML 仍读取 LaserData 的原属性。这里只做内存 DTO 映射，
    // 不重新序列化为旧帧，也不会再次经过旧协议解析器。
    laser_recv_frame legacy = {};
    if (frame.rawFrame.size() >= 3) {
        legacy.frame_header1 = static_cast<quint8>(frame.rawFrame.at(0));
        legacy.frame_header2 = static_cast<quint8>(frame.rawFrame.at(1));
        legacy.frame_header3 = static_cast<quint8>(frame.rawFrame.at(2));
    }
    legacy.frame_status = static_cast<quint8>(
                frame.rawInteger(QStringLiteral("frameStatus")));
    legacy.frame_ID = static_cast<quint8>(
                frame.rawInteger(QStringLiteral("frameId")));
    legacy.dyt_status = static_cast<quint8>(
                frame.rawInteger(QStringLiteral("dytStatus"),
                                 frame.canonicalInteger(
                                     QStringLiteral("tracking.stateRaw"))));
    legacy.detector_status = static_cast<quint8>(
                frame.rawInteger(QStringLiteral("detectorStatus")));
    legacy.fault_info = static_cast<quint8>(
                frame.rawInteger(QStringLiteral("faultInfo")));
    legacy.optical_azimuth = static_cast<qint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.losAzimuthDeg"))
                / 0.01));
    legacy.optical_pitch = static_cast<qint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.losElevationDeg"))
                / 0.01));
    legacy.gyro_azimuth_rate = static_cast<qint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.gyroAzimuthRateDegPerSec"))
                / 0.01));
    legacy.gyro_pitch_rate = static_cast<qint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.gyroElevationRateDegPerSec"))
                / 0.01));
    legacy.los_azimuth_rate = static_cast<qint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.losAzimuthRateDegPerSec"))
                / 0.01));
    legacy.los_pitch_rate = static_cast<qint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.losElevationRateDegPerSec"))
                / 0.01));
    legacy.deviation_azimuth = static_cast<qint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.deviationAzimuthDeg"))
                / 0.001));
    legacy.deviation_pitch = static_cast<qint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.deviationElevationDeg"))
                / 0.001));
    legacy.laser_period = static_cast<quint16>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.laserPeriodMs"))
                / 0.002));
    legacy.gain_status = static_cast<quint8>(
                frame.rawInteger(QStringLiteral("gainStatus")));
    legacy.quadrant1_energy = static_cast<quint8>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.quadrant1Energy"))
                / 0.01));
    legacy.quadrant2_energy = static_cast<quint8>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.quadrant2Energy"))
                / 0.01));
    legacy.quadrant3_energy = static_cast<quint8>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.quadrant3Energy"))
                / 0.01));
    legacy.quadrant4_energy = static_cast<quint8>(qRound(
                frame.canonicalNumber(QStringLiteral("measurement.quadrant4Energy"))
                / 0.01));
    legacy.software_version1 = static_cast<quint16>(qRound(
                frame.canonicalNumber(QStringLiteral("version.processing"))
                / 0.01));
    legacy.software_version2 = static_cast<quint16>(qRound(
                frame.canonicalNumber(QStringLiteral("version.control"))
                / 0.01));

    const qint64 deviceTimeMs = static_cast<qint64>(
                frame.canonicalNumber(QStringLiteral("time.deviceUtcMs")));
    for (int i = 0; i < 8; ++i) {
        legacy.ms_time[i] = static_cast<quint8>(
                    (static_cast<quint64>(deviceTimeMs)
                     >> (8 * (7 - i))) & 0xFFu);
    }
    legacy.us_time = static_cast<quint16>(
                frame.canonicalInteger(
                    QStringLiteral("time.deviceSubMillisecondUs")));

    const QString profileId = frame.profileId;
    if (m_protocolProfileId != profileId) {
        m_protocolProfileId = profileId;
        emit protocolProfileIdChanged();
    }
    const QString trackingName = frame.canonicalText(
                QStringLiteral("tracking.state"), QStringLiteral("unknown"));
    if (m_trackingStatusName != trackingName) {
        m_trackingStatusName = trackingName;
        emit trackingStatusNameChanged();
    }
    const QString trackingText = frame.canonicalText(
                QStringLiteral("tracking.stateLabel"), QStringLiteral("未知"));
    if (m_trackingStatusText != trackingText) {
        m_trackingStatusText = trackingText;
        emit trackingStatusTextChanged();
    }
    const bool detectorPeriodConfigured = frame.canonicalBoolean(
                QStringLiteral("status.detectorPeriodConfigured"));
    if (m_detectorPeriodConfigured != detectorPeriodConfigured) {
        m_detectorPeriodConfigured = detectorPeriodConfigured;
        emit detectorPeriodConfiguredChanged();
    }
    const int selfTestState = static_cast<int>(frame.canonicalInteger(
                QStringLiteral("status.detectorSelfTestRaw"), -1));
    if (m_detectorSelfTestState != selfTestState) {
        m_detectorSelfTestState = selfTestState;
        emit detectorSelfTestStateChanged();
    }
    const QString selfTestText = frame.canonicalText(
                QStringLiteral("status.detectorSelfTestLabel"),
                QStringLiteral("NA"));
    if (m_detectorSelfTestText != selfTestText) {
        m_detectorSelfTestText = selfTestText;
        emit detectorSelfTestTextChanged();
    }
    const int signalState = static_cast<int>(frame.canonicalInteger(
                QStringLiteral("status.detectorSignalRaw"), -1));
    if (m_detectorSignalState != signalState) {
        m_detectorSignalState = signalState;
        emit detectorSignalStateChanged();
    }
    const QString signalText = frame.canonicalText(
                QStringLiteral("status.detectorSignalLabel"), QStringLiteral("未知"));
    if (m_detectorSignalText != signalText) {
        m_detectorSignalText = signalText;
        emit detectorSignalTextChanged();
    }
    const int faultState = static_cast<int>(frame.canonicalInteger(
                QStringLiteral("status.faultStateRaw"), -1));
    if (m_faultStatusState != faultState) {
        m_faultStatusState = faultState;
        emit faultStatusStateChanged();
    }
    const QString faultText = frame.canonicalText(
                QStringLiteral("status.faultStateLabel"), QStringLiteral("NA"));
    if (m_faultStatusText != faultText) {
        m_faultStatusText = faultText;
        emit faultStatusTextChanged();
    }
    const qint64 faultCode = frame.canonicalInteger(
                QStringLiteral("status.faultCode"), 0);
    const QString faultCodeText = faultState == 2
            ? QStringLiteral("0x%1").arg(faultCode, 2, 16, QLatin1Char('0')).toUpper()
            : QStringLiteral("NA");
    if (m_faultCodeText != faultCodeText) {
        m_faultCodeText = faultCodeText;
        emit faultCodeTextChanged();
    }
    int faultFlags = 0;
    static const char *faultPaths[] = {
        "status.axisFault.azimuthGyro",
        "status.axisFault.elevationGyro",
        "status.axisFault.azimuthMotor",
        "status.axisFault.elevationMotor",
        "status.axisFault.resolver",
        "status.axisFault.other"
    };
    for (int bit = 0; bit < 6; ++bit) {
        if (frame.canonicalBoolean(QString::fromLatin1(faultPaths[bit])))
            faultFlags |= (1 << bit);
    }
    if (m_faultFlags != faultFlags) {
        m_faultFlags = faultFlags;
        emit faultFlagsChanged();
    }

    updateFromFrame(legacy);
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
    //每10分钟自动向导引头发送一次时间同步帧：置位时间同步信号 → 构帧发送 → 再复位
    const int timeSyncIntervalMs = 10 * 60 * 1000;   //10分钟
    m_timeSyncTimer = new QTimer(this);
    m_timeSyncTimer->setTimerType(Qt::CoarseTimer);
    m_timeSyncTimer->setInterval(timeSyncIntervalMs);
    connect(m_timeSyncTimer, &QTimer::timeout, this, &LaserSendData::sendTimeSyncFrame);
    m_timeSyncTimer->start();
}

void LaserSendData::sendTimeSyncFrame()
{
    m_timeSync = 1;                 //时间同步信号置1
    buildFrame();                   //构帧并发送（time 字段为当拍北京时间换算的UTC毫秒时间戳）
    m_timeSync = 0;                 //发完立即复位，避免后续人工发送的帧都带时间同步标志
}

void LaserSendData::setCommand(const QString &command)
{
    const QString normalized = command.trimmed();
    if (m_command == normalized)
        return;
    m_command = normalized;

    // m_cmd 仅为旧界面兼容保留；线上命令码由协议配置决定。
    static const QMap<QString, int> legacyCodes = {
        {QStringLiteral("none"), 0x00},
        {QStringLiteral("selfTest"), 0x01},
        {QStringLiteral("lock"), 0x02},
        {QStringLiteral("setLaserPeriod"), 0x03},
        {QStringLiteral("fixedSearch"), 0x06},
        {QStringLiteral("rectangleSearch"), 0x07},
        {QStringLiteral("circleSearch"), 0x08},
        {QStringLiteral("fixedSearchPosition"), 0x09},
        {QStringLiteral("rectangleSearchPosition"), 0x0A},
        {QStringLiteral("circleSearchPosition"), 0x0B}
    };
    const int legacyCode = legacyCodes.value(m_command, -1);
    if (legacyCode >= 0 && m_cmd != legacyCode) {
        m_cmd = legacyCode;
        emit cmdChanged();
    }
    emit commandChanged();
}

void LaserSendData::buildFrame() 
{
    // 发送延迟探针：只统计人工点击发送按钮。
    // 时间同步帧由 10 分钟定时器触发（m_timeSync != 0），不是人工操作，不打点。
    if (!m_timeSync) SendLat::markClick(SendLat::Laser);

    ProtocolCommandRequest request;
    request.specName = QStringLiteral("laser.control");
    request.semanticCommand = m_command;
    request.values.insert(QStringLiteral("frameStatus"), m_frameStatus);
    request.values.insert(QStringLiteral("frameId"), m_frameId);
    request.values.insert(QStringLiteral("laserPeriod"),
                          static_cast<double>(m_laserPeriod));

    // 根据命令字选择不同的参数映射
    if (m_command == QStringLiteral("lock")
            || m_command == QStringLiteral("fixedSearch")
            || m_command == QStringLiteral("fixedSearchPosition")) {
        request.values.insert(QStringLiteral("param2"), m_azimuthAngle);
        request.values.insert(QStringLiteral("param3"), m_elevationAngle);
        request.values.insert(QStringLiteral("param4"), 0.0);
        request.values.insert(QStringLiteral("param5"), 0.0);
    } else if (m_command == QStringLiteral("rectangleSearch")
               || m_command == QStringLiteral("rectangleSearchPosition")) {
        request.values.insert(QStringLiteral("param2"), m_searchCenterAzimuth);
        request.values.insert(QStringLiteral("param3"), m_searchCenterElevation);
        request.values.insert(QStringLiteral("param4"), m_azimuthSearchRange);
        request.values.insert(QStringLiteral("param5"), m_elevationSearchRange);
    } else if (m_command == QStringLiteral("circleSearch")
               || m_command == QStringLiteral("circleSearchPosition")) {
        request.values.insert(QStringLiteral("param2"), m_searchCenterAzimuth);
        request.values.insert(QStringLiteral("param3"), m_searchCenterElevation);
        request.values.insert(QStringLiteral("param4"), m_searchRadius);
        request.values.insert(QStringLiteral("param5"), 0.0);
    } else {
        request.values.insert(QStringLiteral("param2"), 0.0);
        request.values.insert(QStringLiteral("param3"), 0.0);
        request.values.insert(QStringLiteral("param4"), 0.0);
        request.values.insert(QStringLiteral("param5"), 0.0);
    }

    request.values.insert(QStringLiteral("timeSync"), m_timeSync ? 1 : 0);
    if (m_timeSync) {
        request.values.insert(QStringLiteral("deviceTimeMs"),
                              static_cast<qulonglong>(beijingNowUtcMs()));
    }
    emit requestSendCommand(request);
}

// ─────────────────────────────────────────────
// SerialPortLaser
// ─────────────────────────────────────────────

SerialPortLaser::SerialPortLaser(QObject *parent)
    : SerialPort(parent)
    , m_laserData(new LaserData(nullptr))        // 留在主线程，不随 moveToThread 迁移
    , m_laserSendData(new LaserSendData(nullptr))
    //, m_circularbuf(360)
{
    loadDefaultCommandProtocolProfile();
}

SerialPortLaser::~SerialPortLaser() {
    //由app管理，无需手动释放
    // delete m_laserData;
    // delete m_laserSendData;
}

bool SerialPortLaser::loadDefaultCommandProtocolProfile()
{
    const QString profilePath = defaultProtocolProfileLocation();
    ProtocolProfile candidate;
    QString error;
    if (!ProtocolProfile::loadFromFile(
                protocolProfileLocalPath(profilePath), &candidate, &error)) {
        m_defaultCommandProtocolProfile = ProtocolProfile();
        m_protocolProfile = ProtocolProfile();
        m_protocolEncoder.setProfile(m_protocolProfile);
        m_protocolDecoder.clearProfile();
        m_protocolReceiveEnabled = false;
        emit protocolDiagnostic(
                    QStringLiteral("默认发送协议加载失败，激光接收保持内置解析器: ")
                    + error);
        qWarning() << "[protocol-default]" << error;
        return false;
    }

    // 默认 JSON 仅用于维持原有命令编码；接收链路保持内置解析器。
    m_defaultCommandProtocolProfile = candidate;
    m_protocolProfile = candidate;
    m_protocolEncoder.setProfile(m_protocolProfile);
    m_protocolDecoder.clearProfile();
    m_protocolReceiveEnabled = false;
    qInfo() << "[protocol-default] command profile" << candidate.id
            << "from" << profilePath << "; receive uses legacy parser";
    return true;
}

QString SerialPortLaser::defaultProtocolProfileLocation() const
{
    QString profilePath =
            QString::fromLocal8Bit(qgetenv("KYLIN_LASER_PROTOCOL_PROFILE")).trimmed();
    if (profilePath.isEmpty()) {
        const QString deployedProfile = QCoreApplication::applicationDirPath()
                + QStringLiteral("/protocols/legacy_laser_v1.json");
        profilePath = QFileInfo::exists(deployedProfile)
                ? deployedProfile
                : QStringLiteral(":/protocols/legacy_laser_v1.json");
    }
    return profilePath;
}

bool SerialPortLaser::applyProtocolProfile(const QString &profileLocation,
                                           QString *profileId,
                                           QString *error)
{
    ProtocolProfile candidate;
    QString loadError;
    if (!ProtocolProfile::loadFromFile(
                protocolProfileLocalPath(profileLocation), &candidate, &loadError)) {
        if (error)
            *error = loadError;
        return false;
    }

    if (timer)
        timer->stop();
    m_protocolProfile = candidate;
    m_protocolDecoder.setProfile(m_protocolProfile);
    m_protocolEncoder.setProfile(m_protocolProfile);
    m_protocolReceiveEnabled = true;
    m_rxBuffer.clear();
    if (profileId)
        *profileId = m_protocolProfile.id;
    if (error)
        error->clear();
    return true;
}

void SerialPortLaser::onLoadProtocolProfile(const QString &profileLocation)
{
    QString profileId;
    QString error;
    const bool success =
            applyProtocolProfile(profileLocation, &profileId, &error);
    if (success) {
        emit protocolDiagnostic(
                    QStringLiteral("激光协议已切换为 %1").arg(profileId));
    } else {
        emit protocolDiagnostic(
                    QStringLiteral("激光协议 JSON 加载失败，保留原协议: ") + error);
    }
    emit protocolProfileLoadFinished(success, success ? profileId : QString(),
                                     success ? QString() : error);
}

void SerialPortLaser::onRestoreDefaultProtocolProfile()
{
    if (timer)
        timer->stop();
    m_protocolProfile = m_defaultCommandProtocolProfile;
    m_protocolEncoder.setProfile(m_protocolProfile);
    m_protocolDecoder.clearProfile();
    m_protocolReceiveEnabled = false;
    m_rxBuffer.clear();
    emit protocolDiagnostic(QStringLiteral("激光接收已恢复为内置解析器"));
    emit protocolProfileLoadFinished(true, QString(), QString());
}

// ── Worker slots：接收主线程 Data 的请求（QueuedConnection）──
void SerialPortLaser::onOpenPort(const QString &name, int baud) {
    if (SerialPort::openSerialDevice(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}
void SerialPortLaser::onClosePort()
{
    const bool wasExguideSource = (exsrcindex == ExguideSrcLaser);
    if (m_exGuideTimer) {
        m_exGuideTimer->stop();
        disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
    }
    m_lastexguidesetting = -1;
    m_send20msTimeSyncSent = false;
    m_abNeedReset = true;
    SerialPort::close();
    emit portClosed();

    if (wasExguideSource) {
        exsrcindex = -1;
        emit exguideSourceUnavailable();
    }
}
void SerialPortLaser::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }
void SerialPortLaser::onSendCommand(ProtocolCommandRequest request)
{
    if (!isOpen() || !m_protocolEncoder.isConfigured()) {
        SendLat::discardClick(SendLat::Laser);
        if (!m_protocolEncoder.isConfigured())
            emit protocolDiagnostic(QStringLiteral("配置化激光协议未启用，命令未发送"));
        return;
    }

    QByteArray initialFrame;
    QString encodeError;
    if (!m_protocolEncoder.encode(request, &initialFrame, &encodeError)) {
        SendLat::discardClick(SendLat::Laser);
        emit protocolDiagnostic(QStringLiteral("激光命令编码失败: ") + encodeError);
        return;
    }

    disconnect(timer, &QTimer::timeout, this, nullptr);
    int sendCount = 0;
    quint8 dataCount = 0;
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, this,
            [this, request, sendCount, dataCount]() mutable {
        QVariantMap values = request.values;
        values.insert(QStringLiteral("frameStatus"), 0x11 | (dataCount << 6));
        if (sendCount >= 1)
            values.insert(QStringLiteral("timeSync"), 0);

        ProtocolCommandRequest tickRequest = request;
        tickRequest.values = values;
        QByteArray data;
        QString error;
        if (!m_protocolEncoder.encode(tickRequest, &data, &error)) {
            emit protocolDiagnostic(QStringLiteral("激光命令编码失败: ") + error);
            timer->stop();
            return;
        }

        SendLat::markSend(SendLat::Laser);
        const qint64 count = SerialPort::send(data);
        if (count >= data.size()) {
            dataCount = static_cast<quint8>((dataCount + 1) % 3);
        }
        ++sendCount;
        if (sendCount >= 10)
            timer->stop();
    });
    timer->start();
}

void SerialPortLaser::onReadyRead()
{
    if (!m_protocolReceiveEnabled) {
        onReadyReadLegacy();
        return;
    }

    const ProtocolDecodeBatch batch =
            m_protocolDecoder.append(m_serialPort->readAll());
    if (!batch.errors.isEmpty()) {
        const ProtocolDecodeError &error = batch.errors.first();
        emit protocolDiagnostic(
                    QStringLiteral("激光协议帧%1: %2")
                    .arg(error.frameName.isEmpty()
                         ? QString()
                         : QStringLiteral(" [") + error.frameName + QStringLiteral("]"),
                         error.message));
    }
    for (const CanonicalSeekerFrame &frame : batch.frames)
        handleDecodedFrame(frame);
}

void SerialPortLaser::handleDecodedFrame(const CanonicalSeekerFrame &frame)
{
    m_azimuth = frame.canonicalNumber(
                QStringLiteral("measurement.losAzimuthDeg"));
    m_pitch = frame.canonicalNumber(
                QStringLiteral("measurement.losElevationDeg"));

    emit canonicalFrameReceived(frame);
    emit laserRawFrameReceived(frame.rawFrame);

    // 1 秒模式只上报逐帧相对测量，绝对目标合成和预测由转台线程负责。
    if (exindex == ExguideMode && exguidesetting == Exguide_1s) {
        exGuideMeasurement sample;
        sample.sourceId = 4;
        sample.azimuth = m_azimuth;
        sample.elevation = m_pitch;
        sample.timestampMs = QDateTime::currentMSecsSinceEpoch();
        emit exGuideMeasurementReady(sample);
    }
}

void SerialPortLaser::onReadyReadLegacy() {
    // 仿照 BD 串口的处理：串口驱动一次 readyRead 到达的数据不一定是一整帧，
    // 先存入接收缓冲，找到帧头 0x55 0xAA 0xDC 后再按固定帧长切出完整一帧交给 parseData
    m_rxBuffer.append(m_serialPort->readAll());

    const int frameLen = static_cast<int>(sizeof(laser_recv_frame));

    while (m_rxBuffer.size() >= 3) {
        // 1. 逐字节查找帧头：找到 0x55 后判断下一个是否为 0xAA、下下个是否为 0xDC
        int headIdx = -1;
        for (int i = 0; i <= m_rxBuffer.size() - 3; ++i) {
            if (static_cast<quint8>(m_rxBuffer[i])     == 0x55
                    && static_cast<quint8>(m_rxBuffer[i + 1]) == 0xAA
                    && static_cast<quint8>(m_rxBuffer[i + 2]) == 0xDC) {
                headIdx = i;
                break;
            }
        }
        if (headIdx < 0) {
            // 缓冲内没有完整帧头：只保留末尾2字节，
            // 防止帧头被拆成两段到达（如先到 0x55 0xAA，下一批才到 0xDC）
            if (m_rxBuffer.size() > 2)
                m_rxBuffer = m_rxBuffer.right(2);
            break;
        }
        // 2. 丢弃帧头前的噪声字节
        if (headIdx > 0)
            m_rxBuffer.remove(0, headIdx);

        // 3. 数据还不够一帧，等下一批数据
        if (m_rxBuffer.size() < frameLen)
            break;

        // 4. 帧头无误，从帧头开始读取固定字节数的一整帧送入解析
        QByteArray frame = m_rxBuffer.left(frameLen);
        m_rxBuffer.remove(0, frameLen);
        parseData(frame);
    }
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
        qDebug() <<"Received data is too short. Expected at least " << sizeof(laser_recv_frame) << " bytes, but got " << rawData.size() << " bytes.";
        return;
    }

    // const laser_recv_frame* pFrame = reinterpret_cast<const laser_recv_frame*>(rawData.data());
    laser_recv_frame frame{};
    //将大端序数据转化为小端序
    if(rawData.size() >= static_cast<int>(sizeof(laser_recv_frame))) {
        // 按字节访问同一份原始数据（供下方大端序组装使用）
        const uint8_t *rdata = reinterpret_cast<const uint8_t *>(rawData.constData());
        frame.frame_header1 = static_cast<quint8>(rawData[0]);
        frame.frame_header2 = static_cast<quint8>(rawData[1]);
        frame.frame_header3 = static_cast<quint8>(rawData[2]);
        frame.frame_status = static_cast<quint8>(rawData[3]);
        frame.frame_ID = static_cast<quint8>(rawData[4]);
        frame.dyt_status = static_cast<qint8>(rawData[5]);
        frame.detector_status = static_cast<qint8>(rawData[6]);
        frame.fault_info = static_cast<qint8>(rawData[7]);

        // 处理大端序的16位数据
        //以下为原写法（int 表达式直接窄化赋给 qint16/quint16，C++20 之前属实现定义行为），
        //现改为“先按 quint16 组装，再按二进制补码解释”，解析结果完全一致：
        //frame.optical_azimuth = (static_cast<qint16>(static_cast<unsigned char>(rawData[8])) << 8) | static_cast<unsigned char>(rawData[9]);
        //frame.optical_pitch = (static_cast<qint16>(static_cast<unsigned char>(rawData[10])) << 8) | static_cast<unsigned char>(rawData[11]);
        //frame.gyro_azimuth_rate = (static_cast<qint16>(static_cast<unsigned char>(rawData[12])) << 8) | static_cast<unsigned char>(rawData[13]);
        //frame.gyro_pitch_rate = (static_cast<qint16>(static_cast<unsigned char>(rawData[14])) << 8) | static_cast<unsigned char>(rawData[15]);
        //frame.los_azimuth_rate = (static_cast<qint16>(static_cast<unsigned char>(rawData[16])) << 8) | static_cast<unsigned char>(rawData[17]);
        //frame.los_pitch_rate = (static_cast<qint16>(static_cast<unsigned char>(rawData[18])) << 8) | static_cast<unsigned char>(rawData[19]);
        //frame.deviation_azimuth = (static_cast<qint16>(static_cast<unsigned char>(rawData[20])) << 8) | static_cast<unsigned char>(rawData[21]);
        //frame.deviation_pitch = (static_cast<qint16>(static_cast<unsigned char>(rawData[22])) << 8) | static_cast<unsigned char>(rawData[23]);
        //frame.laser_period = (static_cast<quint16>(static_cast<unsigned char>(rawData[24])) << 8) | static_cast<unsigned char>(rawData[25]);
        frame.optical_azimuth   = readBigEndianI16(rdata + 8);
        frame.optical_pitch     = readBigEndianI16(rdata + 10);
        frame.gyro_azimuth_rate = readBigEndianI16(rdata + 12);
        frame.gyro_pitch_rate   = readBigEndianI16(rdata + 14);
        frame.los_azimuth_rate  = readBigEndianI16(rdata + 16);
        frame.los_pitch_rate    = readBigEndianI16(rdata + 18);
        frame.deviation_azimuth = readBigEndianI16(rdata + 20);
        frame.deviation_pitch   = readBigEndianI16(rdata + 22);
        frame.laser_period      = readBigEndianU16(rdata + 24);
        frame.reserved1[0] = (static_cast<quint8>(static_cast<unsigned char>(rawData[27])));
        frame.reserved1[1] = (static_cast<quint8>(static_cast<unsigned char>(rawData[26])));
        frame.gain_status = static_cast<qint8>(rawData[28]);
        frame.quadrant1_energy = static_cast<quint8>(rawData[29]);
        frame.quadrant2_energy = static_cast<quint8>(rawData[30]);
        frame.quadrant3_energy = static_cast<quint8>(rawData[31]);
        frame.quadrant4_energy = static_cast<quint8>(rawData[32]);
        //以下为原写法（除了窄化，版本号字段本身是 quint16，原来却用 qint16 中转）：
        //frame.software_version1 = (static_cast<qint16>(static_cast<unsigned char>(rawData[33])) << 8) | static_cast<unsigned char>(rawData[34]);
        //frame.software_version2 = (static_cast<qint16>(static_cast<unsigned char>(rawData[35])) << 8) | static_cast<unsigned char>(rawData[36]);
        frame.software_version1 = readBigEndianU16(rdata + 33);
        frame.software_version2 = readBigEndianU16(rdata + 35);
        // 返回时间戳：37-44字节为毫秒部分(8字节大端)，45-46字节为微秒部分(大端)
        for (int i = 0; i < 8; ++i) {
            frame.ms_time[i] = static_cast<quint8>(rawData[37 + i]);
        }
        frame.us_time = readBigEndianU16(rdata + 45);
        // 校验位在帧末
        frame.XOR_result = static_cast<quint8>(rawData[47]);
    }
    // memcpy(&frame, rawData.constData(), sizeof(frame));

    if (frame.frame_header1 != 0x55 || frame.frame_header2 != 0xAA || frame.frame_header3 != 0xDC) {
        // return QByteArray();     //数据帧错误
        qDebug() << "Invalid frame header. Expected 0x55, 0xAA, 0xDC, but got "
                 << QString("0x%1").arg(frame.frame_header1, 2, 16, QLatin1Char('0')).toUpper() << ", "
                 << QString("0x%1").arg(frame.frame_header2, 2, 16, QLatin1Char('0')).toUpper() << ", "
                 << QString("0x%1").arg(frame.frame_header3, 2, 16, QLatin1Char('0')).toUpper();
        return;
    }
    
    // 截取需要校验的数据,仍按大端序数据格式来计算校验位
    if (rawData.size() >= 4) {
        checkdata = rawData.mid(3, rawData.size() - 4);
    }

    uint8_t calculatedChecksum = xorChecksum(checkdata);
    if (calculatedChecksum != frame.XOR_result) {
        qDebug() << "Checksum mismatch. Expected: 0x" << QString("0x%1").arg(frame.XOR_result, 2, 16, QLatin1Char('0')).toUpper()
                 << ", Calculated: 0x" << QString("0x%1").arg(calculatedChecksum, 2, 16, QLatin1Char('0')).toUpper();
        return;
    }
    //  在经过解析完的每帧数据中保存光轴方位角与光轴俯仰角参数
    // laserExGuideData m_exGuidedata;
    // m_exGuidedata.azimuth = frame.optical_azimuth * 0.01;
    // m_exGuidedata.pitch = frame.optical_pitch * 0.01;
    // //  将导引头反馈的方位角与俯仰角信息存入环形缓冲区，新数据会覆盖老数据
    // m_circularbuf.push(std::move(m_exGuidedata));
    //  检验无误后更新数据并刷新QML界面显示

    m_azimuth = frame.optical_azimuth * 0.01;
    m_pitch = frame.optical_pitch * 0.01;

    emit laserFrameReceived(frame);
    emit laserRawFrameReceived(rawData);
    
    // 1 秒模式只上报逐帧相对测量，绝对目标合成和预测由转台线程负责。
    if (exindex == ExguideMode && exguidesetting == Exguide_1s) {
        exGuideMeasurement sample;
        sample.sourceId = 4;
        sample.azimuth = m_azimuth;
        sample.elevation = m_pitch;
        sample.timestampMs = QDateTime::currentMSecsSinceEpoch();
        emit exGuideMeasurementReady(sample);
    }
}

void SerialPortLaser::ExmodeChanged(int mode)
{
    //1:外引导  2：程控模式 3：遥控模式  //还需要判断外引导源  //判断跟踪模式
    //判断使用哪个导引头的数据，来决定是否定期向转台串口线程同步数据
    //判断index与外引导模式数据选择提供位，如果被选中，就启动一个定时器，每1秒或20ms发送一次跟踪数据信息
    //1s模式发送Alpha-Beta预测角度；20ms模式直接发送当前解算角度。
    if(mode < 3)
        exindex = mode;  //模式索引赋值
    if(mode > 2 && mode < 6)
        exsrcindex = mode;  //外引导源索引赋值 索引分别为3，4，5
    if(mode > 5)
        exguidesetting = mode; //外引导发送时间间隔选择  索引为6，7

    if (exindex == ExguideMode) //判断是否为外引导模式
    {   
        // 激光导引头被选为外引导源：启动定时器，每1秒或20ms发送一次跟踪数据
        if(exsrcindex == ExguideSrcLaser)
        {
            // 串口未打开时不启动外引导，也不写跟踪日志；只取消本路外引导源
            if (!isOpen()) {
                emit exguideSerialNotOpen(
                    QStringLiteral("激光导引头串口未打开，已取消激光导引头外引导源选择"));
                if (m_exGuideTimer) {
                    m_exGuideTimer->stop();
                    disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
                }
                m_lastexguidesetting = -1;
                m_send20msTimeSyncSent = false;
                exsrcindex = -1;
                emit exguideSourceUnavailable();
                return;
            }

            // 只有时间间隔设置改变时才重新绑定
            if(m_lastexguidesetting != exguidesetting)
            {
                // 先解绑旧连接并停止定时器
                if(m_lastexguidesetting > 0)
                {
                    m_exGuideTimer->stop();
                    disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
                    m_send20msTimeSyncSent = false;
                }
                // 延迟创建定时器
                if (!m_exGuideTimer) {
                    m_exGuideTimer = new QTimer(this);
                    m_exGuideTimer->setTimerType(Qt::PreciseTimer);
                }
                // 跟踪周期变化后，下一帧数据到来时用当时的测量重新初始化滤波器
                m_abNeedReset = true;
                // 判断跟踪模式（20ms模式或者1秒跟踪模式）
                if(exguidesetting == Exguide_1s)
                {
                    connect(m_exGuideTimer, &QTimer::timeout, this, [this]() {
                        if (!isOpen()) {
                            m_exGuideTimer->stop();
                            disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
                            m_lastexguidesetting = -1;
                            m_send20msTimeSyncSent = false;
                            exsrcindex = -1;
                            emit exguideSourceUnavailable();
                            return;
                        }
                        // 按单调时间选下一目标秒，并在目标秒开始前200ms发送。
                        const auto slot = m_periodicTrackSchedule.currentSlot();
                        // 测量已逐帧发送；这里只触发转台生成绝对目标预测包。
                        emit exGuide1sTick(4, slot.firstPredictionDelaySec);
                        LogManager::instance()->logLaserTracking(m_azimuth, m_pitch);
                        m_exGuideTimer->start(static_cast<int>(
                            m_periodicTrackSchedule.delayUntilNextSendMs(
                                slot.rawTargetSecond)));
                    });
                    m_exGuideTimer->setSingleShot(true);
                    m_periodicTrackSchedule.restart();
                    emit reqTimesync(0);
                    m_exGuideTimer->start(static_cast<int>(
                        m_periodicTrackSchedule.currentSlot().delayUntilSendMs));
                    m_lastexguidesetting = Exguide_1s;
                }
                else if(exguidesetting == Exguide_20ms)
                {
                    m_exGuideTimer->setSingleShot(false);
                    connect(m_exGuideTimer, &QTimer::timeout, this, [this]() {
                        if (!isOpen()) {
                            m_exGuideTimer->stop();
                            disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
                            m_lastexguidesetting = -1;
                            m_send20msTimeSyncSent = false;
                            exsrcindex = -1;
                            emit exguideSourceUnavailable();
                            return;
                        }
                        // 20ms模式直接发送当前角度，不使用Kalman外推。
                        emit reqExsend_20ms(m_azimuth, m_pitch);
                        LogManager::instance()->logLaserTracking(m_azimuth, m_pitch);
                    });
                    // 进入20ms模式立即做一次秒时间设置；等首个20ms定时器触发时
                    // 状态帧已刷新，首帧可以直接按转台时基的 next20 发送。
                    if (!m_send20msTimeSyncSent) {
                        emit reqTimesync(0);
                        m_send20msTimeSyncSent = true;
                    }
                    m_exGuideTimer->start(20); // 每20ms触发一次
                    m_lastexguidesetting = Exguide_20ms;
                }
            }
        }
        else
        {   
            if (m_exGuideTimer) {
                m_exGuideTimer->stop(); //切换到其他外引导源时，暂停激光导引头外引导定时器，停止继续发送
                disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
            }
            m_lastexguidesetting = -1;
            m_send20msTimeSyncSent = false;
            // 本路已不是外引导源：下次被选中时重新初始化滤波器
            m_abNeedReset = true;
        }
    }
    else
    {   
        exsrcindex = -1; //非外引导模式下，外引导源索引置0
        exguidesetting = -1; //非外引导模式下，外引导发送时间间隔索引置0
        m_send20msTimeSyncSent = false;
        if (m_exGuideTimer) {
            m_exGuideTimer->stop();
            disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
        }
        // 退出外引导模式：下次进入时重新初始化滤波器
        m_abNeedReset = true;
    }
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
