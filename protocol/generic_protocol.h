#ifndef GENERIC_PROTOCOL_H
#define GENERIC_PROTOCOL_H

#include "canonical_seeker_frame.h"
#include "protocol_profile.h"

#include <QByteArray>
#include <QString>
#include <QVector>

struct ProtocolDecodeError
{
    QString frameName;
    QString message;
    int bufferOffset = 0;
};

struct ProtocolDecodeBatch
{
    QVector<CanonicalSeekerFrame> frames;
    QVector<ProtocolDecodeError> errors;
};

class ConfigurableFrameDecoder
{
public:
    ConfigurableFrameDecoder();

    void setProfile(const ProtocolProfile &profile);
    void clearProfile();
    void reset();

    bool isConfigured() const;
    QString profileId() const;
    int bufferedByteCount() const;
    ProtocolDecodeBatch append(const QByteArray &data);

private:
    ProtocolProfile m_profile;
    QByteArray m_buffer;
    quint64 m_frameSequence = 0;
};

class ProtocolCommandEncoder
{
public:
    ProtocolCommandEncoder();

    void setProfile(const ProtocolProfile &profile);
    bool isConfigured() const;

    bool encode(const ProtocolCommandRequest &request,
                QByteArray *frame,
                QString *error) const;

private:
    ProtocolProfile m_profile;
};

quint16 computeProtocolChecksum(const QByteArray &frame,
                                const ProtocolChecksumSpec &spec,
                                bool *ok = nullptr);

#endif // GENERIC_PROTOCOL_H
