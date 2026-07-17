#ifndef VLCFRAMEITEM_H
#define VLCFRAMEITEM_H

#include <QtQuick/QQuickFramebufferObject>
#include <QImage>

class VlcVideoItem;

/// 轻量级视频帧查看器 — 不创建独立的 VLC 实例，
/// 直接从共享的 VlcVideoItem 帧缓冲读取并渲染。
/// 用在放大镜等"一帧多显"场景。
/// 后期可增加标志位，当使用CCD视频画面时，grabSourceFrame的逻辑需要改一改，确定什么时候显示导引头，什么时候显示CCD
class VlcFrameItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(VlcVideoItem* source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged)
    Q_PROPERTY(qreal centerX READ centerX WRITE setCenterX NOTIFY centerXChanged)
    Q_PROPERTY(qreal centerY READ centerY WRITE setCenterY NOTIFY centerYChanged)

public:
    explicit VlcFrameItem(QQuickItem *parent = nullptr);

    VlcVideoItem *source() const;
    void setSource(VlcVideoItem *src);

    qreal zoomFactor() const;
    void setZoomFactor(qreal factor);

    qreal centerX() const;
    void setCenterX(qreal cx);
    qreal centerY() const;
    void setCenterY(qreal cy);

    // 供 Renderer 调用 —— 从源获取最新帧
    QImage grabSourceFrame() const;

    // QQuickFramebufferObject 接口
    Renderer *createRenderer() const override;

signals:
    void sourceChanged();
    void zoomFactorChanged();
    void centerXChanged();
    void centerYChanged();

private:
    VlcVideoItem *m_source = nullptr;
    qreal m_zoomFactor = 1.0;    // 1.0 = 全帧, >1.0 = 放大
    qreal m_centerX   = -1.0;   // -1 = 帧中心
    qreal m_centerY   = -1.0;
};

#endif // VLCFRAMEITEM_H
