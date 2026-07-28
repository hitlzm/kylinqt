#include "VlcVideoItem.h"
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QMouseEvent>
#include <cerrno>
#include <cstring>
#include <clocale>

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
        if (m_texture)        { glDeleteTextures(1, &m_texture); m_item->m_displayTexId = 0; }
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

        // ── 2. 上载纹理并绘制（与原来完全一致）─────
        if (m_textureDirty && !m_frameCopy.isNull())
            uploadTexture();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (m_texture && !m_vertices.isEmpty()) {
            glUseProgram(m_program);

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                         m_vertices.constData(), GL_DYNAMIC_DRAW);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture);
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

    // ---- 让 mpv 渲染到 1.5x 离屏 FBO，然后读回高清 CPU 帧缓冲 ----
    void renderMpvFrame()
    {
        if (!m_item->m_mpvCtx)
            return;

        uint64_t flags = mpv_render_context_update(m_item->m_mpvCtx);
        if (!(flags & MPV_RENDER_UPDATE_FRAME))
            return;

        // 1.5 倍分辨率离屏渲染：
        //   mpv → 离屏FBO (1008×444) → glReadPixels → QImage (高清)
        //   主画面 quad 缩小显示，放大镜 grabFrame() 拿到更高清帧
        int w = qMax(16, int(m_item->width()))  * 3 / 2;
        int h = qMax(16, int(m_item->height())) * 3 / 2;

        if (w != m_mpvFboWidth || h != m_mpvFboHeight) {
            m_mpvFboWidth = w;
            m_mpvFboHeight = h;
            rebuildOffscreenFbo();
        }
        if (!m_offscreenFbo) return;

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
            QImage &buf = m_item->m_frameBuf[m_item->m_writeIdx];
            if (buf.width() != w || buf.height() != h || buf.isNull())
                buf = QImage(w, h, QImage::Format_RGBA8888);

            glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buf.bits());

            bool sizeChanged = (m_item->m_width != w || m_item->m_height != h);
            m_item->m_width  = w;
            m_item->m_height = h;
            if (sizeChanged) emit m_item->frameSizeChanged();
            m_item->m_readyIdx = m_item->m_writeIdx;
            m_item->m_writeIdx = 1 - m_item->m_writeIdx;
            m_item->m_frameUpdated = true;
        }

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

    mpv_set_option_string(m_mpv, "vo", "libmpv");
    mpv_set_option_string(m_mpv, "hwdec", "no");
    mpv_set_option_string(m_mpv, "config", "no");
    mpv_set_option_string(m_mpv, "msg-level", "all=debug");

    int ret = mpv_initialize(m_mpv);
    if (ret < 0) {
        qWarning() << "[MpvVideo] mpv_initialize failed, error code:" << ret
                   << "(" << mpv_error_string(ret) << ")";
        mpv_terminate_destroy(m_mpv);
        m_mpv = nullptr;
        return;
    }

    qDebug() << "[MpvVideo] mpv_initialize OK";

    mpv_request_log_messages(m_mpv, "info");
    mpv_set_wakeup_callback(m_mpv, onMpvWakeup, this);
}

VlcVideoItem::~VlcVideoItem()
{
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

// ══════════════════════════════════════════════════════════════════
// StreamProcessor 接口
// ══════════════════════════════════════════════════════════════════

QImage VlcVideoItem::grabFrame() const
{
    QMutexLocker lock(&m_frameMutex);
    if (m_readyIdx < 0 || m_frameBuf[m_readyIdx].isNull())
        return QImage();
    return m_frameBuf[m_readyIdx];
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

// ===== 播放控制 =====
void VlcVideoItem::setSource(const QString &url)
{
    if (m_source != url) {
        m_source = url;
        emit sourceChanged();
        ensureMpvCreated();  // QML 对象构造完成后，首次 setSource 时才初始化 mpv
        setupPlayer();
    }
}

void VlcVideoItem::play()
{
    if (!m_mpv) return;
    if (m_source.isEmpty()) return;

    m_playClicked = true;

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

    qDebug() << "[MpvVideo] doSetupPlayer, loading:" << m_source;

    // 先设为暂停状态，等用户点击 play() 再取消暂停
    int paused = 1;
    mpv_set_property(m_mpv, "pause", MPV_FORMAT_FLAG, &paused);

    // 观察属性变化以获取播放状态
    mpv_observe_property(m_mpv, 0, "pause",          MPV_FORMAT_FLAG);
    mpv_observe_property(m_mpv, 0, "duration",       MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "seekable",       MPV_FORMAT_FLAG);
    mpv_observe_property(m_mpv, 0, "video-params",   MPV_FORMAT_NODE);

    // 加载媒体文件/流
    const QByteArray url = m_source.toUtf8();
    const char *cmd[] = {"loadfile", url.constData(), nullptr};
    mpv_command_async(m_mpv, 0, cmd);

    // 设置音量和初始状态
    {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", m_volume);
        mpv_set_property_string(m_mpv, "volume", buf);
    }
}

void VlcVideoItem::releasePlayer()
{
    if (m_mpv) {
        // 不调 stop（会销毁 vo=libmpv），loadfile 会自动替换当前流
        mpv_unobserve_property(m_mpv, 0);
    }

    m_setupInProgress = false;
    m_needClearDisplay = true;   // 通知渲染器清空上一视频流的残留帧

    // 清空帧缓冲
    {
        QMutexLocker lock(&m_frameMutex);
        m_frameBuf[0] = QImage();
        m_frameBuf[1] = QImage();
        m_writeIdx = 0;
        m_readyIdx = -1;
        m_frameUpdated = false;
        m_hasProcessedFrame = false;
        m_processedFrame = QImage();
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
        qDebug() << "[MpvVideo] mousePressEvent at" << event->pos();
        requestPixelAt(event->pos().x(), event->pos().y());
        event->accept();
        return;
    }
    //如果不是左键，默认处理
    QQuickFramebufferObject::mousePressEvent(event);
}

// ══════════════════════════════════════════════════════════════════
// 像素读取 — 从 CPU 帧缓冲同步读取（做 Letterbox 坐标映射）
// ══════════════════════════════════════════════════════════════════

void VlcVideoItem::requestPixelAt(int x, int y)
{
    QImage frame;
    {
        QMutexLocker lock(&m_frameMutex);
        if (m_hasProcessedFrame && !m_processedFrame.isNull()) {
            frame = m_processedFrame;
        } else if (m_readyIdx >= 0 && !m_frameBuf[m_readyIdx].isNull()) {
            frame = m_frameBuf[m_readyIdx];
        }
    }

    if (frame.isNull()) {
        qDebug() << "[MpvVideo] requestPixelAt: no frame available";
        emit errorReadingPixel(QStringLiteral("无可用视频帧"));
        return;
    }

    const qreal itemW = static_cast<qreal>(width());
    const qreal itemH = static_cast<qreal>(height());
    const qreal videoW = static_cast<qreal>(frame.width());
    const qreal videoH = static_cast<qreal>(frame.height());

    if (itemW <= 0.0 || itemH <= 0.0 || videoW <= 0.0 || videoH <= 0.0) {
        emit errorReadingPixel(QStringLiteral("尺寸无效"));
        return;
    }

    const qreal scale = qMin(itemW / videoW, itemH / videoH);
    const qreal displayW = videoW * scale;
    const qreal displayH = videoH * scale;
    const qreal offsetX = (itemW - displayW) / 2.0;
    const qreal offsetY = (itemH - displayH) / 2.0;

    if (x < offsetX || x > offsetX + displayW ||
        y < offsetY || y > offsetY + displayH) {
        qDebug() << "[MpvVideo] click outside video area:" << x << y;
        emit errorReadingPixel(QStringLiteral("点击位置在视频画面之外"));
        return;
    }

    int frameX = qRound((x - offsetX) / displayW * videoW);
    int frameY = qRound((y - offsetY) / displayH * videoH);
    frameX = qBound(0, frameX, frame.width() - 1);
    frameY = qBound(0, frameY, frame.height() - 1);

    QColor color = frame.pixelColor(frameX, frameY);
    qDebug() << "[MpvVideo] pixelRead at (" << x << "," << y << ") → frame("
             << frameX << "," << frameY << ") =" << color;
    emit pixelRead(x, y);
}
