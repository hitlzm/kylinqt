#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include <QObject>
#include <QUrl>

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

// VLC-QT QML版播放器
class VLCPlayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(float position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 length READ length NOTIFY lengthChanged)
    Q_PROPERTY(bool seekable READ isSeekable NOTIFY seekableChanged)

public:
    explicit VLCPlayer(QObject *parent = nullptr);
    ~VLCPlayer();

    QUrl url() const;
    void setUrl(const QUrl &url);   //输入RTSP拉流地址：rtsp://192.168.1.100:554/stream

    bool isPlaying() const;
    int volume() const;
    void setVolume(int volume);
    float position() const;
    void setPosition(float pos);
    qint64 length() const;
    bool isSeekable() const;

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void openLocal();
    Q_INVOKABLE void openUrl(const QUrl &url);

signals:
    void urlChanged();
    void playingChanged();
    void volumeChanged();
    void positionChanged();
    void lengthChanged();
    void seekableChanged();
    void stopped();
    void ended();
    void error(const QString &errorMsg);

private slots:
    void onStateChanged();
    void onLengthChanged(qint64 length);
    void onSeekableChanged(bool seekable);

private:
    VlcInstance *m_instance;
    VlcMedia *m_media;
    VlcMediaPlayer *m_player;

    QUrl m_url;
    int m_volume;
    bool m_playing;
};

#endif // VLCPLAYER_H
