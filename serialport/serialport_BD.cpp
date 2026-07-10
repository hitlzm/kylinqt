#include "serialport_BD.h"
#include <QDebug>
#include <QTimeZone>

SerialPortBD::SerialPortBD(QObject *parent)
    : SerialPort(parent)
    , m_bdData(new BDData(nullptr))        // 留在主线程，不随 moveToThread 迁移
{
    
}

void SerialPortBD::onReadyRead() { SerialPort::onReadyRead(); }

//数据格式：$BDRMC,123400.000,A,4002.217821,N,11618.105743,E,0.026,181.631,180411,,,A*2C
void SerialPortBD::parseData(const QByteArray &rawData)
{
 // 1. 将原始字节转为 QString（NMEA 为 ASCII 字符），将一个Latin-1 (ISO 8859-1) 编码的 8 位字符串转换为 Qt 内部使用的 UTF-16 编码的 QString 对象
    QString sentence = QString::fromLatin1(rawData.trimmed()); // 去除换行符
    //输出数据结构体
    RMCData outData;
    // 2. 基础校验：必须以 '$' 开头且包含 "RMC"
    if (!sentence.startsWith('$') || !sentence.contains("BDRMC")) {
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
    int starIdx1 = sentence.indexOf('*');
    if (starIdx1 != -1) {
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
    
    //把UTC时间转化为北京时间
    QDateTime beijingTime = QDateTime(utcDate, utcTime, Qt::UTC).toTimeZone(QTimeZone("Asia/Beijing"));

    // 8. 转换经纬度
    double latitude = nmeaToDecimal(latStr, latDir);
    double longitude = nmeaToDecimal(lonStr, lonDir);

    // 10. 填充结构体
    outData.isValid = (statusStr == "A");
    outData.isnorth = (latDir == "N");
    outData.iseast = (lonDir == "E");
    outData.BJDateTime = beijingTime;
    outData.latitude = latitude;
    outData.longitude = longitude;
    outData.mode = modeStr;

    emit bdFrameReceived(outData);
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
    double decimalDeg = deg + minutes / 60.0; //把分转化为度
    // 根据方向符号：北纬(N)/东经(E)为正，南纬(S)/西经(W)为负
    // if (dir == "S" || dir == "W") {
    //     decimalDeg = -decimalDeg;
    // }
    return decimalDeg;
}

//串口操作槽函数
void SerialPortBD::onOpenPort(const QString &name, int baud) {
    if (SerialPort::open(name, baud))
        emit portOpened(true);
    else
        emit portError(m_serialPort ? m_serialPort->errorString() : "QSerialPort not created");
}
void SerialPortBD::onClosePort()  { SerialPort::close(); emit portClosed(); }
void SerialPortBD::onScanPorts()  { SerialPort::scanPorts(); emit portsChanged(m_availablePorts); }


// ── BDData 数据属性 READ 函数 ──
QDateTime BDData::bjDateTime() const
{
    return m_BJDateTime;
}

bool BDData::isPosValid() const
{
    return m_isPosValid;
}

double BDData::latitude() const
{
    return m_latitude;
}

bool BDData::isnorth() const
{
    return m_isnorth;
}

double BDData::longitude() const
{
    return m_longitude;
}

bool BDData::iseast() const
{
    return m_iseast;
}

QString BDData::mode() const
{
    return m_mode;
}

void BDData ::updateFromFrame(const RMCData &frame)
{
    //先判断定位是否有效
    if(frame.isValid)
    {
        m_BJDateTime = frame.BJDateTime;
        emit bjDateTimeChanged();
        if(m_latitude != frame.latitude)
        {
            emit latitudeChanged();
        }
        if(m_longitude != frame.longitude)
        {
            emit longitudeChanged();
        }
        //用于判断北纬还是南纬
        if(m_isnorth != frame.isnorth)
        {
            emit isnorthChanged();
        }
        if(m_iseast != frame.iseast)
        {
            emit iseastChanged();
        }
        if(m_mode != frame.mode)
        {
            emit modeChanged();
        }
    }

}

BDData::BDData(QObject *parent)
    : QObject(parent)
{

}

void BDData::openPort(const QString &portName, int baudRate) { emit requestOpenPort(portName, baudRate); }
void BDData::closePort()                                     { emit requestClosePort(); }
void BDData::scanPorts()                                     { emit requestScanPorts(); }

void BDData::setPortOpen(bool open) { if (m_portOpen != open) { m_portOpen = open; emit portOpenChanged(); } }
void BDData::setPortList(const QStringList &ports) { if (m_availablePorts != ports) { m_availablePorts = ports; emit availablePortsChanged(); } }
void BDData::setError(const QString &msg) { if (m_errorString != msg) { m_errorString = msg; emit errorStringChanged(); } }

