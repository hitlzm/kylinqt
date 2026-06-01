#include "vlcplayer.h"

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Audio.h>

VLCPlayer::VLCPlayer(QObject *parent)
    : QObject(parent)
    , m_instance(nullptr)
    , m_media(nullptr)
    , m_player(nullptr)
    , m_volume(100)
    , m_playing(false)
{
    // 添加 --no-video 禁止 C++ 侧弹出独立视频窗口（视频由 QML VlcVideoPlayer 渲染）
    QStringList args = VlcCommon::args();
    args << "--no-video"<< "--vout=direct3d9";  //<< "--no-vout"
    m_instance = new VlcInstance(args, this);
    m_player = new VlcMediaPlayer(m_instance);
    m_media = nullptr;

    connect(m_player, &VlcMediaPlayer::stateChanged,
            this, &VLCPlayer::onStateChanged);
    connect(m_player, &VlcMediaPlayer::lengthChanged,
            this, &VLCPlayer::onLengthChanged);
    connect(m_player, &VlcMediaPlayer::seekableChanged,
            this, &VLCPlayer::onSeekableChanged);
}

VLCPlayer::~VLCPlayer()
{
    stop();
    delete m_media;
    delete m_player;
    delete m_instance;
}

QUrl VLCPlayer::url() const { return m_url; }

void VLCPlayer::setUrl(const QUrl &url)
{
    if (m_url != url) {
        m_url = url;
        emit urlChanged();
    }
}

bool VLCPlayer::isPlaying() const { return m_playing; }

int VLCPlayer::volume() const { return m_volume; }

void VLCPlayer::setVolume(int volume)
{
    volume = qBound(0, volume, 200);
    if (m_volume != volume) {
        m_volume = volume;
        m_player->audio()->setVolume(volume);
        emit volumeChanged();
    }
}

float VLCPlayer::position() const { return m_player->position(); }

void VLCPlayer::setPosition(float pos) { m_player->setPosition(pos); }

qint64 VLCPlayer::length() const { return m_player->length(); }

bool VLCPlayer::isSeekable() const { return m_player->seekable(); }

void VLCPlayer::play()
{
    if (!m_media) {
        delete m_media;
        m_media = new VlcMedia(m_url.toString(QUrl::FullyEncoded), m_instance);
        m_player->open(m_media);
    }
    if (m_player->state() != Vlc::Playing) {
        m_player->play();
    }
}

void VLCPlayer::pause()
{
    m_player->pause();
}

void VLCPlayer::stop()
{
    m_player->stop();
}

void VLCPlayer::openLocal() { /* QML 侧用 FileDialog 选择后 setUrl */ }

void VLCPlayer::openUrl(const QUrl &url)
{
    setUrl(url);
    // 不自动 play()——视频渲染由 QML VlcVideoPlayer 独立控制
    // C++ 侧 player 仅在需要音频/状态同步时才启动
}

void VLCPlayer::onStateChanged()
{
    bool wasPlaying = m_playing;
    m_playing = (m_player->state() == Vlc::Playing
              || m_player->state() == Vlc::Buffering
              || m_player->state() == Vlc::Opening);
    if (wasPlaying != m_playing)
        emit playingChanged();

    if (m_player->state() == Vlc::Ended) {
        emit ended();
    } else if (m_player->state() == Vlc::Error) {
        emit error("Playback error");
    }
}

void VLCPlayer::onLengthChanged(qint64 length)
{
    emit lengthChanged();
}

void VLCPlayer::onSeekableChanged(bool seekable)
{
    emit seekableChanged();
}
