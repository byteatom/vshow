#include "picture_view.h"

#include <QPainter>

#include "picture.h"
#include "region.h"
#include "qlog/qlog.h"

PictureView::PictureView(Picture *picture) :
    picture(picture),
    mouseSelect(this, picture)
{
    setObjectName("PictureCfgeror");
    //setFrameShape(QFrame::NoFrame);
    setMouseTracking(true);
    unsetCursor(); //use parent cursor
    picture->owner->view()->addWidget(this);
    picture->owner->view()->setCurrentWidget(this);

    //setAttribute(Qt::WA_OpaquePaintEvent, true);
}

PictureView::~PictureView()
{
}

void PictureView::paintEvent(QPaintEvent *event)
{
    //qlog << "PictureView::paintEvent begin" << qdebug;
    QPainter painter(this);
    if (ImageAttr::BG_COLOR_NONE != picture->attr()->bgColor) {
        painter.fillRect(rect(), QColor::fromRgba(picture->attr()->bgColor));
    }
    if (!picture->bgCache.isNull()) {
        painter.drawPixmap(Slice::POS_TOPLEFT, picture->bgCache);
    }
    if (!picture->cache.isNull()) {
        painter.drawPixmap(Slice::POS_TOPLEFT, picture->cache);
    }
}

void PictureView::resizeEvent(QResizeEvent * event)
{
    picture->cacheView();
    QWidget::resizeEvent(event);
}
