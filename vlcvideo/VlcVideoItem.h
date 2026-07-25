#ifndef VLCVIDEOITEM_H
#define VLCVIDEOITEM_H

#include <QtQuick/QQuickFramebufferObject>
#include <QImage>
#include <QMutex>
#include <QColor>

struct mpv_handle;
struct mpv_render_context;

class VlcVideoRenderer;

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

    // QML 组件完成布局后调用
    void componentComplete() override;

    // 诊断：确认 Qt 场景图是否调度了这个 item
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

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
    /// QML 调用，传入控件坐标，同步从帧缓冲读取像素颜色并发射 pixelRead 信号
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
    void pixelRead(int x, int y, QColor color);
    void errorReadingPixel(QString message);

private slots:
    // 在主线程处理 mpv 事件（invokeMethod 需要 slot 才能找到）
    void processMpvEvents();

    // 渲染上下文就绪后的回调（由 VlcVideoRenderer 通过 invokeMethod 触发）
    void onRenderContextReady();

private:
    friend class VlcVideoRenderer;

    void setupPlayer();
    void doSetupPlayer();
    void releasePlayer();
    void ensureMpvCreated();

    // ── mpv 回调 ─────────────────────────────────────────
    static void onMpvWakeup(void *ctx);

    QString m_source;

    // mpv 核心句柄：创建/命令/属性访问（线程安全）
    mpv_handle *m_mpv = nullptr;

    // mpv 渲染上下文：由 VlcVideoRenderer 在渲染线程创建和使用
    mpv_render_context *m_mpvCtx = nullptr;

    // 渲染上下文是否已创建（loadfile 必须等它为 true）
    bool m_renderCtxReady = false;
    int  m_setupRetryCount = 0;
    bool m_setupInProgress = false;

    // 双缓冲：渲染线程写 m_frameBuf[m_writeIdx]，消费者读 m_frameBuf[m_readyIdx]
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
    bool m_playClicked = false;
    QTimer *m_playTimer = nullptr;
    int m_volume = 100;
    bool m_seekable = false;
};

#endif
