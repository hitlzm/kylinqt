#ifndef VLCVIDEOITEM_H
#define VLCVIDEOITEM_H

#include <QtQuick/QQuickFramebufferObject>
#include <QImage>
#include <QMutex>
#include <QColor>

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_event_manager_t;
struct libvlc_event_t;

class VlcVideoRenderer;

//后期如果目标主机不支持OPENGL，则可能需要换回QQuickPaintedItem

class VlcVideoItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(float position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 length READ length NOTIFY lengthChanged)
    Q_PROPERTY(bool seekable READ isSeekable NOTIFY seekableChanged)

public:
    explicit VlcVideoItem(QQuickItem *parent = nullptr);
    ~VlcVideoItem() override;

    QString source() const;
    void setSource(const QString &url);

    bool isPlaying() const;
    int volume() const;
    Q_INVOKABLE void setVolume(int vol);
    float position() const;
    void setPosition(float pos);
    qint64 length() const;
    bool isSeekable() const;

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

    // ── 像素读取接口 ──────────────────────────────────────
    /// QML 调用，传入控件坐标，映射为视频帧坐标并发射 pixelRead 信号
    Q_INVOKABLE void requestPixelAt(int x, int y);

protected:
    /// 直接处理鼠标点击，绕过 QML MouseArea 在 FBO 上的事件传递问题
    void mousePressEvent(QMouseEvent *event) override;

public:
    // ── StreamProcessor 接口 ──────────────────────────────
    /// 获取最新解码帧的深拷贝（线程安全，供外部消费者如 StreamProcessor 调用）
    QImage grabFrame() const;

    /// 提交处理后的帧用于渲染显示（线程安全，供 StreamProcessor 回传）
    void submitProcessedFrame(const QImage &frame);

    // ---- QQuickFramebufferObject 接口 ----
    Renderer *createRenderer() const override;

signals:
    void sourceChanged();
    void playingChanged();
    void volumeChanged();
    void positionChanged();
    void lengthChanged();
    void seekableChanged();
    void stopped();
    void ended();
    void error(const QString &errorMsg);

    // ── 像素读取结果信号 ──────────────────────────────────
    void pixelRead(int frameX, int frameY);
    void errorReadingPixel(QString message);

    //通知图像导引头立刻发送偏差像素数据，以实现人工点选功能
    void reqDeviationToImg(int x ,int y);

private:
    friend class VlcVideoRenderer;

    void setupPlayer();
    void releasePlayer();
    void attachEvents();
    void detachEvents();

private:

    // ---- libvlc 视频帧回调 ----
    static void* lockCallback(void *opaque, void **planes);
    static void unlockCallback(void *opaque, void *picture, void *const *planes);
    static void displayCallback(void *opaque, void *picture);
    static unsigned setupFormatCallback(void **opaque, char *chroma,
                                        unsigned *width, unsigned *height,
                                        unsigned *pitches, unsigned *lines);

    // ---- libvlc 事件回调 ----
    friend void onLibVlcEvent(const libvlc_event_t *event, void *opaque);

    QString m_source;
    libvlc_instance_t *m_vlcInstance = nullptr;
    libvlc_media_t *m_media = nullptr;
    libvlc_media_player_t *m_player = nullptr;
    libvlc_event_manager_t *m_eventManager = nullptr;

    // 双缓冲：VLC 写 m_frameBuf[m_writeIdx]，消费者读 m_frameBuf[m_readyIdx]
    QImage m_frameBuf[2];
    int    m_writeIdx = 0;
    int    m_readyIdx = -1;             // -1 = 尚无就绪帧
    mutable QMutex m_frameMutex;
    unsigned m_width = 0;
    unsigned m_height = 0;
    volatile bool m_frameUpdated = false;

    // StreamProcessor 处理后回传的帧
    QImage m_processedFrame;
    bool   m_hasProcessedFrame = false;

    bool m_playing = false;
    int m_volume = 100;
    bool m_seekable = false;
};

#endif
