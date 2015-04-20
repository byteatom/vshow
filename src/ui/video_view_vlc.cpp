#include "video_view_vlc.h"

#include <cassert>

#include <QApplication>
#include <QPainter>

#include "util/win_util.h"
#include "video.h"
#include "qlog/qlog.h"

typedef vlc_chroma_description_t *(*Pf_vlc_fourcc_GetChromaDescription)(
    uint32_t fourcc);

static Pf_vlc_fourcc_GetChromaDescription vlc_fourcc_GetChromaDescription
    = nullptr;

void VlcPainter::handlePaint(QPaintEvent *event) {
    QPainter painter(widget);
    mutex.lock();
    painter.drawImage(drawFrameRect(),
                      QImage(reinterpret_cast<const uchar *>(plane.constData()),
                             frame.width(),
                             frame.height(),
                             frame.bytesPerLine(),
                             frame.format()));
    mutex.unlock();
    event->accept();
}

void *VlcPainter::lockCallback(void **planes) {
    mutex.lock();
    planes[0] = (void *)plane.data();
    return nullptr;
}

void VlcPainter::unlockCallback(void *picture, void *const *planes) {
    mutex.unlock();
}

void VlcPainter::displayCallback(void *picture) {
    // if (widget) widget->update();
}

unsigned VlcPainter::formatCallback(char *chroma,
                                    unsigned *width,
                                    unsigned *height,
                                    unsigned *pitches,
                                    unsigned *lines) {
    if (nullptr == vlc_fourcc_GetChromaDescription) {
        vlc_fourcc_GetChromaDescription
            = (Pf_vlc_fourcc_GetChromaDescription)::GetProcAddress(
                ::GetModuleHandle(TEXT("libvlccore.dll")),
                "vlc_fourcc_GetChromaDescription");
        assert(vlc_fourcc_GetChromaDescription);
    }
    qstrcpy(chroma, "RV32");
    unsigned bufferSize = setPitchAndLines(
        vlc_fourcc_GetChromaDescription(VLC_FOURCC('R', 'V', '3', '2')),
        *width,
        *height,
        pitches,
        lines);
    plane.resize(bufferSize);
    frame = QImage(reinterpret_cast<const uchar *>(plane.constData()),
                   *width,
                   *height,
                   pitches[0],
                   QImage::Format_RGB32);
    return bufferSize;
}

void VlcPainter::formatCleanUpCallback() {
    if (!widget) delete this;
}

QRect VlcPainter::scaleToAspect(QRect srcRect, int w, int h) const {
    float width = srcRect.width();
    float height = srcRect.width() * (float(h) / float(w));
    if (height > srcRect.height()) {
        height = srcRect.height();
        width = srcRect.height() * (float(w) / float(h));
    }
    return QRect(0, 0, (int)width, (int)height);
}

QRect VlcPainter::drawFrameRect() const {
    QRect widgetRect = widget->rect();
    QRect drawFrameRect;
    switch (widget->currentAspectRatio()) {
    case Vlc::R_4_3:
        drawFrameRect = scaleToAspect(widgetRect, 4, 3);
        break;
    case Vlc::R_16_9:
        drawFrameRect = scaleToAspect(widgetRect, 16, 9);
        break;
    case Vlc::Original:
        drawFrameRect = QRect(0, 0, frame.width(), frame.height());
        break;
    }

    float widgetWidth = widgetRect.width();
    float widgetHeight = widgetRect.height();
    float frameWidth = widgetWidth;
    float frameHeight = drawFrameRect.height() * float(widgetWidth)
                        / float(drawFrameRect.width());

    drawFrameRect.setSize(QSize(int(frameWidth), int(frameHeight)));
    drawFrameRect.moveTo(int((widgetWidth - frameWidth) / 2.0f),
                         int((widgetHeight - frameHeight) / 2.0f));
    return drawFrameRect;
}

VideoViewVlc::VideoViewVlc(Video *video)
    : video(video), mouseSelect(this, video), painter(nullptr), pulses(0) {
    setObjectName("VideoViewVlc");
    setFrameShape(QFrame::NoFrame);
    setMouseTracking(true);
    unsetCursor();    // use parent cursor
    // setUpdatesEnabled(false);
    video->owner->view()->addWidget(this);
    video->owner->view()->setCurrentWidget(this);
    painter = new VlcPainter;
    painter->widget = this;
}

VideoViewVlc::~VideoViewVlc() {
    if (painter) painter->widget = nullptr;
}

void VideoViewVlc::setPlayer(MediaPlayerVlc *player) {
    if (painter)
        painter->setCallbacks(player);
    else
        player->setVideoWidget(this);
    setMediaPlayer(player);
}

void VideoViewVlc::paintEvent(QPaintEvent *event) {
    // qlog << "VideoViewVlc::paintEvent " <<
    // QString::fromStdWString(video->attr->name) << qdebug;

    if (painter)
        painter->handlePaint(event);
    else
        VlcWidgetVideo::paintEvent(event);
}
