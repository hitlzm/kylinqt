#include "utils/program_tracking_file.h"

#include <QCoreApplication>
#include <QFile>
#include <QTemporaryDir>
#include <QTextCodec>

#include <cmath>
#include <cstdio>

namespace {

bool writeFile(const QString &path, const QByteArray &data)
{
    QFile file(path);
    return file.open(QIODevice::WriteOnly) && file.write(data) == data.size();
}

bool check(bool condition, const char *message)
{
    if (!condition)
        std::fprintf(stderr, "FAIL: %s\n", message);
    return condition;
}

} // namespace

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTemporaryDir directory;
    if (!check(directory.isValid(), "temporary directory creation"))
        return 1;

    const QString validPath = directory.filePath(QStringLiteral("valid.csv"));
    if (!check(writeFile(validPath,
                         "序号,内框角度,中框角度,外框角度\r\n"
                         "1,1.0000,2.0000,3.0000\r\n"
                         "2,1.0010,2.0010,3.0010\r\n"),
                "write valid UTF-8 fixture")) {
        return 2;
    }
    const ProgramTrackingFile::Result valid =
            ProgramTrackingFile::loadCsv(validPath);
    if (!check(valid.ok, "valid UTF-8 file is accepted")
            || !check(valid.points.size() == 2, "valid point count")
            || !check(std::abs(valid.points.at(1).middleAngleDeg - 2.001) < 1e-9,
                      "middle angle parsed from third column")) {
        return 3;
    }

    const QString gbkPath = directory.filePath(QStringLiteral("gbk.csv"));
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    const QByteArray gbkData = gbk
            ? gbk->fromUnicode(QString::fromUtf8(
                  "序号,内框角度,中框角度,外框角度\n"
                  "1,-2.5,12.5,40.5\n"))
            : QByteArray();
    if (!check(gbk && writeFile(gbkPath, gbkData), "write GBK fixture"))
        return 4;
    const ProgramTrackingFile::Result gbkResult =
            ProgramTrackingFile::loadCsv(gbkPath);
    if (!check(gbkResult.ok, "GBK file is accepted")
            || !check(gbkResult.points.size() == 1, "GBK point count")) {
        return 5;
    }

    const QString wrongHeaderPath =
            directory.filePath(QStringLiteral("wrong_header.csv"));
    if (!check(writeFile(wrongHeaderPath,
                         "序号,内框角度,外框角度,外框角度\n"
                         "1,0,0,0\n"),
                "write invalid header fixture")) {
        return 6;
    }
    const ProgramTrackingFile::Result wrongHeader =
            ProgramTrackingFile::loadCsv(wrongHeaderPath);
    if (!check(!wrongHeader.ok, "duplicate outer header is rejected")
            || !check(wrongHeader.error.contains(QStringLiteral("表头格式不正确")),
                      "header rejection has useful message")) {
        return 7;
    }

    const QString outOfRangePath =
            directory.filePath(QStringLiteral("out_of_range.csv"));
    if (!check(writeFile(outOfRangePath,
                         "序号,内框角度,中框角度,外框角度\n"
                         "1,0,91,0\n"),
                "write out-of-range fixture")) {
        return 8;
    }
    const ProgramTrackingFile::Result outOfRange =
            ProgramTrackingFile::loadCsv(outOfRangePath);
    if (!check(!outOfRange.ok, "out-of-range angle is rejected")
            || !check(outOfRange.error.contains(QStringLiteral("中框")),
                      "range rejection identifies the axis")) {
        return 9;
    }

    const QString oversizedPath =
            directory.filePath(QStringLiteral("oversized.csv"));
    QFile oversizedFile(oversizedPath);
    if (!check(oversizedFile.open(QIODevice::WriteOnly),
               "create oversized fixture")) {
        return 10;
    }
    oversizedFile.write("序号,内框角度,中框角度,外框角度\n");
    const QByteArray filler(1024 * 1024, ' ');
    for (int i = 0; i < 17; ++i)
        oversizedFile.write(filler);
    oversizedFile.close();

    const ProgramTrackingFile::Result oversized =
            ProgramTrackingFile::loadCsv(oversizedPath);
    if (!check(!oversized.ok, "oversized tracking file is rejected")
            || !check(oversized.error.contains(QStringLiteral("过大")),
                      "oversized rejection has useful message")) {
        return 11;
    }

    if (argc >= 3 && QByteArray(argv[1]) == "--expect-invalid") {
        const ProgramTrackingFile::Result external =
                ProgramTrackingFile::loadCsv(QString::fromLocal8Bit(argv[2]));
        if (!check(!external.ok, "requested external file is rejected")
                || !check(external.error.contains(QStringLiteral("表头")),
                          "external file rejection is a header error")) {
            return 12;
        }
        std::printf("external format rejection: %s\n",
                    external.error.toLocal8Bit().constData());
    }

    std::printf("program_tracking_file_selftest: PASS\n");
    return 0;
}
