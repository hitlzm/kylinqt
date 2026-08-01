#include "serialport_CCD.h"
#include <QDebug>
#include <QTimeZone>
#include "../log/LogManager.h"
// ─────────────────────────────────────────────
// 外引导模式常量
// ─────────────────────────────────────────────
#define ExguideMode    0
#define ExguideSrcCCD  5
#define Exguide_5ms    6
#define Exguide_1s     7
#define Maxsendcount   600  // 每10分钟进行一次时间同步

SerialPortCCD::SerialPortCCD(QObject *parent)
    : SerialPort(parent)
    , m_ccdData(new CCDData(nullptr))        // 留在主线程，不随 moveToThread 迁移
{   
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::errorOccurred,
            this, &SerialPort::handleError);
    onScanPorts();
    // CCDData 请求信号 → SerialPortCCD 执行槽
    connect(m_ccdData, &CCDData::req30XFocus,       this, &SerialPortCCD::send30XFocus);
    connect(m_ccdData, &CCDData::req1XFocus,        this, &SerialPortCCD::send1XFocus);
    connect(m_ccdData, &CCDData::reqdigZoomopen,    this, &SerialPortCCD::senddigZoomopen);
    connect(m_ccdData, &CCDData::reqdigZoomclose,   this, &SerialPortCCD::senddigZoomclose);
    connect(m_ccdData, &CCDData::reqBacklightopen,  this, &SerialPortCCD::sendBacklightopen);
    connect(m_ccdData, &CCDData::reqBacklightclose, this, &SerialPortCCD::sendBacklightclose);
    connect(m_ccdData, &CCDData::reqResolutionchange, this, &SerialPortCCD::sendResolutionchange);
}

void SerialPortCCD::send30XFocus()
{
    m_serialPort->write(cmd_30X);
    m_hFov = 2.66f;
    m_vFov = 1.51f;
    updateFovLimits();
}
void SerialPortCCD::send1XFocus()
{
    m_serialPort->write(cmd_1X);
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
}
void SerialPortCCD::senddigZoomopen()
{
    qint64 bytesWritten = m_serialPort->write(cmd_digZoomopen);
}
void SerialPortCCD::senddigZoomclose()
{
    qint64 bytesWritten = m_serialPort->write(cmd_digZoomclose);
}
void SerialPortCCD::sendBacklightopen()
{
    qint64 bytesWritten = m_serialPort->write(cmd_Backlightopen);
}
void SerialPortCCD::sendBacklightclose()
{
    qint64 bytesWritten = m_serialPort->write(cmd_Backlightclose);
}
void SerialPortCCD::sendResolutionchange(int index)
{
    switch(index){
        case 0: {
            m_serialPort->write(cmd_Resolutionchange1);
            m_imageWidth  = 1920;
            m_imageHeight = 1080;
            break;
        }
        case 1: {
            m_serialPort->write(cmd_Resolutionchange2);
            m_imageWidth  = 1280;
            m_imageHeight = 720;
            break;
        }
        case 2: {
            m_serialPort->write(cmd_Resolutionchange1);
            m_imageWidth  = 1920;
            m_imageHeight = 1080;
            break;
        }
        case 3: {
            m_serialPort->write(cmd_Resolutionchange2);
            m_imageWidth  = 1280;
            m_imageHeight = 720;
            break;
        }
        default: break;
    }
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

    // 外引导模式下，将计算出的角度喂入 Alpha-Beta 滤波器
    if (exindex == ExguideMode) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();
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
                        // 记录追踪日志
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
        }
    }
    else
    {
        if (m_exGuideTimer) {
            m_exGuideTimer->stop();
        }
    }
}

// 串口操作槽函数
void SerialPortCCD::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}
void SerialPortCCD::onClosePort()  { SerialPort::close(); emit portClosed(); }
void SerialPortCCD::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }

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
