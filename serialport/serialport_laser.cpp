#include "serialport_laser.h"
#include <QDebug>
#include <QString>
#include "../log/LogManager.h"
#include <QTimer>
// ─────────────────────────────────────────────
// 外引导模式常量
// ─────────────────────────────────────────────
#define ExguideMode     0
#define ExguideSrcLaser 4
#define Exguide_5ms     6
#define Exguide_1s      7
#define Maxsendcount    600  // 每10分钟进行一次时间同步
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
// void LaserData::sendData(const QByteArray &data)                { emit requestSendData(data); }

// ── 工作线程回推：QueuedConnection 调用，更新状态并 NOTIFY QML ──
void LaserData::setPortOpen(bool open) { if (m_portOpen != open) { m_portOpen = open; emit portOpenChanged(); } }
void LaserData::setPortList(const QStringList &ports) { if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); } }
void LaserData::setError(const QString &msg) { if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); } }

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
        emit faultInfoChanged();
        emit faultInfo1Changed();
        emit faultInfo2Changed();
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

void LaserSendData::buildFrame() 
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
    frame.XOR_result = 0;
    
    emit requestSendData(frame);
}

// ─────────────────────────────────────────────
// SerialPortLaser
// ─────────────────────────────────────────────

SerialPortLaser::SerialPortLaser(QObject *parent)
    : SerialPort(parent)
    , m_laserData(new LaserData(nullptr))        // 留在主线程，不随 moveToThread 迁移
    , m_laserSendData(new LaserSendData(nullptr))
    , m_circularbuf(360)
{

}

SerialPortLaser::~SerialPortLaser() {
    //由app管理，无需手动释放
    // delete m_laserData;
    // delete m_laserSendData;
}

// ── Worker slots：接收主线程 Data 的请求（QueuedConnection）──
void SerialPortLaser::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}
void SerialPortLaser::onClosePort()  { SerialPort::close(); emit portClosed(); }
void SerialPortLaser::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }
void SerialPortLaser::onSendData(laser_send_frame frame)
 {  
    const uint8_t* mydata = reinterpret_cast<const uint8_t*>(&frame);
     //进行部分数据大端序转化，大端序转化结束后再计算异或校验位
    frame.laser_period = (static_cast<qint16>(static_cast<unsigned char>(mydata[7])) << 8) | static_cast<unsigned char>(mydata[6]);
    frame.param2 = (static_cast<qint16>(static_cast<unsigned char>(mydata[9])) << 8) | static_cast<unsigned char>(mydata[8]);
    frame.param3 = (static_cast<qint16>(static_cast<unsigned char>(mydata[11])) << 8) | static_cast<unsigned char>(mydata[10]);
    frame.param4 = (static_cast<qint16>(static_cast<unsigned char>(mydata[13])) << 8) | static_cast<unsigned char>(mydata[12]);
    frame.param5 = (static_cast<qint16>(static_cast<unsigned char>(mydata[15])) << 8) | static_cast<unsigned char>(mydata[14]);
    const uint8_t* checkdata = reinterpret_cast<const uint8_t*>(&frame);
    uint8_t checksum = 0;
    //去掉开头的三个字节与结尾的一个校验位字节
    for (size_t i = 3; i < sizeof(frame) - 1; ++i) {
        checksum ^= checkdata[i];
    }
    frame.XOR_result = checksum;
    
    auto data = QByteArray(reinterpret_cast<const char*>(&frame), sizeof(frame));

    //断开上一次的连接
    disconnect(timer, &QTimer::timeout, this, nullptr);

    int sendCount = 0;
    static quint8 datacount=0;
    timer->setInterval(10); // 10ms
    // 连接定时器的超时信号
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        // 发送数据
        SerialPort::send(data);
        
        sendCount++;
        //统计并更改发送次数,重新进行校验位计算
        datacount++;
        if(datacount >= 3){
            datacount=0;
        }
        data[3]= 0x11 | (datacount << 6);

        const char * checkdata2= data.data();
        uint8_t checksum2 = 0;
        //计算异或校验位并更新,不计入帧头与校验位
        for (size_t i = 3; i < data.size() - 1; ++i) {
        checksum2 ^= checkdata2[i];
        }
        data[16] = checksum2;
        // 发送5次后停止并销毁定时器
        if (sendCount >= 10) {
            timer->stop();
            // timer->deleteLater();
        }
    });

    //  启动定时器（立即触发第一次发送，若想先等20ms再发，可改为 timer->start(20) 但默认立即触发）
    timer->start();
 }

void SerialPortLaser::onReadyRead() {
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
        frame.frame_header1 = static_cast<quint8>(rawData[0]);
        frame.frame_header2 = static_cast<quint8>(rawData[1]);
        frame.frame_header3 = static_cast<quint8>(rawData[2]);
        frame.frame_status = static_cast<quint8>(rawData[3]);
        frame.frame_ID = static_cast<quint8>(rawData[4]);
        frame.dyt_status = static_cast<qint8>(rawData[5]);
        frame.detector_status = static_cast<qint8>(rawData[6]);
        frame.fault_info = static_cast<qint8>(rawData[7]);

        // 处理大端序的16位数据
        frame.optical_azimuth = (static_cast<qint16>(static_cast<unsigned char>(rawData[8])) << 8) | static_cast<unsigned char>(rawData[9]);
        frame.optical_pitch = (static_cast<qint16>(static_cast<unsigned char>(rawData[10])) << 8) | static_cast<unsigned char>(rawData[11]);
        frame.gyro_azimuth_rate = (static_cast<qint16>(static_cast<unsigned char>(rawData[12])) << 8) | static_cast<unsigned char>(rawData[13]);
        frame.gyro_pitch_rate = (static_cast<qint16>(static_cast<unsigned char>(rawData[14])) << 8) | static_cast<unsigned char>(rawData[15]);
        frame.los_azimuth_rate = (static_cast<qint16>(static_cast<unsigned char>(rawData[16])) << 8) | static_cast<unsigned char>(rawData[17]);
        frame.los_pitch_rate = (static_cast<qint16>(static_cast<unsigned char>(rawData[18])) << 8) | static_cast<unsigned char>(rawData[19]);
        frame.deviation_azimuth = (static_cast<qint16>(static_cast<unsigned char>(rawData[20])) << 8) | static_cast<unsigned char>(rawData[21]);
        frame.deviation_pitch = (static_cast<qint16>(static_cast<unsigned char>(rawData[22])) << 8) | static_cast<unsigned char>(rawData[23]);
        frame.laser_period = (static_cast<quint16>(static_cast<unsigned char>(rawData[24])) << 8) | static_cast<unsigned char>(rawData[25]);
        frame.reserved1[0] = (static_cast<quint8>(static_cast<unsigned char>(rawData[27])));
        frame.reserved1[1] = (static_cast<quint8>(static_cast<unsigned char>(rawData[26])));
        frame.gain_status = static_cast<qint8>(rawData[28]);
        frame.quadrant1_energy = static_cast<quint8>(rawData[29]);
        frame.quadrant2_energy = static_cast<quint8>(rawData[30]);
        frame.quadrant3_energy = static_cast<quint8>(rawData[31]);
        frame.quadrant4_energy = static_cast<quint8>(rawData[32]);
        frame.software_version1 = (static_cast<qint16>(static_cast<unsigned char>(rawData[33])) << 8) | static_cast<unsigned char>(rawData[34]);
        frame.software_version2 = (static_cast<qint16>(static_cast<unsigned char>(rawData[35])) << 8) | static_cast<unsigned char>(rawData[36]);
        frame.XOR_result = static_cast<quint8>(rawData[37]);
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
    
    //判断激光导引头是否被选中为外引导源，是的话更新数据（1s跟踪模式）
    if(exindex == ExguideMode)
    {
        m_filterTime += 10;  // 激光数据周期10ms
        m_abMgr.FeedData(m_filterTime, m_azimuth, m_pitch);
    }
}

void SerialPortLaser::ExmodeChanged(int mode)
{
    //1:外引导  2：程控模式 3：遥控模式  //还需要判断外引导源  //判断跟踪模式
    //判断使用哪个导引头的数据，来决定是否定期向转台串口线程同步数据
    //判断index与外引导模式数据选择提供位，如果被选中，就启动一个定时器，每1秒或5ms发送一次跟踪数据信息
    //先发送时间同步指令信号，再发送Alpha-Beta预测的跟踪角度数据
    if(mode < 3)
        exindex = mode;  //模式索引赋值
    if(mode > 2 && mode < 6)
        exsrcindex = mode;  //外引导源索引赋值 索引分别为3，4，5
    if(mode > 5)
        exguidesetting = mode; //外引导发送时间间隔选择  索引为6，7

    if (exindex == ExguideMode) //判断是否为外引导模式
    {   
        // 激光导引头被选为外引导源：启动定时器，每1秒或5ms发送一次跟踪数据
        if(exsrcindex == ExguideSrcLaser)
        {
            // 判断激光导引头串口是否开启，未开启时触发弹窗提示
            if (!isOpen()) {
                emit exguideSerialNotOpen("激光导引头串口未打开");
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
                        LogManager::instance()->logLaserTracking(az_pkt.angle1, el_pkt.angle1);
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
                        // 5ms模式直接发送角度数据
                        emit reqExsend_5ms(m_azimuth, m_pitch);
                        LogManager::instance()->logLaserTracking(m_azimuth, m_pitch);
                    });
                    m_exGuideTimer->start(5); // 每5ms触发一次
                    m_lastexguidesetting = Exguide_5ms;
                }
            }
        }
        else
        {   
            if (m_exGuideTimer) {
                m_exGuideTimer->stop(); //切换到其他外引导源时，暂停激光导引头外引导定时器，停止继续发送
            }
        }
    }
    else
    {   
        exsrcindex = -1; //非外引导模式下，外引导源索引置0
        exguidesetting = -1; //非外引导模式下，外引导发送时间间隔索引置0
        if (m_exGuideTimer) {
            m_exGuideTimer->stop();
        }
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
