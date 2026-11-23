#ifndef PROTOCOL_PROFILE_H
#define PROTOCOL_PROFILE_H

#include <QByteArray>
#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QVariantMap>
#include <QVector>

enum class ProtocolFieldType
{
    UInt8,
    Int8,
    UInt16,
    Int16,
    UInt32,
    Int32,
    UInt64,
    Int64,
    Float32,
    Float64,
    ByteArray
};

enum class ProtocolByteOrder
{
    LittleEndian,
    BigEndian
};

enum class ProtocolChecksumType
{
    None,
    Xor8,
    Sum8,
    Crc16Modbus,
    Crc16CcittFalse
};

struct ProtocolEnumOption
{
    qint64 value = 0;
    QString name;
    QString label;
};

struct ProtocolFieldSpec
{
    QString name;
    QString canonical;
    int offset = -1;
    ProtocolFieldType type = ProtocolFieldType::UInt8;
    ProtocolByteOrder byteOrder = ProtocolByteOrder::LittleEndian;
    int bitOffset = 0;
    int bitWidth = 0;
    double scale = 1.0;
    double bias = 0.0;
    QString unit;
    QVector<ProtocolEnumOption> enumOptions;
};

struct ProtocolChecksumSpec
{
    ProtocolChecksumType type = ProtocolChecksumType::None;
    int start = 0;
    int length = 0;
    int storedOffset = -1;
    ProtocolByteOrder storedByteOrder = ProtocolByteOrder::BigEndian;
    quint16 initialValue = 0xFFFF;
};

struct ProtocolFrameSpec
{
    QString name;
    QByteArray header;
    int frameLength = 0;
    QVector<ProtocolFieldSpec> fields;
    ProtocolChecksumSpec checksum;
};

struct ProtocolCommandSpec
{
    QString name;
    QByteArray frameTemplate;
    QVector<ProtocolFieldSpec> fields;
    ProtocolChecksumSpec checksum;
    QString commandCodeField;
    QMap<QString, qint64> commandCodes;
};

struct ProtocolProfile
{
    int schemaVersion = 0;
    QString id;
    QString vendor;
    QString model;
    QString source;
    QString description;
    QVariantMap capabilities;
    QVector<ProtocolFrameSpec> frames;
    QMap<QString, ProtocolCommandSpec> commands;

    bool isValid() const;
    const ProtocolFrameSpec *findFrame(const QString &name) const;
    const ProtocolCommandSpec *findCommand(const QString &name) const;

    static bool loadFromFile(const QString &path,
                             ProtocolProfile *profile,
                             QString *error);
    static bool fromJson(const QByteArray &json,
                         ProtocolProfile *profile,
                         QString *error,
                         const QString &sourceName = QString());
};

int protocolFieldSize(ProtocolFieldType type);
bool protocolFieldIsSigned(ProtocolFieldType type);
QString protocolFieldTypeName(ProtocolFieldType type);

#endif // PROTOCOL_PROFILE_H
