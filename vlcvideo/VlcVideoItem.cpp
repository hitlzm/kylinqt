#include "VlcVideoItem.h"
#include <vlc/vlc.h>
#include <QPainter>
#include <QDebug>

VlcVideoItem::VlcVideoItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
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

// ===== 属性访问 =====
QString VlcVideoItem::source() const { return m_source; }

bool VlcVideoItem::isPlaying() const { return m_playing; }

int VlcVideoItem::volume() const { return m_volume; }

void VlcVideoItem::setVolume(int vol)
{
    vol = qBound(0, vol, 200);
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
        setupPlayer();  // 创建 player 但不自动播放
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
    // 不自动 play() — 等待 QML 侧调用
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
    QMetaObject::invokeMethod(self, "update", Qt::QueuedConnection);
}

unsigned VlcVideoItem::setupFormatCallback(void **opaque, char * chroma, unsigned *width, unsigned *height,
                                           unsigned *pitches, unsigned *lines)
{
    auto *self = static_cast<VlcVideoItem*>(*opaque);
    self->m_width = *width;
    self->m_height = *height;
    memcpy(chroma, "RGBA", 4);
    QMutexLocker lock(&self->m_frameMutex);
    // self->m_frame = QImage(*width, *height, QImage::Format_ARGB32_Premultiplied);
    self->m_frame = QImage(*width, *height, QImage::Format_RGBA8888);
    *pitches = self->m_frame.bytesPerLine();
    *lines = *height;
    return 1;
}

void VlcVideoItem::paint(QPainter *painter)
{
    QMutexLocker lock(&m_frameMutex);
    if (!m_frame.isNull()) {
        // painter->drawImage(boundingRect(), m_frame);
        QSize videoSize = m_frame.size().scaled(width(), height(), Qt::KeepAspectRatio);
        QRect targetRect((width() - videoSize.width()) / 2,
                 (height() - videoSize.height()) / 2,
                 videoSize.width(), videoSize.height());
        painter->drawImage(targetRect, m_frame);
    }
}
