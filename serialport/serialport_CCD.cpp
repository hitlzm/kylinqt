#include "serialport_CCD.h"
#include <QDebug>
#include <QTimeZone>
#include "../log/LogManager.h"
#include "../utils/seeker_coordinate_transform.h"
// ─────────────────────────────────────────────
// 外引导模式常量
// ─────────────────────────────────────────────
#define ExguideMode    0
#define ExguideSrcCCD  5
#define Exguide_5ms    6
#define Exguide_1s     7
#define Maxsendcount   600  // 每10分钟进行一次时间同步

namespace {
constexpr int kMotionSpeed = 4;  // 变焦/调焦连续运动速度 0-7

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
    // CCDData 请求信号 → SerialPortCCD 执行槽（连接在 main.cpp 中统一管理）
}

void SerialPortCCD::send30XFocus()
{
    writeCommand(cmd_30X);
    m_hFov = 2.66f;
    m_vFov = 1.51f;
    updateFovLimits();
}
void SerialPortCCD::send1XFocus()
{
    writeCommand(cmd_1X);
    m_hFov = 55.27f;
    m_vFov = 32.26f;
    updateFovLimits();
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
void SerialPortCCD::senddigZoomopen()
{
    writeCommand(cmd_digZoomopen);
}
void SerialPortCCD::senddigZoomclose()
{
    writeCommand(cmd_digZoomclose);
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
}

void SerialPortCCD::sendZoomStop()
{
    writeCommand(QByteArray::fromHex("8101040700FF"));
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

    // 像素偏移 → 角度：偏移量 / 图像宽度 × 视场角，带正负方向
    m_azimuth = (centerX - m_imageWidth  / 2.0) / m_imageWidth  * m_hFov;
    m_pitch   = (centerY - m_imageHeight / 2.0) / m_imageHeight * m_vFov;

    //考虑到CCD安装有一定倾斜角，要进行CCD到转台内框基坐标系的变换,CCD转角固定为22.5度
    if (m_rotation_angle != 0) {
        auto ang = seeker::deRollAboutBoresightDeg(m_azimuth, m_pitch, m_rotation_angle);
        m_azimuth = ang.az;
        m_pitch = ang.el;
    }

    // 外引导模式下，将计算出的角度喂入 Alpha-Beta 滤波器
    if (exindex == ExguideMode) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();

        if (m_abNeedReset)
        {
            // 进入外引导后首次收到本路数据：用本帧测量重建滤波器，
            // 避免沿用上一次使用时的陈旧角速度（否则第 1 包会被旧速度推到视场边缘）。
            // 复位后同一时间戳喂入的那一帧会被滤波器的时间戳守卫按“重复帧”丢弃，
            // 状态已由 Init 建立，不影响后续帧。
            m_abMgr.Init(m_azimuth, m_pitch, now);
            m_abNeedReset = false;
        }

        m_abMgr.FeedData(now, m_azimuth, m_pitch);
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
        // CCD相机被选为外引导源：启动定时器，每1秒或5ms发送一次跟踪数据
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
                m_sendCount_1s = 0;
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
                    m_sendCount_1s = 0;
                }
                // 延迟创建定时器
                if (!m_exGuideTimer) {
                    m_exGuideTimer = new QTimer(this);
                    m_exGuideTimer->setTimerType(Qt::PreciseTimer);
                }
                // 跟踪周期变化后，下一帧数据到来时用当时的测量重新初始化滤波器
                m_abNeedReset = true;
                // 判断跟踪模式（5ms模式或者1秒跟踪模式）
                if(exguidesetting == Exguide_1s)
                {
                    connect(m_exGuideTimer, &QTimer::timeout, this, [this]() {
                        // 每发送完固定次数后，重新发送时间同步信号并重新计时
                        if(m_sendCount_1s == 0)
                        {
                            emit reqTimesync();
                        }
                        // 生成预测数据包：4点外推（0.25s, 0.5s, 0.75s, 1.0s），time = 发包计数
                        sendExGuideData az_pkt = m_abMgr.GenAxisPacket(true,  m_sendCount_1s);  // 方位轴
                        sendExGuideData el_pkt = m_abMgr.GenAxisPacket(false, m_sendCount_1s);  // 俯仰轴
                        // 发送预测角度给转台
                        emit reqExsend_1s(az_pkt, el_pkt);
                        // 串口未打开时不输出外引导跟踪日志
                        if (isOpen())
                            LogManager::instance()->logCCDTracking(az_pkt.angle1, el_pkt.angle1);
                        if(++m_sendCount_1s >= Maxsendcount) {  // 10分钟重同步
                            m_sendCount_1s = 0;
                        }
                    });
                    m_exGuideTimer->start(1000); // 每1s触发一次
                    m_lastexguidesetting = Exguide_1s;
                }
                else if(exguidesetting == Exguide_5ms)
                {
                    connect(m_exGuideTimer, &QTimer::timeout, this, [this]() {
                        // 5ms模式直接发送当前计算角度
                        emit reqExsend_5ms(m_azimuth, m_pitch);
                        // 串口未打开时不输出外引导跟踪日志
                        if (isOpen())
                            LogManager::instance()->logCCDTracking(m_azimuth, m_pitch);
                    });
                    m_exGuideTimer->start(15); // 每5ms触发一次 ，考虑到数据更新没那么快，切换为15ms模式
                    m_lastexguidesetting = Exguide_5ms;
                }
            }
        }
        else
        {
            if (m_exGuideTimer) {
                m_exGuideTimer->stop(); //切换到其他外引导源时，暂停CCD外引导定时器
            }
            // 本路已不是外引导源：下次被选中时重新初始化滤波器
            m_abNeedReset = true;
        }
    }
    else
    {   
        exsrcindex = -1; //非外引导模式下，外引导源索引置0
        exguidesetting = -1; //非外引导模式下，外引导发送时间间隔索引置0
        if (m_exGuideTimer) {
            m_exGuideTimer->stop();
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
        m_sendCount_1s = 0;
        exsrcindex = -1;
        m_abNeedReset = true;
        emit exguideSourceUnavailable();
    }
}

// 串口操作槽函数
void SerialPortCCD::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
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
