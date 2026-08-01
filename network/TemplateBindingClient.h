#ifndef TEMPLATEBINDINGCLIENT_H
#define TEMPLATEBINDINGCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

// ═══════════════════════════════════════════════════════
// 图片条目结构：存储每张图片的路径和参数
// ═══════════════════════════════════════════════════════
struct ImageEntry {
    QString filePath;           // 图片文件路径
    bool    isSatellite;        // true=卫星图, false=非卫星图
    int     templateId;         // 模板编号
    int     imageId;            // 图片编号

    // ── 通用参数（卫星图和非卫星图共用）──
    int     imageWidth;         // 图像宽度 (像素)
    int     imageHeight;        // 图像高度 (像素)
    int     targetPosX;         // 目标横向坐标 (像素)
    int     targetPosY;         // 目标纵向坐标 (像素)
    int     targetPixelsX;      // 目标横向像素数
    int     targetPixelsY;      // 目标纵向像素数

    // ── 卫星图专用参数（4项）──
    double  scale;              // 比例尺: 一个像素对应的实际尺寸 (米)

    // ── 非卫星图专用参数（9项）──
    int     templateMode;       // 模板图模式: 0=电视, 1=红外
    double  distance;           // 拍摄距离 (米)
    double  azimuth;            // 拍摄方位角 (度, 东北天: 正北0°, 正东90°, 正南180°, 正西270°)
    double  pitchAngle;         // 拍摄俯仰角 (度, 东北天坐标系)
    double  focalLength;        // 相机焦距 (毫米)
    double  pixelSize;          // 像元尺寸 (微米)
};

// ═══════════════════════════════════════════════════════
// QML 交互数据类 —— 主线程，QML 直接绑定
// ═══════════════════════════════════════════════════════
class TemplateBindingData : public QObject
{
    Q_OBJECT

    // ── 连接状态 ──
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

    // ── 图片列表 ──
    Q_PROPERTY(int currentImageIndex READ currentImageIndex WRITE setCurrentImageIndex NOTIFY currentImageIndexChanged)
    Q_PROPERTY(int imageCount READ imageCount NOTIFY imageCountChanged)
    Q_PROPERTY(QString currentImagePath READ currentImagePath NOTIFY currentImagePathChanged)

    // ── 当前图片信息配置 ──
    Q_PROPERTY(bool isSatellite READ isSatellite WRITE setIsSatellite NOTIFY isSatelliteChanged)
    Q_PROPERTY(int templateId READ templateId WRITE setTemplateId NOTIFY templateIdChanged)
    Q_PROPERTY(int imageId READ imageId WRITE setImageId NOTIFY imageIdChanged)

    // ── 通用参数 ──
    Q_PROPERTY(int imageWidth READ imageWidth WRITE setImageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(int imageHeight READ imageHeight WRITE setImageHeight NOTIFY imageHeightChanged)
    Q_PROPERTY(int targetPosX READ targetPosX WRITE setTargetPosX NOTIFY targetPosXChanged)
    Q_PROPERTY(int targetPosY READ targetPosY WRITE setTargetPosY NOTIFY targetPosYChanged)
    Q_PROPERTY(int targetPixelsX READ targetPixelsX WRITE setTargetPixelsX NOTIFY targetPixelsXChanged)
    Q_PROPERTY(int targetPixelsY READ targetPixelsY WRITE setTargetPixelsY NOTIFY targetPixelsYChanged)

    // ── 卫星图专用参数 ──
    Q_PROPERTY(double scale READ scale WRITE setScale NOTIFY scaleChanged)

    // ── 非卫星图专用参数 ──
    Q_PROPERTY(int templateMode READ templateMode WRITE setTemplateMode NOTIFY templateModeChanged)
    Q_PROPERTY(double distance READ distance WRITE setDistance NOTIFY distanceChanged)
    Q_PROPERTY(double azimuth READ azimuth WRITE setAzimuth NOTIFY azimuthChanged)
    Q_PROPERTY(double pitchAngle READ pitchAngle WRITE setPitchAngle NOTIFY pitchAngleChanged)
    Q_PROPERTY(double focalLength READ focalLength WRITE setFocalLength NOTIFY focalLengthChanged)
    Q_PROPERTY(double pixelSize READ pixelSize WRITE setPixelSize NOTIFY pixelSizeChanged)

    // ── TXT 生成状态 ──
    Q_PROPERTY(QString txtFileName READ txtFileName NOTIFY txtFileNameChanged)
    Q_PROPERTY(bool txtGenerated READ txtGenerated NOTIFY txtGeneratedChanged)

    // ── 发送状态 ──
    Q_PROPERTY(double sendProgress READ sendProgress NOTIFY sendProgressChanged)
    Q_PROPERTY(bool imageSent READ imageSent NOTIFY imageSentChanged)
    Q_PROPERTY(bool txtSent READ txtSent NOTIFY txtSentChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit TemplateBindingData(QObject *parent = nullptr);

    // ── 连接状态 ──
    QString host() const { return m_host; }
    int port() const { return m_port; }
    bool connected() const { return m_connected; }

    // ── 图片列表 ──
    int currentImageIndex() const { return m_currentImageIndex; }
    int imageCount() const { return m_imageEntries.size(); }
    QString currentImagePath() const;

    // ── 当前图片信息 ──
    bool isSatellite() const;
    int templateId() const;
    int imageId() const;

    // ── 通用参数 ──
    int imageWidth() const;
    int imageHeight() const;
    int targetPosX() const;
    int targetPosY() const;
    int targetPixelsX() const;
    int targetPixelsY() const;

    // ── 卫星图参数 ──
    double scale() const;

    // ── 非卫星图参数 ──
    int templateMode() const;
    double distance() const;
    double azimuth() const;
    double pitchAngle() const;
    double focalLength() const;
    double pixelSize() const;

    // ── TXT ──
    QString txtFileName() const { return m_txtFileName; }
    bool txtGenerated() const { return m_txtGenerated; }

    // ── 发送状态 ──
    double sendProgress() const { return m_sendProgress; }
    bool imageSent() const { return m_imageSent; }
    bool txtSent() const { return m_txtSent; }
    QString statusMessage() const { return m_statusMessage; }

    // ── Setters ──
    void setHost(const QString &host);
    void setPort(int port);
    void setCurrentImageIndex(int index);
    void setIsSatellite(bool sat);
    void setTemplateId(int id);
    void setImageId(int id);

    void setImageWidth(int val);
    void setImageHeight(int val);
    void setTargetPosX(int val);
    void setTargetPosY(int val);
    void setTargetPixelsX(int val);
    void setTargetPixelsY(int val);

    void setScale(double val);

    void setTemplateMode(int mode);
    void setDistance(double val);
    void setAzimuth(double val);
    void setPitchAngle(double val);
    void setFocalLength(double val);
    void setPixelSize(double val);

    // ── QML 可调用方法 ──
    Q_INVOKABLE void addImage(const QString &filePath);
    Q_INVOKABLE void removeImage(int index);
    Q_INVOKABLE void nextImage();
    Q_INVOKABLE void prevImage();
    Q_INVOKABLE void generateTxt();

    // 获取当前图片条目（供 Worker 使用）
    const ImageEntry* currentEntry() const;
    QJsonObject generateTxtJson() const;

signals:
    void hostChanged();
    void portChanged();
    void connectedChanged();

    void currentImageIndexChanged();
    void imageCountChanged();
    void currentImagePathChanged();

    void isSatelliteChanged();
    void templateIdChanged();
    void imageIdChanged();

    void imageWidthChanged();
    void imageHeightChanged();
    void targetPosXChanged();
    void targetPosYChanged();
    void targetPixelsXChanged();
    void targetPixelsYChanged();

    void scaleChanged();

    void templateModeChanged();
    void distanceChanged();
    void azimuthChanged();
    void pitchAngleChanged();
    void focalLengthChanged();
    void pixelSizeChanged();

    void txtFileNameChanged();
    void txtGeneratedChanged();

    void sendProgressChanged();
    void imageSentChanged();
    void txtSentChanged();
    void statusMessageChanged();

    // ── 请求信号（→ 排队到网络线程）──
    void requestConnect(const QString &host, int port);
    void requestDisconnect();
    void requestSendImages();
    void requestSendTxt(const QByteArray &txtData);

public slots:
    void setConnected(bool conn);
    void setTxtGenerated(bool gen);
    void setSendProgress(double progress);
    void setImageSent(bool sent);
    void setTxtSent(bool sent);
    void setStatusMessage(const QString &msg);

private:
    QString m_host;
    int m_port = 0;
    bool m_connected = false;

    QList<ImageEntry> m_imageEntries;
    int m_currentImageIndex = -1;

    QString m_txtFileName;
    bool m_txtGenerated = false;

    double m_sendProgress = 0.0;
    bool m_imageSent = false;
    bool m_txtSent = false;
    QString m_statusMessage;
};

// ═══════════════════════════════════════════════════════
// TCP 网络传输 Worker —— 可以在子线程运行
// ═══════════════════════════════════════════════════════
class TemplateBindingWorker : public QObject
{
    Q_OBJECT
public:
    explicit TemplateBindingWorker(TemplateBindingData *data, QObject *parent = nullptr);
    ~TemplateBindingWorker() override;

public slots:
    void onConnect(const QString &host, int port);
    void onDisconnect();
    void onSendImages();
    void onSendTxt(const QByteArray &txtData);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    void sendNextImage();
    QByteArray buildImagePacket(const QString &filePath, const ImageEntry &entry);

    QTcpSocket *m_socket;
    TemplateBindingData *m_data;
    int m_currentSendIndex;
    qint64 m_totalBytesSent;
    qint64 m_totalBytesToSend;
};

#endif // TEMPLATEBINDINGCLIENT_H
