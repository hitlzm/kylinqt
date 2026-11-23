#ifndef CANONICAL_SEEKER_FRAME_H
#define CANONICAL_SEEKER_FRAME_H

#include <QByteArray>
#include <QMetaType>
#include <QString>
#include <QVariant>
#include <QVariantMap>

// 协议适配层向业务层提供的厂家无关数据。rawFields、decodedFields 用于保留
// 厂家原始语义，canonicalFields 使用稳定字段名供显示、录制和引导链路消费。
struct CanonicalSeekerFrame
{
    QString profileId;
    QString frameName;
    int frameLength = 0;
    bool checksumValid = false;
    quint64 receiveSequence = 0;
    qint64 receiveMonotonicMs = 0;
    qint64 receiveWallClockMs = 0;
    QByteArray rawFrame;
    QVariantMap rawFields;
    QVariantMap decodedFields;
    QVariantMap canonicalFields;

    bool hasRawField(const QString &name) const
    {
        return rawFields.contains(name);
    }

    bool hasDecodedField(const QString &name) const
    {
        return decodedFields.contains(name);
    }

    bool hasCanonical(const QString &name) const
    {
        return canonicalFields.contains(name);
    }

    QVariant rawValue(const QString &name) const
    {
        return rawFields.value(name);
    }

    QVariant decodedValue(const QString &name) const
    {
        return decodedFields.value(name);
    }

    QVariant canonicalValue(const QString &name) const
    {
        return canonicalFields.value(name);
    }

    qint64 rawInteger(const QString &name, qint64 fallback = 0) const
    {
        const QVariant value = rawFields.value(name);
        bool ok = false;
        const qint64 result = value.toLongLong(&ok);
        return ok ? result : fallback;
    }

    qint64 canonicalInteger(const QString &name, qint64 fallback = 0) const
    {
        const QVariant value = canonicalFields.value(name);
        bool ok = false;
        const qint64 result = value.toLongLong(&ok);
        return ok ? result : fallback;
    }

    double canonicalNumber(const QString &name, double fallback = 0.0) const
    {
        const QVariant value = canonicalFields.value(name);
        bool ok = false;
        const double result = value.toDouble(&ok);
        return ok ? result : fallback;
    }

    bool canonicalBoolean(const QString &name, bool fallback = false) const
    {
        const QVariant value = canonicalFields.value(name);
        if (!value.isValid())
            return fallback;
        return value.toBool();
    }

    QString canonicalText(const QString &name,
                          const QString &fallback = QString()) const
    {
        const QVariant value = canonicalFields.value(name);
        return value.isValid() ? value.toString() : fallback;
    }
};

struct ProtocolCommandRequest
{
    QString specName;
    QString semanticCommand;
    QVariantMap values;
};

Q_DECLARE_METATYPE(CanonicalSeekerFrame)
Q_DECLARE_METATYPE(ProtocolCommandRequest)

#endif // CANONICAL_SEEKER_FRAME_H
