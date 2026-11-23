#include "serialport_image.h"
#include <QDebug>
#include <QString>
#include "../log/LogManager.h"
#include <QThread>
#include <QTimer>
#include <QTimeZone>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include "../utils/sendlatencyprobe.h"
// ─────────────────────────────────────────────
// ImageData
// ─────────────────────────────────────────────
#define ExguideMode 0
#define ExguideSrcImg 3
#define Exguide_20ms 6

#define Exguide_1s 7
#define Exguide_FrameLostMs 1000 //外引导时帧编号超过该时长(ms)未更新，即认为通信中断
#define Image_FramePollMs 500    //图像头串口打开期间，帧编号检查周期
#define Image_FrameLostMs 2000   //图像头串口打开期间，帧编号未变化的判定时长

// ─────────────────────────────────────────────
// 时间戳工具
// ─────────────────────────────────────────────
// 取当前北京时间(UTC+8，本机时钟按北京时间显示)并换算为 UTC 毫秒时间戳
// (1970-01-01 00:00:00 UTC 起算)。
// 做法：先把本地“墙上时间”按 UTC 解释，再减去 8 小时固定偏差，
// 这样无论系统时区是否已配成 Asia/Shanghai，结果都是 UTC 绝对毫秒时间。
static quint64 beijingNowUtcMs()
{
    const QDateTime local = QDateTime::currentDateTime();
    const QDateTime wallClockAsUtc(local.date(), local.time(), Qt::UTC);
    const qint64 ms = wallClockAsUtc.toMSecsSinceEpoch() - 8LL * 3600LL * 1000LL;
    return static_cast<quint64>(ms > 0 ? ms : 0);
}

// 8 字节时间戳按小端(低字节在前)写入/读出，与帧内其它多字节字段的字节序保持一致
static void writeUint64LE(quint8 *dst, quint64 value)
{
    for (int i = 0; i < 8; ++i)
        dst[i] = static_cast<quint8>((value >> (8 * i)) & 0xFFu);
}

static quint64 readUint64LE(const quint8 *src)
{
    quint64 value = 0;
    for (int i = 0; i < 8; ++i)
        value |= static_cast<quint64>(src[i]) << (8 * i);
    return value;
}

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

uint16_t SerialPortImage::crc16_table[256] = {0};

ImageData::ImageData(QObject *parent)
    : QObject(parent)
{
}

// ── QML 调用方法 ──
void ImageData::openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void ImageData::closePort()                                     { emit requestClosePort(); }
void ImageData::scanPorts()                                     { emit requestScanPorts(); }
void ImageData::loadProtocolProfile(const QString &profileLocation)
{
    const QString path = protocolProfileLocalPath(profileLocation);
    if (path.isEmpty()) {
        applyProtocolProfileResult(false, QString(),
                                   QStringLiteral("未选择协议 JSON 文件"));
        return;
    }
    emit requestLoadProtocolProfile(path);
}
void ImageData::restoreDefaultProtocolProfile()
{
    emit requestRestoreDefaultProtocolProfile();
}
// void ImageData::sendData(const QByteArray &data)                { emit requestSendData(data); }

// ── 工作线程回推 ──
void ImageData::setPortOpen(bool open)
{
    if (m_portOpen != open) {
        m_portOpen = open;
        emit portOpenChanged();
    }
    if (!open) {
        m_ackCheckActive = false;
        m_ackCheckFramesRemaining = 0;
        m_previousValidTimeStampUs = 0;
        if (m_timeScaleAccuracyValid) {
            m_timeScaleAccuracyValid = false;
            emit timeScaleAccuracyValidChanged();
        }
        if (m_aFrameValidFlag != 0) {
            m_aFrameValidFlag = 0;
            emit aFrameValidFlagChanged();
        }
    }
}
void ImageData::setPortList(const QStringList &ports) { if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); } }
void ImageData::setError(const QString &msg) { if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); } }

void ImageData::applyProtocolProfileResult(bool success,
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

void ImageData::beginAckCheck()
{
    m_ackCheckActive = true;
    m_ackCheckFramesRemaining = AckCheckFrameCount;

    // 每次发送都清除上一次结果，避免在尚未收到本次回告时仍显示“有效”。
    if (m_aFrameValidFlag != 0) {
        m_aFrameValidFlag = 0;
        emit aFrameValidFlagChanged();
    }
}

int ImageData::frameLength() const { return m_frameLength; }
int ImageData::bFrameSequence() const { return m_bFrameSequence.load(std::memory_order_relaxed); }
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
int ImageData::temperture() const { return m_temperture; }
int ImageData::cbhTv4405() const { return m_cbhTv4405; }
int ImageData::infraredFrameRate() const { return m_infraredFrameRate; }
int ImageData::tvFrameRate() const { return m_tvFrameRate; }
int ImageData::gateSize() const { return m_gateSize; }
int ImageData::softwareVersion1() const { return m_softwareVersion1; }
int ImageData::softwareVersion2() const { return m_softwareVersion2; }
int ImageData::softwareVersion3() const { return m_softwareVersion3; }
qint64 ImageData::msTime() const { return m_msTime; }
int ImageData::usTime() const { return m_usTime; }
qint64 ImageData::timeStampUs() const { return m_timeStampUs; }
qint64 ImageData::timeScaleAccuracyUs() const { return m_timeScaleAccuracyUs; }
bool ImageData::timeScaleAccuracyValid() const { return m_timeScaleAccuracyValid; }
QDateTime ImageData::recvDateTime() const { return m_recvDateTime; }
QString ImageData::recvTimeText() const { return m_recvTimeText; }

// 本函数在 SerialPortImage::parseData() 校验完帧头与 CRC16 后，
// 由 imageFrameReceived 信号(队列连接)触发，完成各字段的解析。
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
    // 本槽是 m_bFrameSequence 的唯一写方（只在主线程执行），
    // 因此“读-比较-写”不需要 CAS，逐个 relaxed 存取即可
    const int bSeq = static_cast<int>(pFrame->b_frame_sequence);
    if (m_bFrameSequence.load(std::memory_order_relaxed) != bSeq) {
        m_bFrameSequence.store(bSeq, std::memory_order_relaxed);
        emit bFrameSequenceChanged();
    }
    if (m_aFrameSequenceReply != pFrame->a_frame_sequence_reply) {
        m_aFrameSequenceReply = pFrame->a_frame_sequence_reply;
        emit aFrameSequenceReplyChanged();
    }
    // 点击发送后连续观察 20 个有效 B 帧，对 A 帧有效回告取并集。
    // 这里不能以“标志发生跳变”为触发条件：设备连续回 0x00 时也必须消费帧计数，
    // 否则检查窗口可能永远不结束。
    if (m_ackCheckActive && m_ackCheckFramesRemaining > 0) {
        if (pFrame->a_frame_valid_flag == 0xAA && m_aFrameValidFlag != 0xAA) {
            m_aFrameValidFlag = 0xAA;
            emit aFrameValidFlagChanged();
        }
        --m_ackCheckFramesRemaining;
        if (m_ackCheckFramesRemaining == 0)
            m_ackCheckActive = false;
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

    // 字节34-41：导引头返回时间戳的毫秒部分(8字节小端)；字节42-43：微秒部分(0~999)
    const qint64 msTime = static_cast<qint64>(
                readUint64LE(pFrame->ms_time));
    const int usTime = static_cast<int>(pFrame->us_time);
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

    //auto infrared_frame_num = static_cast<int>(pFrame->infrared_frame_num);
    if (m_infraredFrameNum != pFrame->infrared_frame_num) {
        m_infraredFrameNum = pFrame->infrared_frame_num;
        emit infraredFrameNumChanged();
    }

    //加入温度检查
    if (m_temperture != pFrame->temperture) {
        m_temperture = pFrame->temperture;
        emit tempertureChanged();
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

    // 本帧各字段已全部更新，通知消费者（数据保存等）按同一份解析结果取用
    emit frameParsed();
}

void ImageData::updateFromCanonicalFrame(const CanonicalSeekerFrame &frame)
{
    // 过渡兼容：保留 ImageData 既有 QML 属性，但数据来自配置化标准模型，
    // 不再依赖 image_recv_frame 的固定内存布局。
    image_recv_frame legacy = {};
    if (frame.rawFrame.size() >= 2) {
        legacy.frame_header1 = static_cast<quint8>(frame.rawFrame.at(0));
        legacy.frame_header2 = static_cast<quint8>(frame.rawFrame.at(1));
    }
    legacy.frame_length = static_cast<quint8>(
                frame.canonicalInteger(QStringLiteral("frame.length")));
    legacy.b_frame_sequence = static_cast<quint16>(
                frame.canonicalInteger(QStringLiteral("frame.sequence")));
    legacy.a_frame_sequence_reply = static_cast<quint16>(
                frame.canonicalInteger(QStringLiteral("command.ackSequence")));
    legacy.a_frame_valid_flag = static_cast<quint8>(
                frame.canonicalInteger(QStringLiteral("command.ackValid")));
    legacy.seeker_ctrl_reply = static_cast<quint8>(
                frame.canonicalInteger(QStringLiteral("command.seekerReply")));
    legacy.optical_param_reply = static_cast<quint8>(
                frame.canonicalInteger(QStringLiteral("command.opticalReply")));
    legacy.current_work_channel = static_cast<quint8>(
                frame.canonicalInteger(QStringLiteral("status.workChannel")));

    int selfCheck = 0;
    static const char *selfCheckPaths[] = {
        "status.selfCheck.overall",
        "status.selfCheck.irVideo",
        "status.selfCheck.tvVideo",
        "status.selfCheck.externalComm",
        "status.selfCheck.comm",
        "status.selfCheck.servo"
    };
    for (int bit = 0; bit < 6; ++bit) {
        if (frame.canonicalBoolean(QString::fromLatin1(selfCheckPaths[bit])))
            selfCheck |= (1 << bit);
    }
    legacy.self_check_flag = static_cast<quint8>(selfCheck);
    legacy.pitch_line_of_sight_ang_vel = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("measurement.losElevationRateDegPerSec"))
                / 0.002));
    legacy.yaw_line_of_sight_ang_vel = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("measurement.losAzimuthRateDegPerSec"))
                / 0.002));
    legacy.target_background_type = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("status.targetBackgroundRaw")));
    legacy.optical_work_state = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("status.opticalWorkState")));
    legacy.pitch_frame_angle = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("measurement.frameElevationDeg"))
                / 0.002));
    legacy.yaw_frame_angle = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("measurement.frameAzimuthDeg"))
                / 0.002));
    legacy.correction_cmd_status = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("status.correctionCommand")));
    legacy.correction_cmd_count = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("status.correctionCount")));
    legacy.pitch_gyro = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("measurement.gyroElevationOutputDegPerSec"))
                / 0.01));
    legacy.yaw_gyro = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("measurement.gyroAzimuthOutputDegPerSec"))
                / 0.01));
    legacy.tracking_state = static_cast<quint8>(
                frame.canonicalInteger(QStringLiteral("tracking.stateCode")));
    legacy.tracker_state = static_cast<quint8>(
                frame.canonicalInteger(QStringLiteral("tracking.trackerCode")));

    const qint64 deviceTimeMs = frame.canonicalInteger(
                QStringLiteral("time.deviceUtcMs"));
    for (int i = 0; i < 8; ++i) {
        legacy.ms_time[i] = static_cast<quint8>(
                    (static_cast<quint64>(deviceTimeMs) >> (8 * i)) & 0xFFu);
    }
    legacy.us_time = static_cast<quint16>(
                frame.canonicalInteger(
                    QStringLiteral("time.deviceSubMillisecondUs")));
    legacy.azimuth_deviation_pixel = static_cast<qint16>(
                frame.canonicalInteger(
                    QStringLiteral("measurement.deviationAzimuthPx")));
    legacy.pitch_deviation_pixel = static_cast<qint16>(
                frame.canonicalInteger(
                    QStringLiteral("measurement.deviationElevationPx")));
    legacy.azimuth_gyro_output = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("measurement.azimuthGyroOutputDegPerSec"))
                / 0.01));
    legacy.pitch_gyro_output = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("measurement.elevationGyroOutputDegPerSec"))
                / 0.01));
    legacy.azimuth_master_cmd = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("control.azimuthMaster"))
                / 0.01));
    legacy.pitch_master_cmd = static_cast<qint16>(qRound(
                frame.canonicalNumber(
                    QStringLiteral("control.elevationMaster"))
                / 0.01));
    legacy.platform_self_check = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("status.platformSelfCheck")));
    legacy.servo_running_time = static_cast<qint8>(
                frame.canonicalInteger(
                    QStringLiteral("status.servoRunningTime")));
    legacy.servo_step = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("status.servoStep")));
    legacy.infrared_frame_num = static_cast<quint32>(
                frame.canonicalInteger(
                    QStringLiteral("video.infraredFrame")));
    legacy.temperture = static_cast<qint16>(
                frame.canonicalInteger(
                    QStringLiteral("status.temperature")));
    legacy.cbh_tv4405 = static_cast<quint32>(
                frame.canonicalInteger(
                    QStringLiteral("video.tvFrame")));
    legacy.infrared_frame_rate = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("video.infraredFrameRateHz")));
    legacy.tv_frame_rate = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("video.tvFrameRateHz")));
    legacy.gate_size = static_cast<quint16>(
                frame.canonicalInteger(
                    QStringLiteral("tracking.gateSize")));
    legacy.software_version1 = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("version.item1")));
    legacy.software_version2 = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("version.item2")));
    legacy.software_version3 = static_cast<quint8>(
                frame.canonicalInteger(
                    QStringLiteral("version.item3")));

    updateFromFrame(QByteArray(reinterpret_cast<const char *>(&legacy),
                               sizeof(legacy)));
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
    //每10分钟自动向导引头发送一次时间同步帧：置位时间同步信号 → 构帧发送 → 再复位
    const int timeSyncIntervalMs = 10 * 60 * 1000;   //10分钟
    m_timeSyncTimer = new QTimer(this);
    m_timeSyncTimer->setTimerType(Qt::CoarseTimer);
    m_timeSyncTimer->setInterval(timeSyncIntervalMs);
    connect(m_timeSyncTimer, &QTimer::timeout, this, &ImageSendData::sendTimeSyncFrame);
    m_timeSyncTimer->start();
}

void ImageSendData::sendTimeSyncFrame()
{
    m_timeSync = 1;                 //字节61：时间同步信号置1
    buildFrame();                   //构帧并发送（62-69字节为当拍北京时间换算的UTC毫秒时间戳）
    m_timeSync = 0;                 //发完立即复位，避免后续人工发送的帧都带时间同步标志
}

ProtocolCommandRequest ImageSendData::makeControlRequest(
        bool deviationFrame,
        int correctionFrameNum,
        int correctedYawPixel,
        int correctedPitchPixel) const
{
    ProtocolCommandRequest request;
    request.specName = QStringLiteral("image.control");
    QVariantMap &values = request.values;
    values.insert(QStringLiteral("aFrameSequence"), m_aFrameSequence);
    values.insert(QStringLiteral("seekerCtrlWord"),
                  deviationFrame ? 0 : m_seekerCtrlWord);
    values.insert(QStringLiteral("opticalParamCtrl"),
                  deviationFrame ? 0 : m_opticalParamCtrl);
    values.insert(QStringLiteral("templateIndex"), m_templateIndex);
    values.insert(QStringLiteral("targetBackgroundType"),
                  (m_targetBackgroundType4 << 5)
                  | (m_targetBackgroundType3 << 4)
                  | (m_targetBackgroundType2 << 3)
                  | m_targetBackgroundType1);
    values.insert(QStringLiteral("missileTargetDistance"),
                  m_missileTargetDistance);
    values.insert(QStringLiteral("missileSpeed"), m_missileSpeed);
    values.insert(QStringLiteral("bodyPitchAngle"), m_bodyPitchAngle);
    values.insert(QStringLiteral("bodyYawAngle"), m_bodyYawAngle);
    values.insert(QStringLiteral("bodyRollAngle"), m_bodyRollAngle);
    values.insert(QStringLiteral("bodyPitchRate"), m_bodyPitchRate);
    values.insert(QStringLiteral("bodyYawRate"), m_bodyYawRate);
    values.insert(QStringLiteral("bodyRollRate"), m_bodyRollRate);
    values.insert(QStringLiteral("bodyVelX"), m_bodyVelX);
    values.insert(QStringLiteral("bodyVelY"), m_bodyVelY);
    values.insert(QStringLiteral("bodyVelZ"), m_bodyVelZ);
    values.insert(QStringLiteral("bodyPosX"), m_bodyPosX);
    values.insert(QStringLiteral("bodyPosY"), m_bodyPosY);
    values.insert(QStringLiteral("bodyPosZ"), m_bodyPosZ);
    values.insert(QStringLiteral("pitchGimbalPreset"), m_pitchGimbalPreset);
    values.insert(QStringLiteral("yawGimbalPreset"), m_yawGimbalPreset);
    values.insert(QStringLiteral("irIntegrationTime"), m_irIntegrationTime);
    values.insert(QStringLiteral("trackingCorrectionCmd"),
                  deviationFrame ? 1 : m_trackingCorrectionCmd);
    const quint32 correctionFrame = static_cast<quint32>(
                deviationFrame ? correctionFrameNum : m_correctionFrameNum);
    values.insert(QStringLiteral("correctionFrameNumLow"),
                  correctionFrame & 0xFF);
    values.insert(QStringLiteral("correctionFrameNumMid"),
                  (correctionFrame >> 8) & 0xFF);
    values.insert(QStringLiteral("correctionFrameNumHigh"),
                  (correctionFrame >> 16) & 0xFF);
    values.insert(QStringLiteral("correctedPitchPos"),
                  deviationFrame ? correctedPitchPixel : m_correctedPitchPos);
    values.insert(QStringLiteral("correctedYawPos"),
                  deviationFrame ? correctedYawPixel : m_correctedYawPos);
    values.insert(QStringLiteral("searchPitchRate"), m_searchPitchRate);
    values.insert(QStringLiteral("searchYawRate"), m_searchYawRate);
    values.insert(QStringLiteral("gateSize"), m_gateSize);
    values.insert(QStringLiteral("captureRefImgCmd"), m_captureRefImgCmd);
    values.insert(QStringLiteral("timeSync"), m_timeSync ? 1 : 0);
    if (m_timeSync)
        values.insert(QStringLiteral("deviceTimeMs"),
                      static_cast<qulonglong>(beijingNowUtcMs()));
    values.insert(QStringLiteral("targetAltitude"), m_targetAltitude);
    values.insert(QStringLiteral("aircraftPitch"), m_aircraftPitch);
    values.insert(QStringLiteral("aircraftYaw"), m_aircraftYaw);
    values.insert(QStringLiteral("aircraftRoll"), m_aircraftRoll);
    values.insert(QStringLiteral("focalLength"), m_focalLength);
    values.insert(QStringLiteral("podPitchAngle"), m_podPitchAngle);
    values.insert(QStringLiteral("podYawAngle"), m_podYawAngle);
    values.insert(QStringLiteral("satelliteMapScale"), m_satelliteMapScale);
    values.insert(QStringLiteral("podType"), m_podType);
    values.insert(QStringLiteral("targetLongitude"),
                  static_cast<qint32>(m_targetLongitude));
    values.insert(QStringLiteral("targetLatitude"),
                  static_cast<qint32>(m_targetLatitude));
    values.insert(QStringLiteral("aircraftLongitude"),
                  static_cast<qint32>(m_aircraftLongitude));
    values.insert(QStringLiteral("aircraftLatitude"),
                  static_cast<qint32>(m_aircraftLatitude));
    values.insert(QStringLiteral("aircraftAltitude"), m_aircraftAltitude);
    values.insert(QStringLiteral("pixelSize"), m_pixelSize);
    return request;
}

void ImageSendData::buildFrame()
{
    // 发送延迟探针：只统计人工点击发送按钮。
    if (!m_timeSync) SendLat::markClick(SendLat::Image);
    emit requestSendCommand(makeControlRequest(false, 0, 0, 0));
    m_seekerCtrlWord = 0;
    emit seekerCtrlWordChanged();
}

void ImageSendData::buildDeviation(int num, int x, int y)
{
    emit requestSendCommand(makeControlRequest(true, num, x, y));
}


// ─────────────────────────────────────────────
// SerialPortImage
// ─────────────────────────────────────────────

SerialPortImage::SerialPortImage(QObject *parent)
    : SerialPort(parent)
    , m_imageData(new ImageData(nullptr))        // 留在主线程，不随 moveToThread 迁移
    , m_imageSendData(new ImageSendData(nullptr))
    //, m_circularbuf(200)
{
    init_crc16_table();
    loadProtocolProfile();
    m_frameWatchTimer = new QTimer(this);
    m_frameWatchTimer->setInterval(Image_FramePollMs);
    m_frameWatchTimer->setTimerType(Qt::CoarseTimer);
    connect(m_frameWatchTimer, &QTimer::timeout,
            this, &SerialPortImage::checkImageFrameAlive);
}

bool SerialPortImage::loadProtocolProfile()
{
    const QString profilePath = defaultProtocolProfileLocation();
    QString profileId;
    QString error;
    if (!applyProtocolProfile(profilePath, &profileId, &error)) {
        m_protocolEnabled = false;
        emit protocolDiagnostic(
                    QStringLiteral("配置化图像协议未启用，回退到内置解析器: ") + error);
        qWarning() << "[protocol-image]" << error;
        return false;
    }
    qInfo() << "[protocol-image] loaded" << profileId << "from" << profilePath;
    return true;
}

QString SerialPortImage::defaultProtocolProfileLocation() const
{
    QString profilePath =
            QString::fromLocal8Bit(qgetenv("KYLIN_IMAGE_PROTOCOL_PROFILE")).trimmed();
    if (profilePath.isEmpty()) {
        const QString deployedProfile = QCoreApplication::applicationDirPath()
                + QStringLiteral("/protocols/legacy_image_v1.json");
        profilePath = QFileInfo::exists(deployedProfile)
                ? deployedProfile
                : QStringLiteral(":/protocols/legacy_image_v1.json");
    }
    return profilePath;
}

bool SerialPortImage::applyProtocolProfile(const QString &profileLocation,
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
    m_protocolEnabled = true;
    m_rxBuffer.clear();
    if (profileId)
        *profileId = m_protocolProfile.id;
    if (error)
        error->clear();
    return true;
}

void SerialPortImage::onLoadProtocolProfile(const QString &profileLocation)
{
    QString profileId;
    QString error;
    const bool success =
            applyProtocolProfile(profileLocation, &profileId, &error);
    if (success) {
        emit protocolDiagnostic(
                    QStringLiteral("图像协议已切换为 %1").arg(profileId));
    } else {
        emit protocolDiagnostic(
                    QStringLiteral("图像协议 JSON 加载失败，保留原协议: ") + error);
    }
    emit protocolProfileLoadFinished(success, success ? profileId : QString(),
                                     success ? QString() : error);
}

void SerialPortImage::onRestoreDefaultProtocolProfile()
{
    onLoadProtocolProfile(defaultProtocolProfileLocation());
}

SerialPortImage::~SerialPortImage() {
    if (m_exGuideTimer) {
        // m_exGuideTimer 是 new QTimer(this)（有父对象），不能单独 moveToThread()
        // （Qt 会直接拒绝并打印 "Cannot move objects with a parent"）。
        // 它随本对象一起析构，这里只在同一线程内停止计时器。
        if (QThread::currentThread() == m_exGuideTimer->thread())
            m_exGuideTimer->stop();
        delete m_exGuideTimer;
        m_exGuideTimer = nullptr;
    }
    // delete m_imageData;
    // delete m_imageSendData;
}

// ── Worker slots ──
void SerialPortImage::onOpenPort(const QString &name, int baud) {
    if (SerialPort::openSerialDevice(name, baud)) {
        startFrameWatch();
        emit portOpened(true);
    } else {
        stopFrameWatch();
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
    }
}
void SerialPortImage::onClosePort()  {
    const bool wasExguideSource = (exsrcindex == ExguideSrcImg);
    stopFrameWatch();
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
        emit exguideLinkLost(
            QStringLiteral("图像导引头串口已关闭，已取消图像导引头外引导源选择"));
    }
}
void SerialPortImage::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }
void SerialPortImage::onSendCommand(ProtocolCommandRequest request)
{
    if (!isOpen() || !m_protocolEnabled || !timer) {
        SendLat::discardClick(SendLat::Image);
        if (!m_protocolEnabled)
            emit protocolDiagnostic(QStringLiteral("配置化图像协议未启用，命令未发送"));
        return;
    }

    QByteArray initialFrame;
    QString encodeError;
    if (!m_protocolEncoder.encode(request, &initialFrame, &encodeError)) {
        SendLat::discardClick(SendLat::Image);
        emit protocolDiagnostic(QStringLiteral("图像命令编码失败: ") + encodeError);
        return;
    }

    disconnect(timer, &QTimer::timeout, this, nullptr);
    int sendCount = 0;
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, this,
            [this, request, sendCount]() mutable {
        QVariantMap values = request.values;
        values.insert(QStringLiteral("aFrameSequence"), m_commandSequence);
        if (sendCount >= 1) {
            values.insert(QStringLiteral("trackingCorrectionCmd"), 0);
            values.insert(QStringLiteral("timeSync"), 0);
        }
        if (sendCount >= 3) {
            values.insert(QStringLiteral("seekerCtrlWord"), 0);
            values.insert(QStringLiteral("opticalParamCtrl"), 0);
            values.insert(QStringLiteral("captureRefImgCmd"), 0);
        }

        ProtocolCommandRequest tickRequest = request;
        tickRequest.values = values;
        QByteArray data;
        QString error;
        if (!m_protocolEncoder.encode(tickRequest, &data, &error)) {
            emit protocolDiagnostic(
                        QStringLiteral("图像命令编码失败: ") + error);
            timer->stop();
            return;
        }

        SendLat::markSend(SendLat::Image);
        const qint64 count = SerialPort::send(data);
        if (count >= data.size())
            ++m_commandSequence;
        ++sendCount;
        if (sendCount >= 10)
            timer->stop();
    });
    timer->start();
}
void SerialPortImage::onReadyRead()
{
    if (!m_protocolEnabled) {
        onReadyReadLegacy();
        return;
    }

    const ProtocolDecodeBatch batch =
            m_protocolDecoder.append(m_serialPort->readAll());
    if (!batch.errors.isEmpty()) {
        const ProtocolDecodeError &error = batch.errors.first();
        emit protocolDiagnostic(
                    QStringLiteral("图像协议帧%1: %2")
                    .arg(error.frameName.isEmpty()
                         ? QString()
                         : QStringLiteral(" [") + error.frameName + QStringLiteral("]"),
                         error.message));
    }
    for (const CanonicalSeekerFrame &frame : batch.frames)
        handleDecodedFrame(frame);
}

void SerialPortImage::handleDecodedFrame(const CanonicalSeekerFrame &frame)
{
    m_azimuth = frame.canonicalNumber(
                QStringLiteral("measurement.frameAzimuthDeg"));
    m_pitch = frame.canonicalNumber(
                QStringLiteral("measurement.frameElevationDeg"));

    emit canonicalFrameReceived(frame);

    if (exindex == ExguideMode && exguidesetting == Exguide_1s) {
        exGuideMeasurement sample;
        sample.sourceId = 3;
        sample.azimuth = m_azimuth;
        sample.elevation = m_pitch;
        sample.timestampMs = QDateTime::currentMSecsSinceEpoch();
        emit exGuideMeasurementReady(sample);
    }
}

void SerialPortImage::onReadyReadLegacy()
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
    //判断index与外引导模式数据选择提供位，如果被选中，就启动一个定时器，每1秒或20ms发送一次跟踪数据信息
    //1s模式发送Kalman预测角度；20ms模式直接发送当前解算角度。
    if(mode < 3)
    exindex = mode;  //模式索引赋值
    if(mode > 2 && mode < 6)
    exsrcindex = mode;  //外引导源索引赋值 索引分别为3，4，5
    if(mode >5)
    exguidesetting = mode; //外引导发送时间间隔选择  索引为6，7

    if (exindex == ExguideMode) //判断是否为外引导模式
    {
        // 图像导引头被选为外引导源：启动定时器，每1秒或20ms发送一次跟踪数据
        if(exsrcindex == ExguideSrcImg )
        {
            // 串口未打开时不启动外引导，也不写跟踪日志；只取消本路外引导源
            if (!isOpen()) {
                if (m_exGuideTimer) {
                    m_exGuideTimer->stop();
                    disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
                }
                m_lastexguidesetting = -1;
                m_send20msTimeSyncSent = false;
                exsrcindex = -1;
                emit exguideLinkLost(
                    QStringLiteral("图像导引头串口未打开，已取消图像导引头外引导源选择"));
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
                    //清除上一模式的周期状态
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
                            emit exguideLinkLost(
                                QStringLiteral("图像导引头串口已断开，已取消图像导引头外引导源选择"));
                            return;
                        }
                        //可以加入一个帧编号的判断，如果帧编号没有更新，就不再进行外引导，而是弹窗提示，并退出外引导模式.帧编号从data指针中的bFrameSequence()获取
                        //读取帧编号如果帧编号没变化，则认为通信中断
                        // 帧编号停止更新 → 导引头已不再回传数据：本拍不再外引导，
                        // 直接退出外引导模式并弹窗提示（见 abortExGuideByLinkLost）
                        if (!isImageFrameUpdated()) {
                            abortExGuideByLinkLost();
                            return;
                        }
                        // 按单调时间选下一目标秒，并在目标秒开始前200ms发送。
                        const auto slot = m_periodicTrackSchedule.currentSlot();
                        // 测量样本已经逐帧发送；这里只触发转台生成下一组
                        // 基于绝对目标角的 1 秒预测轨迹。
                        emit exGuide1sTick(3, slot.firstPredictionDelaySec);
                        LogManager::instance()->logImageTracking(m_azimuth, m_pitch);
                        m_exGuideTimer->start(static_cast<int>(
                            m_periodicTrackSchedule.delayUntilNextSendMs(
                                slot.rawTargetSecond)));
                    });
                    // 开始外引导：以当前帧编号为基准启动看门狗
                    armExGuideFrameWatch();
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
                            emit exguideLinkLost(
                                QStringLiteral("图像导引头串口已断开，已取消图像导引头外引导源选择"));
                            return;
                        }
                        // 20ms 模式同样用帧编号判断通信是否正常：
                        // 帧编号未更新则不再发送外推角度，退出外引导并弹窗提示
                        if (!isImageFrameUpdated()) {
                            abortExGuideByLinkLost();
                            return;
                        }
                        // 20ms模式直接发送当前方位角与俯仰角，不使用Kalman外推。
                        emit reqExsend_20ms(m_azimuth, m_pitch);
                        LogManager::instance()->logImageTracking(m_azimuth, m_pitch);    
                    });
                    // 开始外引导：以当前帧编号为基准启动看门狗
                    armExGuideFrameWatch();
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
                m_exGuideTimer->stop(); //切换到其他外引导源时，暂停图像导引头外引导定时器，停止继续发送
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
        // 非外引导模式：停止并清除本次外引导绑定，
        // 再次进入外引导时必须重新选择外引导源和跟踪周期。
        if (m_exGuideTimer) {
            m_exGuideTimer->stop();
            disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
        }
        exsrcindex = -1;
        exguidesetting = -1;
        m_lastexguidesetting = -1;
        m_send20msTimeSyncSent = false;
        // 退出外引导模式：下次进入时重新初始化滤波器
        m_abNeedReset = true;
    }
}

// ── 图像头串口通信看门狗（帧编号）──
// 串口成功打开后每 500ms 检查一次帧编号，连续 2s 未变化则报警；
// 帧号恢复变化后清除已提示状态，
// 这样同一次打开期间发生多次通信中断时，每次都能重新弹窗。
void SerialPortImage::startFrameWatch()
{
    m_lastWatchedFrameSeq = m_imageData ? m_imageData->bFrameSequence() : -1;
    m_frameFaultNotified = false;
    m_frameWatchClock.start();
    m_frameWatchTimer->start(Image_FramePollMs);
}

void SerialPortImage::stopFrameWatch()
{
    m_frameWatchTimer->stop();
    m_lastWatchedFrameSeq = -1;
    m_frameFaultNotified = false;
}

void SerialPortImage::checkImageFrameAlive()
{
    // 不能以 isOpen() 作为停止条件：拔掉串口线时 QSerialPort 往往先报
    // ResourceError 并由 SerialPort::handleError() 关闭端口，此时帧号看门狗
    // 若立即退出，就永远等不到下面的 2s 帧号未变化判定。
    // 用户主动关闭串口的路径已经在 onClosePort() 中先调用 stopFrameWatch()，
    // 因此这里继续计时不会把正常关闭误报成通信故障。
    const int seq = m_imageData ? m_imageData->bFrameSequence() : -1;
    if (seq != m_lastWatchedFrameSeq) {
        m_lastWatchedFrameSeq = seq;
        m_frameWatchClock.restart();
        m_frameFaultNotified = false;
        return;
    }

    // 帧号未变化时按实际经过时间判断，避免把“检查周期”误当成“未变化时长”。
    if (m_frameWatchClock.elapsed() < Image_FrameLostMs)
        return;

    if (!m_frameFaultNotified) {
        m_frameFaultNotified = true;
        m_frameWatchTimer->stop();
        emit imageCommunicationFault(
            QStringLiteral("图像头通信故障：帧编号连续2s未变化"));

        // 通信故障后本次串口会话已经失效。即使 QSerialPort 因拔线已自行
        // 关闭，也仍需发送 portClosed，让主线程的 imageData.portOpen 复位；
        // QML 的串口按钮据此从“关闭串口”切回“打开串口”。
        SerialPort::close();
        emit portClosed();
    }
}

// ── 外引导通信看门狗（帧编号）──
// 外引导定时器启动前调用：以当前帧编号为基准开始计时，
// 之后每拍检查帧编号是否还在变化
void SerialPortImage::armExGuideFrameWatch()
{
    m_lastExGuideFrameSeq = m_imageData ? m_imageData->bFrameSequence() : -1;
    m_exGuideFrameClock.start();
}

// 判断导引头是否仍在回传数据：帧编号超过 Exguide_FrameLostMs 未变化即认为通信中断
bool SerialPortImage::isImageFrameUpdated()
{
    // 没有注入 ImageData 时无从判断链路，保持原有行为（继续外引导）
    if (!m_imageData)
        return true;

    // 帧编号由主线程解析帧数据时写入（原子量），这里只取一次快照
    const int seq = m_imageData->bFrameSequence();
    if (seq != m_lastExGuideFrameSeq) {
        m_lastExGuideFrameSeq = seq;
        m_exGuideFrameClock.restart();
        return true;
    }

    // 帧编号没变：还没超过判定时长就再等等（5ms 定时器同一帧内会多次进入本函数）
    if (!m_exGuideFrameClock.isValid())
        return true;
    return m_exGuideFrameClock.elapsed() < Exguide_FrameLostMs;
}

// 通信中断处理：停止本路外引导，保留外引导模式并请求弹窗提示
void SerialPortImage::abortExGuideByLinkLost()
{
    // 先停掉定时器并解绑，保证中断期间不会再把外推角度发给转台。
    if (m_exGuideTimer) {
        m_exGuideTimer->stop();
        disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
    }

    // 外引导数据源已经停止，立即请求转台三轴停车，避免继续执行已下发的轨迹。
    emit reqStopExGuideAxes();

    m_send20msTimeSyncSent = false;
    m_abNeedReset = true;          // 重新选择图像源时用当帧测量重建滤波器
    m_lastexguidesetting = -1;     // 本次绑定作废，重新选中图像源时会重新绑定并重启定时器
    m_lastExGuideFrameSeq = -1;    // 看门狗复位
    m_exGuideFrameClock.invalidate();
    // 保留外引导模式，只清除本路导引头来源；界面取消图像导引头选择后，
    // 可重新选择图像源以恢复外引导（届时会重新收到 ExguideSrcImg）。
    exsrcindex = -1;

    // 工作线程只发信号：主线程收到后弹窗提示，并只取消图像导引头外引导源选择
    emit exguideLinkLost(QStringLiteral("图像导引头帧编号未更新，已取消图像导引头外引导源选择"));
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

    // 1 秒模式只上报逐帧相对测量。绝对目标合成和 AB 预测由转台线程完成，
    // 避免把转台自身运动误识别成目标角速度。
    if (exindex == ExguideMode && exguidesetting == Exguide_1s) {
        exGuideMeasurement sample;
        sample.sourceId = 3;
        sample.azimuth = m_azimuth;
        sample.elevation = m_pitch;
        sample.timestampMs = QDateTime::currentMSecsSinceEpoch();
        emit exGuideMeasurementReady(sample);
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
