#include "protocol_profile.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QSet>
#include <QtMath>

namespace {

bool fail(QString *error, const QString &message)
{
    if (error)
        *error = message;
    return false;
}

QString location(const QString &sourceName, const QString &suffix)
{
    return sourceName.isEmpty() ? suffix : sourceName + QStringLiteral(": ") + suffix;
}

bool parseIntegerValue(const QJsonValue &value, qint64 *result)
{
    bool ok = false;
    if (value.isDouble()) {
        *result = static_cast<qint64>(value.toDouble());
        ok = qFuzzyCompare(value.toDouble(), static_cast<double>(*result));
        return ok;
    }
    if (!value.isString())
        return false;
    const QString text = value.toString().trimmed();
    if (text.startsWith(QStringLiteral("0x"), Qt::CaseInsensitive)) {
        *result = text.mid(2).toLongLong(&ok, 16);
        return ok;
    }
    *result = text.toLongLong(&ok, 10);
    return ok;
}

bool parseByteOrder(const QJsonValue &value,
                    ProtocolByteOrder fallback,
                    ProtocolByteOrder *result,
                    QString *error,
                    const QString &fieldName)
{
    if (value.isUndefined()) {
        *result = fallback;
        return true;
    }
    const QString text = value.toString().trimmed().toLower();
    if (text == QStringLiteral("little") || text == QStringLiteral("le")) {
        *result = ProtocolByteOrder::LittleEndian;
        return true;
    }
    if (text == QStringLiteral("big") || text == QStringLiteral("be")) {
        *result = ProtocolByteOrder::BigEndian;
        return true;
    }
    return fail(error, QStringLiteral("%1 的字节序必须是 little/big").arg(fieldName));
}

bool parseFieldType(const QJsonValue &value,
                    ProtocolFieldType *result,
                    QString *error,
                    const QString &fieldName)
{
    const QString text = value.toString().trimmed().toLower();
    if (text == QStringLiteral("uint8") || text == QStringLiteral("u8")) {
        *result = ProtocolFieldType::UInt8;
    } else if (text == QStringLiteral("int8") || text == QStringLiteral("i8")) {
        *result = ProtocolFieldType::Int8;
    } else if (text == QStringLiteral("uint16") || text == QStringLiteral("u16")) {
        *result = ProtocolFieldType::UInt16;
    } else if (text == QStringLiteral("int16") || text == QStringLiteral("i16")) {
        *result = ProtocolFieldType::Int16;
    } else if (text == QStringLiteral("uint32") || text == QStringLiteral("u32")) {
        *result = ProtocolFieldType::UInt32;
    } else if (text == QStringLiteral("int32") || text == QStringLiteral("i32")) {
        *result = ProtocolFieldType::Int32;
    } else if (text == QStringLiteral("uint64") || text == QStringLiteral("u64")) {
        *result = ProtocolFieldType::UInt64;
    } else if (text == QStringLiteral("int64") || text == QStringLiteral("i64")) {
        *result = ProtocolFieldType::Int64;
    } else if (text == QStringLiteral("float32") || text == QStringLiteral("f32")) {
        *result = ProtocolFieldType::Float32;
    } else if (text == QStringLiteral("float64") || text == QStringLiteral("f64")) {
        *result = ProtocolFieldType::Float64;
    } else if (text == QStringLiteral("bytes")) {
        *result = ProtocolFieldType::ByteArray;
    } else {
        return fail(error, QStringLiteral("%1 使用未知字段类型 %2")
                    .arg(fieldName, value.toString()));
    }
    return true;
}

bool parseChecksum(const QJsonValue &value,
                   ProtocolChecksumSpec *checksum,
                   QString *error,
                   const QString &locationName)
{
    *checksum = ProtocolChecksumSpec();
    if (value.isUndefined() || value.isNull())
        return true;
    if (!value.isObject())
        return fail(error, locationName + QStringLiteral(": checksum 必须是对象"));

    const QJsonObject object = value.toObject();
    const QString type = object.value(QStringLiteral("type"))
            .toString(QStringLiteral("none")).trimmed().toLower();
    if (type == QStringLiteral("none")) {
        checksum->type = ProtocolChecksumType::None;
        return true;
    }
    if (type == QStringLiteral("xor8")) {
        checksum->type = ProtocolChecksumType::Xor8;
    } else if (type == QStringLiteral("sum8")) {
        checksum->type = ProtocolChecksumType::Sum8;
    } else if (type == QStringLiteral("crc16_modbus")
               || type == QStringLiteral("crc16modbus")) {
        checksum->type = ProtocolChecksumType::Crc16Modbus;
    } else if (type == QStringLiteral("crc16_ccitt")
               || type == QStringLiteral("crc16ccitt")
               || type == QStringLiteral("crc16_ccitt_false")
               || type == QStringLiteral("crc16ccittfalse")) {
        checksum->type = ProtocolChecksumType::Crc16CcittFalse;
    } else {
        return fail(error, locationName + QStringLiteral(": 不支持的校验类型 ") + type);
    }

    checksum->start = object.value(QStringLiteral("start")).toInt(-1);
    checksum->length = object.value(QStringLiteral("length")).toInt(-1);
    checksum->storedOffset = object.value(QStringLiteral("storedOffset")).toInt(-1);
    if (checksum->start < 0 || checksum->length <= 0 || checksum->storedOffset < 0) {
        return fail(error, locationName
                    + QStringLiteral(": checksum 必须配置 start、length、storedOffset"));
    }
    if (object.contains(QStringLiteral("initialValue"))) {
        qint64 initialValue = 0;
        if (!parseIntegerValue(object.value(QStringLiteral("initialValue")),
                               &initialValue)
                || initialValue < 0 || initialValue > 0xFFFF) {
            return fail(error, locationName
                        + QStringLiteral(": checksum.initialValue 必须是16位整数"));
        }
        checksum->initialValue = static_cast<quint16>(initialValue);
    }
    return parseByteOrder(object.value(QStringLiteral("storedByteOrder")),
                          ProtocolByteOrder::BigEndian,
                          &checksum->storedByteOrder,
                          error,
                          locationName + QStringLiteral(".checksum.storedByteOrder"));
}

bool parseField(const QJsonValue &value,
                ProtocolFieldSpec *field,
                QString *error,
                const QString &locationName)
{
    if (!value.isObject())
        return fail(error, locationName + QStringLiteral(": 字段必须是对象"));
    const QJsonObject object = value.toObject();
    *field = ProtocolFieldSpec();
    field->name = object.value(QStringLiteral("name")).toString().trimmed();
    field->canonical = object.value(QStringLiteral("canonical")).toString().trimmed();
    if (field->name.isEmpty())
        return fail(error, locationName + QStringLiteral(": 字段缺少 name"));
    if (!object.contains(QStringLiteral("offset"))
            || !object.contains(QStringLiteral("type"))) {
        return fail(error, locationName + QStringLiteral(": 字段 ")
                    + field->name + QStringLiteral(" 缺少 offset/type"));
    }
    field->offset = object.value(QStringLiteral("offset")).toInt(-1);
    if (field->offset < 0)
        return fail(error, locationName + QStringLiteral(": 字段 ")
                    + field->name + QStringLiteral(" 的 offset 无效"));

    if (!parseFieldType(object.value(QStringLiteral("type")),
                        &field->type, error, field->name)) {
        return false;
    }
    if (!parseByteOrder(object.value(QStringLiteral("endian")),
                        ProtocolByteOrder::LittleEndian,
                        &field->byteOrder,
                        error,
                        field->name + QStringLiteral(".endian"))) {
        return false;
    }
    field->bitOffset = object.value(QStringLiteral("bitOffset")).toInt(0);
    field->bitWidth = object.value(QStringLiteral("bitWidth")).toInt(0);
    field->scale = object.value(QStringLiteral("scale")).toDouble(1.0);
    field->bias = object.value(QStringLiteral("bias")).toDouble(0.0);
    field->unit = object.value(QStringLiteral("unit")).toString();
    if (qFuzzyIsNull(field->scale)) {
        return fail(error, locationName + QStringLiteral(": 字段 ")
                    + field->name + QStringLiteral(" 的 scale 不能为 0"));
    }

    const int size = protocolFieldSize(field->type);
    if (size == 0)
        return fail(error, locationName + QStringLiteral(": 当前版本不支持 bytes 字段"));
    if ((field->type == ProtocolFieldType::Float32
         || field->type == ProtocolFieldType::Float64)
            && (field->bitWidth > 0 || field->bitOffset > 0)) {
        return fail(error, locationName + QStringLiteral(": 浮点字段 ")
                    + field->name + QStringLiteral(" 不支持 bitOffset/bitWidth"));
    }
    if (field->bitOffset > 0 && field->bitWidth == 0) {
        return fail(error, locationName + QStringLiteral(": 字段 ")
                    + field->name + QStringLiteral(" 配置 bitOffset 时必须配置 bitWidth"));
    }
    if (field->bitWidth < 0 || field->bitWidth > size * 8) {
        return fail(error, locationName + QStringLiteral(": 字段 ")
                    + field->name + QStringLiteral(" 的 bitWidth 超出类型宽度"));
    }
    if (field->bitOffset < 0
            || field->bitOffset + qMax(1, field->bitWidth) > size * 8) {
        return fail(error, locationName + QStringLiteral(": 字段 ")
                    + field->name + QStringLiteral(" 的 bitOffset 超出类型宽度"));
    }

    const QJsonArray enums = object.value(QStringLiteral("enum")).toArray();
    QSet<qint64> enumValues;
    for (int i = 0; i < enums.size(); ++i) {
        if (!enums.at(i).isObject())
            return fail(error, locationName + QStringLiteral(": 字段 ")
                        + field->name + QStringLiteral(" 的 enum 必须是对象数组"));
        const QJsonObject enumObject = enums.at(i).toObject();
        ProtocolEnumOption option;
        if (!parseIntegerValue(enumObject.value(QStringLiteral("value")), &option.value)) {
            return fail(error, locationName + QStringLiteral(": 字段 ")
                        + field->name + QStringLiteral(" 的枚举值无效"));
        }
        if (enumValues.contains(option.value)) {
            return fail(error, locationName + QStringLiteral(": 字段 ")
                        + field->name + QStringLiteral(" 存在重复枚举值"));
        }
        enumValues.insert(option.value);
        option.name = enumObject.value(QStringLiteral("name")).toString().trimmed();
        option.label = enumObject.value(QStringLiteral("label")).toString().trimmed();
        if (option.name.isEmpty())
            option.name = QString::number(option.value);
        if (option.label.isEmpty())
            option.label = option.name;
        field->enumOptions.append(option);
    }
    return true;
}

bool parseFields(const QJsonValue &value,
                 QVector<ProtocolFieldSpec> *fields,
                 QString *error,
                 const QString &locationName)
{
    if (!value.isArray())
        return fail(error, locationName + QStringLiteral(": fields 必须是数组"));
    const QJsonArray array = value.toArray();
    if (array.isEmpty())
        return fail(error, locationName + QStringLiteral(": fields 不能为空"));

    QSet<QString> names;
    for (int i = 0; i < array.size(); ++i) {
        ProtocolFieldSpec field;
        const QString fieldLocation =
                locationName + QStringLiteral(".fields[%1]").arg(i);
        if (!parseField(array.at(i), &field, error, fieldLocation))
            return false;
        if (names.contains(field.name))
            return fail(error, locationName + QStringLiteral(": 字段名重复 ")
                        + field.name);
        names.insert(field.name);
        fields->append(field);
    }
    return true;
}

bool validateChecksumRange(const ProtocolChecksumSpec &checksum,
                           int frameLength,
                           QString *error,
                           const QString &locationName)
{
    if (checksum.type == ProtocolChecksumType::None)
        return true;
    const int checksumBytes =
            checksum.type == ProtocolChecksumType::Crc16Modbus
            || checksum.type == ProtocolChecksumType::Crc16CcittFalse ? 2 : 1;
    if (checksum.start < 0 || checksum.length <= 0
            || checksum.start + checksum.length > frameLength
            || checksum.storedOffset < 0
            || checksum.storedOffset + checksumBytes > frameLength) {
        return fail(error, locationName
                    + QStringLiteral(": checksum 范围超出帧长度"));
    }
    return true;
}

bool validateFields(const QVector<ProtocolFieldSpec> &fields,
                    int frameLength,
                    QString *error,
                    const QString &locationName)
{
    for (const ProtocolFieldSpec &field : fields) {
        const int size = protocolFieldSize(field.type);
        if (size <= 0 || field.offset + size > frameLength) {
            return fail(error, locationName + QStringLiteral(": 字段 ")
                        + field.name + QStringLiteral(" 超出帧长度"));
        }
    }
    return true;
}

} // namespace

int protocolFieldSize(ProtocolFieldType type)
{
    switch (type) {
    case ProtocolFieldType::UInt8:
    case ProtocolFieldType::Int8:
        return 1;
    case ProtocolFieldType::UInt16:
    case ProtocolFieldType::Int16:
        return 2;
    case ProtocolFieldType::UInt32:
    case ProtocolFieldType::Int32:
    case ProtocolFieldType::Float32:
        return 4;
    case ProtocolFieldType::UInt64:
    case ProtocolFieldType::Int64:
    case ProtocolFieldType::Float64:
        return 8;
    case ProtocolFieldType::ByteArray:
        return 0;
    }
    return 0;
}

bool protocolFieldIsSigned(ProtocolFieldType type)
{
    return type == ProtocolFieldType::Int8
            || type == ProtocolFieldType::Int16
            || type == ProtocolFieldType::Int32
            || type == ProtocolFieldType::Int64;
}

QString protocolFieldTypeName(ProtocolFieldType type)
{
    switch (type) {
    case ProtocolFieldType::UInt8: return QStringLiteral("uint8");
    case ProtocolFieldType::Int8: return QStringLiteral("int8");
    case ProtocolFieldType::UInt16: return QStringLiteral("uint16");
    case ProtocolFieldType::Int16: return QStringLiteral("int16");
    case ProtocolFieldType::UInt32: return QStringLiteral("uint32");
    case ProtocolFieldType::Int32: return QStringLiteral("int32");
    case ProtocolFieldType::UInt64: return QStringLiteral("uint64");
    case ProtocolFieldType::Int64: return QStringLiteral("int64");
    case ProtocolFieldType::Float32: return QStringLiteral("float32");
    case ProtocolFieldType::Float64: return QStringLiteral("float64");
    case ProtocolFieldType::ByteArray: return QStringLiteral("bytes");
    }
    return QStringLiteral("unknown");
}

bool ProtocolProfile::isValid() const
{
    return schemaVersion == 1 && !id.isEmpty() && !frames.isEmpty();
}

const ProtocolFrameSpec *ProtocolProfile::findFrame(const QString &name) const
{
    for (const ProtocolFrameSpec &frame : frames) {
        if (frame.name == name)
            return &frame;
    }
    return nullptr;
}

const ProtocolCommandSpec *ProtocolProfile::findCommand(const QString &name) const
{
    const auto it = commands.constFind(name);
    return it == commands.constEnd() ? nullptr : &it.value();
}

bool ProtocolProfile::loadFromFile(const QString &path,
                                   ProtocolProfile *profile,
                                   QString *error)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return fail(error, QStringLiteral("无法读取协议配置 %1: %2")
                    .arg(path, file.errorString()));
    }
    return fromJson(file.readAll(), profile, error, path);
}

bool ProtocolProfile::fromJson(const QByteArray &json,
                               ProtocolProfile *profile,
                               QString *error,
                               const QString &sourceName)
{
    if (!profile)
        return fail(error, QStringLiteral("协议配置输出指针为空"));

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(json, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        return fail(error, location(sourceName,
                    QStringLiteral("JSON 解析失败，偏移 %1: %2")
                    .arg(parseError.offset)
                    .arg(parseError.errorString())));
    }
    if (!document.isObject())
        return fail(error, location(sourceName, QStringLiteral("根节点必须是对象")));

    const QJsonObject root = document.object();
    ProtocolProfile parsed;
    parsed.schemaVersion = root.value(QStringLiteral("schemaVersion")).toInt(0);
    parsed.id = root.value(QStringLiteral("id")).toString().trimmed();
    parsed.vendor = root.value(QStringLiteral("vendor")).toString().trimmed();
    parsed.model = root.value(QStringLiteral("model")).toString().trimmed();
    parsed.source = root.value(QStringLiteral("source")).toString().trimmed();
    parsed.description = root.value(QStringLiteral("description")).toString();
    parsed.capabilities =
            root.value(QStringLiteral("capabilities")).toObject().toVariantMap();

    if (parsed.schemaVersion != 1) {
        return fail(error, location(sourceName,
                    QStringLiteral("仅支持 schemaVersion=1")));
    }
    if (parsed.id.isEmpty())
        return fail(error, location(sourceName, QStringLiteral("缺少 id")));
    if (parsed.source.isEmpty())
        return fail(error, location(sourceName, QStringLiteral("缺少 source")));

    const QJsonArray frames = root.value(QStringLiteral("frames")).toArray();
    if (frames.isEmpty())
        return fail(error, location(sourceName, QStringLiteral("frames 不能为空")));

    QSet<QString> frameNames;
    for (int i = 0; i < frames.size(); ++i) {
        if (!frames.at(i).isObject())
            return fail(error, location(sourceName,
                        QStringLiteral("frames[%1] 必须是对象").arg(i)));
        const QJsonObject object = frames.at(i).toObject();
        ProtocolFrameSpec frame;
        frame.name = object.value(QStringLiteral("name")).toString().trimmed();
        if (frame.name.isEmpty())
            return fail(error, location(sourceName,
                        QStringLiteral("frames[%1] 缺少 name").arg(i)));
        if (frameNames.contains(frame.name))
            return fail(error, location(sourceName,
                        QStringLiteral("frame name 重复: ") + frame.name));
        frameNames.insert(frame.name);

        frame.header = QByteArray::fromHex(
                    object.value(QStringLiteral("header")).toString()
                    .remove(QLatin1Char(' ')).toLatin1());
        frame.frameLength = object.value(QStringLiteral("length")).toInt(0);
        if (frame.header.isEmpty())
            return fail(error, location(sourceName,
                        QStringLiteral("frame %1 缺少有效 header").arg(frame.name)));
        if (frame.frameLength <= frame.header.size())
            return fail(error, location(sourceName,
                        QStringLiteral("frame %1 的 length 必须大于 header")
                        .arg(frame.name)));

        const QString frameLocation = location(sourceName,
                                                QStringLiteral("frame.")
                                                + frame.name);
        if (!parseFields(object.value(QStringLiteral("fields")),
                         &frame.fields, error, frameLocation)
                || !parseChecksum(object.value(QStringLiteral("checksum")),
                                  &frame.checksum, error, frameLocation)
                || !validateFields(frame.fields, frame.frameLength,
                                   error, frameLocation)
                || !validateChecksumRange(frame.checksum, frame.frameLength,
                                          error, frameLocation)) {
            return false;
        }
        parsed.frames.append(frame);
    }

    const QJsonObject commands =
            root.value(QStringLiteral("commands")).toObject();
    for (auto it = commands.constBegin(); it != commands.constEnd(); ++it) {
        if (it.key().startsWith(QLatin1Char('_')))
            continue;
        if (!it.value().isObject())
            return fail(error, location(sourceName,
                        QStringLiteral("command %1 必须是对象").arg(it.key())));
        const QJsonObject object = it.value().toObject();
        ProtocolCommandSpec command;
        command.name = it.key();
        command.frameTemplate = QByteArray::fromHex(
                    object.value(QStringLiteral("template")).toString()
                    .remove(QLatin1Char(' ')).toLatin1());
        if (command.frameTemplate.isEmpty())
            return fail(error, location(sourceName,
                        QStringLiteral("command %1 缺少有效 template")
                        .arg(command.name)));
        const QString commandLocation = location(sourceName,
                                                 QStringLiteral("command.")
                                                 + command.name);
        if (!parseFields(object.value(QStringLiteral("fields")),
                         &command.fields, error, commandLocation)
                || !parseChecksum(object.value(QStringLiteral("checksum")),
                                  &command.checksum, error, commandLocation)
                || !validateFields(command.fields,
                                   command.frameTemplate.size(),
                                   error, commandLocation)
                || !validateChecksumRange(command.checksum,
                                          command.frameTemplate.size(),
                                          error, commandLocation)) {
            return false;
        }

        command.commandCodeField =
                object.value(QStringLiteral("commandCodeField"))
                .toString().trimmed();
        const QJsonObject codes =
                object.value(QStringLiteral("commandCodes")).toObject();
        for (auto codeIt = codes.constBegin(); codeIt != codes.constEnd(); ++codeIt) {
            if (codeIt.key().startsWith(QLatin1Char('_')))
                continue;
            qint64 code = 0;
            if (!parseIntegerValue(codeIt.value(), &code)) {
                return fail(error, commandLocation
                            + QStringLiteral(": commandCodes.%1 无效")
                            .arg(codeIt.key()));
            }
            command.commandCodes.insert(codeIt.key(), code);
        }
        parsed.commands.insert(command.name, command);
    }

    if (!parsed.isValid())
        return fail(error, location(sourceName, QStringLiteral("协议配置不完整")));
    if (profile)
        *profile = parsed;
    if (error)
        error->clear();
    return true;
}
