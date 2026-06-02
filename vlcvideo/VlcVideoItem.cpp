#include "VlcVideoItem.h"
#include <vlc/vlc.h>
#include <QPainter>
#include <QDebug>

VlcVideoItem::VlcVideoItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    // 准备 libvlc 实例（通常整个应用一个即可，这里简单示例）
    const char *args[] = {
        "--intf", "dummy",        // 无界面
        "--no-video-title-show",
        "--no-xlib"               // Linux 避免 X11 依赖（如不需要）
    };
    m_vlcInstance = libvlc_new(sizeof(args)/sizeof(args[0]), args);
}

VlcVideoItem::~VlcVideoItem()
{
    releasePlayer();
    if (m_vlcInstance)
        libvlc_release(m_vlcInstance);
}

QString VlcVideoItem::source() const
{
    return m_source;
}

void VlcVideoItem::setSource(const QString &url)
{
    if (m_source != url) {
        m_source = url;
        emit sourceChanged();
        setupPlayer();
    }
}

void VlcVideoItem::setupPlayer()
{
    releasePlayer();

    if (m_source.isEmpty() || !m_vlcInstance)
        return;

    m_media = libvlc_media_new_location(m_vlcInstance, m_source.toUtf8().constData());
    if (!m_media)
        return;

    m_player = libvlc_media_player_new_from_media(m_media);
    libvlc_media_release(m_media); // player 持有引用，可以安全释放

    // 禁用内置视频输出，走回调
    libvlc_video_set_callbacks(m_player, lockCallback, unlockCallback, displayCallback, this);
    // 注册格式协商回调 → setupFormatCallback 里根据视频尺寸创建 m_frame
    libvlc_video_set_format_callbacks(m_player, setupFormatCallback, nullptr);
    libvlc_media_player_play(m_player);
}

void VlcVideoItem::releasePlayer()
{
    if (m_player) {
        libvlc_media_player_stop(m_player);
        libvlc_media_player_release(m_player);
        m_player = nullptr;
    }
}

// 静态回调：锁帧缓冲区，VLC 会在这里分配 planes
void* VlcVideoItem::lockCallback(void *opaque, void **planes)
{
    auto *self = static_cast<VlcVideoItem*>(opaque);
    self->m_frameMutex.lock();

    // 返回一个足够大的缓冲区，用 QImage 管理
    *planes = self->m_frame.bits();
    return nullptr; // 用于 unlock 的不透明对象，这里不需要
}

void VlcVideoItem::unlockCallback(void *opaque, void *picture, void *const *planes)
{
    auto *self = static_cast<VlcVideoItem*>(opaque);
    self->m_frameMutex.unlock();
}

void VlcVideoItem::displayCallback(void *opaque, void *picture)
{
    auto *self = static_cast<VlcVideoItem*>(opaque);
    // 数据已写入 planes[0]，直接触发重绘
    QMetaObject::invokeMethod(self, "update", Qt::QueuedConnection);
    // 或者使用信号槽，确保 UI 线程刷新
}

void VlcVideoItem::paint(QPainter *painter)
{
    QMutexLocker lock(&m_frameMutex);
    if (!m_frame.isNull()) {
        painter->drawImage(boundingRect(), m_frame);
    }
}

// 额外：在格式协商时动态调整缓冲区大小
unsigned VlcVideoItem::setupFormatCallback(void **opaque, char *chroma,
                                    unsigned *width, unsigned *height,
                                    unsigned *pitches,
                                    unsigned *lines)
{
    auto *self = static_cast<VlcVideoItem*>(*opaque);
    self->m_width = *width;
    self->m_height = *height;

    // 重新创建 QImage
    QMutexLocker lock(&self->m_frameMutex);
    self->m_frame = QImage(*width, *height, QImage::Format_ARGB32_Premultiplied);
    *pitches = self->m_frame.bytesPerLine();
    *lines = *height;
    return 1; // 返回分配的缓冲区个数
}