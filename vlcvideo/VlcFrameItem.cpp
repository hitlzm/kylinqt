#include "VlcFrameItem.h"
#include "VlcVideoItem.h"
#include "videodisplaygeometry.h"
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

        // 源帧尺寸：改用 VlcVideoItem 的属性（离屏 FBO 尺寸），
        // 不再依赖 grabFrame()——导引头模式关闭 CPU 回读后 grabFrame 返回空
        int fullW = qRound(src->frameWidth());
        int fullH = qRound(src->frameHeight());
        if (fullW <= 0 || fullH <= 0) return;

        // 主播放器的离屏纹理已经是视频原生画面。放大镜必须复用完全相同的
        // 旋转和裁剪参数，否则会出现主画面已摆正、放大镜仍然倾斜。
        const VideoDisplayGeometry::Geometry geometry =
                VideoDisplayGeometry::calculate(
                    fullW, fullH, src->displayRotationDegrees());
        if (geometry.isEmpty())
            return;

        // 当前放大镜默认 zoomFactor=1。保留原有缩放参数能力：缩放区域仍以
        // 旋转后的安全裁剪区中心为基准，避免切换到旧的源纹理坐标。
        qreal zf = m_item->zoomFactor();
        const QSizeF displaySize(geometry.cropWidth / zf,
                                 geometry.cropHeight / zf);

        // 几何参数变化时重建 quad
        if (displaySize != m_displaySize ||
            geometry.centerX != m_cropCenter.x() ||
            geometry.centerY != m_cropCenter.y() ||
            geometry.rotationCos != m_rotationCos ||
            geometry.rotationSin != m_rotationSin ||
            m_sharedTexWidth != fullW || m_sharedTexHeight != fullH) {
            m_geometry = geometry;
            m_displaySize = displaySize;
            m_cropCenter = QPointF(geometry.centerX, geometry.centerY);
            m_rotationCos = geometry.rotationCos;
            m_rotationSin = geometry.rotationSin;
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
            glUniform2f(glGetUniformLocation(m_program, "uSourceSize"),
                        static_cast<float>(m_geometry.sourceWidth),
                        static_cast<float>(m_geometry.sourceHeight));
            glUniform2f(glGetUniformLocation(m_program, "uCropSize"),
                        static_cast<float>(m_displaySize.width()),
                        static_cast<float>(m_displaySize.height()));
            glUniform2f(glGetUniformLocation(m_program, "uCropCenter"),
                        static_cast<float>(m_cropCenter.x()),
                        static_cast<float>(m_cropCenter.y()));
            glUniform2f(glGetUniformLocation(m_program, "uRotationCosSin"),
                        static_cast<float>(m_rotationCos),
                        static_cast<float>(m_rotationSin));

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
        m_program = buildProgram(kVertexShader,
                                 VideoDisplayGeometry::fragmentShader());
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
        float zoomW = m_displaySize.width();
        float zoomH = m_displaySize.height();
        float itemW = m_itemSize.width();
        float itemH = m_itemSize.height();

        float scale = qMin(itemW / zoomW, itemH / zoomH);
        float displayW = zoomW * scale;
        float displayH = zoomH * scale;

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

    // 共享纹理（来自 VlcVideoItem，不归我们创建/删除）
    GLuint m_sharedTexId = 0;
    int    m_sharedTexWidth  = 0;
    int    m_sharedTexHeight = 0;

    VideoDisplayGeometry::Geometry m_geometry;
    QSizeF m_displaySize;
    QPointF m_cropCenter;
    double m_rotationCos = 1.0;
    double m_rotationSin = 0.0;

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
