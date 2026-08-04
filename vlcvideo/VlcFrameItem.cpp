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
        // m_sharedTexId 是 VlcVideoItem 的纹理，不归我们删除
        if (m_vbo)     glDeleteBuffers(1, &m_vbo);
        if (m_program) glDeleteProgram(m_program);
    }

    void synchronize(QQuickFramebufferObject *item) override
    {
        Q_UNUSED(item);

        VlcVideoItem *src = m_item->source();
        if (!src) return;

        // 直接从 VlcVideoItem 获取共享的 GPU 纹理 ID，省掉 CPU 拷贝
        unsigned int texId = src->displayTextureId();
        if (texId == 0) return;

        m_sharedTexId = static_cast<GLuint>(texId);

        // 源帧尺寸（从 VlcVideoItem 的离屏 FBO）
        QImage refFrame = src->grabFrame();
        if (refFrame.isNull()) return;
        int fullW = refFrame.width();
        int fullH = refFrame.height();

        // ── 先裁剪掉帧内 mpv 留下的黑边，得到真实视频内容区域 ──
        // 离屏 FBO 的宽高比 = 主播放器控件的宽高比，视频内容在其中保持宽高比居中
        // （可能上下或左右留黑边）。放大镜若直接整帧贴图，这些黑边会被一并放大，
        // 出现四边黑框。这里用视频固有显示尺寸（dw/dh）反推出内容在帧中的位置。
        double ox = 0.0, oy = 0.0, cw = fullW, ch = fullH;
        {
            const QSize native = src->videoNativeSize();
            if (native.width() > 0 && native.height() > 0) {
                const double scale = qMin(double(fullW) / native.width(),
                                          double(fullH) / native.height());
                cw = native.width() * scale;
                ch = native.height() * scale;
                ox = (fullW - cw) / 2.0;
                oy = (fullH - ch) / 2.0;
            }
        }

        // ── 计算 zoom 区域（基于内容区域，单位为完整帧像素） ──
        qreal zf = m_item->zoomFactor();
        double zx = ox, zy = oy, zw = cw, zh = ch;
        if (zf > 1.0) {
            qreal cx = m_item->centerX();
            qreal cy = m_item->centerY();
            if (cx < 0) cx = ox + cw / 2.0;
            if (cy < 0) cy = oy + ch / 2.0;
            zw = cw / zf;
            zh = ch / zf;
            zx = qBound(ox, cx - zw / 2.0, ox + cw - zw);
            zy = qBound(oy, cy - zh / 2.0, oy + ch - zh);
        }

        // 纹理坐标变化时重建 quad
        if (zx != m_texU0 || zy != m_texV0 || zw != m_texW || zh != m_texH ||
            m_sharedTexWidth != fullW || m_sharedTexHeight != fullH) {
            m_texU0 = zx; m_texV0 = zy;
            m_texW  = zw; m_texH  = zh;
            m_sharedTexWidth = fullW;
            m_sharedTexHeight = fullH;
            updateQuadVertices();
        }

        QSize curItemSize(int(m_item->width()), int(m_item->height()));
        if (m_itemSize != curItemSize) {
            m_itemSize = curItemSize;
            updateQuadVertices();
        }
    }

    void render() override
    {
        initializeOpenGLFunctions();

        if (!m_glInitialized)
            initGL();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (m_sharedTexId && !m_vertices.isEmpty()) {
            glUseProgram(m_program);

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                         m_vertices.constData(), GL_DYNAMIC_DRAW);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_sharedTexId);
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

    // uploadTexture() 已移除 —— 放大镜现在直接绑定主播放器的共享 GL 纹理

    void updateQuadVertices()
    {
        m_vertices.clear();
        if (m_sharedTexWidth <= 0 || m_sharedTexHeight <= 0 || m_itemSize.isEmpty())
            return;

        // 缩放后的显示尺寸（letterbox，保持宽高比）
        float zoomW = m_texW;
        float zoomH = m_texH;
        float itemW = m_itemSize.width();
        float itemH = m_itemSize.height();

        float scale = qMin(itemW / zoomW, itemH / zoomH);
        float displayW = zoomW * scale;
        float displayH = zoomH * scale;

        float halfW = displayW / itemW;
        float halfH = displayH / itemH;

        // 归一化纹理坐标（zoom 区域在完整纹理中的位置）
        float u0 = m_texU0 / m_sharedTexWidth;
        float v0 = m_texV0 / m_sharedTexHeight;
        float u1 = (m_texU0 + zoomW) / m_sharedTexWidth;
        float v1 = (m_texV0 + zoomH) / m_sharedTexHeight;

        // 三角形带：左下 → 右下 → 左上 → 右上
        m_vertices = {
            -halfW, -halfH,   u0, v0,
             halfW, -halfH,   u1, v0,
            -halfW,  halfH,   u0, v1,
             halfW,  halfH,   u1, v1,
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

    // 共享纹理（来自 VlcVideoItem，不归我们创建/删除）
    GLuint m_sharedTexId = 0;
    int    m_sharedTexWidth  = 0;
    int    m_sharedTexHeight = 0;

    // Zoom 区域在共享纹理中的位置和大小（像素坐标）
    double m_texU0 = 0.0, m_texV0 = 0.0;
    double m_texW  = 0.0, m_texH  = 0.0;

    QVector<float> m_vertices;
    QSize m_itemSize;

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
