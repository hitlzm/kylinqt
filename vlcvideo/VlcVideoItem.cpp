#include "VlcVideoItem.h"
#include <vlc/vlc.h>
#include <QOpenGLFunctions>
#include <QQuickWindow>
#include <QDebug>

// ══════════════════════════════════════════════════════════════════
// 简易 GLSL Shader
// ══════════════════════════════════════════════════════════════════
static const char *kVertexShader =
    "attribute highp vec2 aPosition;\n"
    "attribute highp vec2 aTexCoord;\n"
    "varying highp vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
    "    vTexCoord = aTexCoord;\n"
    "}\n";

static const char *kFragmentShader =
    "varying highp vec2 vTexCoord;\n"
    "uniform sampler2D uTexture;\n"
    "void main() {\n"
    "    gl_FragColor = texture2D(uTexture, vTexCoord);\n"
    "}\n";

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
        if (m_texture)  glDeleteTextures(1, &m_texture);
        if (m_vbo)      glDeleteBuffers(1, &m_vbo);
        if (m_program)  glDeleteProgram(m_program);
    }

    void synchronize(QQuickFramebufferObject *item) override
    {
        Q_UNUSED(item);
        // GUI 线程被阻塞，可以安全读 m_item 的属性（不含互斥锁）
        QMutexLocker lock(&m_item->m_frameMutex);

        if (m_item->m_frameUpdated && !m_item->m_frame.isNull()) {
            m_frameCopy = m_item->m_frame.copy();   // 深拷贝，渲染线程独占
            m_videoSize = m_frameCopy.size();
            m_textureDirty = true;
            m_item->m_frameUpdated = false;
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

        if (!m_glInitialized)
            initGL();

        if (m_textureDirty && !m_frameCopy.isNull())
            uploadTexture();

        // 清背景为黑色
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

            // position: 2 floats, stride 4 floats
            GLint posLoc = glGetAttribLocation(m_program, "aPosition");
            glEnableVertexAttribArray(posLoc);
            glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE,
                                  4 * sizeof(float), reinterpret_cast<void*>(0));

            // texcoord: 2 floats, stride 4 floats, offset 2 floats
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
    }

    // ---- 上传 QImage 到 GL 纹理 ----
    void uploadTexture()
    {
        QImage tex = m_frameCopy.convertToFormat(QImage::Format_RGBA8888);

        if (!m_texture)
            glGenTextures(1, &m_texture);

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     tex.width(), tex.height(), 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);
        m_textureDirty = false;
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

        // 保持宽高比，居中缩放
        float scale = qMin(itemW / videoW, itemH / videoH);
        float displayW = videoW * scale;
        float displayH = videoH * scale;

        // 归一化到 NDC [-1, 1]
        float halfW = displayW / itemW;
        float halfH = displayH / itemH;

        // 三角形带：左下 → 右下 → 左上 → 右上
        // VLC 解码输出为 bottom-up（首行 = 画面底部），
        // 经 glTexImage2D 后 GL 纹理 V=0 处为画面底部，V=1 处为画面顶部
        // clang-format off
        m_vertices = {
            -halfW, -halfH,   0.0f, 0.0f,   // 左下 → V=0 → 画面底部
             halfW, -halfH,   1.0f, 0.0f,   // 右下 → V=0 → 画面底部
            -halfW,  halfH,   0.0f, 1.0f,   // 左上 → V=1 → 画面顶部
             halfW,  halfH,   1.0f, 1.0f,   // 右上 → V=1 → 画面顶部
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
            qWarning() << "[VlcVideo] Shader link failed:" << log;
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
            qWarning() << "[VlcVideo] Shader compile failed:" << log;
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
};

// ══════════════════════════════════════════════════════════════════
// VlcVideoItem — 主线程 / QML 可见部分
// ══════════════════════════════════════════════════════════════════

VlcVideoItem::VlcVideoItem(QQuickItem *parent)
    : QQuickFramebufferObject(parent)
{
    const char *args[] = {
        "--intf", "dummy",
        "--no-video-title-show",
        "--no-xlib"
    };
    m_vlcInstance = libvlc_new(sizeof(args)/sizeof(args[0]), args);
}

VlcVideoItem::~VlcVideoItem()
{
    releasePlayer();
    if (m_vlcInstance)
        libvlc_release(m_vlcInstance);
}

// ---- QQuickFramebufferObject 接口 ----
QQuickFramebufferObject::Renderer *VlcVideoItem::createRenderer() const
{
    return new VlcVideoRenderer(const_cast<VlcVideoItem*>(this));
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
        if (m_player) {
            libvlc_audio_set_volume(m_player, vol);
        }
        emit volumeChanged();
    }
}

float VlcVideoItem::position() const
{
    if (!m_player) return 0.0f;
    return libvlc_media_player_get_position(m_player);
}

void VlcVideoItem::setPosition(float pos)
{
    if (m_player)
        libvlc_media_player_set_position(m_player, pos);
}

qint64 VlcVideoItem::length() const
{
    if (!m_player) return 0;
    return libvlc_media_player_get_length(m_player);
}

bool VlcVideoItem::isSeekable() const { return m_seekable; }

// ===== 播放控制 =====
void VlcVideoItem::setSource(const QString &url)
{
    if (m_source != url) {
        m_source = url;
        emit sourceChanged();
        setupPlayer();
    }
}

void VlcVideoItem::play()
{
    if (!m_player) {
        if (m_source.isEmpty()) return;
        setupPlayer();
    }
    if (m_player) {
        libvlc_media_player_play(m_player);
    }
}

void VlcVideoItem::pause()
{
    if (m_player)
        libvlc_media_player_pause(m_player);
}

void VlcVideoItem::stop()
{
    releasePlayer();
    m_playing = false;
    emit playingChanged();
    emit stopped();
}

// ===== 内部 =====
void VlcVideoItem::setupPlayer()
{
    releasePlayer();

    if (m_source.isEmpty() || !m_vlcInstance)
        return;

    m_media = libvlc_media_new_location(m_vlcInstance, m_source.toUtf8().constData());
    if (!m_media) return;

    m_player = libvlc_media_player_new_from_media(m_media);
    libvlc_media_release(m_media);

    libvlc_video_set_callbacks(m_player, lockCallback, unlockCallback, displayCallback, this);
    libvlc_video_set_format_callbacks(m_player, setupFormatCallback, nullptr);
    libvlc_audio_set_volume(m_player, m_volume);

    attachEvents();
}

void VlcVideoItem::releasePlayer()
{
    if (m_player) {
        detachEvents();
        libvlc_media_player_stop(m_player);
        libvlc_media_player_release(m_player);
        m_player = nullptr;
        m_eventManager = nullptr;
    }
}

// ===== libvlc 事件回调（自由函数，C 调用约定兼容 libvlc_callback_t）=====
void onLibVlcEvent(const libvlc_event_t *event, void *opaque)
{
    auto *self = static_cast<VlcVideoItem*>(opaque);
    switch (event->type) {
    case libvlc_MediaPlayerPlaying: {
        bool was = self->m_playing;
        self->m_playing = true;
        if (!was) self->playingChanged();
        break;
    }
    case libvlc_MediaPlayerPaused:
        self->m_playing = false;
        self->playingChanged();
        break;
    case libvlc_MediaPlayerStopped:
        self->m_playing = false;
        self->playingChanged();
        self->stopped();
        break;
    case libvlc_MediaPlayerEndReached:
        self->m_playing = false;
        self->playingChanged();
        self->ended();
        break;
    case libvlc_MediaPlayerEncounteredError:
        self->m_playing = false;
        self->playingChanged();
        self->error("Playback error");
        break;
    case libvlc_MediaPlayerLengthChanged:
        self->lengthChanged();
        break;
    case libvlc_MediaPlayerSeekableChanged:
        self->m_seekable = libvlc_media_player_is_seekable(self->m_player);
        self->seekableChanged();
        break;
    default: break;
    }
}

// ===== libvlc 事件绑定 =====
void VlcVideoItem::attachEvents()
{
    if (!m_player) return;
    m_eventManager = libvlc_media_player_event_manager(m_player);
    if (!m_eventManager) return;

    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerPlaying,       onLibVlcEvent, this);
    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerPaused,        onLibVlcEvent, this);
    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerStopped,       onLibVlcEvent, this);
    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerEndReached,    onLibVlcEvent, this);
    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerEncounteredError, onLibVlcEvent, this);
    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerLengthChanged, onLibVlcEvent, this);
    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerSeekableChanged, onLibVlcEvent, this);
}

void VlcVideoItem::detachEvents()
{
    if (!m_eventManager) return;
    libvlc_event_detach(m_eventManager, libvlc_MediaPlayerPlaying,       onLibVlcEvent, this);
    libvlc_event_detach(m_eventManager, libvlc_MediaPlayerPaused,        onLibVlcEvent, this);
    libvlc_event_detach(m_eventManager, libvlc_MediaPlayerStopped,       onLibVlcEvent, this);
    libvlc_event_detach(m_eventManager, libvlc_MediaPlayerEndReached,    onLibVlcEvent, this);
    libvlc_event_detach(m_eventManager, libvlc_MediaPlayerEncounteredError, onLibVlcEvent, this);
    libvlc_event_detach(m_eventManager, libvlc_MediaPlayerLengthChanged, onLibVlcEvent, this);
    libvlc_event_detach(m_eventManager, libvlc_MediaPlayerSeekableChanged, onLibVlcEvent, this);
}

// ===== 视频帧回调 =====
void* VlcVideoItem::lockCallback(void *opaque, void **planes)
{
    auto *self = static_cast<VlcVideoItem*>(opaque);
    self->m_frameMutex.lock();
    *planes = self->m_frame.bits();
    return nullptr;
}

void VlcVideoItem::unlockCallback(void *opaque, void *, void *const *)
{
    auto *self = static_cast<VlcVideoItem*>(opaque);
    self->m_frameMutex.unlock();
}

void VlcVideoItem::displayCallback(void *opaque, void *)
{
    auto *self = static_cast<VlcVideoItem*>(opaque);
    {
        QMutexLocker lock(&self->m_frameMutex);
        self->m_frameUpdated = true;
    }
    // 触发 scene graph → synchronize() → render()
    QMetaObject::invokeMethod(self, "update", Qt::QueuedConnection);
}

unsigned VlcVideoItem::setupFormatCallback(void **opaque, char *chroma, unsigned *width, unsigned *height,
                                           unsigned *pitches, unsigned *lines)
{
    auto *self = static_cast<VlcVideoItem*>(*opaque);
    self->m_width = *width;
    self->m_height = *height;
    memcpy(chroma, "RGBA", 4);
    QMutexLocker lock(&self->m_frameMutex);
    self->m_frame = QImage(*width, *height, QImage::Format_RGBA8888);
    *pitches = self->m_frame.bytesPerLine();
    *lines = *height;
    return 1;
}
