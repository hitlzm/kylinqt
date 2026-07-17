#include "VlcFrameItem.h"
#include "VlcVideoItem.h"
#include <QOpenGLFunctions>
#include <QQuickWindow>
#include <QtMath>

// ═══════════════════════════════════════════════════════════
// GLSL Shader — 与 VlcVideoRenderer 共用相同的着色器代码
// ═══════════════════════════════════════════════════════════
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

// ═══════════════════════════════════════════════════════════
// VlcFrameRenderer — 独立的 OpenGL 渲染器
// ═══════════════════════════════════════════════════════════
class VlcFrameRenderer : public QQuickFramebufferObject::Renderer,
                          protected QOpenGLFunctions
{
public:
    VlcFrameRenderer(VlcFrameItem *item)
        : m_item(item) {}

    ~VlcFrameRenderer() override
    {
        if (!m_glInitialized) return;
        if (m_texture) glDeleteTextures(1, &m_texture);
        if (m_vbo)     glDeleteBuffers(1, &m_vbo);
        if (m_program) glDeleteProgram(m_program);
    }

    void synchronize(QQuickFramebufferObject *item) override
    {
        Q_UNUSED(item);

        VlcVideoItem *src = m_item->source();
        if (!src) return;

        // 注意：不能用 cacheKey 判断帧变化！
        // VLC 回调每次都往同一个 QImage 缓冲区覆写像素，
        // QImage 底层数据指针不变 → cacheKey 永远相同。
        // 因此必须每帧都拷贝和上传。
        QImage srcFrame = src->grabFrame();
        if (srcFrame.isNull()) return;

        // ── Zoom 区域计算 ──────────────────────────────
        QRectF srcRect(0, 0, srcFrame.width(), srcFrame.height());
        qreal zf = m_item->zoomFactor();
        if (zf > 1.0) {
            qreal cx = m_item->centerX();
            qreal cy = m_item->centerY();
            if (cx < 0) cx = srcFrame.width()  / 2.0;
            if (cy < 0) cy = srcFrame.height() / 2.0;

            qreal zw = srcFrame.width()  / zf;
            qreal zh = srcFrame.height() / zf;
            qreal zx = qBound(0.0, cx - zw / 2.0, srcFrame.width()  - zw);
            qreal zy = qBound(0.0, cy - zh / 2.0, srcFrame.height() - zh);
            srcRect = QRectF(zx, zy, zw, zh);
        }

        // 每帧都强制拷贝（VLC 随时在覆写源帧数据）
        m_frameCopy = srcFrame.copy(QRect(qRound(srcRect.x()), qRound(srcRect.y()),
                                          qRound(srcRect.width()), qRound(srcRect.height())));
        m_videoSize = m_frameCopy.size();
        m_textureDirty = true;

        // 窗口尺寸变化 → 重算 quad 顶点
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

        m_item->window()->resetOpenGLState();
    }

private:
    void initGL()
    {
        m_program = buildProgram(kVertexShader, kFragmentShader);
        glGenBuffers(1, &m_vbo);
        m_glInitialized = true;
    }

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

    void updateQuadVertices()
    {
        m_vertices.clear();
        if (m_videoSize.isEmpty() || m_itemSize.isEmpty()) return;

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
        m_vertices = {
            -halfW, -halfH,   0.0f, 0.0f,
             halfW, -halfH,   1.0f, 0.0f,
            -halfW,  halfH,   0.0f, 1.0f,
             halfW,  halfH,   1.0f, 1.0f,
        };
    }

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
            qWarning() << "[VlcFrame] Shader link failed:" << log;
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
            qWarning() << "[VlcFrame] Shader compile failed:" << log;
            glDeleteShader(s);
            return 0;
        }
        return s;
    }

    VlcFrameItem *m_item;

    QImage   m_frameCopy;
    QSize    m_videoSize;
    QSize    m_lastQuadVideoSize;
    bool     m_textureDirty = false;

    QVector<float> m_vertices;
    QSize m_itemSize;

    GLuint m_texture = 0;
    GLuint m_program = 0;
    GLuint m_vbo     = 0;
    bool   m_glInitialized = false;

};

// ═══════════════════════════════════════════════════════════
// VlcFrameItem 实现
// ═══════════════════════════════════════════════════════════

VlcFrameItem::VlcFrameItem(QQuickItem *parent)
    : QQuickFramebufferObject(parent)
{
}

QQuickFramebufferObject::Renderer *VlcFrameItem::createRenderer() const
{
    return new VlcFrameRenderer(const_cast<VlcFrameItem*>(this));
}

// ── 属性访问 ─────────────────────────────────────────────

VlcVideoItem *VlcFrameItem::source() const { return m_source; }

void VlcFrameItem::setSource(VlcVideoItem *src)
{
    if (m_source != src) {
        m_source = src;
        emit sourceChanged();
        update();   // 触发重绘
    }
}

qreal VlcFrameItem::zoomFactor() const { return m_zoomFactor; }

void VlcFrameItem::setZoomFactor(qreal factor)
{
    factor = qMax(1.0, factor);
    if (!qFuzzyCompare(m_zoomFactor, factor)) {
        m_zoomFactor = factor;
        emit zoomFactorChanged();
        update();
    }
}

qreal VlcFrameItem::centerX() const { return m_centerX; }
void  VlcFrameItem::setCenterX(qreal cx) { if (m_centerX != cx) { m_centerX = cx; emit centerXChanged(); update(); } }
qreal VlcFrameItem::centerY() const { return m_centerY; }
void  VlcFrameItem::setCenterY(qreal cy) { if (m_centerY != cy) { m_centerY = cy; emit centerYChanged(); update(); } }

QImage VlcFrameItem::grabSourceFrame() const
{
    if (!m_source) return QImage();
    return m_source->grabFrame();
}
