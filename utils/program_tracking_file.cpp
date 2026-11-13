#include "program_tracking_file.h"

#include <QFile>
#include <QFileInfo>
#include <QLocale>
#include <QRegularExpression>
#include <QTextCodec>

#include <cmath>

namespace {

constexpr qint64 kMaxTrackingFileBytes = 16 * 1024 * 1024;

constexpr double kInnerMinDeg = -201;
constexpr double kInnerMaxDeg = 201;
constexpr double kMiddleMinDeg = -11;
constexpr double kMiddleMaxDeg = 90.0;
constexpr double kOuterMinDeg = -101;
constexpr double kOuterMaxDeg = 101;

QString decodeText(const QByteArray &raw)
{
    QByteArray data = raw;
    if (data.startsWith("\xEF\xBB\xBF"))
        data.remove(0, 3);

    const QString utf8Text = QString::fromUtf8(data);
    if (!utf8Text.contains(QChar::ReplacementCharacter))
        return utf8Text;

    QTextCodec *gbCodec = QTextCodec::codecForName("GB18030");
    if (!gbCodec)
        gbCodec = QTextCodec::codecForName("GBK");
    return gbCodec ? gbCodec->toUnicode(data) : utf8Text;
}

QString normalizedCell(const QString &cell)
{
    QString value = cell.trimmed();
    if (value.size() >= 2 && value.startsWith('"') && value.endsWith('"'))
        value = value.mid(1, value.size() - 2).trimmed();
    return value;
}

QStringList parseCsvRecord(const QString &line)
{
    QStringList fields;
    QString field;
    bool inQuotes = false;

    for (int i = 0; i < line.size(); ++i) {
        const QChar ch = line.at(i);
        if (ch == '"') {
            if (inQuotes && i + 1 < line.size() && line.at(i + 1) == '"') {
                field.append('"');
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && !inQuotes) {
            fields.append(normalizedCell(field));
            field.clear();
        } else {
            field.append(ch);
        }
    }
    fields.append(normalizedCell(field));
    return fields;
}

bool parseFiniteNumber(const QString &text, double *value)
{
    bool ok = false;
    const double parsed = QLocale::c().toDouble(text, &ok);
    if (!ok || !std::isfinite(parsed))
        return false;
    *value = parsed;
    return true;
}

QString rangeError(const QString &axisName, double value,
                   double minimum, double maximum)
{
    return QStringLiteral("%1角度 %2° 超出允许范围 %3°～%4°")
            .arg(axisName)
            .arg(value, 0, 'f', 4)
            .arg(minimum, 0, 'f', 1)
            .arg(maximum, 0, 'f', 1);
}

} // namespace

namespace ProgramTrackingFile {

Result loadCsv(const QString &filePath)
{
    Result result;

    const QString trimmedPath = filePath.trimmed();
    if (trimmedPath.isEmpty()) {
        result.error = QStringLiteral("未指定跟踪文件路径。");
        return result;
    }
    const QFileInfo fileInfo(trimmedPath);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        result.error = QStringLiteral("跟踪文件不存在：%1").arg(filePath);
        return result;
    }
    const QString canonicalPath = fileInfo.canonicalFilePath();
    if (canonicalPath.isEmpty()) {
        result.error = QStringLiteral("无法解析跟踪文件路径：%1").arg(filePath);
        return result;
    }
    if (fileInfo.suffix().compare(QStringLiteral("csv"), Qt::CaseInsensitive) != 0) {
        result.error = QStringLiteral(
                    "当前版本仅支持 CSV 跟踪文件。请在 Excel 中将文件"
                    "另存为“CSV（逗号分隔）(*.csv)”后重新导入。");
        return result;
    }
    if (fileInfo.size() < 0 || fileInfo.size() > kMaxTrackingFileBytes) {
        result.error = QStringLiteral(
                    "跟踪文件过大（上限 %1 MB）：%2")
                .arg(kMaxTrackingFileBytes / (1024 * 1024))
                .arg(fileInfo.fileName());
        return result;
    }

    QFile file(canonicalPath);
    if (!file.open(QIODevice::ReadOnly)) {
        result.error = QStringLiteral("无法读取跟踪文件：%1\n%2")
                .arg(fileInfo.fileName(), file.errorString());
        return result;
    }

    const QByteArray raw = file.read(kMaxTrackingFileBytes + 1);
    if (file.error() != QFileDevice::NoError) {
        result.error = QStringLiteral("读取跟踪文件失败：%1\n%2")
                .arg(fileInfo.fileName(), file.errorString());
        return result;
    }
    if (raw.size() > kMaxTrackingFileBytes) {
        result.error = QStringLiteral("跟踪文件过大（上限 %1 MB）：%2")
                .arg(kMaxTrackingFileBytes / (1024 * 1024))
                .arg(fileInfo.fileName());
        return result;
    }
    const QString text = decodeText(raw);
    const QStringList lines =
            text.split(QRegularExpression(QStringLiteral("\\r\\n|\\n|\\r")));

    int lineNumber = 0;
    while (lineNumber < lines.size() && lines.at(lineNumber).trimmed().isEmpty())
        ++lineNumber;
    if (lineNumber >= lines.size()) {
        result.error = QStringLiteral("跟踪文件为空。");
        return result;
    }

    const QStringList header = parseCsvRecord(lines.at(lineNumber));
    const QStringList expectedHeader = {
        QStringLiteral("序号"),
        QStringLiteral("内框角度"),
        QStringLiteral("中框角度"),
        QStringLiteral("外框角度")
    };
    if (header != expectedHeader) {
        result.error = QStringLiteral(
                    "跟踪文件表头格式不正确。\n"
                    "应为：序号,内框角度,中框角度,外框角度\n"
                    "实际：%1")
                .arg(header.join(QStringLiteral(",")));
        return result;
    }

    int expectedSequence = 1;
    for (++lineNumber; lineNumber < lines.size(); ++lineNumber) {
        const QString line = lines.at(lineNumber);
        if (line.trimmed().isEmpty())
            continue;

        const QStringList fields = parseCsvRecord(line);
        if (fields.size() != 4) {
            result.error = QStringLiteral(
                        "第 %1 行应有 4 列，实际为 %2 列。")
                    .arg(lineNumber + 1)
                    .arg(fields.size());
            return result;
        }

        double sequence = 0.0;
        if (!parseFiniteNumber(fields.at(0), &sequence)
                || std::abs(sequence - expectedSequence) > 1e-9) {
            result.error = QStringLiteral(
                        "第 %1 行序号无效，应连续填写 %2。")
                    .arg(lineNumber + 1)
                    .arg(expectedSequence);
            return result;
        }

        ProgramTrackingPoint point;
        if (!parseFiniteNumber(fields.at(1), &point.innerAngleDeg)
                || !parseFiniteNumber(fields.at(2), &point.middleAngleDeg)
                || !parseFiniteNumber(fields.at(3), &point.outerAngleDeg)) {
            result.error = QStringLiteral(
                        "第 %1 行角度不是有效数字。")
                    .arg(lineNumber + 1);
            return result;
        }

        if (point.innerAngleDeg < kInnerMinDeg
                || point.innerAngleDeg > kInnerMaxDeg) {
            result.error = QStringLiteral("第 %1 行：%2")
                    .arg(lineNumber + 1)
                    .arg(rangeError(QStringLiteral("内框"),
                                    point.innerAngleDeg,
                                    kInnerMinDeg, kInnerMaxDeg));
            return result;
        }
        if (point.middleAngleDeg < kMiddleMinDeg
                || point.middleAngleDeg > kMiddleMaxDeg) {
            result.error = QStringLiteral("第 %1 行：%2")
                    .arg(lineNumber + 1)
                    .arg(rangeError(QStringLiteral("中框"),
                                    point.middleAngleDeg,
                                    kMiddleMinDeg, kMiddleMaxDeg));
            return result;
        }
        if (point.outerAngleDeg < kOuterMinDeg
                || point.outerAngleDeg > kOuterMaxDeg) {
            result.error = QStringLiteral("第 %1 行：%2")
                    .arg(lineNumber + 1)
                    .arg(rangeError(QStringLiteral("外框"),
                                    point.outerAngleDeg,
                                    kOuterMinDeg, kOuterMaxDeg));
            return result;
        }

        result.points.append(point);
        ++expectedSequence;
    }

    if (result.points.isEmpty()) {
        result.error = QStringLiteral("跟踪文件没有有效的数据行。");
        return result;
    }

    result.ok = true;
    return result;
}

} // namespace ProgramTrackingFile
