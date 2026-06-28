#include "serialport_turntable.h"
#include <QDebug>
SerialPortTurntable::SerialPortTurntable(QObject *parent)
    : SerialPort(parent)
{
}

SerialPortTurntable::~SerialPortTurntable() = default;

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
    if (!ok || seconds < 0 || seconds > 3599) {
        qDebug() << "[SerialPort] 秒时间无效:" << fields[0];
        return;
    }
    // 5. 序号（范围：00-99，表示10ms计数值）
    int seq = fields[1].toInt(&ok);
    if (!ok || seq < 0 || seq > 99) {
        qDebug() << "[SerialPort] 序号无效:" << fields[1];
        return;
    }

    // 6. 内框状态码（"正常数字"，如 0, 1, 2...）
    int innerStatus = fields[2].toInt(&ok);
    if (!ok) { qDebug() << "[SerialPort] 内框状态码无效:" << fields[2]; return; }

    // 7. 内框角度（范围：±000.0000 ~ ±359.9999）
    float innerAngle = fields[3].toFloat(&ok);
    if (!ok || innerAngle < -359.9999f || innerAngle > 359.9999f) {
        qDebug() << "[SerialPort] 内框角度无效:" << fields[3];
        return;
    }

    // 8. 内框控制偏差（范围同上）
    float innerDeviation = fields[4].toFloat(&ok);
    if (!ok || innerDeviation < -359.9999f || innerDeviation > 359.9999f) {
        qDebug() << "[SerialPort] 内框偏差无效:" << fields[4];
        return;
    }

    // 9-11. 中框（fields[5], [6], [7]）
    int middleStatus = fields[5].toInt(&ok);
    if (!ok) { qDebug() << "[SerialPort] 中框状态码无效:" << fields[5]; return; }

    float middleAngle = fields[6].toFloat(&ok);
    if (!ok || middleAngle < -359.9999f || middleAngle > 359.9999f) {
        qDebug() << "[SerialPort] 中框角度无效:" << fields[6];
        return;
    }

    float middleDeviation = fields[7].toFloat(&ok);
    if (!ok || middleDeviation < -359.9999f || middleDeviation > 359.9999f) {
        qDebug() << "[SerialPort] 中框偏差无效:" << fields[7];
        return;
    }

    // 12-14. 外框（fields[8], [9], [10]）
    int outerStatus = fields[8].toInt(&ok);
    if (!ok) { qDebug() << "[SerialPort] 外框状态码无效:" << fields[8]; return; }

    float outerAngle = fields[9].toFloat(&ok);
    if (!ok || outerAngle < -359.9999f || outerAngle > 359.9999f) {
        qDebug() << "[SerialPort] 外框角度无效:" << fields[9];
        return;
    }

    float outerDeviation = fields[10].toFloat(&ok);
    if (!ok || outerDeviation < -359.9999f || outerDeviation > 359.9999f) {
        qDebug() << "[SerialPort] 外框偏差无效:" << fields[10];
        return;
    }

    return;
}
