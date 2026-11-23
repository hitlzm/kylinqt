#include "protocol/generic_protocol.h"

#include <QCoreApplication>
#include <QString>

#include <cmath>
#include <cstdio>

namespace {

bool check(bool condition, const char *message)
{
    if (!condition)
        std::fprintf(stderr, "FAIL: %s\n", message);
    return condition;
}

bool closeTo(double actual, double expected, double tolerance = 1e-6)
{
    return std::abs(actual - expected) <= tolerance;
}

} // namespace

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const QString profilePath = QStringLiteral(KYLINQT_PROTOCOL_PROFILE_PATH);
    ProtocolProfile profile;
    QString error;
    if (!check(ProtocolProfile::loadFromFile(profilePath, &profile, &error),
               "load legacy laser profile")) {
        std::fprintf(stderr, "profile error: %s\n", error.toLocal8Bit().constData());
        return 1;
    }
    if (!check(profile.id == QStringLiteral("legacy-laser-v1"),
               "profile id") ||
        !check(profile.frames.size() == 1, "profile frame count") ||
        !check(profile.commands.contains(QStringLiteral("laser.control")),
               "profile command")) {
        return 2;
    }

    const QByteArray sampledFrame = QByteArray::fromHex(
                "55AADC112508184004D2FF160064FF9C0032FFCE007BFE3827100000"
                "B20A141E2800C900CA00000199C82CC0000159A8");
    if (!check(sampledFrame.size() == 48, "sample frame length"))
        return 3;

    ConfigurableFrameDecoder decoder;
    decoder.setProfile(profile);
    ProtocolDecodeBatch partial = decoder.append(sampledFrame.left(19));
    if (!check(partial.frames.isEmpty(), "partial frame is buffered") ||
        !check(partial.errors.isEmpty(), "partial frame has no error")) {
        return 4;
    }

    ProtocolDecodeBatch complete = decoder.append(sampledFrame.mid(19));
    if (!check(complete.errors.isEmpty(), "complete frame has no decode error") ||
        !check(complete.frames.size() == 1, "complete frame is emitted")) {
        for (const ProtocolDecodeError &decodeError : complete.errors)
            std::fprintf(stderr, "decode error: %s\n",
                         decodeError.message.toLocal8Bit().constData());
        return 5;
    }

    const CanonicalSeekerFrame &frame = complete.frames.first();
    if (!check(frame.frameName == QStringLiteral("laserStatus"),
               "decoded frame name") ||
        !check(frame.checksumValid, "checksum valid") ||
        !check(closeTo(frame.canonicalNumber("measurement.losAzimuthDeg"), 12.34),
               "azimuth canonical value") ||
        !check(closeTo(frame.canonicalNumber("measurement.losElevationDeg"), -2.34),
               "elevation canonical value") ||
        !check(closeTo(frame.canonicalNumber("measurement.deviationAzimuthDeg"), 0.123),
               "deviation azimuth canonical value") ||
        !check(closeTo(frame.canonicalNumber("measurement.laserPeriodMs"), 20.0),
               "laser period canonical value") ||
        !check(frame.canonicalText("tracking.state") == QStringLiteral("tracking"),
               "tracking enum name") ||
        !check(frame.canonicalText("tracking.stateLabel") == QStringLiteral("稳定跟踪"),
               "tracking enum label") ||
        !check(frame.canonicalBoolean("status.detectorPeriodConfigured"),
               "detector period bit") ||
        !check(frame.canonicalText("status.detectorSelfTest") == QStringLiteral("normal"),
               "detector self-test enum") ||
        !check(frame.canonicalInteger("status.faultCode") == 0,
               "fault code bitfield")) {
        return 6;
    }

    QByteArray badFrame = sampledFrame;
    badFrame[47] = static_cast<char>(badFrame.at(47) ^ 0x01);
    decoder.reset();
    ProtocolDecodeBatch bad = decoder.append(badFrame);
    if (!check(bad.frames.isEmpty(), "bad checksum frame is rejected") ||
        !check(!bad.errors.isEmpty(), "bad checksum emits diagnostic")) {
        return 7;
    }

    decoder.reset();
    ProtocolDecodeBatch resynchronized = decoder.append(
                QByteArray::fromHex("00FF55")
                + sampledFrame + sampledFrame);
    if (!check(resynchronized.errors.isEmpty(),
               "noise before frame is discarded without false error") ||
        !check(resynchronized.frames.size() == 2,
               "two concatenated frames are both emitted")) {
        return 8;
    }

    ProtocolCommandEncoder encoder;
    encoder.setProfile(profile);
    ProtocolCommandRequest request;
    request.specName = QStringLiteral("laser.control");
    request.semanticCommand = QStringLiteral("lock");
    request.values.insert(QStringLiteral("frameStatus"), 0x51);
    request.values.insert(QStringLiteral("frameId"), 0x15);
    request.values.insert(QStringLiteral("laserPeriod"), 20.0);
    request.values.insert(QStringLiteral("param2"), 12.34);
    request.values.insert(QStringLiteral("param3"), -2.34);
    request.values.insert(QStringLiteral("param4"), 0.0);
    request.values.insert(QStringLiteral("param5"), 0.0);
    request.values.insert(QStringLiteral("timeSync"), 1);
    request.values.insert(QStringLiteral("deviceTimeMs"), 1760000000000.0);

    QByteArray encoded;
    if (!check(encoder.encode(request, &encoded, &error),
               "encode configured command")) {
        std::fprintf(stderr, "encode error: %s\n", error.toLocal8Bit().constData());
        return 9;
    }
    const QByteArray expected = QByteArray::fromHex(
                "55AADC511502271004D2FF16000000000100000199C82CC000F3");
    if (!check(encoded == expected, "encoded command is byte-for-byte compatible")) {
        std::fprintf(stderr, "encoded: %s\nexpected: %s\n",
                     encoded.toHex().constData(),
                     expected.toHex().constData());
        return 10;
    }

    request.semanticCommand = QStringLiteral("notConfigured");
    if (!check(!encoder.encode(request, &encoded, &error),
               "unknown semantic command is rejected")) {
        return 11;
    }

    ProtocolProfile imageProfile;
    if (!check(ProtocolProfile::loadFromFile(
                   QStringLiteral(KYLINQT_IMAGE_PROTOCOL_PROFILE_PATH),
                   &imageProfile, &error),
               "load image profile")) {
        std::fprintf(stderr, "image profile error: %s\n",
                     error.toLocal8Bit().constData());
        return 12;
    }
    const QByteArray imageFrame = QByteArray::fromHex(
                "77AB9834127856AA040300013F0CFEFA002A03D4FE9001000000010285FFC801"
                "221100000000000000005901000000000000000000F4FF0F0064009CFFC80038"
                "FF000000000000000001000000000000000000FE0000000000000504030201C4"
                "09000000000D0C0B0A0000000000001E19000000002C01000000000000000000"
                "00000001020300000000000000000000000000000000000000006249");
    ConfigurableFrameDecoder imageDecoder;
    imageDecoder.setProfile(imageProfile);
    ProtocolDecodeBatch imageBatch = imageDecoder.append(imageFrame);
    if (!check(imageBatch.errors.isEmpty(), "image frame has no decode error") ||
        !check(imageBatch.frames.size() == 1, "image frame is decoded") ||
        !check(imageBatch.frames.first().canonicalInteger("frame.sequence")
               == 0x1234, "image frame sequence") ||
        !check(closeTo(imageBatch.frames.first().canonicalNumber(
                           "measurement.frameElevationDeg"), -0.6),
               "image pitch frame angle") ||
        !check(closeTo(imageBatch.frames.first().canonicalNumber(
                           "measurement.frameAzimuthDeg"), 0.8),
               "image yaw frame angle") ||
        !check(imageBatch.frames.first().canonicalBoolean(
                   "status.selfCheck.servo"),
               "image self-check bit field")) {
        return 13;
    }

    ProtocolCommandEncoder imageEncoder;
    imageEncoder.setProfile(imageProfile);
    ProtocolCommandRequest imageRequest;
    imageRequest.specName = QStringLiteral("image.control");
    imageRequest.values.insert(QStringLiteral("aFrameSequence"), 0x1234);
    imageRequest.values.insert(QStringLiteral("seekerCtrlWord"), 4);
    imageRequest.values.insert(QStringLiteral("opticalParamCtrl"), 3);
    imageRequest.values.insert(QStringLiteral("templateIndex"), 1);
    imageRequest.values.insert(QStringLiteral("targetBackgroundType"), 0x2A);
    imageRequest.values.insert(QStringLiteral("missileTargetDistance"), 1234);
    imageRequest.values.insert(QStringLiteral("missileSpeed"), 12.3);
    imageRequest.values.insert(QStringLiteral("bodyPitchAngle"), -1.5);
    imageRequest.values.insert(QStringLiteral("bodyYawAngle"), 2.5);
    imageRequest.values.insert(QStringLiteral("pitchGimbalPreset"), -3.0);
    imageRequest.values.insert(QStringLiteral("irIntegrationTime"), 2.0);
    imageRequest.values.insert(QStringLiteral("correctedPitchPos"), 300);
    imageRequest.values.insert(QStringLiteral("correctedYawPos"), 400);
    imageRequest.values.insert(QStringLiteral("searchPitchRate"), 1.25);
    imageRequest.values.insert(QStringLiteral("searchYawRate"), 2.54);
    imageRequest.values.insert(QStringLiteral("gateSize"), 0xAA);
    imageRequest.values.insert(QStringLiteral("captureRefImgCmd"), 0x55);
    imageRequest.values.insert(QStringLiteral("targetAltitude"), 1000);
    imageRequest.values.insert(QStringLiteral("focalLength"), 50);
    imageRequest.values.insert(QStringLiteral("satelliteMapScale"), 1.0);
    imageRequest.values.insert(QStringLiteral("aircraftAltitude"), 2000);
    imageRequest.values.insert(QStringLiteral("pixelSize"), 5);

    QByteArray imageEncoded;
    if (!check(imageEncoder.encode(imageRequest, &imageEncoded, &error),
               "encode image command")) {
        std::fprintf(stderr, "image encode error: %s\n",
                     error.toLocal8Bit().constData());
        return 14;
    }
    const QByteArray imageExpected = QByteArray::fromHex(
                "77AADC34120403012AD2047B006AFFFA00000000000000000000000000000000"
                "000000000024FA00001400000000000000002C0190017D00FE0000AA55000000"
                "0000000000000000000000000000000000000000000000000000000000000000"
                "0000000000000000000000000000000000000000000000000000000000000000"
                "00000000000000000000000000000000000000000000000000E8030000000000"
                "0032000000000000000000000000000000000000000000000000000000000000"
                "0000000000000000000A0000000000000000000000000000000000D00705545F");
    if (!check(imageEncoded == imageExpected,
               "image command matches legacy bytes")) {
        std::fprintf(stderr, "image encoded: %s\nimage expected: %s\n",
                     imageEncoded.toHex().constData(),
                     imageExpected.toHex().constData());
        return 15;
    }

    ProtocolProfile invalidProfile;
    if (!check(!ProtocolProfile::fromJson(
                   "{\"schemaVersion\":1,\"id\":\"bad\",\"source\":\"laser\","
                   "\"frames\":[{\"name\":\"x\",\"header\":\"AA\",\"length\":4,"
                   "\"fields\":[{\"name\":\"v\",\"offset\":3,"
                   "\"type\":\"uint16\"}]}]}",
                   &invalidProfile, &error),
               "out-of-range field is rejected") ||
        !check(!invalidProfile.isValid(), "invalid profile remains invalid")) {
        return 16;
    }

    std::printf("protocol_selftest: PASS\n");
    return 0;
}
