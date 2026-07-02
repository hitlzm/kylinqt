#ifndef SERIALPORT_TURNTABLE_H
#define SERIALPORT_TURNTABLE_H


/*
brief:1.转台串口类负责实时接收转台状态反馈信息
      2.根据运行模式的不同发送不同的数据
        遥控模式下，接收手柄数据，根据数据判断是三轴速度控制，还是角度控制
        程控模式下，线程内部实现计算并发送数据
        外引导模式下，接收指定导引头传来的角度数据，使用转台的跟踪模式
*/

#include "serialport.h"

//转台信息反馈结构体
typedef struct {
    int m_time;
    int m_ctlnumber;
    int m_inner_statusnumber;
    float m_inner_angle;
    float m_inner_ctlDeviation;
    int m_middle_statusnumber;
    float m_middle_angle;
    float m_middle_ctlDeviation;
    int m_outter_statusnumber;
    float m_outter_angle;
    float m_outter_ctlDeviation;
} StatusFeedback;

typedef struct{
    int runtime;
    int index;  //用来记录是对哪个轴的控制
    // float inner_startangle;
    float current_inner_angle;
    float inner_endangle;
    // float middle_startangle;
    float current_middle_angle;
    float middle_endangle;
    // float outter_startangle;
    float current_outter_angle;
    float outter_endangle;  
}programSend_frame;

typedef struct PositionModeCmd1{
    int axis;              
    int acceleration;      
    float  velocity;      
    float  anglePos;       
} PositionModeCmd1;


class TurntableData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int time MEMBER m_time NOTIFY timeChanged)
    Q_PROPERTY(int ctlnumber MEMBER m_ctlnumber NOTIFY ctlnumberChanged)
    Q_PROPERTY(int inner_statusnumber MEMBER m_inner_statusnumber NOTIFY inner_statusnumberChanged)
    Q_PROPERTY(float inner_angle MEMBER m_inner_angle NOTIFY inner_angleChanged)
    Q_PROPERTY(float inner_ctlDeviation MEMBER m_inner_ctlDeviation NOTIFY inner_ctlDeviationChanged)
    Q_PROPERTY(int middle_statusnumber MEMBER m_middle_statusnumber NOTIFY middle_statusnumberChanged)
    Q_PROPERTY(float middle_angle MEMBER m_middle_angle NOTIFY middle_angleChanged)
    Q_PROPERTY(float middle_ctlDeviation MEMBER m_middle_ctlDeviation NOTIFY middle_ctlDeviationChanged)
    Q_PROPERTY(int outter_statusnumber MEMBER m_outter_statusnumber NOTIFY outter_statusnumberChanged)
    Q_PROPERTY(float outter_angle MEMBER m_outter_angle NOTIFY outter_angleChanged)
    Q_PROPERTY(float outter_ctlDeviation MEMBER m_outter_ctlDeviation NOTIFY outter_ctlDeviationChanged)
public:
    explicit TurntableData(QObject *parent = nullptr);
    ~TurntableData() override{};
    
signals:
    void timeChanged();
    void ctlnumberChanged();
    void inner_statusnumberChanged();
    void inner_angleChanged();
    void inner_ctlDeviationChanged();
    void middle_statusnumberChanged();
    void middle_angleChanged();
    void middle_ctlDeviationChanged();
    void outter_statusnumberChanged();
    void outter_angleChanged();
    void outter_ctlDeviationChanged();

    //把信号连接到发送类，更新发送类中的当前角度
    void myinner_angleChanged(float inner_angle);
    void mymiddle_angleChanged(float middle_angle);
    void myoutter_angleChanged(float outter_angle);

public slots:

    void updateframe(StatusFeedback recvdata);    

private:
    
    int m_time;
    int m_ctlnumber;
    int m_inner_statusnumber;
    float m_inner_angle;
    float m_inner_ctlDeviation;
    int m_middle_statusnumber;
    float m_middle_angle;
    float m_middle_ctlDeviation;
    int m_outter_statusnumber;
    float m_outter_angle;
    float m_outter_ctlDeviation;
};

class TurntableSendData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int runtime READ runtime NOTIFY runtimeChanged)
    Q_PROPERTY(float inner_startangle READ inner_startangle NOTIFY inner_startangleChanged)
    Q_PROPERTY(float inner_endangle READ inner_endangle NOTIFY inner_endangleChanged)
    Q_PROPERTY(float middle_startangle READ middle_startangle NOTIFY middle_startangleChanged)
    Q_PROPERTY(float middle_endangle READ middle_endangle NOTIFY middle_endangleChanged)
    Q_PROPERTY(float outter_startangle READ outter_startangle NOTIFY outter_startangleChanged)
    Q_PROPERTY(float outter_endangle READ outter_endangle NOTIFY outter_endangleChanged)
    Q_PROPERTY(int index READ index NOTIFY indexChanged)

public:
    explicit TurntableSendData(QObject *parent = nullptr);
    ~TurntableSendData() override{};

    int runtime() const { return m_runtime; }
    float inner_startangle() const { return m_inner_startangle; }
    float inner_endangle() const { return m_inner_endangle; }
    float middle_startangle() const { return m_middle_startangle; }
    float middle_endangle() const { return m_middle_endangle; }
    float outter_startangle() const { return m_outter_startangle; }
    float outter_endangle() const { return m_outter_endangle; }
    int index() const { return m_index; }

    Q_INVOKABLE void buildFrame() ;
public slots:
    void recvinner_angle(float angle){m_current_inner_angle = angle;}
    void recvmiddle_angle(float angle){m_current_middle_angle = angle;}
    void recvoutter_angle(float angle){m_current_outter_angle = angle;}
    //这里存放各个轴的角度数据及运动时间，实现最基础的程控模式

    //发送信号并在串口类编写槽函数，实现开机，停机，回零，复位，程控模式的实现
signals:
    void requestOpenTurntable();
    void requestCloseTurntable();
    void requestResetTurntable();
    void requestZeroTurntable();
    void requestSendProgramMode(programSend_frame &frame);
    void runtimeChanged();
    void inner_startangleChanged();
    void inner_endangleChanged();
    void middle_startangleChanged();
    void middle_endangleChanged();
    void outter_startangleChanged();
    void outter_endangleChanged();
    void indexChanged();
private:
    //这里存放各个轴的角度数据及运动时间
    int m_index;    //用来记录是对哪个轴的控制
    int m_runtime;
    float m_inner_startangle;
    float m_current_inner_angle;
    float m_inner_endangle;
    float m_middle_startangle;
    float m_current_middle_angle;
    float m_middle_endangle;
    float m_outter_startangle;
    float m_current_outter_angle;
    float m_outter_endangle;  
};

class SerialPortTurntable : public SerialPort
{
    Q_OBJECT

public:

    explicit SerialPortTurntable(QObject *parent = nullptr);
    ~SerialPortTurntable() override;
    TurntableData * m_turntableData;
    TurntableSendData * m_turntableSendData;
signals:
    void requpdateframe(StatusFeedback recvdata);
    
    
public slots:

    void openTurntable();
    void closeTurntable();
    void resetTurntable();
    void zeroTurntable();

    void sendProgramMode(programSend_frame &frame); 
    void dowork() { 
        SerialPort::dowork();     //初始化串口并做一些信号连接操作
                    // onScanPorts(); 
        }

protected:
    void parseData(const QByteArray &rawData) override;  //解析转台的反馈数据（实现ASCII字符向数字的转换），后期仍需要加入其他反馈指令解析
    void sendCommands(const QStringList &commands, int repeatTimes = 5);
    void sendPositionCmd(const PositionModeCmd1 &cmd);
    QString formatNumberWithSignAndDecimals(float value, int intDigits, int fracDigits);
private:
    
};




#pragma pack(push,1)

// ------------------------------ 位置模式 ------------------------------//对应程控模式
typedef struct PositionModeCmd{
    uint16_t axis;              // 轴号 (2字节)
    uint32_t acceleration;      // 加速度 (4字节)
    int8_t  velocity[10];      // 速度 (10字节)
    int8_t  anglePos[9];       // 角度位置 (9字节)
} PositionModeCmd;

// ------------------------------ 速度模式 ------------------------------
typedef struct {
    uint16_t axis;              // 轴号 (2字节)
    uint32_t acceleration;      // 加速度 (4字节)
    int8_t  velocity[10];      // 速度 (10字节)
} SpeedModeCmd;

// ------------------------------ 摇摆模式 ------------------------------
typedef struct {
    uint16_t axis;              // 轴号 (2字节)
    uint8_t  amplitude[8];      // 摇摆幅度 (8字节)
    uint8_t  frequency[10];     // 摇摆频率 (10字节)
} SwingModeCmd;

// ------------------------------ 跟踪模式1 发送 ------------------------------//对应外引导模式
typedef struct {
    uint16_t axis;          // 轴号 (2字节)
    uint32_t trackTime;     // 跟踪时间 (4字节)
    uint8_t  angle11[9];    // 角度11 (9字节)
    uint8_t  angle12[9];    // 角度12 (9字节)
    uint8_t  angle13[9];    // 角度13 (9字节)
    uint8_t  angle14[9];    // 角度14 (9字节)
    uint8_t  angle21[9];    // 角度21 (9字节)
    uint8_t  angle22[9];    // 角度22 (9字节)
    uint8_t  angle23[9];    // 角度23 (9字节)
    uint8_t  angle24[9];    // 角度24 (9字节)
    uint8_t  angle31[9];    // 角度31 (9字节)
    uint8_t  angle32[9];    // 角度32 (9字节)
    uint8_t  angle33[9];    // 角度33 (9字节)
    uint8_t  angle34[9];    // 角度34 (9字节)
} Tracking1SendCmd;

// ------------------------------ 跟踪模式1 接收 ------------------------------
typedef struct {
    uint16_t axis;      // 轴号 (2字节)
    uint32_t time;      // 时间 (4字节)
} Tracking1ReceiveResp;

// ------------------------------ 跟踪模式1 修正指令发送 ------------------------------
typedef struct {
    uint8_t  axis[3];         // 轴号 (3字节)
    uint8_t  correctAngle[9]; // 修正角度 (9字节)
} Tracking1CorrectionCmd;

// ------------------------------ 跟踪模式2 发送 ------------------------------
typedef struct {
    uint16_t axis;          // 轴号 (2字节)
    uint8_t  trackTime[6];  // 跟踪时间 (6字节)
    uint8_t  angle1[9];     // 跟踪角度1 (9字节)
    uint8_t  angle2[9];     // 跟踪角度2 (9字节)
    uint8_t  angle3[9];     // 跟踪角度3 (9字节)
} Tracking2SendCmd;

// ------------------------------ 速度环模式发送 ------------------------------
typedef struct {
    uint16_t axis;              // 轴号 (2字节)
    uint8_t  innerSpeed[10];    // 内框速度 (10字节)
    uint8_t  middleSpeed[10];   // 中框速度 (10字节)
    uint8_t  outerSpeed[10];    // 外框速度 (10字节)
} SpeedLoopCmd;

// ------------------------------ 时间同步指令 ------------------------------
typedef struct {
    uint8_t axis[3];        // 轴号 (3字节)
} TimeSyncCmd;

// ------------------------------ 秒脉冲查询发送 ------------------------------
typedef struct {
    uint8_t axis[3];        // 轴号 (3字节)
} PulseQuerySend;

// ------------------------------ 秒脉冲查询接收 ------------------------------
typedef struct {
    uint16_t axis;          // 轴号 (2字节)
} PulseQueryReceive;

// ------------------------------ 复位发送 ------------------------------
typedef struct {
    uint8_t reset[3];       // 复位 (3字节)
} ResetSend;
#pragma pack(pop)

// ------------------------------ 状态反馈接收 ------------------------------
// typedef struct {
//     uint32_t time;          // 时间 (4字节)
//     uint16_t seq;           // 序号 (2字节)
//     uint16_t innerState;    // 内框状态 (2字节)
//     uint8_t  innerAngle[9]; // 内框角度 (9字节)
//     uint8_t  innerErr[9];   // 内框偏差 (9字节)
//     uint16_t middleState;   // 中框状态 (2字节)
//     uint8_t  middleAngle[9];// 中框角度 (9字节)
//     uint8_t  middleErr[9];  // 中框偏差 (9字节)
//     uint16_t outerState;    // 外框状态 (2字节)
//     uint8_t  outerAngle[9]; // 外框角度 (9字节)
//     uint8_t  outerErr[9];   // 外框偏差 (9字节)
// } StatusFeedback;

#endif // SERIALPORT_TURNTABLE_H
