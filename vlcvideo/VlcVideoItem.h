#ifndef VLCVIDEOITEM_H
#define VLCVIDEOITEM_H

#include <QtQuick/QQuickPaintedItem>
#include <QImage>
#include <QMutex>

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_event_manager_t;
struct libvlc_event_t;

class VlcVideoItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(float position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 length READ length NOTIFY lengthChanged)
    Q_PROPERTY(bool seekable READ isSeekable NOTIFY seekableChanged)

public:
    explicit VlcVideoItem(QQuickItem *parent = nullptr);
    ~VlcVideoItem();

    QString source() const;
    void setSource(const QString &url);

    bool isPlaying() const;
    int volume() const;
    void setVolume(int vol);
    float position() const;
    void setPosition(float pos);
    qint64 length() const;
    bool isSeekable() const;

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

signals:
    void sourceChanged();
    void playingChanged();
    void volumeChanged();
    void positionChanged();
    void lengthChanged();
    void seekableChanged();
    void stopped();
    void ended();
    void error(const QString &errorMsg);

protected:
    void paint(QPainter *painter) override;

private:
    void setupPlayer();
    void releasePlayer();
    void attachEvents();
    void detachEvents();

    // ---- libvlc 视频帧回调 ----
    static void* lockCallback(void *opaque, void **planes);
    static void unlockCallback(void *opaque, void *picture, void *const *planes);
    static void displayCallback(void *opaque, void *picture);
    static unsigned setupFormatCallback(void **opaque, char *chroma,
                                        unsigned *width, unsigned *height,
                                        unsigned *pitches, unsigned *lines);

    // ---- libvlc 事件回调 ----
    friend void onLibVlcEvent(const libvlc_event_t *event, void *opaque);

    QString m_source;
    libvlc_instance_t *m_vlcInstance = nullptr;
    libvlc_media_t *m_media = nullptr;
    libvlc_media_player_t *m_player = nullptr;
    libvlc_event_manager_t *m_eventManager = nullptr;

    QImage m_frame;
    QMutex m_frameMutex;
    unsigned m_width = 0;
    unsigned m_height = 0;

    bool m_playing = false;
    int m_volume = 100;
    bool m_seekable = false;
};

#endif
