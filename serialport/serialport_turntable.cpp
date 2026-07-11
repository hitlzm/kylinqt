#include "serialport_turntable.h"
#include <QDebug>
#include <QByteArray>
#include <cmath>

#define MAX_SPEED 12.0f
SerialPortTurntable::SerialPortTurntable(QObject *parent)
    : SerialPort(parent)
{
}

SerialPortTurntable::~SerialPortTurntable() {
    delete m_turntableData;
};

void SerialPortTurntable::parseData(const QByteArray &rawData)
{
    // TODO: implement turntable-specific data parsing
    // 解析接收到的79个字符，实时显示转台状态。后续可判断读取字节数，确定转台返回的哪个数据帧
    // 1. 帧头帧尾校验
    if(rawData.size() < 79)
    {
        qDebug() << "接收字节数据不完整";
        return;
    }
    
    if (!rawData.startsWith('$') || !rawData.endsWith("\r\n")) {
        qDebug() << "[SerialPort] 帧头/帧尾错误";
        return;
    }
    StatusFeedback feedback;
    // 2. 提取有效载荷（去掉 '$' 和 '\r\n'）
    QByteArray payload = rawData.mid(1, rawData.size() - 3);
    
    // 3. 按空格分割为 11 个字段
    QList<QByteArray> fields = payload.split(' ');
    if (fields.size() != 11) {
        qDebug() << "[SerialPort] 字段数量错误，期望11，实际" << fields.size();
        return;
    }

    bool ok = false;
    // 4. 秒时间（范围：0000-3599，表示半天内的秒数）
    int seconds = fields[0].toInt(&ok);
    feedback.m_time = seconds;
    if (!ok || seconds < 0 || seconds > 3599) {
        qDebug() << "[SerialPort] 秒时间无效:" << fields[0];
        return;
    }
    // 5. 序号（范围：00-99，表示10ms计数值）
    int seq = fields[1].toInt(&ok);
    feedback.m_ctlnumber = seq;
    if (!ok || seq < 0 || seq > 99) {
        qDebug() << "[SerialPort] 序号无效:" << fields[1];
        return;
    }

    // 6. 内框状态码（"正常数字"，如 0, 1, 2...）
    int innerStatus = fields[2].toInt(&ok);
    feedback.m_inner_statusnumber = innerStatus;
    if (!ok) { qDebug() << "[SerialPort] 内框状态码无效:" << fields[2]; return; }

    // 7. 内框角度（范围：±000.0000 ~ ±359.9999）
    float innerAngle = fields[3].toFloat(&ok);
    feedback.m_inner_angle = innerAngle;
    if (!ok || innerAngle < -359.9999f || innerAngle > 359.9999f) {
        qDebug() << "[SerialPort] 内框角度无效:" << fields[3];
        return;
    }

    // 8. 内框控制偏差（范围同上）
    float innerDeviation = fields[4].toFloat(&ok);
    feedback.m_inner_ctlDeviation = innerDeviation;
    if (!ok || innerDeviation < -359.9999f || innerDeviation > 359.9999f) {
        qDebug() << "[SerialPort] 内框偏差无效:" << fields[4];
        return;
    }

    // 9-11. 中框（fields[5], [6], [7]）
    int middleStatus = fields[5].toInt(&ok);
    feedback.m_middle_statusnumber = middleStatus;
    if (!ok) { qDebug() << "[SerialPort] 中框状态码无效:" << fields[5]; return; }

    float middleAngle = fields[6].toFloat(&ok);
    feedback.m_middle_angle = middleAngle;
    if (!ok || middleAngle < -359.9999f || middleAngle > 359.9999f) {
        qDebug() << "[SerialPort] 中框角度无效:" << fields[6];
        return;
    }

    float middleDeviation = fields[7].toFloat(&ok);
    feedback.m_middle_ctlDeviation = middleDeviation;
    if (!ok || middleDeviation < -359.9999f || middleDeviation > 359.9999f) {
        qDebug() << "[SerialPort] 中框偏差无效:" << fields[7];
        return;
    }

    // 12-14. 外框（fields[8], [9], [10]）
    int outerStatus = fields[8].toInt(&ok);
    feedback.m_outter_statusnumber = outerStatus;
    if (!ok) { qDebug() << "[SerialPort] 外框状态码无效:" << fields[8]; return; }

    float outerAngle = fields[9].toFloat(&ok);
    feedback.m_outter_angle = outerAngle;
    if (!ok || outerAngle < -359.9999f || outerAngle > 359.9999f) {
        qDebug() << "[SerialPort] 外框角度无效:" << fields[9];
        return;
    }

    float outerDeviation = fields[10].toFloat(&ok);
    feedback.m_outter_ctlDeviation = outerDeviation;
    if (!ok || outerDeviation < -359.9999f || outerDeviation > 359.9999f) {
        qDebug() << "[SerialPort] 外框偏差无效:" << fields[10];
        return;
    }

    emit requpdateframe(feedback);
}


void SerialPortTurntable::sendCommands(const QStringList &commands, int repeatTimes)
{
    if (!m_serialPort || !m_serialPort->isOpen()) {
        qWarning() << "串口未打开，无法发送指令";
        return;
    }

    for (const QString &cmd : commands) {
        for (int i = 0; i < repeatTimes; ++i) {
            QString fullFrame = QString("$%1\r\n").arg(cmd);
            QByteArray data = fullFrame.toUtf8();

            qint64 written = m_serialPort->write(data);
            if (written == -1) {
                qWarning() << "发送指令失败：" << cmd << m_serialPort->errorString();
                return; // 或 break/continue 根据需求
            }
        }
        }
}
void SerialPortTurntable::openTurntable()
{
     sendCommands({"1mo=0", "2mo=0", "3mo=0"});
     sendCommands({"1mo=1", "2mo=1", "3mo=1"});
};
void SerialPortTurntable::closeTurntable()
{
    sendCommands({"1st=0", "2st=0", "3st=0"});
};
void SerialPortTurntable::resetTurntable()
{
    sendCommands({"RST"});
};
void SerialPortTurntable::zeroTurntable()
{
    sendCommands({"1z", "2z", "3z"});
};

QString SerialPortTurntable::formatNumberWithSignAndDecimals(float value, int intDigits, int fracDigits) {
        // 检查非正常值（NaN、无穷大）
        if (std::isnan(value) || std::isinf(value)) {
            qDebug() << "formatNumberWithSignAndDecimals: 收到非正常浮点值，返回零值";
            value = 0.0f;
        }

        // 处理符号
        QString sign = (value >= 0) ? "+" : "-";
        float absVal = std::abs(value);

        // 检查整数部分是否超出指定位数，防止 int 溢出和格式错乱
        double maxVal = std::pow(10, intDigits) - 1;   // 如 intDigits=4 → 9999
        if (absVal > maxVal) {
            qDebug() << "formatNumberWithSignAndDecimals: 值" << value << "超出" << intDigits << "位整数范围，已钳位至" << sign + QString::number((int)maxVal);
            absVal = static_cast<float>(maxVal);
        }

        // 分离整数和小数部分（四舍五入到指定小数位数）
        double rounded = std::round(absVal * std::pow(10, fracDigits)) / std::pow(10, fracDigits);
        int intPart = static_cast<int>(rounded);
        double fracPart = rounded - intPart;

        // 格式化整数部分（补零）
        QString intStr = QString("%1").arg(intPart, intDigits, 10, QChar('0'));

        // 格式化小数部分（补零，截断到指定位数）
        QString fracStr = QString::number(fracPart, 'f', fracDigits);
        // 去掉开头的 "0."，只保留小数数字
        if (fracStr.startsWith("0.")) {
            fracStr = fracStr.mid(2);
        } else if (fracStr.startsWith("-0.")) {
            fracStr = fracStr.mid(3);
        }
         // 确保小数位数足够（补零）
        while (fracStr.length() < fracDigits) {
            fracStr.append('0');
        }
        if (fracStr.length() > fracDigits) {
            fracStr = fracStr.left(fracDigits);
        }

        return sign + intStr + "." + fracStr;
}


void  SerialPortTurntable::sendPositionCmd(const PositionModeCmd1 &cmd)
{
    if (!m_serialPort->isOpen()) {
            // qWarning() << "串口未打开！";
            return;
        }
        // ---------- 构建数据帧 ----------
        QString data = "$";

        // 轴号 + p + 加速度（4位十进制，补零）
        data += QString::number(cmd.axis);
        data += "p";
        data += QString("%1").arg(cmd.acceleration, 4, 10, QChar('0'));

        // 速度（带符号，4位整数，4位小数）
        data += formatNumberWithSignAndDecimals(cmd.velocity, 4, 4);

        // 角度（带符号，3位整数，4位小数）
        data += formatNumberWithSignAndDecimals(cmd.anglePos, 3, 4);

        // 帧尾
        data += "\r\n";
         // 发送 ASCII 流
        QByteArray frame = data.toLatin1();
        qint64 bytesWritten = m_serialPort->write(frame);
        if (bytesWritten == -1) {
            qCritical() << "位置模式发送失败：" << m_serialPort->errorString();
        } 
        // else {
        //     qDebug() << "位置模式已发送帧：" << frame;
        // }
}

void SerialPortTurntable::sendVecCmd(const SpeedModeCmd1 &cmd)  //参数
{
        if (!m_serialPort->isOpen()) {
            qWarning() << "串口未打开！";
            return;
        }
        // ---------- 构建数据帧 ----------
        QString data = "$";

        // 轴号 + v + 加速度（4位十进制，补零）
        data += QString::number(cmd.axis);
        data += "v";
        data += QString("%1").arg(cmd.acceleration, 4, 10, QChar('0'));

        // 速度（带符号，4位整数，4位小数）
        data += formatNumberWithSignAndDecimals(cmd.velocity, 4, 4);

        // 帧尾
        data += "\r\n";
         // 发送 ASCII 流
        QByteArray frame = data.toLatin1();
        qint64 bytesWritten = m_serialPort->write(frame);
        if (bytesWritten == -1) {
            qCritical() << "速度模式发送失败：" << m_serialPort->errorString();
        } 
        // else {
        //     qDebug() << "速度模式已发送帧：" << frame;
        // }
}

void SerialPortTurntable::sendTrackCmd(const TrackingSendCmd1 &cmd)
{
    if (!m_serialPort->isOpen()) {
            qWarning() << "串口未打开！";
            return;
        }
        // ---------- 构建数据帧 ----------
        QString data = "$";

        // 轴号 + v + 加速度（4位十进制，补零）
        data += "1r";
        data += QString("%1").arg(cmd.trackTime, 4, 10, QChar('0'));

        // 速度（带符号，4位整数，4位小数）
        data += formatNumberWithSignAndDecimals(cmd.angle11, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle12, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle13, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle14, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle21, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle22, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle23, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle24, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle31, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle32, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle33, 3, 4);
        data += formatNumberWithSignAndDecimals(cmd.angle34, 3, 4);
        // 帧尾
        data += "\r\n";
        // 发送 ASCII 流
        QByteArray frame = data.toLatin1();
        qint64 bytesWritten = m_serialPort->write(frame);
        if (bytesWritten == -1) {
            qCritical() << "跟踪模式发送失败：" << m_serialPort->errorString();
        } 

}

void SerialPortTurntable::ProgramModeChanged(int mode)
{
    if(mode==1){
        m_isProgramMode=true;
    }else{
        m_isProgramMode=false;
    }
}

void SerialPortTurntable::sendTrackMode(const sendExGuideData &frame1 , const sendExGuideData &frame2)
{
    //读取两轴数据
    TrackingSendCmd1 m_cmd{};
    //外框对应方位角,中框对应俯仰角
    m_cmd.trackTime = frame1.time;

    m_cmd.angle31 = frame1.angle1 + m_current_outter_angle;
    m_cmd.angle32 = frame1.angle2 + m_current_outter_angle;
    m_cmd.angle33 = frame1.angle3 + m_current_outter_angle;
    m_cmd.angle14 = frame1.angle4 + m_current_outter_angle;
    m_cmd.angle21 = frame2.angle1 + m_current_middle_angle;
    m_cmd.angle22 = frame2.angle2 + m_current_middle_angle;
    m_cmd.angle23 = frame2.angle3 + m_current_middle_angle;
    m_cmd.angle24 = frame2.angle4 + m_current_middle_angle;
    //默认内框不动
    m_cmd.angle11 = m_current_inner_angle;
    m_cmd.angle12 = m_current_inner_angle;
    m_cmd.angle13 = m_current_inner_angle;
    m_cmd.angle14 = m_current_inner_angle;
    //发送数据
    sendTrackCmd(m_cmd);
};

void SerialPortTurntable::sendTimesync()
{
    QString data = "$";
    // 轴号 + v + 加速度（4位十进制，补零）
    data += QString::number(1);
    data += "tm";
    // 帧尾
    data += "\r\n";
    // 发送 ASCII 流
    QByteArray frame = data.toLatin1();
    qint64 bytesWritten = m_serialPort->write(frame);
    if (bytesWritten == -1) {
        qCritical() << "时间同步发送失败：" << m_serialPort->errorString();
    } 
}

void SerialPortTurntable::sendProgramMode(programSend_frame frame)
    {   
        //判断程控模式是否被选中
        if(!m_isProgramMode){
            qDebug() << "未进入程控模式，无法发送程控指令";
            return;
        }

        //判断索引，区分是三轴控制还是单轴控制
        if(frame.index==0){
            PositionModeCmd1 cmd1;
            cmd1.axis = 1; // 内框
            cmd1.acceleration = 1000; // 示例加速度
            cmd1.velocity = (frame.current_inner_angle-frame.inner_endangle)/frame.runtime; // 使用当前角度作为速度示例
            cmd1.anglePos = frame.inner_endangle; // 使用结束角度作为目标位置示例
            sendPositionCmd(cmd1);

            PositionModeCmd1 cmd2;
            cmd2.axis = 2; // 中框
            cmd2.acceleration = 1000;
            cmd2.velocity = (frame.current_middle_angle-frame.middle_endangle)/frame.runtime;
            cmd2.anglePos = frame.middle_endangle;
            sendPositionCmd(cmd2);

            PositionModeCmd1 cmd3;
            cmd3.axis = 3; // 外框
            cmd3.acceleration = 1000;
            cmd3.velocity = (frame.current_outter_angle-frame.outter_endangle)/frame.runtime;
            cmd3.anglePos = frame.outter_endangle;
            sendPositionCmd(cmd3);
        }else if(frame.index==1){
            PositionModeCmd1 cmd1;
            cmd1.axis = 1; // 内框
            cmd1.acceleration = 1000; // 示例加速度
            cmd1.velocity = (frame.current_inner_angle-frame.inner_endangle)/frame.runtime; // 使用当前角度作为速度示例
            cmd1.anglePos = frame.inner_endangle; // 使用结束角度作为目标位置示例
            sendPositionCmd(cmd1);
        }else if(frame.index==2){
            PositionModeCmd1 cmd2;
            cmd2.axis = 2; // 中框
            cmd2.acceleration = 1000;
            cmd2.velocity = (frame.current_middle_angle-frame.middle_endangle)/frame.runtime;
            cmd2.anglePos = frame.middle_endangle;
            sendPositionCmd(cmd2);
        }else if(frame.index==3){
            PositionModeCmd1 cmd3;
            cmd3.axis = 3; // 外框
            cmd3.acceleration = 1000;
            cmd3.velocity = (frame.current_outter_angle-frame.outter_endangle)/frame.runtime;
            cmd3.anglePos = frame.outter_endangle;
            sendPositionCmd(cmd3);
        }else{
             qDebug() << "程控模式发送失败";
        }
        
}

void SerialPortTurntable::sendHandleMode(float axisLeftX, float axisLeftY, float axisRightX, float buttonL2, float buttonR2, bool buttonA, bool buttonB, int Acount, int Bcount)
{
    //根据手柄数据判断是速率模式还是位置模式，发送对应的指令。当两个扳机有一个输出不为0时，认为使用位置模式，提供步进与步减功能
    //当两个扳机都为0时，认为使用速率模式，手柄左轴与右轴的输出直接对应到三轴转台各轴速度
    if( buttonL2!=0 || buttonR2!=0){  //两个扳机任意一个不为0，使用位置模式。左扳机按下：内框；右扳机按下：中框；两扳机同时按下，外框
        //位置模式
        if(buttonR2 >0.9 && buttonL2 >0.9){
            PositionModeCmd1 cmd;
            cmd.axis = 1; // 内框
            cmd.acceleration = 1000;
            if(buttonA)
            {
                if(Acount>=1)
                {
                cmd.velocity = MAX_SPEED/2; // 步进
                cmd.anglePos = m_current_inner_angle+3; 
                sendPositionCmd(cmd);
                }
            }else if(buttonB)
            {
                if(Bcount>=1)
                {
                cmd.velocity = -MAX_SPEED/2; // 步退
                cmd.anglePos = m_current_inner_angle-3; 
                sendPositionCmd(cmd);
                }
            }
        }else if(buttonL2!=0){
            PositionModeCmd1 cmd;
            cmd.axis = 3; // 外框框
            cmd.acceleration = 1000; // 示例加速度
            //判断步进还是步减，每次变动角度为3度
            if(buttonA)
            {   
                if(Acount>=1)
                {
                cmd.velocity = MAX_SPEED/2; // 步进
                cmd.anglePos = m_current_outter_angle+3; 
                sendPositionCmd(cmd);
                }
            }else if(buttonB)
            {   
                if(Bcount>=1)
                {
                cmd.velocity = -MAX_SPEED/2; // 步退
                cmd.anglePos = m_current_outter_angle-3; 
                sendPositionCmd(cmd);
                }
            }
            
        }else if(buttonR2!=0){
            PositionModeCmd1 cmd;
            cmd.axis = 2; // 中框
            cmd.acceleration = 1000;
            if(buttonA)
            {   
                if(Acount>=1)
                {
                cmd.velocity = MAX_SPEED/2; // 步进
                cmd.anglePos = m_current_middle_angle+3; 
                sendPositionCmd(cmd);
                }
            }else if(buttonB)
            {   
                if(Bcount>=1)
                {
                cmd.velocity = -MAX_SPEED/2; // 步退
                cmd.anglePos = m_current_middle_angle-3; 
                sendPositionCmd(cmd);
                }
            }
            
        }
    }else if(buttonL2 == 0 && buttonR2 == 0)
    {
        //当两个扳机都未按下时，使用速度模式
        //axisLeftX对应外框，axisLeftY对应中框，axisRightX对应内框
        SpeedModeCmd1 innercmd;
        SpeedModeCmd1 middlecmd;
        SpeedModeCmd1 outtercmd;
        innercmd.axis = 1;
        innercmd.acceleration = 1000;
        innercmd.velocity = axisRightX*MAX_SPEED;
        middlecmd.axis = 2;
        middlecmd.acceleration = 1000;
        middlecmd.velocity = axisLeftY*MAX_SPEED;
        outtercmd.axis= 3;
        outtercmd.acceleration = 1000;
        outtercmd.velocity = axisLeftX*MAX_SPEED;
        sendVecCmd(innercmd);
        sendVecCmd(middlecmd);
        sendVecCmd(outtercmd);
    }

}



void SerialPortTurntable::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}
void SerialPortTurntable::onClosePort()  { SerialPort::close(); emit portClosed(); }
void SerialPortTurntable::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }

TurntableSendData :: TurntableSendData(QObject *parent)
    : QObject(parent)
{

}

void TurntableSendData :: buildFrame(int m_index)
{
    // 运行时必须大于0，防止除零
    if (m_runtime <= 0) {
        qDebug() << "程控模式发送失败: runtime 无效 (" << m_runtime << ")";
        return;
    }
    // 构建转台控制帧
    programSend_frame m_frame;
    m_frame.index = m_index;
    switch(m_index){
        case 0:     //三轴控制
        m_frame.current_inner_angle=m_current_inner_angle;
        m_frame.inner_endangle=m_inner_endangle;
        m_frame.current_middle_angle=m_current_middle_angle;
        m_frame.middle_endangle=m_middle_endangle;
        m_frame.current_outter_angle=m_current_outter_angle;
        m_frame.outter_endangle=m_outter_endangle;
        m_frame.runtime=m_runtime;
        break;
        case 1:
        m_frame.current_inner_angle=m_current_inner_angle;
        m_frame.inner_endangle=m_inner_endangle;
        m_frame.runtime=m_runtime;
        break;
        case 2:
        m_frame.current_middle_angle=m_current_middle_angle;
        m_frame.middle_endangle=m_middle_endangle;
        m_frame.runtime=m_runtime;
        break;
        case 3:
        m_frame.current_outter_angle=m_current_outter_angle;
        m_frame.outter_endangle=m_outter_endangle;
        m_frame.runtime=m_runtime;
        break;
        default: qDebug() << "程控模式发送失败";
    }
    // 发送指令到串口
    emit requestSendProgramMode(m_frame);
}

TurntableData :: TurntableData(QObject *parent)
    : QObject(parent)
{

}

// ── 工作线程回推 ──
void TurntableData::setPortOpen(bool open) { if (m_portOpen != open) { m_portOpen = open; emit portOpenChanged(); } }
void TurntableData::setPortList(const QStringList &ports) { if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); } }
void TurntableData::setError(const QString &msg) { if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); } }

void TurntableData :: updateframe(StatusFeedback recvdata)
{
    if (m_time != recvdata.m_time) {
        m_time = recvdata.m_time;
        emit timeChanged();
    }
    if (m_ctlnumber != recvdata.m_ctlnumber) {
        m_ctlnumber = recvdata.m_ctlnumber;
        emit ctlnumberChanged();
    }
    if (m_inner_statusnumber != recvdata.m_inner_statusnumber) {
        m_inner_statusnumber = recvdata.m_inner_statusnumber;
        emit inner_statusnumberChanged();
    }
    if (m_inner_angle != recvdata.m_inner_angle) {
        m_inner_angle = recvdata.m_inner_angle;
        emit inner_angleChanged();
        //也可以不使用信号与槽的方式，仅在需要数据时，调用数据对象方法返回
        emit myinner_angleChanged(m_inner_angle);
    }
    if (m_inner_ctlDeviation != recvdata.m_inner_ctlDeviation) {
        m_inner_ctlDeviation = recvdata.m_inner_ctlDeviation;
        emit inner_ctlDeviationChanged();
    }
    if (m_middle_statusnumber != recvdata.m_middle_statusnumber) {
        m_middle_statusnumber = recvdata.m_middle_statusnumber;
        emit middle_statusnumberChanged();
    }
    if (m_middle_angle != recvdata.m_middle_angle) {
        m_middle_angle = recvdata.m_middle_angle;
        emit middle_angleChanged();
        emit mymiddle_angleChanged(m_middle_angle);
    }
    if (m_middle_ctlDeviation != recvdata.m_middle_ctlDeviation) {
        m_middle_ctlDeviation = recvdata.m_middle_ctlDeviation;
        emit middle_ctlDeviationChanged();
    }
    if (m_outter_statusnumber != recvdata.m_outter_statusnumber) {
        m_outter_statusnumber = recvdata.m_outter_statusnumber;
        emit outter_statusnumberChanged();
    }
    if (m_outter_angle != recvdata.m_outter_angle) {
        m_outter_angle = recvdata.m_outter_angle;
        emit outter_angleChanged();
        emit myoutter_angleChanged(m_outter_angle);
    }
    if (m_outter_ctlDeviation != recvdata.m_outter_ctlDeviation) {
        m_outter_ctlDeviation = recvdata.m_outter_ctlDeviation;
        emit outter_ctlDeviationChanged();
    }
}

void TurntableData :: openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void TurntableData :: closePort()                                     { emit requestClosePort(); }
void TurntableData :: scanPorts()                                     { emit requestScanPorts(); }