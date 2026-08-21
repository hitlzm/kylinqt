#ifndef VLCVIDEOITEM_H
#define VLCVIDEOITEM_H

#include <QtQuick/QQuickFramebufferObject>
#include <QImage>
#include <QMutex>
#include <QColor>
#include <QSize>
#include <atomic>

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
    Q_PROPERTY(qreal frameWidth READ frameWidth NOTIFY frameSizeChanged)
    Q_PROPERTY(qreal frameHeight READ frameHeight NOTIFY frameSizeChanged)
    Q_PROPERTY(bool cpuFrameConsumer READ cpuFrameConsumer WRITE setCpuFrameConsumer NOTIFY cpuFrameConsumerChanged)
    Q_PROPERTY(bool recording READ recording NOTIFY recordingChanged)

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

    // ── 原始码流录制（mpv record-file，零转码）──
    // filePath 例如 video143025.ts；停止时内部把 record-file 置空以收尾文件
    Q_INVOKABLE void startRecord(const QString &filePath);
    Q_INVOKABLE void stopRecord();
    bool recording() const { return m_recording; }

    // ── 像素读取接口 ──────────────────────────────────────
    /// QML 调用，传入控件坐标，映射为视频帧坐标并发射 reqDeviationToImg 信号
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

    /// 返回当前显示的 GL 纹理 ID（放大镜直接绑定，省 GPU→CPU→GPU 拷贝）
    unsigned int displayTextureId() const { return m_displayTexId; }

    qreal frameWidth() const { return m_width; }
    qreal frameHeight() const { return m_height; }

    // ── CPU 帧消费开关 ────────────────────────────────────
    /// 是否有 CPU 侧消费者（如 StreamProcessor 检测）需要读回像素。
    /// false（导引头纯显示）时渲染线程跳过 glReadPixels，
    /// 显示/放大镜直接绑离屏 FBO 纹理，点击走纯几何映射。
    bool cpuFrameConsumer() const { return m_cpuConsumerActive.load(); }
    Q_INVOKABLE void setCpuFrameConsumer(bool on);

    /// 视频固有显示尺寸（mpv video-params 的 dw/dh）。
    /// 供放大镜等"一帧多显"场景裁剪 mpv 离屏渲染时在帧内留下的黑边。
    QSize videoNativeSize() const { return QSize(m_videoDw, m_videoDh); }

    // ---- QQuickFramebufferObject 接口 ----
    Renderer *createRenderer() const override;

signals:
    void sourceChanged();
    void playingChanged();
    void volumeChanged();
    void positionChanged();
    void lengthChanged();
    void seekableChanged();
    void frameSizeChanged();
    void cpuFrameConsumerChanged();
    void recordingChanged();
    void stopped();
    void ended();
    void error(const QString &errorMsg);
    /// 媒体流/文件加载成功（mpv MPV_EVENT_FILE_LOADED）
    void fileLoaded();
    /// mpv 解析到真实视频参数（video-params）时发出，表示连接成功
    void videoReady();

    // ── 像素读取结果信号 ──────────────────────────────────
    /// 点击到视频画面内时发出，携带帧坐标，请求图像导引头发送偏差像素
    void reqDeviationToImg(int frameX, int frameY);
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

    // 视频固有显示尺寸（mpv video-params 的 dw/dh）。
    // 用于判断点击是否落在帧内真实视频画面上（剔除 mpv 渲染时留下的黑边）。
    int m_videoDw = 0;
    int m_videoDh = 0;
    bool m_videoReady = false;   // 本次连接是否已判定成功（每次 releasePlayer 复位）

    // StreamProcessor 处理后回传的帧
    QImage m_processedFrame;
    bool   m_hasProcessedFrame = false;

    unsigned int m_displayTexId = 0;

    // 是否有 CPU 侧消费者需要读回像素（渲染线程读 / GUI 线程写，须 atomic）
    std::atomic<bool> m_cpuConsumerActive{true};

    bool m_playing = false;
    bool m_playClicked = false;
    bool m_recording = false;
    bool m_needClearDisplay = false;
    QTimer *m_playTimer = nullptr;
    int m_volume = 100;
    bool m_seekable = false;
};

#endif
