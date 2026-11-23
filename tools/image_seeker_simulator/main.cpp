#include <QByteArray>
#include <QCoreApplication>
#include <QDateTime>
#include <QElapsedTimer>
#include <QSerialPort>
#include <QTimer>
#include <QtGlobal>

#include <cstdio>

namespace {

constexpr int FrameSize = 156;
constexpr int SendIntervalMs = 20;
constexpr int FrameLengthField = FrameSize - 4;

void putU16Le(QByteArray &frame, int offset, quint16 value)
{
    frame[offset] = static_cast<char>(value & 0xFF);
    frame[offset + 1] = static_cast<char>((value >> 8) & 0xFF);
}

void putU32Le(QByteArray &frame, int offset, quint32 value)
{
    for (int i = 0; i < 4; ++i)
        frame[offset + i] = static_cast<char>((value >> (8 * i)) & 0xFF);
}

void putU64Le(QByteArray &frame, int offset, quint64 value)
{
    for (int i = 0; i < 8; ++i)
        frame[offset + i] = static_cast<char>((value >> (8 * i)) & 0xFF);
}

quint16 crc16Ccitt(const QByteArray &data)
{
    quint16 crc = 0x0000;
    for (const char byte : data) {
        crc ^= static_cast<quint16>(static_cast<quint8>(byte)) << 8;
        for (int bit = 0; bit < 8; ++bit) {
            crc = (crc & 0x8000)
                    ? static_cast<quint16>((crc << 1) ^ 0x1021)
                    : static_cast<quint16>(crc << 1);
        }
    }
    return crc;
}

// Generates a continuous triangular sweep. Every adjacent frame changes by
// "step", including at the turn-around points.
qint16 triangleRaw(quint64 frame, int phase, int period, int amplitude, int step)
{
    const int halfPeriod = period / 2;
    const int position = static_cast<int>((frame + phase) % period);
    if (position < halfPeriod)
        return static_cast<qint16>(-amplitude + position * step);
    return static_cast<qint16>(amplitude - (position - halfPeriod) * step);
}

QByteArray buildStatusFrame(quint16 sequence,
                            qint16 pitchRaw,
                            qint16 azimuthRaw,
                            quint64 utcMs)
{
    QByteArray frame(FrameSize, '\0');

    frame[0] = static_cast<char>(0x77);
    frame[1] = static_cast<char>(0xAB);
    frame[2] = static_cast<char>(FrameLengthField);
    putU16Le(frame, 3, sequence);       // B-frame sequence.
    putU16Le(frame, 5, 0);              // No A-frame received.
    frame[7] = 0x00;                    // A-frame valid flag.
    frame[8] = 0x00;                    // Seeker control reply.
    frame[9] = 0x00;                    // Optical parameter reply.
    frame[11] = 0x00;                   // TV channel.
    frame[12] = static_cast<char>(0x7F); // Self-check bits 0..6 passed.

    putU16Le(frame, 19, static_cast<quint16>(pitchRaw));
    putU16Le(frame, 21, static_cast<quint16>(azimuthRaw));
    frame[18] = 0x03;                   // Tracking state.
    frame[32] = 0x33;                   // Target locked.
    frame[33] = 0x01;                   // Tracker active.
    putU64Le(frame, 34, utcMs);

    frame[73] = 0x01;                   // Platform self-check passed.
    putU32Le(frame, 91, sequence);      // Infrared frame number.
    putU16Le(frame, 95, 2500);          // 25.00 C.
    putU32Le(frame, 101, sequence);     // TV frame number.
    frame[111] = 50;                    // Infrared frame rate.
    frame[112] = 25;                    // TV frame rate.
    putU16Le(frame, 117, 64);           // Gate size.
    frame[131] = 1;                     // Software version 1.0.0.

    const QByteArray checksumData = frame.left(FrameSize - 2);
    putU16Le(frame, FrameSize - 2, crc16Ccitt(checksumData));
    return frame;
}

int selfTest()
{
    quint16 previousSequence = 0;
    qint16 previousPitch = 0;
    qint16 previousAzimuth = 0;

    for (int index = 0; index < 100; ++index) {
        const quint16 sequence = static_cast<quint16>(index + 1);
        const qint16 pitch = triangleRaw(index, 0, 480, 6000, 50);
        const qint16 azimuth = triangleRaw(index, 160, 640, 8000, 50);
        const QByteArray frame =
                buildStatusFrame(sequence, pitch, azimuth, 1000 + index * 20);
        const quint16 storedCrc =
                static_cast<quint8>(frame[154])
                | (static_cast<quint16>(static_cast<quint8>(frame[155])) << 8);

        if (frame.size() != FrameSize
                || static_cast<quint8>(frame[0]) != 0x77
                || static_cast<quint8>(frame[1]) != 0xAB
                || static_cast<quint8>(frame[2]) != FrameLengthField
                || storedCrc != crc16Ccitt(frame.left(FrameSize - 2))) {
            std::fprintf(stderr, "self-test failed at frame %d\n", index);
            return 1;
        }
        if (index > 0
                && (sequence == previousSequence
                    || pitch == previousPitch
                    || azimuth == previousAzimuth)) {
            std::fprintf(stderr, "values did not change at frame %d\n", index);
            return 1;
        }

        previousSequence = sequence;
        previousPitch = pitch;
        previousAzimuth = azimuth;
    }

    std::puts("self-test passed");
    return 0;
}

} // namespace

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    if (app.arguments().contains(QStringLiteral("--self-test")))
        return selfTest();

    QSerialPort port;
    port.setPortName(QStringLiteral("COM4"));
    port.setBaudRate(460800);
    port.setDataBits(QSerialPort::Data8);
    port.setParity(QSerialPort::NoParity);
    port.setStopBits(QSerialPort::OneStop);
    port.setFlowControl(QSerialPort::NoFlowControl);

    if (!port.open(QIODevice::ReadWrite)) {
        std::fprintf(stderr, "cannot open COM4 at 460800: %s\n",
                     qPrintable(port.errorString()));
        return 1;
    }

    std::puts("image seeker simulator: COM4, 460800, 8N1, 20 ms");
    std::puts("sending 156-byte B frames continuously; press Ctrl+C to stop");

    quint64 frameIndex = 0;
    const qint64 utcAnchorMs = QDateTime::currentMSecsSinceEpoch();
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);

    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (!port.isOpen()) {
            app.exit(1);
            return;
        }

        const quint16 sequence = static_cast<quint16>(frameIndex + 1);
        const qint16 pitchRaw = triangleRaw(frameIndex, 0, 480, 6000, 50);
        const qint16 azimuthRaw = triangleRaw(frameIndex, 160, 640, 8000, 50);
        const quint64 utcMs = static_cast<quint64>(
                    utcAnchorMs + static_cast<qint64>(frameIndex) * SendIntervalMs);
        const QByteArray frame =
                buildStatusFrame(sequence, pitchRaw, azimuthRaw, utcMs);

        if (port.write(frame) < 0) {
            std::fprintf(stderr, "serial write failed: %s\n",
                         qPrintable(port.errorString()));
            app.exit(1);
            return;
        }

        ++frameIndex;
        if (frameIndex % 50 == 0) {
            std::printf("seq=%u pitch=%.3f deg azimuth=%.3f deg\n",
                        sequence, pitchRaw * 0.002, azimuthRaw * 0.002);
            std::fflush(stdout);
        }
    });

    QObject::connect(&port, &QSerialPort::errorOccurred,
                     [&](QSerialPort::SerialPortError error) {
        if (error == QSerialPort::NoError)
            return;
        std::fprintf(stderr, "serial error: %s\n",
                     qPrintable(port.errorString()));
        if (error == QSerialPort::ResourceError
                || error == QSerialPort::DeviceNotFoundError
                || error == QSerialPort::PermissionError) {
            port.close();
            app.exit(1);
        }
    });

    timer.start(SendIntervalMs);
    return app.exec();
}
