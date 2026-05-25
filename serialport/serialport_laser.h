#ifndef SERIALPORT_LASER_H
#define SERIALPORT_LASER_H

#include "serialport.h"

class LaserData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int frameStatus READ frameStatus NOTIFY frameStatusChanged)
    Q_PROPERTY(int frameId READ frameId NOTIFY frameIdChanged)
    Q_PROPERTY(int dytStatus READ dytStatus NOTIFY dytStatusChanged)
    Q_PROPERTY(int detectorStatus READ detectorStatus NOTIFY detectorStatusChanged)
    Q_PROPERTY(int faultInfo READ faultInfo NOTIFY faultInfoChanged)
    Q_PROPERTY(float opticalAzimuth READ opticalAzimuth NOTIFY opticalAzimuthChanged)
    Q_PROPERTY(float opticalPitch READ opticalPitch NOTIFY opticalPitchChanged)
    Q_PROPERTY(float gyroAzimuthRate READ gyroAzimuthRate NOTIFY gyroAzimuthRateChanged)
    Q_PROPERTY(float gyroPitchRate READ gyroPitchRate NOTIFY gyroPitchRateChanged)
    Q_PROPERTY(float losAzimuthRate READ losAzimuthRate NOTIFY losAzimuthRateChanged)
    Q_PROPERTY(float losPitchRate READ losPitchRate NOTIFY losPitchRateChanged)
    Q_PROPERTY(float deviationAzimuth READ deviationAzimuth NOTIFY deviationAzimuthChanged)
    Q_PROPERTY(float deviationPitch READ deviationPitch NOTIFY deviationPitchChanged)
    Q_PROPERTY(int laserPeriod READ laserPeriod NOTIFY laserPeriodChanged)
    Q_PROPERTY(int gainStatus READ gainStatus NOTIFY gainStatusChanged)
    Q_PROPERTY(float quadrant1Energy READ quadrant1Energy NOTIFY quadrant1EnergyChanged)
    Q_PROPERTY(float quadrant2Energy READ quadrant2Energy NOTIFY quadrant2EnergyChanged)
    Q_PROPERTY(float quadrant3Energy READ quadrant3Energy NOTIFY quadrant3EnergyChanged)
    Q_PROPERTY(float quadrant4Energy READ quadrant4Energy NOTIFY quadrant4EnergyChanged)
    Q_PROPERTY(float softwareVersion1 READ softwareVersion1 NOTIFY softwareVersion1Changed)
    Q_PROPERTY(float softwareVersion2 READ softwareVersion2 NOTIFY softwareVersion2Changed)

public:
    explicit LaserData(QObject *parent = nullptr);

    int frameStatus() const;
    int frameId() const;
    int dytStatus() const;
    int detectorStatus() const;
    int faultInfo() const;
    float opticalAzimuth() const;
    float opticalPitch() const;
    float gyroAzimuthRate() const;
    float gyroPitchRate() const;
    float losAzimuthRate() const;
    float losPitchRate() const;
    float deviationAzimuth() const;
    float deviationPitch() const;
    int laserPeriod() const;
    int gainStatus() const;
    float quadrant1Energy() const;
    float quadrant2Energy() const;
    float quadrant3Energy() const;
    float quadrant4Energy() const;
    float softwareVersion1() const;
    float softwareVersion2() const;

    void updateFromFrame(const QByteArray &frame);

signals:
    void frameStatusChanged();
    void frameIdChanged();
    void dytStatusChanged();
    void detectorStatusChanged();
    void faultInfoChanged();
    void opticalAzimuthChanged();
    void opticalPitchChanged();
    void gyroAzimuthRateChanged();
    void gyroPitchRateChanged();
    void losAzimuthRateChanged();
    void losPitchRateChanged();
    void deviationAzimuthChanged();
    void deviationPitchChanged();
    void laserPeriodChanged();
    void gainStatusChanged();
    void quadrant1EnergyChanged();
    void quadrant2EnergyChanged();
    void quadrant3EnergyChanged();
    void quadrant4EnergyChanged();
    void softwareVersion1Changed();
    void softwareVersion2Changed();

private:
    inline float fromRawValue_a(qint16 raw)
    {
    return static_cast<float>(raw)
            * 0.01f;
    }
    inline float fromRawValue_b(qint16 raw)
    {
    return static_cast<float>(raw)
            * 0.001f;
    }
    int m_frameStatus = 0;
    int m_frameId = 0;
    int m_dytStatus = 0;
    int m_detectorStatus = 0;
    int m_faultInfo = 0;
    float m_opticalAzimuth = 0;
    float m_opticalPitch = 0;
    float m_gyroAzimuthRate = 0;
    float m_gyroPitchRate = 0;
    float m_losAzimuthRate = 0;
    float m_losPitchRate = 0;
    float m_deviationAzimuth = 0;
    float m_deviationPitch = 0;
    int m_laserPeriod = 0;
    int m_gainStatus = 0;
    float m_quadrant1Energy = 0;
    float m_quadrant2Energy = 0;
    float m_quadrant3Energy = 0;
    float m_quadrant4Energy = 0;
    float m_softwareVersion1 = 0;
    float m_softwareVersion2 = 0;
};

class LaserSendData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int frameStatus READ frameStatus WRITE setFrameStatus NOTIFY frameStatusChanged)
    Q_PROPERTY(int frameId READ frameId WRITE setFrameId NOTIFY frameIdChanged)
    Q_PROPERTY(int cmd READ cmd WRITE setCmd NOTIFY cmdChanged)
    Q_PROPERTY(int laserPeriod READ laserPeriod WRITE setLaserPeriod NOTIFY laserPeriodChanged)
    Q_PROPERTY(float param2 READ param2 WRITE setParam2 NOTIFY param2Changed)
    Q_PROPERTY(float param3 READ param3 WRITE setParam3 NOTIFY param3Changed)
    Q_PROPERTY(float param4 READ param4 WRITE setParam4 NOTIFY param4Changed)
    Q_PROPERTY(float param5 READ param5 WRITE setParam5 NOTIFY param5Changed)

public:
    explicit LaserSendData(QObject *parent = nullptr);

    int frameStatus() const;
    void setFrameStatus(int value);
    int frameId() const;
    void setFrameId(int value);
    int cmd() const;
    void setCmd(int value);
    int laserPeriod() const;
    void setLaserPeriod(int value);
    float param2() const;
    void setParam2(float value);
    float param3() const;
    void setParam3(float value);
    float param4() const;
    void setParam4(float value);
    float param5() const;
    void setParam5(float value);

    QByteArray buildFrame() const;

signals:
    void frameStatusChanged();
    void frameIdChanged();
    void cmdChanged();
    void laserPeriodChanged();
    void param2Changed();
    void param3Changed();
    void param4Changed();
    void param5Changed();

private:
    inline qint16 toRawValue(float value) const
    {
        return static_cast<qint16>(
                qRound(value / 0.01f));
    }

    int m_frameStatus = 0;
    int m_frameId = 1;
    int m_cmd = 0;
    int m_laserPeriod = 0;
    float m_param2 = 0;
    float m_param3 = 0;
    float m_param4 = 0;
    float m_param5 = 0;
};

class SerialPortLaser : public SerialPort
{
    Q_OBJECT
    Q_PROPERTY(LaserData* laserData READ laserData CONSTANT)
public:
    explicit SerialPortLaser(QObject *parent = nullptr);
    ~SerialPortLaser() override;

    LaserData* laserData() const;
    LaserSendData* laserSendData() const;

protected:
    QByteArray parseData(const QByteArray &rawData) override;

private:
    uint8_t xorChecksumcore(const uint8_t* data, size_t len);
    uint8_t xorChecksum(const QByteArray& data);
    LaserData *m_laserData;
    LaserSendData *m_laserSendData;
};


//激光导引头接收结构体
#pragma pack(push,1)
typedef struct {
    
    quint8 frame_header1;       // 0x55
    quint8 frame_header2;       // 0xAA
    quint8 frame_header3;       // 0xDC

    quint8 frame_status;        //帧长与帧计数器
    quint8 frame_ID;            //帧ID
    //数据包字节
        // 字节1: DYT状态
    quint8 dyt_status;          // 0x00:待机, 0x01:自检中, 0x04:定轴搜索, 0x05:矩形搜索,
                                // 0x06:圆形搜索, 0x07:捕获, 0x08:稳定跟踪, 0x09:记忆跟踪,
                                // 0x0A:定轴搜索(位置回路), 0x0B:矩形搜索(位置回路), 0x0C:圆形搜索(位置回路)

    // 字节2: 激光探测器状态信息 (位域)
    quint8 detector_status;     // bit7-5: 备用
                                // bit4:   0-激光周期未设定, 1-已设定
                                // bit3-2: 00:NA, 01:自检中, 10:自检正常, 11:自检故障
                                // bit1-0: 00:正常, 01:丢光, 10:不同步, 11:能量饱和

    // 字节3: 故障信息
    quint8 fault_info;          // bit7-6: 00:NA, 01:无故障, 10:有故障
                                // bit5-0: 故障码 (全0代表无故障)

    // 字节4-5: 光轴方位角 (有符号, 1bit=0.01°)
    qint16 optical_azimuth;

    // 字节6-7: 光轴俯仰角 (有符号, 1bit=0.01°)
    qint16 optical_pitch;

    // 字节8-9: 方位陀螺输出角速度 (有符号, 1bit=0.01°/s)
    qint16 gyro_azimuth_rate;

    // 字节10-11: 俯仰陀螺输出角速度 (有符号, 1bit=0.01°/s)
    qint16 gyro_pitch_rate;

    // 字节12-13: 方位视线角速度 (有符号, 1bit=0.01°/s)
    qint16 los_azimuth_rate;

    // 字节14-15: 俯仰视线角速度 (有符号, 1bit=0.01°/s)
    qint16 los_pitch_rate;

    // 字节16-17: 方位偏差角 (有符号, 1bit=0.001°)
    qint16 deviation_azimuth;

    // 字节18-19: 俯仰偏差角 (有符号, 1bit=0.001°)
    qint16 deviation_pitch;

    // 字节20-21: 激光周期 (无符号, 1bit=2us)
    quint16 laser_period;

    // 字节22-23: 备用
    quint8 reserved1[2];

    // 字节24: 增益状态 (0xB0~0xB4: 5级起控状态)
    quint8 gain_status;

    // 字节25: 第一象限能量强度 (UINT8, 1bit=0.01, 范围-1750~1750)
    qint8 quadrant1_energy;     // 注意：文档写UINT8但取值范围有负数，实际应为有符号

    // 字节26: 第二象限能量强度
    qint8 quadrant2_energy;

    // 字节27: 第三象限能量强度
    qint8 quadrant3_energy;

    // 字节28: 第四象限能量强度
    qint8 quadrant4_energy;

    // 字节29-30: 软件1版本号 (处理板, UINT16, 1bit=0.01, 例0x00C9=2.01)
    quint16 software_version1;

    // 字节31-32: 软件2版本号 (控制板, UINT16, 1bit=0.01, 例0x00C9=2.01)
    quint16 software_version2;
    //以下为异或校验位
    quint8 XOR_result;            //按位异或校验位

}laser_recv_frame;
#pragma pack(pop)

//激光导引头发送结构体
#pragma pack(push,1)
typedef struct {
    
    quint8 frame_header1;       // 0x55
    quint8 frame_header2;       // 0xAA
    quint8 frame_header3;       // 0xDC

    quint8 frame_status;        //帧长与帧计数器
    quint8 frame_ID;            //帧ID
    //以下为发送数据包
    // 字节1: 控制指令1
    quint8 cmd;                     // 0x00:无动作, 0x01:自检, 0x02:锁定, 0x03:参数装订(改激光周期),
                                    // 0x06:定轴搜索, 0x07:矩形搜索, 0x08:圆形搜索,
                                    // 0x09:定轴搜索(位置回路), 0x0A:矩形搜索(位置回路), 0x0B:圆形搜索(位置回路)

    // 字节2-3: 参数1 (激光周期, 1bit=2us, 无符号整型)
    quint16 laser_period;           // 仅当cmd=0x03时有效

    // 字节4-5: 参数2 (INT16, 精度0.01°)
    qint16 param2;                  // 含义取决于cmd:
                                    // - 锁定/定轴搜索: 方位角度
                                    // - 矩形/圆形搜索: 搜索中心方位角度

    // 字节6-7: 参数3 (INT16, 精度0.01°)
    qint16 param3;                  // 含义取决于cmd:
                                    // - 锁定/定轴搜索: 俯仰角度
                                    // - 矩形/圆形搜索: 搜索中心俯仰角度

    // 字节8-9: 参数4 (INT16, 精度0.01°)
    qint16 param4;                  // 含义取决于cmd:
                                    // - 矩形搜索: 方位搜索范围(半幅)
                                    // - 圆形搜索: 搜索半径
                                    // - 其他指令: 无意义(填0)

    // 字节10-11: 参数5 (INT16, 精度0.01°)
    qint16 param5;                  // 含义取决于cmd:
                                    // - 矩形搜索: 俯仰搜索范围(半幅)
                                    // - 其他指令: 无意义(填0)
    //以下为异或校验位
    quint8 XOR_result;            //按位异或校验位
}laser_send_frame;
#pragma pack(pop)
#endif // SERIALPORT_LASER_H
