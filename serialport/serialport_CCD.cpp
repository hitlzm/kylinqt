#include "serialport_CCD.h"
#include <QDebug>
#include <QtMath>
#include <QTimeZone>
#include <cmath>
#include "../log/LogManager.h"
#include "../utils/seeker_coordinate_transform.h"
// ─────────────────────────────────────────────
// 外引导模式常量
// ─────────────────────────────────────────────
#define ExguideMode    0
#define ExguideSrcCCD  5
#define Exguide_20ms   6
#define Exguide_1s     7

namespace {
constexpr int kMotionSpeed = 2;  // 变焦/调焦连续运动速度 0-7

constexpr double kZoomWideFocalMm = 5.1;
constexpr double kZoomTeleFocalMm = 153.0;
constexpr double kZoomWideHFov = 55.27;
constexpr double kZoomWideVFov = 32.26;
constexpr double kZoomTeleHFov = 2.66;
constexpr double kZoomTeleVFov = 1.51;

// 用广角/远焦两组实测视场角拟合针孔模型。指数由两端标定反算，
// 可保证倍率 1x/30x 时与说明书参数完全一致。
constexpr double kHFovCalibration = 0.9160818305959834;
constexpr double kVFovCalibration = 0.9080856044332373;

const quint8 kResolutionCodes[] = {
    0x07, 0x08, 0x0A, 0x0C, 0x0F, 0x11, 0x10, 0x12, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D
};
}

SerialPortCCD::SerialPortCCD(QObject *parent)
    : SerialPort(parent)
    , m_ccdData(new CCDData(nullptr))        // 主线程 Data 对象，main.cpp 中替换为 ccdData
{
    // QSerialPort 由 dowork() 创建（CCD 串口固定运行在主线程，dowork() 在 main.cpp 中直接调用）
    onScanPorts();
    updateFovLimits();
    // CCDData 请求信号 → SerialPortCCD 执行槽（连接在 main.cpp 中统一管理）
}

void SerialPortCCD::send30XFocus()
{
    writeCommand(cmd_30X);
    updateOpticalZoom(30.0);
}
void SerialPortCCD::send1XFocus()
{
    writeCommand(cmd_1X);
    updateOpticalZoom(1.0);
}

void SerialPortCCD::updateFovLimits()
{
    // 半视场角 = 限幅范围
    double azLimit = m_hFov / 2.0;
    double elLimit = m_vFov / 2.0;
    m_abMgr.AzTracker().setClampRange(-azLimit, azLimit);
    m_abMgr.ElTracker().setClampRange(-elLimit, elLimit);

    // 远焦视场更小，像素抖动折算到角度后的异常波动也应更小。
    const bool tele = m_hFov < 10.0f;
    const double gateMin = tele ? 0.20 : 0.50;
    const double gateMax = tele ? 0.60 : 1.50;
    m_abMgr.AzTracker().setInnovationGate(gateMin, gateMax, 4.0, 0.5, 3);
    m_abMgr.ElTracker().setInnovationGate(gateMin, gateMax, 4.0, 0.5, 3);
}

void SerialPortCCD::requestZoomRatio()
{
    writeCommand(QByteArray::fromHex("8109098314FF"));
}

void SerialPortCCD::updateOpticalZoom(double zoomRatio)
{
    const double zoom = qBound(1.0, zoomRatio, 30.0);
    if (qAbs(m_opticalZoom - zoom) < 0.005)
        return;

    // 焦距与倍率近似成正比；指数项用广角/远焦两端标定修正镜头模型。
    const double focalMm = kZoomWideFocalMm * zoom;
    const double focalScale = kZoomWideFocalMm / focalMm;
    const double hHalfRad = qDegreesToRadians(kZoomWideHFov / 2.0);
    const double vHalfRad = qDegreesToRadians(kZoomWideVFov / 2.0);
    const double hFov = qRadiansToDegrees(
                2.0 * std::atan(std::tan(hHalfRad)
                                * std::pow(focalScale, kHFovCalibration)));
    const double vFov = qRadiansToDegrees(
                2.0 * std::atan(std::tan(vHalfRad)
                                * std::pow(focalScale, kVFovCalibration)));

    // 两端标定在浮点运算后可能有极小偏差，直接使用保证边界值准确。
    if (zoom <= 1.0001) {
        m_hFov = static_cast<float>(kZoomWideHFov);
        m_vFov = static_cast<float>(kZoomWideVFov);
    } else if (zoom >= 29.9999) {
        m_hFov = static_cast<float>(kZoomTeleHFov);
        m_vFov = static_cast<float>(kZoomTeleVFov);
    } else {
        m_hFov = static_cast<float>(hFov);
        m_vFov = static_cast<float>(vFov);
    }
    m_opticalZoom = zoom;
    updateFovLimits();
}

void SerialPortCCD::senddigZoomopen()
{
    writeCommand(cmd_digZoomopen);
    requestZoomRatio();
}
void SerialPortCCD::senddigZoomclose()
{
    writeCommand(cmd_digZoomclose);
    requestZoomRatio();
}
void SerialPortCCD::sendBacklightopen()
{
    writeCommand(cmd_Backlightopen);
}
void SerialPortCCD::sendBacklightclose()
{
    writeCommand(cmd_Backlightclose);
}
void SerialPortCCD::sendResolutionchange(int index)
{
    const int count = static_cast<int>(sizeof(kResolutionCodes)
                                       / sizeof(kResolutionCodes[0]));
    if (index < 0 || index >= count)
        return;

    QByteArray command;
    command.reserve(8);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(0x24)).append(char(0x73)).append(char(0x00))
            .append(char(kResolutionCodes[index])).append(char(0xFF));
    writeCommand(command);

    switch (index) {
    case 0: case 1: case 8: case 9:
        m_imageWidth = 1920; m_imageHeight = 1080; break;
    case 2: case 3: case 4: case 5:
        m_imageWidth = 1280; m_imageHeight = 720; break;
    case 6: case 7:
        m_imageWidth = 1280; m_imageHeight = 960; break;
    case 10: case 11: case 12: case 13:
        m_imageWidth = 2688; m_imageHeight = 1520; break;
    case 14: case 15: case 16: case 17:
        m_imageWidth = 3840; m_imageHeight = 2160; break;
    default: break;
    }
}

void SerialPortCCD::sendZoomMove(int direction)
{
    if (direction == 0) {
        sendZoomStop();
        return;
    }

    QByteArray command;
    command.reserve(6);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(0x07))
            .append(char((direction > 0 ? 0x20 : 0x30) | kMotionSpeed))
            .append(char(0xFF));
    writeCommand(command);
    requestZoomRatio();
}

void SerialPortCCD::sendZoomStop()
{
    writeCommand(QByteArray::fromHex("8101040700FF"));
    requestZoomRatio();
}

void SerialPortCCD::sendFocusMode(int mode)
{
    writeCommand(mode == 0
                 ? QByteArray::fromHex("8101043802FF")
                 : QByteArray::fromHex("8101043803FF"));
}

void SerialPortCCD::sendFocusMove(int direction)
{
    if (direction == 0) {
        sendFocusStop();
        return;
    }

    QByteArray command;
    command.reserve(6);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(0x08))
            .append(char((direction > 0 ? 0x20 : 0x30) | kMotionSpeed))
            .append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendFocusStop()
{
    writeCommand(QByteArray::fromHex("8101040800FF"));
}

void SerialPortCCD::sendDefogSetting(bool enabled, int level)
{
    QByteArray command;
    command.reserve(7);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(0x37));
    if (enabled) {
        command.append(char(0x02))
                .append(char(qBound(1, level, 3)))
                .append(char(0xFF));
    } else {
        command.append(char(0x03)).append(char(0x00)).append(char(0xFF));
    }
    writeCommand(command);
}

void SerialPortCCD::sendHeatHazeLevel(int level)
{
    QByteArray command;
    command.reserve(7);
    command.append(char(0x81)).append(char(0x01)).append(char(0x09))
            .append(char(0x83)).append(char(0x08))
            .append(char(qBound(0, level, 10))).append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendNoiseReductionSetting(bool enabled, int level)
{
    QByteArray command;
    command.reserve(11);
    command.append(char(0x81)).append(char(0x01)).append(char(0x09))
            .append(char(0x83)).append(char(0x0B));
    if (enabled) {
        const int strength = qBound(0, level, 100);
        command.append(char(0x01))
                .append(char((strength >> 8) & 0xFF))
                .append(char(strength & 0xFF))
                .append(char(0x00)).append(char(0x00));
    } else {
        command.append(QByteArray(5, '\0'));
    }
    command.append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendStabilizationSetting(bool enabled, int level)
{
    QByteArray command;
    command.reserve(7);
    command.append(char(0x81)).append(char(0x01)).append(char(0x09))
            .append(char(0x83)).append(char(0x07))
            .append(char(enabled ? qBound(1, level, 3) : 0))
            .append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::sendWhiteBalanceMode(int mode)
{
    writeCommand(mode == 1
                 ? QByteArray::fromHex("8101043505FF")
                 : QByteArray::fromHex("8101043500FF"));
}

void SerialPortCCD::sendWhiteBalanceGain(bool red, int gain)
{
    const int value = qBound(0, gain, 255);
    QByteArray command;
    command.reserve(9);
    command.append(char(0x81)).append(char(0x01)).append(char(0x04))
            .append(char(red ? 0x43 : 0x44))
            .append(char(0x00)).append(char(0x00))
            .append(char((value >> 8) & 0xFF)).append(char(value & 0xFF))
            .append(char(0xFF));
    writeCommand(command);
}

void SerialPortCCD::recvTargetCenter(int centerX, int centerY)
{
    m_targetCenterX = centerX;
    m_targetCenterY = centerY;

    // 目标无效（未检测到），不计算角度
    if (centerX < 0 || centerY < 0)
        return;

    // 当前 FOV 下的针孔投影：归一化像素先乘 tan(FOV/2)，再取 atan。
    // 直接按像素比例乘 FOV 会在线性近似下产生误差，变焦到窄视场后尤其明显。
    const double normalizedX = (centerX - m_imageWidth / 2.0)
            / (m_imageWidth / 2.0);
    const double normalizedY = (centerY - m_imageHeight / 2.0)
            / (m_imageHeight / 2.0);
    m_azimuth = qRadiansToDegrees(
                std::atan(normalizedX * std::tan(qDegreesToRadians(m_hFov / 2.0))));
    m_pitch = qRadiansToDegrees(
                std::atan(normalizedY * std::tan(qDegreesToRadians(m_vFov / 2.0))));

    //考虑到CCD安装有一定倾斜角，要进行CCD到转台内框基坐标系的变换,CCD转角固定为22.5度
    if (m_rotation_angle != 0) {
        auto ang = seeker::deRollAboutBoresightDeg(m_azimuth, m_pitch, m_rotation_angle);
        m_azimuth = ang.az;
        m_pitch = ang.el;
    }

    // 1 秒模式只上报逐帧相对测量，绝对目标合成和预测由转台线程负责。
    if (exindex == ExguideMode && exguidesetting == Exguide_1s) {
        exGuideMeasurement sample;
        sample.sourceId = 5;
        sample.azimuth = m_azimuth;
        sample.elevation = m_pitch;
        sample.timestampMs = QDateTime::currentMSecsSinceEpoch();
        emit exGuideMeasurementReady(sample);
    }
}

void SerialPortCCD::ExmodeChanged(int mode)
{
    // CCD 模式切换处理
    // 判断当前模式：
    //   0 — 外引导模式
    //   1 — 程控模式
    //   2 — 遥控模式  ,运行模式，外引导源，外引导跟踪周期都是通过mode参数来传递
    if(mode < 3)
        exindex = mode;  //模式索引赋值
    if(mode > 2 && mode < 6)
        exsrcindex = mode;  //外引导源索引赋值 索引分别为3，4，5
    if(mode > 5)
        exguidesetting = mode; //外引导发送时间间隔选择  索引为6，7

    if (exindex == ExguideMode) //判断是否为外引导模式
    {
        // CCD相机被选为外引导源：启动定时器，每1秒或20ms发送一次跟踪数据
        if(exsrcindex == ExguideSrcCCD)
        {
            // CCD 外引导依赖真实视频流；未接入时不启动外引导并取消本路外引导源
            if (!m_ccdData || !m_ccdData->videoStreamConnected()) {
                emit exguideSerialNotOpen(
                    QStringLiteral("CCD视频流未接入，已取消CCD相机外引导源选择"));
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
                        // 按单调时间选下一目标秒，并在目标秒开始前200ms发送。
                        const auto slot = m_periodicTrackSchedule.currentSlot();
                        // 测量已逐帧发送；这里只触发转台生成绝对目标预测包。
                        emit exGuide1sTick(5);
                        // 串口未打开时不输出外引导跟踪日志
                        if (isOpen())
                            LogManager::instance()->logCCDTracking(m_azimuth, m_pitch);
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
                        // 20ms模式直接发送当前计算角度，不使用Kalman外推。
                        emit reqExsend_20ms(m_azimuth, m_pitch);
                        // 串口未打开时不输出外引导跟踪日志
                        if (isOpen())
                            LogManager::instance()->logCCDTracking(m_azimuth, m_pitch);
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
                m_exGuideTimer->stop(); //切换到其他外引导源时，暂停CCD外引导定时器
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

void SerialPortCCD::onVideoStreamConnectedChanged()
{
    if (m_ccdData && !m_ccdData->videoStreamConnected()
            && exsrcindex == ExguideSrcCCD) {
        if (m_exGuideTimer) {
            m_exGuideTimer->stop();
            disconnect(m_exGuideTimer, &QTimer::timeout, nullptr, nullptr);
        }
        m_lastexguidesetting = -1;
        m_send20msTimeSyncSent = false;
        exsrcindex = -1;
        m_abNeedReset = true;
        emit exguideSourceUnavailable();
    }
}

void SerialPortCCD::parseData(const QByteArray &rawData)
{
    m_zoomReplyBuffer.append(rawData);

    // 应答可能与其他命令的 ACK 粘连，按 FF 分帧后只处理倍率查询应答。
    while (true) {
        const int endIndex = m_zoomReplyBuffer.indexOf(char(0xFF));
        if (endIndex < 0) {
            if (m_zoomReplyBuffer.size() > 128)
                m_zoomReplyBuffer.clear();
            return;
        }

        const QByteArray frame = m_zoomReplyBuffer.left(endIndex + 1);
        m_zoomReplyBuffer.remove(0, endIndex + 1);

        if (frame.size() < 7)
            continue;

        const quint8 address = static_cast<quint8>(frame.at(0));
        const quint8 reply = static_cast<quint8>(frame.at(1));
        if ((address & 0x0F) != 0x00 || reply != 0x50)
            continue;

        const int encoded = ((static_cast<quint8>(frame.at(2)) & 0x0F) << 12)
                | ((static_cast<quint8>(frame.at(3)) & 0x0F) << 8)
                | ((static_cast<quint8>(frame.at(4)) & 0x0F) << 4)
                | (static_cast<quint8>(frame.at(5)) & 0x0F);
        if (encoded > 0)
            updateOpticalZoom(encoded / 10.0);
    }
}

// 串口操作槽函数
void SerialPortCCD::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud)) {
        emit portOpened(true);
        requestZoomRatio();
    } else {
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
    }
}
void SerialPortCCD::onClosePort()
{
    // 关闭串口前先结束连续运动，避免机芯停留在变焦/调焦运动状态。
    if (isOpen()) {
        sendZoomStop();
        sendFocusStop();
        if (m_serialPort)
            m_serialPort->waitForBytesWritten(50);
    }
    SerialPort::close();
    emit portClosed();
}
void SerialPortCCD::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }

void SerialPortCCD::writeCommand(const QByteArray &command)
{
    if (!m_serialPort || !m_serialPort->isOpen())
        return;
    m_serialPort->write(command);
}

CCDData::CCDData(QObject *parent)
    : QObject(parent)
{

}

void CCDData::openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void CCDData::closePort()                                     { emit requestClosePort(); }
void CCDData::scanPorts()                                     { emit requestScanPorts(); }

void CCDData::setPortOpen(bool open) { if (m_portOpen != open) { m_portOpen = open; emit portOpenChanged(); } }
void CCDData::setPortList(const QStringList &ports) { if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); } }
void CCDData::setError(const QString &msg) { if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); } }

void CCDData::setFocusMode(int mode) {
    mode = qBound(0, mode, 1);
    if (m_focusMode != mode) {
        m_focusMode = mode;
        emit focusModeChanged();
        if (mode == 0)
            emit reqFocusStop();
        emit reqFocusMode(mode);
    }
}

void CCDData::startZoom(int direction)
{
    if (direction != 0)
        emit reqZoomMove(direction > 0 ? 1 : -1);
}

void CCDData::stopZoom()
{
    emit reqZoomStop();
}

void CCDData::startFocus(int direction)
{
    if (m_focusMode != 1 || direction == 0)
        return;
    emit reqFocusMove(direction > 0 ? 1 : -1);
}

void CCDData::stopFocus()
{
    emit reqFocusStop();
}

void CCDData::setDefog(bool enabled)
{
    if (m_defogEnabled == enabled)
        return;
    m_defogEnabled = enabled;
    emit defogEnabledChanged();
    emit reqDefogSetting(m_defogEnabled, m_defogLevel);
}

void CCDData::setDefogLevel(int level)
{
    level = qBound(1, level, 3);
    if (m_defogLevel == level)
        return;
    m_defogLevel = level;
    emit defogLevelChanged();
    if (m_defogEnabled)
        emit reqDefogSetting(true, m_defogLevel);
}

void CCDData::setHeatHazeLevel(int level)
{
    level = qBound(0, level, 10);
    if (m_heatHazeLevel == level)
        return;
    m_heatHazeLevel = level;
    emit heatHazeLevelChanged();
    emit reqHeatHazeLevel(m_heatHazeLevel);
}

void CCDData::setNoiseReduction(bool enabled)
{
    if (m_noiseReductionEnabled == enabled)
        return;
    m_noiseReductionEnabled = enabled;
    emit noiseReductionEnabledChanged();
    emit reqNoiseReductionSetting(enabled, m_noiseReductionLevel);
}

void CCDData::setNoiseReductionLevel(int level)
{
    level = qBound(0, level, 100);
    if (m_noiseReductionLevel == level)
        return;
    m_noiseReductionLevel = level;
    emit noiseReductionLevelChanged();
    if (m_noiseReductionEnabled)
        emit reqNoiseReductionSetting(true, level);
}

void CCDData::setStabilization(bool enabled)
{
    if (m_stabilizationEnabled == enabled)
        return;
    m_stabilizationEnabled = enabled;
    emit stabilizationEnabledChanged();
    emit reqStabilizationSetting(enabled, m_stabilizationLevel);
}

void CCDData::setStabilizationLevel(int level)
{
    level = qBound(1, level, 3);
    if (m_stabilizationLevel == level)
        return;
    m_stabilizationLevel = level;
    emit stabilizationLevelChanged();
    if (m_stabilizationEnabled)
        emit reqStabilizationSetting(true, level);
}

void CCDData::setBacklight(bool on) {
    if (m_backlightOn != on) {
        m_backlightOn = on;
        emit backlightOnChanged();
        if (on) emit reqBacklightopen();
        else emit reqBacklightclose();
    }
}

void CCDData::setResolution(int index) {
    if (index < 0 || index > 17) return;
    if (m_resolutionIndex != index) {
        m_resolutionIndex = index;
        emit resolutionIndexChanged();
        emit reqResolutionchange(index);
    }
}

void CCDData::setWhiteBalanceMode(int mode)
{
    mode = qBound(0, mode, 1);
    if (m_whiteBalanceMode == mode)
        return;
    m_whiteBalanceMode = mode;
    emit whiteBalanceModeChanged();
    emit reqWhiteBalanceMode(mode);
}

void CCDData::setRedGain(int gain)
{
    gain = qBound(0, gain, 255);
    if (m_redGain == gain)
        return;
    m_redGain = gain;
    emit redGainChanged();
    if (m_whiteBalanceMode == 1)
        emit reqWhiteBalanceGain(true, gain);
}

void CCDData::setBlueGain(int gain)
{
    gain = qBound(0, gain, 255);
    if (m_blueGain == gain)
        return;
    m_blueGain = gain;
    emit blueGainChanged();
    if (m_whiteBalanceMode == 1)
        emit reqWhiteBalanceGain(false, gain);
}

void CCDData::setVideoStreamConnected(bool connected)
{
    if (m_videoStreamConnected != connected) {
        m_videoStreamConnected = connected;
        emit videoStreamConnectedChanged();
    }
}
