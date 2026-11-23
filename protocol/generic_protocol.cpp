#include "generic_protocol.h"

#include <QDateTime>
#include <QElapsedTimer>
#include <QSet>
#include <QtEndian>
#include <cmath>
#include <cstring>

namespace {

quint64 readUnsignedValue(const QByteArray &data,
                          int offset,
                          int size,
                          ProtocolByteOrder byteOrder)
{
    quint64 value = 0;
    if (byteOrder == ProtocolByteOrder::BigEndian) {
        for (int i = 0; i < size; ++i) {
            value = (value << 8)
                    | static_cast<quint8>(data.at(offset + i));
        }
    } else {
        for (int i = size - 1; i >= 0; --i) {
            value = (value << 8)
                    | static_cast<quint8>(data.at(offset + i));
        }
    }
    return value;
}

qint64 signExtend(quint64 value, int bitWidth)
{
    if (bitWidth <= 0 || bitWidth >= 64)
        return static_cast<qint64>(value);
    const quint64 signBit = quint64(1) << (bitWidth - 1);
    if ((value & signBit) == 0)
        return static_cast<qint64>(value);
    const quint64 mask = (~quint64(0)) << bitWidth;
    return static_cast<qint64>(value | mask);
}

QVariant decodeField(const QByteArray &frame,
                     const ProtocolFieldSpec &field)
{
    const int size = protocolFieldSize(field.type);
    if (size <= 0 || field.offset < 0 || field.offset + size > frame.size())
        return QVariant();

    if (field.type == ProtocolFieldType::Float32) {
        const quint32 bits = static_cast<quint32>(
                    readUnsignedValue(frame, field.offset, 4, field.byteOrder));
        float value = 0.0f;
        std::memcpy(&value, &bits, sizeof(value));
        return static_cast<double>(value) * field.scale + field.bias;
    }
    if (field.type == ProtocolFieldType::Float64) {
        const quint64 bits =
                readUnsignedValue(frame, field.offset, 8, field.byteOrder);
        double value = 0.0;
        std::memcpy(&value, &bits, sizeof(value));
        return value * field.scale + field.bias;
    }

    quint64 raw = readUnsignedValue(frame, field.offset, size, field.byteOrder);
    if (field.bitWidth > 0) {
        const quint64 mask = field.bitWidth >= 64
                ? ~quint64(0) : (quint64(1) << field.bitWidth) - 1;
        raw = (raw >> field.bitOffset) & mask;
        if (protocolFieldIsSigned(field.type))
            return static_cast<double>(signExtend(raw, field.bitWidth))
                    * field.scale + field.bias;
    } else if (protocolFieldIsSigned(field.type)) {
        raw = static_cast<quint64>(signExtend(raw, size * 8));
    }
    return static_cast<double>(static_cast<qint64>(raw))
            * field.scale + field.bias;
}

QVariant rawFieldValue(const QByteArray &frame,
                       const ProtocolFieldSpec &field)
{
    const int size = protocolFieldSize(field.type);
    if (size <= 0 || field.offset < 0 || field.offset + size > frame.size())
        return QVariant();
    if (field.type == ProtocolFieldType::Float32
            || field.type == ProtocolFieldType::Float64) {
        return decodeField(frame, field);
    }
    quint64 raw = readUnsignedValue(frame, field.offset, size, field.byteOrder);
    if (field.bitWidth > 0) {
        const quint64 mask = field.bitWidth >= 64
                ? ~quint64(0) : (quint64(1) << field.bitWidth) - 1;
        raw = (raw >> field.bitOffset) & mask;
        if (protocolFieldIsSigned(field.type))
            return signExtend(raw, field.bitWidth);
    } else if (protocolFieldIsSigned(field.type)) {
        raw = static_cast<quint64>(signExtend(raw, size * 8));
    }
    return static_cast<qint64>(raw);
}

bool findEnumOption(const ProtocolFieldSpec &field,
                    qint64 raw,
                    ProtocolEnumOption *option)
{
    for (const ProtocolEnumOption &candidate : field.enumOptions) {
        if (candidate.value == raw) {
            if (option)
                *option = candidate;
            return true;
        }
    }
    return false;
}

int longestMatchingSuffix(const QByteArray &buffer,
                          const ProtocolProfile &profile)
{
    int keep = 0;
    for (const ProtocolFrameSpec &frame : profile.frames) {
        const int limit = qMin(frame.header.size() - 1, buffer.size());
        for (int length = 1; length <= limit; ++length) {
            if (buffer.right(length) == frame.header.left(length))
                keep = qMax(keep, length);
        }
    }
    return keep;
}

bool findNextHeader(const QByteArray &buffer,
                    const ProtocolProfile &profile,
                    int *position,
                    const ProtocolFrameSpec **frameSpec)
{
    int bestPosition = -1;
    int bestHeaderLength = -1;
    const ProtocolFrameSpec *bestFrame = nullptr;
    for (const ProtocolFrameSpec &frame : profile.frames) {
        int from = 0;
        while (from <= buffer.size() - frame.header.size()) {
            const int found = buffer.indexOf(frame.header, from);
            if (found < 0)
                break;
            if (bestPosition < 0 || found < bestPosition
                    || (found == bestPosition
                        && frame.header.size() > bestHeaderLength)) {
                bestPosition = found;
                bestHeaderLength = frame.header.size();
                bestFrame = &frame;
            }
            from = found + 1;
        }
    }
    if (bestPosition < 0)
        return false;
    *position = bestPosition;
    *frameSpec = bestFrame;
    return true;
}

void putCommandInteger(QByteArray *frame,
                       int offset,
                       int size,
                       ProtocolByteOrder byteOrder,
                       quint64 value)
{
    for (int i = 0; i < size; ++i) {
        const int destination = byteOrder == ProtocolByteOrder::BigEndian
                ? offset + size - 1 - i : offset + i;
        (*frame)[destination] = static_cast<char>(value & 0xFF);
        value >>= 8;
    }
}

bool writeCommandField(QByteArray *frame,
                       const ProtocolFieldSpec &field,
                       const QVariant &value,
                       QString *error)
{
    const int size = protocolFieldSize(field.type);
    if (size <= 0 || field.offset < 0 || field.offset + size > frame->size()) {
        if (error) {
            *error = QStringLiteral("命令字段 %1 超出帧长度").arg(field.name);
        }
        return false;
    }

    if (field.type == ProtocolFieldType::Float32) {
        const float number = static_cast<float>(
                    (value.toDouble() - field.bias) / field.scale);
        quint32 bits = 0;
        std::memcpy(&bits, &number, sizeof(bits));
        putCommandInteger(frame, field.offset, 4, field.byteOrder, bits);
        return true;
    }
    if (field.type == ProtocolFieldType::Float64) {
        const double number = (value.toDouble() - field.bias) / field.scale;
        quint64 bits = 0;
        std::memcpy(&bits, &number, sizeof(bits));
        putCommandInteger(frame, field.offset, 8, field.byteOrder, bits);
        return true;
    }

    const double physical = (value.toDouble() - field.bias) / field.scale;
    qint64 rawValue = static_cast<qint64>(qRound64(physical));
    if (field.bitWidth > 0) {
        const quint64 mask = field.bitWidth >= 64
                ? ~quint64(0) : (quint64(1) << field.bitWidth) - 1;
        quint64 base = readUnsignedValue(*frame, field.offset, size,
                                         field.byteOrder);
        const quint64 shiftedMask = mask << field.bitOffset;
        base = (base & ~shiftedMask)
                | ((static_cast<quint64>(rawValue) & mask)
                   << field.bitOffset);
        putCommandInteger(frame, field.offset, size, field.byteOrder, base);
        return true;
    }

    quint64 raw = static_cast<quint64>(rawValue);
    if (size < 8) {
        const quint64 typeMask = (quint64(1) << (size * 8)) - 1;
        raw &= typeMask;
    }
    putCommandInteger(frame, field.offset, size, field.byteOrder, raw);
    return true;
}

} // namespace

quint16 computeProtocolChecksum(const QByteArray &frame,
                                const ProtocolChecksumSpec &spec,
                                bool *ok)
{
    if (ok)
        *ok = false;
    if (spec.type == ProtocolChecksumType::None) {
        if (ok)
            *ok = true;
        return 0;
    }
    if (spec.start < 0 || spec.length <= 0
            || spec.start + spec.length > frame.size()) {
        return 0;
    }

    const quint8 *data = reinterpret_cast<const quint8 *>(frame.constData())
            + spec.start;
    if (spec.type == ProtocolChecksumType::Xor8) {
        quint8 value = 0;
        for (int i = 0; i < spec.length; ++i)
            value ^= data[i];
        if (ok)
            *ok = true;
        return value;
    }
    if (spec.type == ProtocolChecksumType::Sum8) {
        quint8 value = 0;
        for (int i = 0; i < spec.length; ++i)
            value = static_cast<quint8>(value + data[i]);
        if (ok)
            *ok = true;
        return value;
    }
    if (spec.type == ProtocolChecksumType::Crc16Modbus) {
        quint16 crc = 0xFFFF;
        for (int i = 0; i < spec.length; ++i) {
            crc ^= data[i];
            for (int bit = 0; bit < 8; ++bit) {
                if (crc & 0x0001)
                    crc = static_cast<quint16>((crc >> 1) ^ 0xA001);
                else
                    crc >>= 1;
            }
        }
        if (ok)
            *ok = true;
        return crc;
    }
    if (spec.type == ProtocolChecksumType::Crc16CcittFalse) {
        quint16 crc = spec.initialValue;
        for (int i = 0; i < spec.length; ++i) {
            crc ^= static_cast<quint16>(data[i]) << 8;
            for (int bit = 0; bit < 8; ++bit) {
                crc = (crc & 0x8000)
                        ? static_cast<quint16>((crc << 1) ^ 0x1021)
                        : static_cast<quint16>(crc << 1);
            }
        }
        if (ok)
            *ok = true;
        return crc;
    }
    return 0;
}

ConfigurableFrameDecoder::ConfigurableFrameDecoder()
{
    m_frameSequence = 0;
}

void ConfigurableFrameDecoder::setProfile(const ProtocolProfile &profile)
{
    m_profile = profile;
    reset();
}

void ConfigurableFrameDecoder::clearProfile()
{
    m_profile = ProtocolProfile();
    reset();
}

void ConfigurableFrameDecoder::reset()
{
    m_buffer.clear();
    m_frameSequence = 0;
}

bool ConfigurableFrameDecoder::isConfigured() const
{
    return m_profile.isValid();
}

QString ConfigurableFrameDecoder::profileId() const
{
    return m_profile.id;
}

int ConfigurableFrameDecoder::bufferedByteCount() const
{
    return m_buffer.size();
}

ProtocolDecodeBatch ConfigurableFrameDecoder::append(const QByteArray &data)
{
    ProtocolDecodeBatch result;
    if (!isConfigured()) {
        result.errors.append({QString(), QStringLiteral("没有加载有效协议配置"), 0});
        return result;
    }
    if (data.isEmpty())
        return result;
    m_buffer.append(data);

    static QElapsedTimer monotonicClock;
    if (!monotonicClock.isValid())
        monotonicClock.start();

    while (!m_buffer.isEmpty()) {
        int headerPosition = -1;
        const ProtocolFrameSpec *frameSpec = nullptr;
        if (!findNextHeader(m_buffer, m_profile, &headerPosition, &frameSpec)) {
            const int keep = longestMatchingSuffix(m_buffer, m_profile);
            if (m_buffer.size() > keep) {
                m_buffer.remove(0, m_buffer.size() - keep);
            }
            break;
        }
        if (headerPosition > 0)
            m_buffer.remove(0, headerPosition);
        if (m_buffer.size() < frameSpec->frameLength)
            break;

        const QByteArray candidate = m_buffer.left(frameSpec->frameLength);
        bool checksumRangeOk = true;
        const quint16 calculated = computeProtocolChecksum(
                    candidate, frameSpec->checksum, &checksumRangeOk);
        const int checksumBytes =
                frameSpec->checksum.type == ProtocolChecksumType::Crc16Modbus
                || frameSpec->checksum.type == ProtocolChecksumType::Crc16CcittFalse
                ? 2 : 1;
        quint64 stored = 0;
        if (frameSpec->checksum.type != ProtocolChecksumType::None
                && checksumRangeOk) {
            stored = readUnsignedValue(candidate,
                                       frameSpec->checksum.storedOffset,
                                       checksumBytes,
                                       frameSpec->checksum.storedByteOrder);
        }
        if (!checksumRangeOk
                || (frameSpec->checksum.type != ProtocolChecksumType::None
                    && stored != calculated)) {
            result.errors.append({
                frameSpec->name,
                QStringLiteral("校验失败，期望 0x%1，计算值 0x%2")
                    .arg(stored, checksumBytes * 2, 16, QLatin1Char('0'))
                    .arg(calculated, checksumBytes * 2, 16, QLatin1Char('0')),
                0
            });
            m_buffer.remove(0, 1);
            continue;
        }

        CanonicalSeekerFrame frame;
        frame.profileId = m_profile.id;
        frame.frameName = frameSpec->name;
        frame.frameLength = candidate.size();
        frame.checksumValid = true;
        frame.receiveSequence = ++m_frameSequence;
        frame.receiveMonotonicMs = monotonicClock.elapsed();
        frame.receiveWallClockMs = QDateTime::currentMSecsSinceEpoch();
        frame.rawFrame = candidate;

        bool fieldsValid = true;
        for (const ProtocolFieldSpec &field : frameSpec->fields) {
            const QVariant decoded = decodeField(candidate, field);
            const QVariant raw = rawFieldValue(candidate, field);
            if (!decoded.isValid() || !raw.isValid()) {
                result.errors.append({
                    frameSpec->name,
                    QStringLiteral("字段 %1 无法解码").arg(field.name),
                    0
                });
                fieldsValid = false;
                break;
            }
            frame.decodedFields.insert(field.name, decoded);
            frame.rawFields.insert(field.name, raw);
            if (field.canonical.isEmpty())
                continue;

            ProtocolEnumOption option;
            if (findEnumOption(field, raw.toLongLong(), &option)) {
                frame.canonicalFields.insert(field.canonical, option.name);
                frame.canonicalFields.insert(field.canonical + QStringLiteral("Label"),
                                             option.label);
                frame.canonicalFields.insert(field.canonical + QStringLiteral("Raw"),
                                             option.value);
            } else {
                frame.canonicalFields.insert(field.canonical, decoded);
                if (!field.unit.isEmpty())
                    frame.canonicalFields.insert(field.canonical
                                                 + QStringLiteral("Unit"),
                                                 field.unit);
            }
        }
        if (fieldsValid)
            result.frames.append(frame);
        m_buffer.remove(0, candidate.size());
    }
    return result;
}

ProtocolCommandEncoder::ProtocolCommandEncoder()
{
}

void ProtocolCommandEncoder::setProfile(const ProtocolProfile &profile)
{
    m_profile = profile;
}

bool ProtocolCommandEncoder::isConfigured() const
{
    return m_profile.isValid();
}

bool ProtocolCommandEncoder::encode(const ProtocolCommandRequest &request,
                                    QByteArray *frame,
                                    QString *error) const
{
    if (!frame)
        return false;
    if (!isConfigured()) {
        if (error)
            *error = QStringLiteral("没有加载有效协议配置");
        return false;
    }
    const ProtocolCommandSpec *command = m_profile.findCommand(request.specName);
    if (!command) {
        if (error) {
            *error = QStringLiteral("协议 %1 中没有命令 %2")
                    .arg(m_profile.id, request.specName);
        }
        return false;
    }

    QByteArray output = command->frameTemplate;
    QVariantMap values = request.values;
    if (!command->commandCodeField.isEmpty()
            && !request.semanticCommand.isEmpty()) {
        if (!command->commandCodes.contains(request.semanticCommand)) {
            if (error) {
                *error = QStringLiteral("命令 %1 没有配置协议码")
                        .arg(request.semanticCommand);
            }
            return false;
        }
        values.insert(command->commandCodeField,
                      command->commandCodes.value(request.semanticCommand));
    }

    for (const ProtocolFieldSpec &field : command->fields) {
        if (!values.contains(field.name))
            continue;
        if (!writeCommandField(&output, field, values.value(field.name), error))
            return false;
    }

    if (command->checksum.type != ProtocolChecksumType::None) {
        bool checksumOk = false;
        const quint16 checksum =
                computeProtocolChecksum(output, command->checksum, &checksumOk);
        if (!checksumOk) {
            if (error)
                *error = QStringLiteral("命令帧校验范围无效");
            return false;
        }
        const int checksumBytes =
                command->checksum.type == ProtocolChecksumType::Crc16Modbus
                || command->checksum.type == ProtocolChecksumType::Crc16CcittFalse
                ? 2 : 1;
        putCommandInteger(&output,
                          command->checksum.storedOffset,
                          checksumBytes,
                          command->checksum.storedByteOrder,
                          checksum);
    }
    *frame = output;
    if (error)
        error->clear();
    return true;
}
