#ifndef VLCVIDEOITEM_H
#define VLCVIDEOITEM_H

#include <QtQuick/QQuickPaintedItem>
#include <QImage>
#include <QMutex>
#include <memory>

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_player_t;

class VlcVideoItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit VlcVideoItem(QQuickItem *parent = nullptr);
    ~VlcVideoItem();

    QString source() const;
    void setSource(const QString &url);

signals:
    void sourceChanged();

protected:
    void paint(QPainter *painter) override;

private:
    void setupPlayer();
    void releasePlayer();

    static void* lockCallback(void *opaque, void **planes);
    static void unlockCallback(void *opaque, void *picture, void *const *planes);
    static void displayCallback(void *opaque, void *picture);
    static unsigned setupFormatCallback(void **opaque, char *chroma,
                                        unsigned *width, unsigned *height,
                                        unsigned *pitches, unsigned *lines);

    QString m_source;
    libvlc_instance_t *m_vlcInstance = nullptr;
    libvlc_media_t *m_media = nullptr;
    libvlc_media_player_t *m_player = nullptr;

    QImage m_frame;
    QMutex m_frameMutex;
    unsigned m_width = 0;
    unsigned m_height = 0;
};

#endif