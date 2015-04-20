#ifndef VIDEOVIEWVLC_H
#define VIDEOVIEWVLC_H

#include <stdint.h>

#include <vlc-qt/WidgetVideo.h>
#include <vlc-qt/VideoMemoryStream.h>

#include "mouse_select.h"
#include "util/media_player_vlc.h"

class VideoViewVlc;

#define VLC_FOURCC( a, b, c, d ) \
        ( ((uint32_t)a) | ( ((uint32_t)b) << 8 ) \
           | ( ((uint32_t)c) << 16 ) | ( ((uint32_t)d) << 24 ) )

class VlcPainter : public VlcVideoMemoryStream
{
public:
    void handlePaint(QPaintEvent *event);

    VideoViewVlc *widget;
private:
    virtual void *lockCallback(void **planes) override;
    virtual void unlockCallback(void *picture,void *const *planes) override;
    virtual void displayCallback(void *picture) override;
    virtual unsigned formatCallback(char *chroma,
                                    unsigned *width, unsigned *height,
                                    unsigned *pitches,
                                    unsigned *lines) override;
    virtual void formatCleanUpCallback() override;
    QRect scaleToAspect(QRect srcRect, int w, int h) const;
    QRect drawFrameRect() const;

    QImage frame;
    QByteArray plane;
    QMutex mutex;
};

class Video;

class VideoViewVlc : public VlcWidgetVideo
{
    Q_OBJECT
public:
    explicit VideoViewVlc(Video *video);
    virtual ~VideoViewVlc();

    virtual void paintEvent(QPaintEvent *event) override;
    virtual void setPlayer(MediaPlayerVlc *player);

private:
    Video *video;
    MouseSelect mouseSelect;
    VlcPainter *painter;
    int pulses;
};

#endif // VIDEOVIEWVLC_H
