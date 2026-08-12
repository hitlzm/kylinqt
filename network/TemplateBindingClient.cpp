#include "TemplateBindingClient.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDataStream>
#include <QDateTime>
#include <QImage>
#include <QBuffer>

namespace {

// JSON 字符串转义（与 QJsonDocument 输出一致）
QByteArray jsonEscape(const QString &s)
{
    QByteArray out;
    out += '"';
    for (const QChar ch : s) {
        const ushort u = ch.unicode();
        switch (u) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\b': out += "\\b";  break;
        case '\f': out += "\\f";  break;
        case '\n': out += "\\n";  break;
        case '\r': out += "\\r";  break;
        case '\t': out += "\\t";  break;
        default:
            if (u < 0x20)
                out += QString("\\u%1").arg(u, 4, 16, QChar('0')).toUtf8();
            else
                out += QString(ch).toUtf8();
        }
    }
    out += '"';
    return out;
}

// 双精度数值序列化（与 Qt5 QJsonDocument 一致：'g' 格式、15 位有效数字）
QByteArray jsonNumber(double v)
{
    return QString::number(v, 'g', 15).toUtf8();
}

} // namespace

// ═══════════════════════════════════════════════════════
// TemplateBindingData 实现
// ═══════════════════════════════════════════════════════

TemplateBindingData::TemplateBindingData(QObject *parent)
    : QObject(parent)
{
}

QString TemplateBindingData::currentImagePath() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].filePath;
    return QString();
}

bool TemplateBindingData::isSatellite() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].isSatellite;
    return false;
}

int TemplateBindingData::templateId() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].templateId;
    return 0;
}

int TemplateBindingData::imageId() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].imageId;
    return 0;
}

// ── 通用参数 getters ──

int TemplateBindingData::imageWidth() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].imageWidth;
    return 0;
}

int TemplateBindingData::imageHeight() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].imageHeight;
    return 0;
}

int TemplateBindingData::targetPosX() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].targetPosX;
    return 0;
}

int TemplateBindingData::targetPosY() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].targetPosY;
    return 0;
}

int TemplateBindingData::targetPixelsX() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].targetPixelsX;
    return 0;
}

int TemplateBindingData::targetPixelsY() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].targetPixelsY;
    return 0;
}

// ── 卫星图参数 getters ──

double TemplateBindingData::scale() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].scale;
    return 0.0;
}

// ── 非卫星图参数 getters ──

int TemplateBindingData::templateMode() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].templateMode;
    return 0;
}

double TemplateBindingData::distance() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].distance;
    return 0.0;
}

double TemplateBindingData::azimuth() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].azimuth;
    return 0.0;
}

double TemplateBindingData::pitchAngle() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].pitchAngle;
    return 0.0;
}

double TemplateBindingData::focalLength() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].focalLength;
    return 0.0;
}

double TemplateBindingData::pixelSize() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return m_imageEntries[m_currentImageIndex].pixelSize;
    return 0.0;
}

const ImageEntry* TemplateBindingData::currentEntry() const
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size())
        return &m_imageEntries[m_currentImageIndex];
    return nullptr;
}

// ── Setters ──

void TemplateBindingData::setHost(const QString &host)
{
    if (m_host != host) { m_host = host; emit hostChanged(); }
}

void TemplateBindingData::setPort(int port)
{
    if (m_port != port) { m_port = port; emit portChanged(); }
}

void TemplateBindingData::setCurrentImageIndex(int index)
{
    if (index < 0) index = 0;
    if (index >= m_imageEntries.size()) index = m_imageEntries.size() - 1;
    m_currentImageIndex = index;
    emit currentImageIndexChanged();
    emit currentImagePathChanged();
    // 刷新所有参数绑定
    emit isSatelliteChanged();
    emit templateIdChanged();
    emit imageIdChanged();
    emit imageWidthChanged();
    emit imageHeightChanged();
    emit targetPosXChanged();
    emit targetPosYChanged();
    emit targetPixelsXChanged();
    emit targetPixelsYChanged();
    emit scaleChanged();
    emit templateModeChanged();
    emit distanceChanged();
    emit azimuthChanged();
    emit pitchAngleChanged();
    emit focalLengthChanged();
    emit pixelSizeChanged();
}

void TemplateBindingData::setIsSatellite(bool sat)
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size()) {
        if (m_imageEntries[m_currentImageIndex].isSatellite != sat) {
            m_imageEntries[m_currentImageIndex].isSatellite = sat;
            emit isSatelliteChanged();
        }
    }
}

void TemplateBindingData::setTemplateId(int id)
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size()) {
        if (m_imageEntries[m_currentImageIndex].templateId != id) {
            m_imageEntries[m_currentImageIndex].templateId = id;
            emit templateIdChanged();
        }
    }
}

void TemplateBindingData::setImageId(int id)
{
    if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size()) {
        if (m_imageEntries[m_currentImageIndex].imageId != id) {
            m_imageEntries[m_currentImageIndex].imageId = id;
            emit imageIdChanged();
        }
    }
}

// ── 通用参数 setters ──

#define ENTRY_SETTER(Type, FuncName, FieldName, Signal) \
    void TemplateBindingData::set##FuncName(Type val) { \
        if (m_currentImageIndex >= 0 && m_currentImageIndex < m_imageEntries.size()) { \
            if (m_imageEntries[m_currentImageIndex].FieldName != val) { \
                m_imageEntries[m_currentImageIndex].FieldName = val; \
                emit Signal(); \
            } \
        } \
    }

ENTRY_SETTER(int, ImageWidth,  imageWidth,  imageWidthChanged)
ENTRY_SETTER(int, ImageHeight, imageHeight, imageHeightChanged)
ENTRY_SETTER(int, TargetPosX,  targetPosX,  targetPosXChanged)
ENTRY_SETTER(int, TargetPosY,  targetPosY,  targetPosYChanged)
ENTRY_SETTER(int, TargetPixelsX, targetPixelsX, targetPixelsXChanged)
ENTRY_SETTER(int, TargetPixelsY, targetPixelsY, targetPixelsYChanged)

// ── 卫星图参数 setters ──

ENTRY_SETTER(double, Scale, scale, scaleChanged)

// ── 非卫星图参数 setters ──

ENTRY_SETTER(int,    TemplateMode, templateMode, templateModeChanged)
ENTRY_SETTER(double, Distance,     distance,     distanceChanged)
ENTRY_SETTER(double, Azimuth,      azimuth,      azimuthChanged)
ENTRY_SETTER(double, PitchAngle,   pitchAngle,   pitchAngleChanged)
ENTRY_SETTER(double, FocalLength,  focalLength,  focalLengthChanged)
ENTRY_SETTER(double, PixelSize,    pixelSize,    pixelSizeChanged)

#undef ENTRY_SETTER

// ── QML 可调用方法 ──

void TemplateBindingData::addImage(const QString &filePath)
{
    ImageEntry entry;
    entry.filePath      = filePath;
    entry.isSatellite   = false;
    entry.templateId    = m_imageEntries.size() + 1;
    entry.imageId       = m_imageEntries.size() + 1;
    // 通用参数默认值
    entry.imageWidth    = 0;
    entry.imageHeight   = 0;
    entry.targetPosX    = 0;
    entry.targetPosY    = 0;
    entry.targetPixelsX = 0;
    entry.targetPixelsY = 0;
    // 卫星图默认值
    entry.scale         = 0.0;
    // 非卫星图默认值
    entry.templateMode  = 0;
    entry.distance      = 0.0;
    entry.azimuth       = 0.0;
    entry.pitchAngle    = 0.0;
    entry.focalLength   = 0.0;
    entry.pixelSize     = 0.0;

    m_imageEntries.append(entry);
    emit imageCountChanged();

    if (m_currentImageIndex < 0)
        setCurrentImageIndex(0);
}

void TemplateBindingData::removeImage(int index)
{
    if (index < 0 || index >= m_imageEntries.size())
        return;

    m_imageEntries.removeAt(index);
    emit imageCountChanged();

    if (m_imageEntries.isEmpty()) {
        m_currentImageIndex = -1;
        emit currentImagePathChanged();
    } else if (m_currentImageIndex >= m_imageEntries.size()) {
        setCurrentImageIndex(m_imageEntries.size() - 1);
    } else {
        // 无论索引是否变化，都刷新（图已切到下一张）
        setCurrentImageIndex(m_currentImageIndex);
    }
}

void TemplateBindingData::nextImage()
{
    if (m_imageEntries.isEmpty()) return;
    setCurrentImageIndex(m_currentImageIndex + 1);
}

void TemplateBindingData::prevImage()
{
    if (m_imageEntries.isEmpty()) return;
    setCurrentImageIndex(m_currentImageIndex - 1);
}

void TemplateBindingData::generateTxt()
{
    QByteArray txtData = generateTxtData();

    QString fileName = QString("template%1.txt")
        .arg(m_imageEntries.size() > 0 ? m_imageEntries[0].templateId : 0, 3, 10, QChar('0'));

    QString filePath = QFileInfo(m_imageEntries.size() > 0
        ? m_imageEntries[0].filePath : QString()).absolutePath() + "/" + fileName;

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(txtData);
        file.close();
        m_txtFileName = fileName;
        m_txtGenerated = true;
    } else {
        m_txtGenerated = false;
    }

    emit txtFileNameChanged();
    emit txtGeneratedChanged();
}

QByteArray TemplateBindingData::generateTxtData() const
{
    QByteArray out;
    out += "{\n";
    out += "    \"templateType\": \"templateBinding\",\n";
    out += "    \"imageCount\": " + QByteArray::number(m_imageEntries.size()) + ",\n";
    out += "    \"images\": [\n";

    for (int i = 0; i < m_imageEntries.size(); ++i) {
        const ImageEntry &entry = m_imageEntries.at(i);
        out += "        {\n";

        // 公共字段：文件名 → 是否卫星图 → 模板ID（即装订界面的图片序号）
        out += "            \"filePath\": " + jsonEscape(QFileInfo(entry.filePath).fileName()) + ",\n";
        out += "            \"isSatellite\": " + QByteArray(entry.isSatellite ? "true" : "false") + ",\n";
        out += "            \"templateId\": " + QByteArray::number(entry.templateId) + ",\n";

        if (entry.isSatellite) {
            // 卫星图：比例尺放在图像宽高之前
            out += "            \"scale\": " + jsonNumber(entry.scale) + ",\n";
        } else {
            // 非卫星图：模板图模式 → 距离 → 方位角 → 俯仰角 → 焦距 → 像元尺寸
            out += "            \"templateMode\": " + QByteArray::number(entry.templateMode) + ",\n";
            out += "            \"distance\": "     + jsonNumber(entry.distance) + ",\n";
            out += "            \"azimuth\": "      + jsonNumber(entry.azimuth) + ",\n";
            out += "            \"pitchAngle\": "   + jsonNumber(entry.pitchAngle) + ",\n";
            out += "            \"focalLength\": "  + jsonNumber(entry.focalLength) + ",\n";
            out += "            \"pixelSize\": "    + jsonNumber(entry.pixelSize) + ",\n";
        }

        // 图像分辨率 → 目标位置 → 目标像素数
        out += "            \"imageWidth\": "    + QByteArray::number(entry.imageWidth) + ",\n";
        out += "            \"imageHeight\": "   + QByteArray::number(entry.imageHeight) + ",\n";
        out += "            \"targetPosX\": "    + QByteArray::number(entry.targetPosX) + ",\n";
        out += "            \"targetPosY\": "    + QByteArray::number(entry.targetPosY) + ",\n";
        out += "            \"targetPixelsX\": " + QByteArray::number(entry.targetPixelsX) + ",\n";
        out += "            \"targetPixelsY\": " + QByteArray::number(entry.targetPixelsY) + "\n";

        out += "        }";
        if (i < m_imageEntries.size() - 1)
            out += ",\n";
        else
            out += "\n";
    }

    out += "    ]\n";
    out += "}\n";
    return out;
}

// ── Slots ──

void TemplateBindingData::setConnected(bool conn)
{
    if (m_connected != conn) { m_connected = conn; emit connectedChanged(); }
}

void TemplateBindingData::setTxtGenerated(bool gen)
{
    if (m_txtGenerated != gen) { m_txtGenerated = gen; emit txtGeneratedChanged(); }
}

void TemplateBindingData::setSendProgress(double progress)
{
    if (m_sendProgress != progress) { m_sendProgress = progress; emit sendProgressChanged(); }
}

void TemplateBindingData::setImageSent(bool sent)
{
    if (m_imageSent != sent) { m_imageSent = sent; emit imageSentChanged(); }
}

void TemplateBindingData::setTxtSent(bool sent)
{
    if (m_txtSent != sent) { m_txtSent = sent; emit txtSentChanged(); }
}

void TemplateBindingData::setStatusMessage(const QString &msg)
{
    if (m_statusMessage != msg) { m_statusMessage = msg; emit statusMessageChanged(); }
}

void TemplateBindingData::provideTxtSnapshot()
{
    // Runs on the GUI thread via QueuedConnection, so generateTxtData()
    // never iterates m_imageEntries concurrently with GUI-thread mutations.
    emit txtSnapshotReady(generateTxtData());
}

void TemplateBindingData::provideImageSnapshot()
{
    // 主线程提供当前图片快照（完整路径 + 模板ID），网络线程据此发送图片报文
    emit imageSnapshotReady(currentImagePath(), templateId());
}


// ═══════════════════════════════════════════════════════
// TemplateBindingWorker 实现
// ═══════════════════════════════════════════════════════

TemplateBindingWorker::TemplateBindingWorker(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_currentSendIndex(0)
    , m_totalBytesSent(0)
    , m_totalBytesToSend(0)
{
    connect(m_socket, &QTcpSocket::connected,
            this, &TemplateBindingWorker::onSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected,
            this, &TemplateBindingWorker::onSocketDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &TemplateBindingWorker::onSocketError);
}

TemplateBindingWorker::~TemplateBindingWorker()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_socket->disconnectFromHost();
}

void TemplateBindingWorker::onConnect(const QString &host, int port)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_socket->disconnectFromHost();

    emit statusMessageChanged("正在连接 " + host + ":" + QString::number(port) + "...");
    m_socket->connectToHost(host, static_cast<quint16>(port));
}

void TemplateBindingWorker::onDisconnect()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_socket->disconnectFromHost();
}

void TemplateBindingWorker::onSocketConnected()
{
    emit connectedStatusChanged(true);
    emit statusMessageChanged("已连接到远程主机");
}

void TemplateBindingWorker::onSocketDisconnected()
{
    emit connectedStatusChanged(false);
    emit statusMessageChanged("已断开连接");
}

void TemplateBindingWorker::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    emit connectedStatusChanged(false);
    emit statusMessageChanged("连接错误: " + m_socket->errorString());
}

// ── 发送图片 ──

void TemplateBindingWorker::onSendImages()
{
    m_currentSendIndex = 0;
    m_totalBytesSent = 0;
    m_totalBytesToSend = 0;
    emit imageSentStatusChanged(false);
    emit sendProgressChanged(0.0);

    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        emit statusMessageChanged("未连接到远程主机，无法发送图片");
        return;
    }

    // 请求主线程提供当前图片快照，再真正发送图片报文
    emit requestImageSnapshot();
}

void TemplateBindingWorker::onImageSnapshotReady(const QString &filePath, int templateId)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
        return;

    QByteArray packet = buildImagePacket(filePath, templateId);
    if (packet.isEmpty()) {
        emit statusMessageChanged("图片加载失败，无法发送: " + filePath);
        return;
    }

    qint64 written = m_socket->write(packet);
    if (written > 0) {
        m_socket->flush();
        emit imageSentStatusChanged(true);
        emit sendProgressChanged(1.0);
        emit statusMessageChanged("图片发送成功");
    } else {
        emit statusMessageChanged("图片发送失败: " + m_socket->errorString());
    }
}

QByteArray TemplateBindingWorker::buildImagePacket(const QString &filePath, int templateId)
{
    QByteArray packet;

    // 使用 QImage 加载 → 统一转为 BMP 格式发送
    QImage image(filePath);
    if (image.isNull())
        return packet;

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "BMP");

    // meta JSON 只包含文件名与模板ID
    QJsonObject meta;
    meta["filePath"]   = QFileInfo(filePath).fileName();
    meta["templateId"] = templateId;

    QByteArray metaJson = QJsonDocument(meta).toJson(QJsonDocument::Compact);

    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << static_cast<quint8>(0x77) << static_cast<quint8>(0xCC);
    stream << static_cast<quint32>(metaJson.size());
    stream << static_cast<quint32>(imageData.size());
    packet.append(metaJson);
    packet.append(imageData);

    return packet;
}

// ── 发送 TXT ──

void TemplateBindingWorker::onSendTxt(const QByteArray &txtData)
{
    emit txtSentStatusChanged(false);
    emit statusMessageChanged("正在发送 TXT 文件...");

    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        emit statusMessageChanged("未连接到远程主机，无法发送 TXT");
        return;
    }

    if (txtData.isEmpty()) {
        // TXT JSON must be generated on the GUI thread; request a snapshot
        // instead of generating it from the network thread.
        emit requestTxtSnapshot();
        return;
    }

    sendTxtPacket(txtData);
}

void TemplateBindingWorker::onTxtSnapshotReady(const QByteArray &data)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
        return;

    sendTxtPacket(data);
}

void TemplateBindingWorker::sendTxtPacket(const QByteArray &data)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << static_cast<quint8>(0x77) << static_cast<quint8>(0xDD);
    stream << static_cast<quint32>(data.size());
    packet.append(data);

    qint64 written = m_socket->write(packet);
    if (written > 0) {
        m_socket->flush();
        emit txtSentStatusChanged(true);
        emit statusMessageChanged("TXT 文件发送成功");
    } else {
        emit statusMessageChanged("TXT 文件发送失败: " + m_socket->errorString());
    }
}
