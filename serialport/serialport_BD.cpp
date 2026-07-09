#include "serialport_BD.h"
#include <QDebug>
SerialPortBD::SerialPortBD(QObject *parent)
    : SerialPort(parent)
    , m_bdData(new BDData(nullptr))        // 留在主线程，不随 moveToThread 迁移
{
    
}

//数据格式：$BDRMC,123400.000,A,4002.217821,N,11618.105743,E,0.026,181.631,180411,,,A*2C
void SerialPortBD::parseData(const QByteArray &rawData)
{
 // 1. 将原始字节转为 QString（NMEA 为 ASCII 字符）
    QString sentence = QString::fromLatin1(rawData.trimmed()); // 去除换行符
    //输出数据结构体
    RMCData outData;
    // 2. 基础校验：必须以 '$' 开头且包含 "RMC"
    if (!sentence.startsWith('$') || !sentence.contains("RMC")) {
        qWarning() << "Invalid RMC sentence prefix";
        return;
    }
    // 3. 提取校验和（如果有）并验证，校验和为16进制，转换为ASCII码后占两个字节
        int starIdx = sentence.lastIndexOf('*');
        if (starIdx != -1 && starIdx + 2 < sentence.length()) {
            QString receivedChecksum = sentence.mid(starIdx + 1, 2);
            // 计算异或校验和（从 '$' 之后到 '*' 之前）
            QByteArray dataToCheck = rawData.mid(1, starIdx - 1); // 不含 '$' 和 '*'
            quint8 calc = 0;
            for (char c : dataToCheck) {
                calc ^= static_cast<quint8>(c);
            }
            QString calcStr = QString("%1").arg(calc, 2, 16, QChar('0')).toUpper();
            if (calcStr != receivedChecksum) {
                qWarning() << "Checksum mismatch: received" << receivedChecksum << "calculated" << calcStr;
                return;
            }
        } else {
            qWarning() << "No checksum found in RMC sentence";
            return; // 若要求验证但无校验和，视为无效
        }
    
     // 4. 去除校验和部分，按逗号切分
    int starIdx = sentence.indexOf('*');
    if (starIdx != -1) {
        sentence = sentence.left(starIdx);
    }
    QStringList fields = sentence.split(',');

    // RMC 标准至少有 13 个字段（含空字段）
    if (fields.size() < 13) {
        qWarning() << "RMC field count too few:" << fields.size();
        return;
    }

    // 5. 解析各字段（索引不变）
    QString timeStr = fields[1];
    QString statusStr = fields[2];
    QString latStr = fields[3];
    QString latDir = fields[4];
    QString lonStr = fields[5];
    QString lonDir = fields[6];
    QString speedStr = fields[7];
    QString headingStr = fields[8];

    QString dateStr = fields[9];
    QString modeStr = (fields.size() > 12) ? fields[12] : "N";

    // 6. 解析时间
    if (timeStr.length() < 6) {
        qWarning() << "Invalid time format:" << timeStr;
        return;
    }
    int hour = timeStr.mid(0, 2).toInt();
    int minute = timeStr.mid(2, 2).toInt();
    double secondsDouble = timeStr.mid(4).toDouble();
    int second = static_cast<int>(secondsDouble);
    int millisecond = static_cast<int>((secondsDouble - second) * 1000);
    QTime utcTime(hour, minute, second, millisecond);
    if (!utcTime.isValid()) {
        qWarning() << "Invalid time parsed:" << timeStr;
        return;
    }
    // 7. 解析日期
    if (dateStr.length() != 6) {
        qWarning() << "Invalid date format:" << dateStr;
        return;
    }
    int day = dateStr.mid(0, 2).toInt();
    int month = dateStr.mid(2, 2).toInt();
    int year = 2000 + dateStr.mid(4, 2).toInt(); // 注意：此处理仅适用于 2000~2099
    QDate utcDate(year, month, day);
    if (!utcDate.isValid()) {
        qWarning() << "Invalid date parsed:" << dateStr;
        return;
    }

    // 8. 转换经纬度
    double latitude = nmeaToDecimal(latStr, latDir);
    double longitude = nmeaToDecimal(lonStr, lonDir);

    // 10. 填充结构体
    outData.isValid = (statusStr == "A");
    outData.isnorth = (latDir == "N");
    outData.iseast = (lonDir == "E");
    outData.utcDateTime = QDateTime(utcDate, utcTime, Qt::UTC);
    outData.latitude = latitude;
    outData.longitude = longitude;
    outData.mode = modeStr;
}

double SerialPortBD::nmeaToDecimal(const QString& coord, const QString& dir)
{
    bool ok = false;
    double value = coord.toDouble(&ok);
    if (!ok || value == 0.0) return 0.0;
    // 纬度：ddmm.mmmmm -> 度 = 整数部分/100 取整，分 = 余数
    // 经度：dddmm.mmmmm -> 度 = 整数部分/100 取整，分 = 余数
    int deg = static_cast<int>(value / 100.0);
    double minutes = value - deg * 100.0;
    double decimalDeg = deg + minutes / 60.0;
    // 根据方向符号：北纬(N)/东经(E)为正，南纬(S)/西经(W)为负
    if (dir == "S" || dir == "W") {
        decimalDeg = -decimalDeg;
    }
    return decimalDeg;
}