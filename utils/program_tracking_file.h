#ifndef PROGRAM_TRACKING_FILE_H
#define PROGRAM_TRACKING_FILE_H

#include <QMetaType>
#include <QString>
#include <QVector>

// 程控文件中的一个跟踪点，三个角度均为转台绝对角，单位：度。
struct ProgramTrackingPoint
{
    double innerAngleDeg = 0.0;
    double middleAngleDeg = 0.0;
    double outerAngleDeg = 0.0;
};

Q_DECLARE_METATYPE(ProgramTrackingPoint)

namespace ProgramTrackingFile {

struct Result
{
    bool ok = false;
    QString error;
    QVector<ProgramTrackingPoint> points;
    int intervalMs = 20;
};

// 读取按“序号,内框角度,中框角度,外框角度”表头导出的 CSV 文件。
// 支持 UTF-8（含 BOM）与 GB18030/GBK 编码。
Result loadCsv(const QString &filePath);

} // namespace ProgramTrackingFile

#endif // PROGRAM_TRACKING_FILE_H
