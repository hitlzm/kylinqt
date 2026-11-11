#include "VlcVideoItem.h"
#include "utils/ccdlatencyprobe.h"
#include "utils/videolatencyprobe.h"
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QtMath>
#include <QMouseEvent>
#include <QUrl>
#include <cerrno>
#include <cstring>
#include <clocale>

// ══════════════════════════════════════════════════════════════════
// 图像头 UDP 解码模式（二选一）
//
// 当前使用软件解码：CPU 占用较高，但实测接收侧延迟约 59 ms。
// 需要改回硬件解码时：
//   1. 注释下面 KYLIN_GUIDE_HWDEC_MODE 的软件解码定义；
//   2. 取消下面硬件解码定义前的注释；
//   3. 重新完整编译。
//
// 可用硬件模式：auto-safe、auto-copy-safe、v4l2m2m（需平台支持）。
// 运行时仍可用 KYLIN_MPV_HWDEC 临时覆盖这里的默认值。
// ══════════════════════════════════════════════════════════════════
#define KYLIN_GUIDE_HWDEC_MODE "no"       // ← 当前：软件解码
// #define KYLIN_GUIDE_HWDEC_MODE "auto-safe"  // ← 启用硬件解码时打开

static QByteArray defaultHwdecForStream(const QString &source, bool ccdSource)
{
    if (source.startsWith(QStringLiteral("udp://"), Qt::CaseInsensitive)
            && !ccdSource) {
        return QByteArray(KYLIN_GUIDE_HWDEC_MODE);
    }
    return QByteArrayLiteral("auto-safe");
}

// ══════════════════════════════════════════════════════════════════
// GLSL Shader — 用于把一帧纹理绘制到 Qt Quick FBO 中
// ══════════════════════════════════════════════════════════════════
static const char *kVertexShader =
    "attribute vec2 aPosition;\n"
    "attribute vec2 aTexCoord;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
    "    vTexCoord = aTexCoord;\n"
    "}\n";

static const char *kFragmentShader =
    "varying vec2 vTexCoord;\n"
    "uniform sampler2D uTexture;\n"
    "void main() {\n"
    "    gl_FragColor = texture2D(uTexture, vTexCoord);\n"
    "}\n";

// ══════════════════════════════════════════════════════════════════
// mpv OpenGL 函数地址获取（给 mpv render context 用）
// ══════════════════════════════════════════════════════════════════
static void *mpvGetProcAddress(void * /*ctx*/, const char *name)
{
    QOpenGLContext *glCtx = QOpenGLContext::currentContext();
    if (!glCtx)
        return nullptr;
    return reinterpret_cast<void *>(glCtx->getProcAddress(QByteArray(name)));
}

static QString redactedStreamUrl(const QString &url)
{
    QUrl parsed(url);
    if (!parsed.userInfo().isEmpty())
        parsed.setUserInfo(QStringLiteral("***"));
    return parsed.toString();
}

static bool setMpvOptionChecked(mpv_handle *mpv, const char *name, const char *value)
{
    const int ret = mpv_set_option_string(mpv, name, value);
    if (ret < 0) {
        qWarning() << "[MpvVideo] mpv option rejected:" << name << "=" << value
                   << "error:" << mpv_error_string(ret);
        return false;
    }
    return true;
}

static void reportMpvPipelineDiagnostics(mpv_handle *mpv)
{
    static thread_local QElapsedTimer timer;
    static thread_local qint64 nextReportMs = 5000;
    if (!timer.isValid())
        timer.start();
    const qint64 nowMs = timer.elapsed();
    if (nowMs < nextReportMs)
        return;
    nextReportMs = nowMs + 5000;

    double demuxCacheSec = -1.0;
    double videoPts = -1.0;
    double timePos = -1.0;
    double videoBitrate = -1.0;
    qint64 frameNumber = -1;
    qint64 decoderDrops = -1;
    qint64 outputDrops = -1;
    qint64 mistimed = -1;
    qint64 voDelayed = -1;
    qint64 bufferingState = -1;
    int cacheIdle = -1;
    int untimed = -1;

    mpv_get_property(mpv, "demuxer-cache-duration", MPV_FORMAT_DOUBLE, &demuxCacheSec);
    mpv_get_property(mpv, "video-pts",             MPV_FORMAT_DOUBLE, &videoPts);
    mpv_get_property(mpv, "time-pos",              MPV_FORMAT_DOUBLE, &timePos);
    mpv_get_property(mpv, "video-bitrate",         MPV_FORMAT_DOUBLE, &videoBitrate);
    mpv_get_property(mpv, "estimated-frame-number", MPV_FORMAT_INT64, &frameNumber);
    mpv_get_property(mpv, "decoder-frame-drop-count", MPV_FORMAT_INT64, &decoderDrops);
    mpv_get_property(mpv, "frame-drop-count",      MPV_FORMAT_INT64, &outputDrops);
    mpv_get_property(mpv, "mistimed-frame-count",  MPV_FORMAT_INT64, &mistimed);
    mpv_get_property(mpv, "vo-delayed-frame-count", MPV_FORMAT_INT64, &voDelayed);
    mpv_get_property(mpv, "cache-buffering-state", MPV_FORMAT_INT64, &bufferingState);
    mpv_get_property(mpv, "demuxer-cache-idle",    MPV_FORMAT_FLAG, &cacheIdle);
    mpv_get_property(mpv, "untimed",               MPV_FORMAT_FLAG, &untimed);

    char *hwdecRaw = mpv_get_property_string(mpv, "hwdec-current");
    const QString hwdec = hwdecRaw
            ? QString::fromUtf8(hwdecRaw)
            : QStringLiteral("n/a");
    if (hwdecRaw)
        mpv_free(hwdecRaw);

    qDebug().nospace()
        << "[LatProbeDiag] demux-cache="
        << (demuxCacheSec >= 0.0
            ? QString::number(demuxCacheSec * 1000.0, 'f', 1) + QStringLiteral("ms")
            : QStringLiteral("n/a"))
        << "  cache-idle=" << cacheIdle
        << "  buffering=" << bufferingState
        << "  decoder-drop=" << decoderDrops
        << "  output-drop=" << outputDrops
        << "  mistimed=" << mistimed
        << "  vo-delayed=" << voDelayed
        << "  frame=" << frameNumber
        << "  video-pts=" << QString::number(videoPts, 'f', 3)
        << "  time-pos=" << QString::number(timePos, 'f', 3)
        << "  bitrate=" << (videoBitrate >= 0.0
            ? QString::number(videoBitrate / 1000000.0, 'f', 2) + QStringLiteral("Mbps")
            : QStringLiteral("n/a"))
        << "  untimed=" << untimed
        << "  hwdec=" << hwdec;
}

// ══════════════════════════════════════════════════════════════════
// VlcVideoRenderer — 运行在 Qt Quick 渲染线程，负责 OpenGL 绘制
// ══════════════════════════════════════════════════════════════════
class VlcVideoRenderer : public QQuickFramebufferObject::Renderer,
                         protected QOpenGLFunctions
{
public:
    VlcVideoRenderer(VlcVideoItem *item)
        : m_item(item) {}

    ~VlcVideoRenderer() override
    {
        if (!m_glInitialized) return;
        m_item->m_displayTexId = 0;   // 可能指向 m_texture 或 m_offscreenTex，统一复位
        if (m_texture)        glDeleteTextures(1, &m_texture);
        if (m_vbo)            glDeleteBuffers(1, &m_vbo);
        if (m_program)        glDeleteProgram(m_program);
        if (m_offscreenTex)   glDeleteTextures(1, &m_offscreenTex);
        if (m_offscreenFbo)   glDeleteFramebuffers(1, &m_offscreenFbo);
        // mpv_render_context 必须在渲染线程释放
        if (m_item->m_mpvCtx) {
            mpv_render_context_free(m_item->m_mpvCtx);
            m_item->m_mpvCtx = nullptr;
        }
    }

    void synchronize(QQuickFramebufferObject *item) override
    {
        Q_UNUSED(item);

        // ── 首次 sync：初始化 GL 资源 ──
        if (!m_glInitialized) {
            initializeOpenGLFunctions();
            initGL();
        }

        // ── mpv render context 需要 GL 和 mpv 两者都就绪才能创建 ──
        // （两者可能在不同时机就绪：GL 在首次窗口渲染，mpv 在用户点击"连接"后）
        if (!m_item->m_mpvCtx && m_item->m_mpv) {
            initMpvRenderContext();     // 成功后通知 VlcVideoItem::onRenderContextReady
        }

        // GUI 线程被阻塞，可以安全读 m_item 的属性（不含互斥锁）
        QMutexLocker lock(&m_item->m_frameMutex);

        // 切换视频源时清空渲染器中上一视频流的残留帧
        if (m_item->m_needClearDisplay) {
            m_frameCopy = QImage();
            m_textureDirty = false;
            m_item->m_needClearDisplay = false;
        }

        if (m_item->m_frameUpdated && m_item->m_readyIdx >= 0) {
            // 未点击播放前：消费帧但不显示（StreamProcessor 仍可通过 grabFrame 获取）
            if (!m_item->m_playClicked) {
                m_item->m_frameUpdated = false;
            } else {
                // 若有 StreamProcessor 处理后的帧则优先显示，否则显示原始解码帧
                const QImage &rawFrame = m_item->m_frameBuf[m_item->m_readyIdx];
                const QImage &src = m_item->m_hasProcessedFrame
                                    && !m_item->m_processedFrame.isNull()
                                    ? m_item->m_processedFrame
                                    : rawFrame;
                m_frameCopy = src.copy();
                m_videoSize = m_frameCopy.size();
                m_textureDirty = true;
                m_item->m_frameUpdated = false;
                // static int syncCount = 0;
                // if (syncCount++ < 3)
                //     qDebug() << "[MpvVideo] synchronize picked frame:" << m_videoSize;
            }
        }

        // 导引头模式（无 CPU 回读）：用离屏 FBO 尺寸兜底，保证 quad 有合法尺寸
        if (!m_item->m_cpuConsumerActive.load()) {
            QSize off(int(m_item->m_width), int(m_item->m_height));
            if (!off.isEmpty() && off != m_videoSize)
                m_videoSize = off;
        }

        // 窗口尺寸变化 → 重新计算 quad 顶点
        QSize curItemSize(int(m_item->width()), int(m_item->height()));
        if (m_itemSize != curItemSize || m_videoSize != m_lastQuadVideoSize) {
            m_itemSize = curItemSize;
            m_lastQuadVideoSize = m_videoSize;
            updateQuadVertices();
        }
    }

    void render() override
    {
        initializeOpenGLFunctions();

        // ── 1. 让 mpv 渲染最新的视频帧到离屏 FBO ──
        renderMpvFrame();

        // static int renderCount = 0;
        // if (renderCount++ < 3)
        //     qDebug() << "[MpvVideo] render:" << renderCount-1
        //              << "textureDirty:" << m_textureDirty
        //              << "texture:" << m_texture
        //              << "vertices:" << m_vertices.size();

        // ── 2. 选择显示源纹理并绘制 ──
        //    有 CPU 消费者（CCD）：上载 raw/processed 帧到 m_texture 显示
        //    无 CPU 消费者（导引头）：直接采样离屏 FBO 纹理，纯 GPU、零回读
        GLuint srcTex = 0;
        if (m_item->m_cpuConsumerActive.load()) {
            if (m_textureDirty && !m_frameCopy.isNull())
                uploadTexture();
            srcTex = m_texture;
        } else {
            srcTex = m_offscreenTex;
        }
        m_item->m_displayTexId = srcTex;   // 放大镜绑定跟随

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (srcTex && !m_vertices.isEmpty()) {
            glUseProgram(m_program);

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                         m_vertices.constData(), GL_DYNAMIC_DRAW);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, srcTex);
            glUniform1i(glGetUniformLocation(m_program, "uTexture"), 0);

            GLint posLoc = glGetAttribLocation(m_program, "aPosition");
            glEnableVertexAttribArray(posLoc);
            glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE,
                                  4 * sizeof(float), reinterpret_cast<void*>(0));

            GLint texLoc = glGetAttribLocation(m_program, "aTexCoord");
            glEnableVertexAttribArray(texLoc);
            glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE,
                                  4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisableVertexAttribArray(posLoc);
            glDisableVertexAttribArray(texLoc);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glUseProgram(0);
        }

        // 重置 OpenGL 状态，避免干扰 Qt Quick 后续渲染
        m_item->window()->resetOpenGLState();

        // ── 3. 视频延迟探针打点 ──────────────────────────────────────
        //    仅导引头直显、且探针已打开时执行；内部只读一次 mpv 属性，
        //    不改变任何渲染行为。探针关闭时在 reportLatencySample 里直接返回。
        m_item->reportLatencySample(m_renderedNewFrame);
        m_item->reportCcdLatencySample(m_renderedNewFrame);
    }

private:
    // ---- 延迟初始化 GL 资源 ----
    void initGL()
    {
        m_program = buildProgram(kVertexShader, kFragmentShader);
        glGenBuffers(1, &m_vbo);
        m_glInitialized = true;
        qDebug() << "[MpvVideo] Vendor  :" << (const char*)glGetString(GL_VENDOR);
        qDebug() << "[MpvVideo] Renderer:" << (const char*)glGetString(GL_RENDERER);
        qDebug() << "[MpvVideo] Version :" << (const char*)glGetString(GL_VERSION);
    }

    // ---- 创建 mpv render context ----
    void initMpvRenderContext()
    {
        if (!m_item->m_mpv || m_item->m_mpvCtx)
            return;

        mpv_opengl_init_params glParams = {
            .get_proc_address = mpvGetProcAddress,
            .get_proc_address_ctx = nullptr,
        };

        // advanced=0: mpv 自行管理帧时序，不阻塞视频管线
        // advanced=1 需要调用 report_swap，容易与 Qt 渲染循环形成死锁
        int advanced = 0;
        mpv_render_param params[] = {
            {MPV_RENDER_PARAM_API_TYPE,           const_cast<char *>("opengl")},
            {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &glParams},
            {MPV_RENDER_PARAM_ADVANCED_CONTROL,   &advanced},
            {MPV_RENDER_PARAM_INVALID,            nullptr}
        };

        if (mpv_render_context_create(&m_item->m_mpvCtx, m_item->m_mpv, params) < 0) {
            qWarning() << "[MpvVideo] Failed to create mpv render context";
            m_item->m_mpvCtx = nullptr;
            return;
        }

        // 设置帧就绪回调 → 触发 Qt 重绘
        mpv_render_context_set_update_callback(m_item->m_mpvCtx, onMpvRenderUpdate, m_item);

        qDebug() << "[MpvVideo] mpv render context created";

        // 通知 VlcVideoItem：render context 已就绪，可以安全 loadfile 了
        m_item->m_renderCtxReady = true;
        QMetaObject::invokeMethod(m_item, "onRenderContextReady", Qt::QueuedConnection);
    }

    // ---- 让 mpv 渲染到离屏 FBO（优先视频原生分辨率），然后读回 CPU 帧缓冲 ----
    void renderMpvFrame()
    {
        m_renderedNewFrame = false;

        if (!m_item->m_mpvCtx)
            return;

        uint64_t flags = mpv_render_context_update(m_item->m_mpvCtx);
        if (!(flags & MPV_RENDER_UPDATE_FRAME))
            return;

        // 离屏渲染目标尺寸：优先取视频原生分辨率（video-params 的 dw/dh）。
        //   这样读回的帧就是原始画面，不带 mpv 按控件比例渲染留下的黑边，
        //   且分辨率不随控件大小变化；上限 1920×1080，小于上限的源按原尺寸。
        //   视频参数尚未解析到时，退回“控件尺寸 × 1.5”作为过渡。
        constexpr int kMaxFboWidth  = 1920;
        constexpr int kMaxFboHeight = 1080;
        int w, h;
        // dw/dh 由主线程写入，这里在 m_frameMutex 下取一份“成对”的快照，
        // 避免拿到“新宽 + 旧高”这类跨帧组合而算出错误比例
        int videoW, videoH;
        {
            QMutexLocker lock(&m_item->m_frameMutex);
            videoW = m_item->m_videoDw;
            videoH = m_item->m_videoDh;
        }
        if (videoW > 0 && videoH > 0) {
            const float capScale = qMin(1.0f,
                                        qMin(static_cast<float>(kMaxFboWidth) / videoW,
                                             static_cast<float>(kMaxFboHeight) / videoH));
            w = qMax(16, static_cast<int>(videoW * capScale + 0.5f));
            h = qMax(16, static_cast<int>(videoH * capScale + 0.5f));
        } else {
            w = qMax(16, int(m_item->width()))  * 3 / 2;
            h = qMax(16, int(m_item->height())) * 3 / 2;
        }

        if (w != m_mpvFboWidth || h != m_mpvFboHeight) {
            m_mpvFboWidth = w;
            m_mpvFboHeight = h;
            rebuildOffscreenFbo();
            qDebug() << "[MpvVideo] offscreen FBO resized to:" << w << "x" << h;
        }
        if (!m_offscreenFbo) return;

        // CCD 直显探针的 T1：mpv 已输出本帧，离屏 FBO 也已就绪。
        // 视觉推理开启时 cpuFrameConsumer=true，不进入该计时路径。
        // 起点选在 mpv 实际绘制之前，使统计覆盖 mpv 离屏渲染和 Qt 绘制。
        if (m_item->m_ccdSource.load()
                && !m_item->m_cpuConsumerActive.load()
                && m_item->m_ccdLatencyProbe) {
            m_item->m_ccdLatencyProbe->onFrameDecoded();
        }

        // 保存 Qt 的 FBO + viewport
        GLint qtFbo = 0, qtVp[4] = {};
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &qtFbo);
        glGetIntegerv(GL_VIEWPORT, qtVp);

        // ── 离屏渲染 ──
        glBindFramebuffer(GL_FRAMEBUFFER, m_offscreenFbo);
        glViewport(0, 0, w, h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mpv_opengl_fbo mpvFbo = { .fbo = static_cast<int>(m_offscreenFbo), .w = w, .h = h, .internal_format = 0 };
        int flipY = 0;
        mpv_render_param renderParams[] = {
            {MPV_RENDER_PARAM_OPENGL_FBO, &mpvFbo},
            {MPV_RENDER_PARAM_FLIP_Y,     &flipY},
            {MPV_RENDER_PARAM_INVALID,    nullptr}
        };
        mpv_render_context_render(m_item->m_mpvCtx, renderParams);

        // mpv 可能改了 FBO/状态，读像素前重新绑定离屏 FBO
        glBindFramebuffer(GL_FRAMEBUFFER, m_offscreenFbo);

        {
            QMutexLocker lock(&m_item->m_frameMutex);

            // 帧尺寸总是更新（requestPixelAt / 放大镜依赖，即使不回读）
            bool sizeChanged = (m_item->m_width != w || m_item->m_height != h);
            m_item->m_width  = w;
            m_item->m_height = h;
            if (sizeChanged) emit m_item->frameSizeChanged();

            // 有 CPU 消费者（如 CCD 检测）时才读回像素；
            // 导引头纯显示模式跳过 glReadPixels，显示直接采样离屏纹理
            if (m_item->m_cpuConsumerActive.load()) {
                QImage &buf = m_item->m_frameBuf[m_item->m_writeIdx];
                if (buf.width() != w || buf.height() != h || buf.isNull())
                    buf = QImage(w, h, QImage::Format_RGBA8888);

                glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buf.bits());

                m_item->m_readyIdx = m_item->m_writeIdx;
                m_item->m_writeIdx = 1 - m_item->m_writeIdx;
                m_item->m_frameUpdated = true;
            }
        }

        m_renderedNewFrame = true;   // 本轮确实渲染出了新的一帧
        m_item->m_renderedFrameSerial.fetch_add(1, std::memory_order_relaxed);

        // ── 恢复 Qt FBO + viewport（关键！否则后续绘制错位） ──
        glBindFramebuffer(GL_FRAMEBUFFER, qtFbo);
        glViewport(qtVp[0], qtVp[1], qtVp[2], qtVp[3]);
    }

    // ---- 重建离屏 FBO（窗口尺寸变化时调用） ----
    void rebuildOffscreenFbo()
    {
        if (m_offscreenFbo)
            glDeleteFramebuffers(1, &m_offscreenFbo);
        if (m_offscreenTex)
            glDeleteTextures(1, &m_offscreenTex);

        glGenFramebuffers(1, &m_offscreenFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_offscreenFbo);

        glGenTextures(1, &m_offscreenTex);
        glBindTexture(GL_TEXTURE_2D, m_offscreenTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     m_mpvFboWidth, m_mpvFboHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, m_offscreenTex, 0);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            qWarning() << "[MpvVideo] Offscreen FBO incomplete:" << status;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // ---- mpv 帧就绪回调（渲染线程 → 触发 Qt update） ----
    static void onMpvRenderUpdate(void *ctx)
    {
        // static int cbCount = 0;
    // if (++cbCount <= 5 || cbCount % 60 == 0)
    //     qDebug() << "[MpvVideo] onMpvRenderUpdate #" << cbCount;
        auto *item = static_cast<VlcVideoItem *>(ctx);
        QMetaObject::invokeMethod(item, "update", Qt::QueuedConnection);
    }

    // ---- 上载 QImage 到 GL 纹理 ----
    void uploadTexture()
    {
        QImage tex = m_frameCopy.convertToFormat(QImage::Format_RGBA8888);
        int tw = tex.width(), th = tex.height();

        if (!m_texture) {
            glGenTextures(1, &m_texture);
        }

        glBindTexture(GL_TEXTURE_2D, m_texture);

        // 仅在首次分配或尺寸变化时重新分配 GPU 显存，后续用 glTexSubImage2D
        if (tw != m_texWidth || th != m_texHeight) {
            m_texWidth = tw;
            m_texHeight = th;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
        } else {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tw, th,
                            GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);
        m_textureDirty = false;
        m_item->m_displayTexId = m_texture;  // 暴露给放大镜直接绑定
    }

    // ---- 计算保持宽高比的居中 quad（NDC 坐标） ----
    void updateQuadVertices()
    {
        m_vertices.clear();

        if (m_videoSize.isEmpty() || m_itemSize.isEmpty())
            return;

        float videoW = m_videoSize.width();
        float videoH = m_videoSize.height();
        float itemW  = m_itemSize.width();
        float itemH  = m_itemSize.height();

        float scale = qMin(itemW / videoW, itemH / videoH);
        float displayW = videoW * scale;
        float displayH = videoH * scale;

        float halfW = displayW / itemW;
        float halfH = displayH / itemH;

        // 三角形带：左下 → 右下 → 左上 → 右上
        // glReadPixels 读出的数据 bottom-up（首行 = 画面底部），
        // 经 glTexImage2D 后 GL 纹理 V=0 处为画面底部，V=1 处为画面顶部
        // clang-format off
        m_vertices = {
            -halfW, -halfH,   0.0f, 0.0f,
             halfW, -halfH,   1.0f, 0.0f,
            -halfW,  halfH,   0.0f, 1.0f,
             halfW,  halfH,   1.0f, 1.0f,
        };
        // clang-format on
    }

    // ---- 编译 Vertex + Fragment Shader → Program ----
    GLuint buildProgram(const char *vsSrc, const char *fsSrc)
    {
        GLuint vs = compileShader(GL_VERTEX_SHADER,   vsSrc);
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
        if (!vs || !fs) {
            if (vs) glDeleteShader(vs);
            if (fs) glDeleteShader(fs);
            return 0;
        }

        GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);

        GLint linked = 0;
        glGetProgramiv(prog, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint len = 0;
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
            QByteArray log;
            log.resize(len);
            glGetProgramInfoLog(prog, len, &len, log.data());
            qWarning() << "[MpvVideo] Shader link failed:" << log;
            glDeleteProgram(prog);
            prog = 0;
        }

        glDetachShader(prog, vs);
        glDetachShader(prog, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return prog;
    }

    GLuint compileShader(GLenum type, const char *src)
    {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);

        GLint compiled = 0;
        glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint len = 0;
            glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
            QByteArray log;
            log.resize(len);
            glGetShaderInfoLog(s, len, &len, log.data());
            qWarning() << "[MpvVideo] Shader compile failed:" << log;
            glDeleteShader(s);
            return 0;
        }
        return s;
    }

    VlcVideoItem *m_item;

    // 帧数据
    QImage  m_frameCopy;
    QSize   m_videoSize;
    QSize   m_lastQuadVideoSize;
    bool    m_textureDirty = false;
    bool    m_renderedNewFrame = false;   // 本轮 render() 是否真的画出了新的一帧

    // 顶点数据（动态更新以保持宽高比）
    QVector<float> m_vertices;
    QSize m_itemSize;

    // GL 资源
    GLuint m_texture = 0;
    GLuint m_program = 0;
    GLuint m_vbo     = 0;
    bool   m_glInitialized = false;
    int    m_texWidth  = 0;
    int    m_texHeight = 0;

    // mpv 离屏渲染目标
    GLuint m_offscreenFbo = 0;
    GLuint m_offscreenTex = 0;
    int    m_mpvFboWidth  = 0;
    int    m_mpvFboHeight = 0;
};

// ══════════════════════════════════════════════════════════════════
// mpv 日志回调 —— 打印解码器选择等内部日志
// ══════════════════════════════════════════════════════════════════
static void onMpvLog(void * /*data*/,
                     mpv_event_log_message *msg)
{
    qDebug() << "[mpv]" << msg->prefix << ":" << msg->text;
}

// ══════════════════════════════════════════════════════════════════
// VlcVideoItem — 主线程 / QML 可见部分
// ══════════════════════════════════════════════════════════════════

VlcVideoItem::VlcVideoItem(QQuickItem *parent)
    : QQuickFramebufferObject(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);

    // 视频接收侧延迟探针：默认关闭，关闭时不抓包、不查询 mpv、零开销。
    // 打开方式：环境变量 KYLIN_LATPROBE=1，或调用 m_latencyProbe->setEnabled(true)。
    m_latencyProbe = new VideoLatencyProbe(this);
    m_ccdLatencyProbe = new CcdLatencyProbe(this);
}

// QML 组件完成布局后才调用 —— 此时 width/height 有效，update() 能触发 render()
void VlcVideoItem::componentComplete()
{
    QQuickFramebufferObject::componentComplete();
    qDebug() << "[MpvVideo] componentComplete, size:" << width() << "x" << height();
    // 如果之前 setSource() 因尺寸为 0 被延迟，现在重新触发
    update();
}

// ── 延迟初始化 mpv（首次 playback 或 setSource 时调用） ──
void VlcVideoItem::ensureMpvCreated()
{
    if (m_mpv)
        return;

    // mpv 要求 LC_NUMERIC="C"（小数点必须用 '.' 而非 ',' 等区域符号）
    // Qt 在 QCoreApplication 初始化时会 setlocale(LC_ALL, "")，需要在这里重置
    setlocale(LC_NUMERIC, "C");

    qDebug() << "[MpvVideo] creating mpv instance...";

    m_mpv = mpv_create();
    if (!m_mpv) {
        qWarning() << "[MpvVideo] mpv_create failed, errno:" << errno
                   << "(" << strerror(errno) << ")";
        return;
    }

    qDebug() << "[MpvVideo] mpv_create OK";

    setMpvOptionChecked(m_mpv, "vo", "libmpv");
    // 图像头默认按顶部 KYLIN_GUIDE_HWDEC_MODE 选择软件/硬件解码；
    // CCD 等其它源使用安全的 auto-safe，并在不支持时由 mpv 回退。
    QByteArray hwdec = qgetenv("KYLIN_MPV_HWDEC").trimmed();
    if (hwdec.isEmpty()) {
        // 在 mpv_initialize() 之前确定默认值。旧版 mpv 不一定允许运行时
        // 切换 hwdec，因此不能只依赖 doSetupPlayer() 中的属性设置。
        hwdec = defaultHwdecForStream(m_source, m_ccdSource.load());
    }
    setMpvOptionChecked(m_mpv, "hwdec", hwdec.constData());
    qDebug() << "[MpvVideo] initial hwdec =" << hwdec;
    const QByteArray hwdecExtraFrames =
            qgetenv("KYLIN_MPV_HWDEC_EXTRA_FRAMES").trimmed();
    if (!hwdecExtraFrames.isEmpty()) {
        setMpvOptionChecked(m_mpv, "hwdec-extra-frames",
                            hwdecExtraFrames.constData());
        qDebug() << "[MpvVideo] hwdec-extra-frames =" << hwdecExtraFrames;
    }
    // 本应用只显示视频：不选择音频轨，避免音频解码和播放。
    setMpvOptionChecked(m_mpv, "aid", "no");
    setMpvOptionChecked(m_mpv, "config", "no");
    setMpvOptionChecked(m_mpv, "msg-level", "all=warn");

    // 不要只依赖内置 low-latency profile。不同发行版的 mpv 版本内容不同，
    // 有些版本仍保留默认的 1 秒 demuxer readahead，表现为接收端约 1 秒
    // 的稳定固定延迟。直播流没有回看需求，因此这里显式关闭 readahead 和回退缓存。
    setMpvOptionChecked(m_mpv, "profile", "low-latency");
    setMpvOptionChecked(m_mpv, "cache-pause", "no");
    setMpvOptionChecked(m_mpv, "cache-pause-wait", "0");
    setMpvOptionChecked(m_mpv, "video-timing-offset", "0");
    QByteArray readaheadSecs = qgetenv("KYLIN_MPV_READAHEAD_SECS").trimmed();
    if (readaheadSecs.isEmpty())
        readaheadSecs = "0";
    setMpvOptionChecked(m_mpv, "demuxer-readahead-secs", readaheadSecs.constData());
    setMpvOptionChecked(m_mpv, "demuxer-max-bytes", "1048576");
    setMpvOptionChecked(m_mpv, "demuxer-max-back-bytes", "0");
    setMpvOptionChecked(m_mpv, "stream-buffer-size", "4096");
    setMpvOptionChecked(m_mpv, "vd-lavc-threads", "1");
    setMpvOptionChecked(m_mpv, "vd-queue-enable", "no");
    setMpvOptionChecked(m_mpv, "video-latency-hacks", "yes");

    // FFmpeg 的 UDP 协议层还有独立的环形 FIFO 和 socket 接收缓冲。
    // ffmpeg 默认 fifo_size=28672 个 188 字节包（约 5.4 MiB），即使
    // demuxer-readahead-secs=0 也可能积压接近 1 秒。这里给直播 UDP 设置
    // 明确的低延迟上限；现场若出现花屏，可把两个值逐步调大。
    QByteArray udpFifoPackets = qgetenv("KYLIN_MPV_UDP_FIFO_PACKETS").trimmed();
    QByteArray udpBufferBytes = qgetenv("KYLIN_MPV_UDP_BUFFER_BYTES").trimmed();
    if (udpFifoPackets.isEmpty())
        udpFifoPackets = "512";
    if (udpBufferBytes.isEmpty())
        udpBufferBytes = "131072";
    const QByteArray lavfOptions =
            "fflags=+nobuffer,fifo_size=" + udpFifoPackets
            + ",buffer_size=" + udpBufferBytes
            + ",overrun_nonfatal=1";
    setMpvOptionChecked(m_mpv, "demuxer-lavf-o", lavfOptions.constData());
    qDebug() << "[MpvVideo] UDP latency limits: fifo_size=" << udpFifoPackets
             << "buffer_size=" << udpBufferBytes;

    int ret = mpv_initialize(m_mpv);
    if (ret < 0) {
        qWarning() << "[MpvVideo] mpv_initialize failed, error code:" << ret
                   << "(" << mpv_error_string(ret) << ")";
        mpv_terminate_destroy(m_mpv);
        m_mpv = nullptr;
        return;
    }

    qDebug() << "[MpvVideo] mpv_initialize OK";

    mpv_request_log_messages(m_mpv, "warn");
    mpv_set_wakeup_callback(m_mpv, onMpvWakeup, this);
}

VlcVideoItem::~VlcVideoItem()
{
    // 先停探针（会等待抓包线程退出并落盘 CSV），避免析构期间渲染线程还在打点。
    // m_latencyProbe 是 this 的子对象，仍由 QObject 负责释放，这里只置空引用。
    if (m_latencyProbe) {
        m_latencyProbe->setEnabled(false);
        m_latencyProbe = nullptr;
    }
    if (m_ccdLatencyProbe) {
        m_ccdLatencyProbe->setEnabled(false);
        m_ccdLatencyProbe = nullptr;
    }

    releasePlayer();

    // 注意：m_mpvCtx 由 VlcVideoRenderer 在其析构函数中释放（渲染线程）
    // 这里只释放 m_mpv
    if (m_mpv) {
        mpv_terminate_destroy(m_mpv);
        m_mpv = nullptr;
    }
}

// ---- QQuickFramebufferObject 接口 ----
QQuickFramebufferObject::Renderer *VlcVideoItem::createRenderer() const
{
    qDebug() << "[MpvVideo] createRenderer() called";
    return new VlcVideoRenderer(const_cast<VlcVideoItem *>(this));
}

QSGNode *VlcVideoItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    // qDebug() << "[MpvVideo] updatePaintNode() called, node:" << (node ? "exists" : "NULL");
    return QQuickFramebufferObject::updatePaintNode(node, data);
}

// ===== 属性访问 =====
QString VlcVideoItem::source() const { return m_source; }

bool VlcVideoItem::isPlaying() const { return m_playing; }

int VlcVideoItem::volume() const { return m_volume; }

void VlcVideoItem::setVolume(int vol)
{
    vol = qBound(0, vol, 100);
    if (m_volume != vol) {
        m_volume = vol;
        if (m_mpv) {
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", vol);
            mpv_set_property_string(m_mpv, "volume", buf);
        }
        emit volumeChanged();
    }
}

float VlcVideoItem::position() const
{
    if (!m_mpv) return 0.0f;
    double pct = 0.0;
    mpv_get_property(m_mpv, "percent-pos", MPV_FORMAT_DOUBLE, &pct);
    return static_cast<float>(pct / 100.0);
}

void VlcVideoItem::setPosition(float pos)
{
    if (m_mpv) {
        double pct = qBound(0.0, static_cast<double>(pos) * 100.0, 100.0);
        mpv_set_property_async(m_mpv, 0, "percent-pos", MPV_FORMAT_DOUBLE, &pct);
    }
}

qint64 VlcVideoItem::length() const
{
    if (!m_mpv) return 0;
    double duration = 0.0;
    mpv_get_property(m_mpv, "duration", MPV_FORMAT_DOUBLE, &duration);
    return static_cast<qint64>(duration * 1000.0);  // 秒 → 毫秒
}

bool VlcVideoItem::isSeekable() const { return m_seekable; }

// ===== CPU 帧消费开关 =====
void VlcVideoItem::setCpuFrameConsumer(bool on)
{
    if (m_cpuConsumerActive.load() != on) {
        m_cpuConsumerActive.store(on);
        if (!on) {
            // Guide-head mode: drop the stale CPU frame so StreamProcessor
            // cannot keep consuming the previous CCD frame.
            QMutexLocker lock(&m_frameMutex);
            m_readyIdx = -1;
            m_frameUpdated = false;
        }
        emit cpuFrameConsumerChanged();
        // 强制触发重绘，让渲染线程尽快按新模式走
        update();
    }
}

void VlcVideoItem::setCcdSource(bool on)
{
    if (m_ccdSource.load() != on) {
        m_ccdSource.store(on);
        if (m_ccdLatencyProbe)
            m_ccdLatencyProbe->notePlaybackStart();
        emit ccdSourceChanged();
        update();
    }
}

// ══════════════════════════════════════════════════════════════════
// StreamProcessor 接口
// ══════════════════════════════════════════════════════════════════

QImage VlcVideoItem::grabFrame() const
{
    QMutexLocker lock(&m_frameMutex);
    if (m_readyIdx < 0 || m_frameBuf[m_readyIdx].isNull())
        return QImage();
    // 渲染线程会在原缓冲上原位覆写（glReadPixels 直接写 buf.bits()），
    // 必须深拷贝，否则调用方持有的 QImage 会在下一帧渲染时被改写，产生撕裂帧
    return m_frameBuf[m_readyIdx].copy();
}

void VlcVideoItem::submitProcessedFrame(const QImage &frame)
{
    {
        QMutexLocker lock(&m_frameMutex);
        m_processedFrame = frame;
        m_hasProcessedFrame = true;
        m_frameUpdated = true;
    }
    QMetaObject::invokeMethod(const_cast<VlcVideoItem *>(this),
                              "update", Qt::QueuedConnection);
}

// ══════════════════════════════════════════════════════════════════
// 视频固有尺寸（dw/dh）
//
// 写方在主线程（mpv video-params 事件 / releasePlayer 复位），读方在渲染线程
// （renderMpvFrame、VlcFrameItem 的渲染器）。dw 与 dh 必须成对读取，因此不直接
// 暴露成员，统一走这里；调用点若已持有 m_frameMutex 就不能再走本函数（QMutex
// 非递归，会自锁）。
// ══════════════════════════════════════════════════════════════════

QSize VlcVideoItem::videoNativeSize() const
{
    QMutexLocker lock(&m_frameMutex);
    return QSize(m_videoDw, m_videoDh);
}

quint64 VlcVideoItem::renderedFrameSerial() const
{
    return m_renderedFrameSerial.load(std::memory_order_relaxed);
}

// ══════════════════════════════════════════════════════════════════
// 视频接收侧延迟探针
//
// 由渲染线程在 render() 末尾调用。只有在"本轮画出了新帧 + 探针已打开 +
// 导引头直显（无 CPU 回读）"三个条件同时满足时，才做一次只读的 mpv 属性
// 查询，再交给探针换算延迟。探针关闭时这里只剩一次空指针与一次 bool 判断。
// ══════════════════════════════════════════════════════════════════
void VlcVideoItem::reportLatencySample(bool newFrameRendered)
{
    if (!m_latencyProbe || !newFrameRendered)
        return;
    if (!m_latencyProbe->isEnabled())
        return;
    if (m_ccdSource.load())            // CCD 由独立探针统计
        return;
    if (m_cpuConsumerActive.load())     // 仅导引头直显路径，CCD 模式不测
        return;
    if (!m_mpv)
        return;

    // 优先用 video-pts；个别版本读不到时退回 time-pos。
    // 两者都在 mpv 的归一化时间轴上，探针里的基准逻辑对二者同样适用。
    double pts = 0.0;
    if (mpv_get_property(m_mpv, "video-pts", MPV_FORMAT_DOUBLE, &pts) < 0
            && mpv_get_property(m_mpv, "time-pos", MPV_FORMAT_DOUBLE, &pts) < 0)
        return;                          // 视频参数尚未就绪时读不到，忽略即可

    m_latencyProbe->onFrameDisplayed(pts);
    reportMpvPipelineDiagnostics(m_mpv);
}

void VlcVideoItem::reportCcdLatencySample(bool newFrameRendered)
{
    if (!m_ccdLatencyProbe || !newFrameRendered)
        return;
    if (!m_ccdSource.load())
        return;
    if (m_cpuConsumerActive.load())     // 视觉模型推理开启时不测 CCD
        return;
    m_ccdLatencyProbe->onFrameRendered(true);
}

// ── 供 QML 使用的探针控制与读数（测试用）─────────────────────────
void VlcVideoItem::setLatencyProbeEnabled(bool on)
{
    if (m_latencyProbe)
        m_latencyProbe->setEnabled(on);
}

bool VlcVideoItem::latencyProbeEnabled() const
{
    return m_latencyProbe && m_latencyProbe->isEnabled();
}

QVariantMap VlcVideoItem::latencyProbeStats() const
{
    QVariantMap m;
    if (!m_latencyProbe)
        return m;
    const VideoLatencyProbe::Stats s = m_latencyProbe->stats();
    m.insert(QStringLiteral("samples"),     QVariant::fromValue(s.samples));
    m.insert(QStringLiteral("misses"),      QVariant::fromValue(s.misses));
    m.insert(QStringLiteral("avgMs"),       s.avgMs);
    m.insert(QStringLiteral("p50Ms"),       s.p50Ms);
    m.insert(QStringLiteral("p95Ms"),       s.p95Ms);
    m.insert(QStringLiteral("p99Ms"),       s.p99Ms);
    m.insert(QStringLiteral("minMs"),       s.minMs);
    m.insert(QStringLiteral("maxMs"),       s.maxMs);
    m.insert(QStringLiteral("arrivalLeadMs"), s.arrivalLeadMs);
    m.insert(QStringLiteral("arrivalStepMs"), s.arrivalStepMs);
    m.insert(QStringLiteral("anchorReady"), s.anchorReady);
    m.insert(QStringLiteral("frameMs"),     s.ptsStepMs);
    return m;
}

QVariantMap VlcVideoItem::ccdLatencyProbeStats() const
{
    QVariantMap m;
    if (!m_ccdLatencyProbe)
        return m;
    const CcdLatencyProbe::Stats s = m_ccdLatencyProbe->stats();
    m.insert(QStringLiteral("samples"), QVariant::fromValue(s.samples));
    m.insert(QStringLiteral("misses"), QVariant::fromValue(s.misses));
    m.insert(QStringLiteral("avgMs"), s.avgMs);
    m.insert(QStringLiteral("p50Ms"), s.p50Ms);
    m.insert(QStringLiteral("p95Ms"), s.p95Ms);
    m.insert(QStringLiteral("p99Ms"), s.p99Ms);
    m.insert(QStringLiteral("minMs"), s.minMs);
    m.insert(QStringLiteral("maxMs"), s.maxMs);
    m.insert(QStringLiteral("metric"),
             QStringLiteral("ccd_direct_decoded_to_render"));
    m.insert(QStringLiteral("inferenceActive"), cpuFrameConsumer());
    return m;
}

// ===== 播放控制 =====
void VlcVideoItem::setSource(const QString &url)
{
    if (m_source != url) {
        m_source = url;
        emit sourceChanged();
        ensureMpvCreated();  // QML 对象构造完成后，首次 setSource 时才初始化 mpv
        // 延迟探针跟随视频源：udp:// 组播地址开始抓包；空串或非组播协议则停止
        if (m_latencyProbe) {
            // CCD 使用独立的进程内直显探针，不尝试解析 RTSP/PTS 到达表。
            m_latencyProbe->setStreamUrl(m_ccdSource.load() ? QString() : url);
        }
        setupPlayer();
    }
}

// ── 原始码流录制（record-file：解包后原样写盘，不转码不重封装）──
void VlcVideoItem::startRecord(const QString &filePath)
{
    if (!m_mpv) {
        qWarning() << "[MpvVideo] startRecord 被忽略：mpv 尚未创建";
        return;
    }
    if (filePath.isEmpty()) {
        qWarning() << "[MpvVideo] startRecord 被忽略：文件路径为空";
        return;
    }
    mpv_set_property_string(m_mpv, "record-file", filePath.toUtf8().constData());
    m_recording = true;
    emit recordingChanged();
    qDebug() << "[MpvVideo] 开始录制原始码流:" << filePath;
}

void VlcVideoItem::stopRecord()
{
    if (m_mpv)
        mpv_set_property_string(m_mpv, "record-file", "");
    if (m_recording) {
        m_recording = false;
        emit recordingChanged();
    }
    qDebug() << "[MpvVideo] 停止录制原始码流";
}

void VlcVideoItem::play()
{
    if (!m_mpv) return;
    if (m_source.isEmpty()) return;

    m_playClicked = true;

    // 直播流在等待视频参数期间可能已经积压了一段缓冲。取消暂停前先丢弃
    // 旧队列，否则即使 readahead=0，启动阶段的积压仍会长期保持约 1 秒延迟。
    const char *dropCmd[] = {"drop-buffers", nullptr};
    const int dropRet = mpv_command(m_mpv, dropCmd);
    if (dropRet < 0)
        qWarning() << "[MpvVideo] drop-buffers failed:" << mpv_error_string(dropRet);

    // 使用 MPV_FORMAT_FLAG，避免 "yes"/"no" 字符串解析问题
    int paused = 0;  // 0 = 不暂停 = 播放
    mpv_set_property(m_mpv, "pause", MPV_FORMAT_FLAG, &paused);

    // 校验是否设置成功
    int check = 1;
    mpv_get_property(m_mpv, "pause", MPV_FORMAT_FLAG, &check);
    // qDebug() << "[MpvVideo] play() pause =" << check;

    update();

    // 兜底：启动一个定时器驱动渲染，防止 onMpvRenderUpdate 回调失效
    if (!m_playTimer) {
        m_playTimer = new QTimer(this);
        m_playTimer->setInterval(33);  // ~30fps
        connect(m_playTimer, &QTimer::timeout, this, [this]() { update(); });
    }
    m_playTimer->start();
}

void VlcVideoItem::pause()
{
    if (m_mpv) {
        int paused = 1;
        mpv_set_property(m_mpv, "pause", MPV_FORMAT_FLAG, &paused);
    }
    if (m_playTimer)
        m_playTimer->stop();
}

void VlcVideoItem::stop()
{
    // 不要用 "stop" 命令（会销毁 vo=libmpv，导致后续 loadfile 无法恢复视频输出）
    // 改用以暂停 + 清画面代替
    if (m_mpv) {
        int paused = 1;
        mpv_set_property(m_mpv, "pause", MPV_FORMAT_FLAG, &paused);
    }

    m_playing = false;
    m_playClicked = false;

    if (m_playTimer)
        m_playTimer->stop();

    {
        QMutexLocker lock(&m_frameMutex);
        m_processedFrame = QImage();
        m_hasProcessedFrame = false;
    }

    emit playingChanged();
    emit stopped();
}

// ===== 内部 =====
void VlcVideoItem::setupPlayer()
{
    releasePlayer();

    if (m_source.isEmpty() || !m_mpv)
        return;

    // ══ 关键：必须先有 render context，才能 loadfile ══
    // 否则 mpv 会报 "No render context set" 并关闭视频输出
    if (!m_renderCtxReady) {
        if (m_setupRetryCount >= 30) {  // 最多等 3 秒
            qWarning() << "[MpvVideo] render context never ready, giving up";
            return;
        }
        m_setupRetryCount++;

        // qDebug() << "[MpvVideo] setup retry" << m_setupRetryCount ...;
        if (width() <= 0 || height() <= 0) {
            // waiting for layout...
        } else if (!window()) {
            // no window yet...
        } else {
            update();
            window()->update();
        }
        QTimer::singleShot(100, this, [this]() {
            if (!m_renderCtxReady && !m_source.isEmpty())
                setupPlayer();
        });
        return;
    }
    m_setupRetryCount = 0;

    doSetupPlayer();
}

void VlcVideoItem::doSetupPlayer()
{
    if (m_source.isEmpty() || !m_mpv)
        return;

    // 防止 onRenderContextReady 和重试定时器重复调用
    if (m_setupInProgress)
        return;
    m_setupInProgress = true;

    qDebug() << "[MpvVideo] doSetupPlayer, loading:" << redactedStreamUrl(m_source);

    // 图像头的解码模式由文件顶部的 KYLIN_GUIDE_HWDEC_MODE 统一控制。
    // 现场实测：imgvideo/omx 硬件解码约积压 25 帧，软件解码约 59ms。
    // CCD 等其它源仍默认 auto-safe；环境变量可覆盖两条路径的默认值。
    QByteArray streamHwdec = qgetenv("KYLIN_MPV_HWDEC").trimmed();
    if (streamHwdec.isEmpty())
        streamHwdec = defaultHwdecForStream(m_source, m_ccdSource.load());
    const int hwdecRet = mpv_set_property_string(
                m_mpv, "hwdec", streamHwdec.constData());
    if (hwdecRet < 0) {
        qWarning() << "[MpvVideo] failed to set hwdec:" << mpv_error_string(hwdecRet);
    } else {
        qDebug() << "[MpvVideo] hwdec =" << streamHwdec;
    }

    // 无音频的直播 UDP 流没有音画同步需求，优先追最新帧。码流缺少有效
    // PTS 时尤其重要：若继续按生成的 25 fps 时间轴排队，会稳定积压近 1 秒。
    // 默认仅对导引头 UDP 生效；可用 KYLIN_MPV_UNTIMED=0/1 强制关闭/打开。
    const QByteArray untimedOverride = qgetenv("KYLIN_MPV_UNTIMED").trimmed().toLower();
    const bool untimed = !untimedOverride.isEmpty()
            ? (untimedOverride != "0" && untimedOverride != "false" && untimedOverride != "no")
            : (m_source.startsWith(QStringLiteral("udp://"), Qt::CaseInsensitive)
               && !m_ccdSource.load());
    int untimedFlag = untimed ? 1 : 0;
    const int untimedRet = mpv_set_property(
                m_mpv, "untimed", MPV_FORMAT_FLAG, &untimedFlag);
    if (untimedRet < 0) {
        qWarning() << "[MpvVideo] failed to set untimed:" << mpv_error_string(untimedRet);
    } else {
        qDebug() << "[MpvVideo] untimed =" << untimed;
    }

    // 先设为暂停状态，等用户点击 play() 再取消暂停
    int paused = 1;
    mpv_set_property(m_mpv, "pause", MPV_FORMAT_FLAG, &paused);
    // RTSP 优先使用 UDP，减少 TCP 重传和队头阻塞带来的额外延迟。
    // 若现场网络丢包导致花屏，可改回 "tcp"。
    mpv_set_property_string(m_mpv, "rtsp-transport", "udp");
    // 观察属性变化以获取播放状态
    mpv_observe_property(m_mpv, 0, "pause",          MPV_FORMAT_FLAG);
    mpv_observe_property(m_mpv, 0, "duration",       MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "seekable",       MPV_FORMAT_FLAG);
    mpv_observe_property(m_mpv, 0, "video-params",   MPV_FORMAT_NODE);

    // 加载媒体文件/流
    // 延迟探针：把 loadfile 之后收到的第一帧作为 PTS 基准（mpv 会把时间轴归零，
    // 而 TS 里是原始 PTS，两侧需要这个基准才能对上同一帧）。
    if (m_latencyProbe)
        m_latencyProbe->notePlaybackStart();
    if (m_ccdLatencyProbe)
        m_ccdLatencyProbe->notePlaybackStart();

    const QByteArray url = m_source.toUtf8();
    const char *cmd[] = {"loadfile", url.constData(), nullptr};
    mpv_command_async(m_mpv, 0, cmd);

}

void VlcVideoItem::releasePlayer()
{
    if (m_mpv) {
        // 不调 stop（会销毁 vo=libmpv），loadfile 会自动替换当前流
        mpv_unobserve_property(m_mpv, 0);
    }

    m_setupInProgress = false;
    m_needClearDisplay = true;   // 通知渲染器清空上一视频流的残留帧
    // 换源时确保不再往旧文件写
    if (m_recording)
        stopRecord();

    // “连接成功”标志：仅主线程访问，每次重新加载都重新判定
    m_videoReady = false;

    // 清空帧缓冲 + 复位视频参数（同一把锁：渲染线程要么看到“旧尺寸 + 旧帧”，
    // 要么看到“0 + 空帧”，不会出现半新半旧的组合）
    {
        QMutexLocker lock(&m_frameMutex);
        m_videoDw = 0;
        m_videoDh = 0;
        m_frameBuf[0] = QImage();
        m_frameBuf[1] = QImage();
        m_writeIdx = 0;
        m_readyIdx = -1;
        m_frameUpdated = false;
        m_hasProcessedFrame = false;
        m_processedFrame = QImage();
        // 帧尺寸清零：下一次收到真实视频帧时必定触发 frameSizeChanged，
        // 供 QML 以“收到视频帧”作为连接成功的判定依据
        m_width = 0;
        m_height = 0;
    }
}

// ===== mpv 事件处理 =====

void VlcVideoItem::onMpvWakeup(void *ctx)
{
    auto *self = static_cast<VlcVideoItem *>(ctx);
    QMetaObject::invokeMethod(self, "processMpvEvents", Qt::QueuedConnection);
}

// ── 渲染上下文就绪后，继续之前被延迟的 setupPlayer ──
void VlcVideoItem::onRenderContextReady()
{
    qDebug() << "[MpvVideo] render context ready";
    // 走统一入口 setupPlayer()，避免与重试定时器产生竞态
    setupPlayer();
}

// 从 mpv video-params 节点中解析出视频固有显示尺寸（dw/dh）
static void parseVideoParamsNode(mpv_node *node, int *dw, int *dh)
{
    if (!node) return;

    if (node->format == MPV_FORMAT_NODE_MAP) {
        mpv_node_list *list = node->u.list;
        for (int i = 0; list && i < list->num; ++i) {
            const char *key = list->keys ? list->keys[i] : nullptr;
            if (!key || list->values[i].format != MPV_FORMAT_INT64) continue;
            if      (strcmp(key, "dw") == 0) *dw = static_cast<int>(list->values[i].u.int64);
            else if (strcmp(key, "dh") == 0) *dh = static_cast<int>(list->values[i].u.int64);
        }
    } else if (node->format == MPV_FORMAT_NODE_ARRAY) {
        // 旧版 mpv：视频参数以 [key, value, key, value, ...] 数组形式返回
        mpv_node_list *list = node->u.list;
        for (int i = 0; list && i + 1 < list->num; i += 2) {
            mpv_node &k = list->values[i];
            mpv_node &v = list->values[i + 1];
            if (k.format != MPV_FORMAT_STRING || !k.u.string || v.format != MPV_FORMAT_INT64) continue;
            if      (strcmp(k.u.string, "dw") == 0) *dw = static_cast<int>(v.u.int64);
            else if (strcmp(k.u.string, "dh") == 0) *dh = static_cast<int>(v.u.int64);
        }
    }
}

void VlcVideoItem::processMpvEvents()
{
    if (!m_mpv) return;

    while (true) {
        mpv_event *event = mpv_wait_event(m_mpv, 0.0);  // 非阻塞轮询
        if (event->event_id == MPV_EVENT_NONE)
            break;

        switch (event->event_id) {

        case MPV_EVENT_LOG_MESSAGE: {
            auto *log = static_cast<mpv_event_log_message *>(event->data);
            onMpvLog(nullptr, log);
            break;
        }

        case MPV_EVENT_FILE_LOADED:
            qDebug() << "[MpvVideo] file loaded";
            // 通知 QML：流已加载/连接成功，只有此时才应显示“已就绪”
            emit fileLoaded();
            // 重定向/新文件加载后，kickstart 渲染管线
            update();
            break;

        case MPV_EVENT_VIDEO_RECONFIG:
            qDebug() << "[MpvVideo] video reconfig";
            break;

        case MPV_EVENT_PLAYBACK_RESTART:
            qDebug() << "[MpvVideo] playback restart";
            update();
            break;

        case MPV_EVENT_END_FILE: {
            auto *ef = static_cast<mpv_event_end_file *>(event->data);
            qDebug() << "[MpvVideo] end file, reason:" << ef->reason;

            // REDIRECT 是流媒体的正常重定向流程，不改变播放状态
            if (ef->reason == MPV_END_FILE_REASON_REDIRECT)
                break;

            bool wasPlaying = m_playing;
            m_playing = false;
            if (wasPlaying) emit playingChanged();

            if (ef->reason == MPV_END_FILE_REASON_EOF) {
                emit ended();
            } else if (ef->reason == MPV_END_FILE_REASON_ERROR) {
                emit error("Playback error");
            }
            break;
        }

        case MPV_EVENT_PROPERTY_CHANGE: {
            auto *prop = static_cast<mpv_event_property *>(event->data);
            if (!prop->name) break;

            if (strcmp(prop->name, "pause") == 0) {
                bool wasPlaying = m_playing;
                m_playing = (prop->format == MPV_FORMAT_FLAG)
                            ? !(*static_cast<int *>(prop->data))
                            : m_playing;
                if (wasPlaying != m_playing)
                    emit playingChanged();
            } else if (strcmp(prop->name, "duration") == 0) {
                emit lengthChanged();
            } else if (strcmp(prop->name, "seekable") == 0) {
                if (prop->format == MPV_FORMAT_FLAG)
                    m_seekable = !!(*static_cast<int *>(prop->data));
                emit seekableChanged();
            } else if (strcmp(prop->name, "video-params") == 0) {
                // 记录视频固有显示尺寸，供 requestPixelAt 剔除黑边、判定点击是否落在视频画面内
                if (prop->format == MPV_FORMAT_NODE) {
                    // mpv 只回传发生变化的 key，所以先取当前值当初值
                    int dw, dh;
                    {
                        QMutexLocker lock(&m_frameMutex);
                        dw = m_videoDw;
                        dh = m_videoDh;
                    }
                    // 解析本身是纯计算，放到锁外，避免占着 m_frameMutex
                    parseVideoParamsNode(static_cast<mpv_node *>(prop->data), &dw, &dh);

                    bool sizeChanged = false;
                    {
                        QMutexLocker lock(&m_frameMutex);
                        if (dw != m_videoDw || dh != m_videoDh) {
                            m_videoDw = dw;
                            m_videoDh = dh;
                            sizeChanged = true;
                        }
                    }
                    if (sizeChanged) {
                        qDebug() << "[MpvVideo] video-params (native size):"
                                 << dw << "x" << dh;
                    }
                }
                // 解析到真实视频参数即代表流中确实有视频数据，判定连接成功
                // （UDP/RTSP 仅打开协议、无数据时不会触发）
                bool hasNativeSize = false;
                {
                    QMutexLocker lock(&m_frameMutex);
                    hasNativeSize = (m_videoDw > 0 && m_videoDh > 0);
                }
                if (hasNativeSize && !m_videoReady) {
                    m_videoReady = true;
                    emit videoReady();
                }
            }
            break;
        }

        default:
            break;
        }
    }

    // eventPollCount++;
    // if (hadEvents || eventPollCount <= 5 || eventPollCount % 50 == 0)
    //     qDebug() << "[MpvVideo] processMpvEvents #" << eventPollCount
    //              << "hadEvents:" << hadEvents << "playing:" << m_playing;
}

// ══════════════════════════════════════════════════════════════════
// 鼠标事件 — 直接处理绕过 QML MouseArea 在 FBO 上的事件传递问题
// ══════════════════════════════════════════════════════════════════

void VlcVideoItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 坐标映射与“是否落在视频画面内”的判定都在 requestPixelAt 中完成；
        // 点击在视频画面之外（黑边）时静默忽略，不输出、不发送偏差像素。
        requestPixelAt(event->pos().x(), event->pos().y());
        event->accept();
        return;
    }
    //如果不是左键，默认处理
    QQuickFramebufferObject::mousePressEvent(event);
}

// ══════════════════════════════════════════════════════════════════
// 像素读取 — 纯几何映射（做 Letterbox 坐标映射），不依赖 CPU 帧缓冲
// ══════════════════════════════════════════════════════════════════

void VlcVideoItem::requestPixelAt(int x, int y)
{
    // 纯几何映射：不再依赖 CPU 像素（点击发的是坐标，颜色此前仅作 debug）。
    // 帧尺寸 = 离屏 FBO 尺寸（m_width/m_height，渲染线程始终更新），
    // 视频固有尺寸 dw/dh 来自 video-params。导引头/CCD 模式统一走这里。
    const qreal itemW = static_cast<qreal>(width());
    const qreal itemH = static_cast<qreal>(height());
    const qreal videoW = static_cast<qreal>(m_width);
    const qreal videoH = static_cast<qreal>(m_height);

    if (itemW <= 0.0 || itemH <= 0.0) {
        emit errorReadingPixel(QStringLiteral("尺寸无效"));
        return;
    }
    if (videoW <= 0.0 || videoH <= 0.0) {
        qDebug() << "[MpvVideo] requestPixelAt: no frame available";
        emit errorReadingPixel(QStringLiteral("无可用视频帧"));
        return;
    }

    // 显示时帧在控件内是“等比缩放 + 居中”（由渲染器 quad 决定），
    // 黑边只出现在控件内视频内容区之外。先算内容区在控件坐标中的位置，
    // 点击落在内容区之外（黑边）时静默忽略，不发偏差像素。
    const qreal scale = qMin(itemW / videoW, itemH / videoH);
    const qreal displayW = videoW * scale;
    const qreal displayH = videoH * scale;
    const qreal ox = (itemW - displayW) / 2.0;
    const qreal oy = (itemH - displayH) / 2.0;

    if (x < ox || x > ox + displayW || y < oy || y > oy + displayH) {
        qDebug() << "[MpvVideo] click outside video area, ignored: ("
                 << x << "," << y << ")";
        return;
    }

    // 内容区内的点击 → 帧坐标（帧 = 视频原生画面，坐标即视频原生坐标）
    const int frameXpx = qBound(0, qRound((x - ox) / displayW * videoW), int(videoW) - 1);
    const int frameYpx = qBound(0, qRound((y - oy) / displayH * videoH), int(videoH) - 1);
    qDebug() << "[MpvVideo] pixelRead at (" << x << "," << y << ") → frame("
             << frameXpx << "," << frameYpx << ")";
    emit reqDeviationToImg(frameXpx, frameYpx);
}
