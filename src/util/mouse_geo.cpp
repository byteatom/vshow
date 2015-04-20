#include "util/mouse_geo.h"

#include <QObjectList>

#include "util/qt_util.h"
#include "qlog/qlog.h"

MouseGeometry::MouseGeometry(QWidget *widget) :
    widget(widget)
{
    cursor = Qt::ArrowCursor;
    widget->installEventFilter(this);
    enabled = true;
}

MouseGeometry::~MouseGeometry()
{
    widget->removeEventFilter(this);
    enabled = false;
}

void MouseGeometry::setEnabled(bool enable)
{
    if (enabled == enable) return;

    enabled = enable;
    if (enable) {
        widget->installEventFilter(this);
    } else {
        widget->removeEventFilter(this);
        widget->unsetCursor();
    }
}

bool MouseGeometry::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != widget || !widget->isEnabled()) return false;

    QMouseEvent *e = static_cast<QMouseEvent *>(event);
    switch (event->type()) {
    case QEvent::MouseButtonPress:
        press(e);
        break;
    case QEvent::MouseMove:
        move(e);
        break;
    default:
        return false;
    }

    return true;
}

void MouseGeometry::press(QMouseEvent *event)
{
    //qlog << "MouseGeometry::press" << qdebug;
    if (event->button() == Qt::LeftButton) {
        dragPos = event->screenPos();
        dragRect = widget->frameGeometry();
    }
}

void MouseGeometry::move(QMouseEvent *event)
{
    /*qlog << "pos: " << event->pos() <<
            "window pos: " << event->windowPos() <<
            "local pos: " << event->localPos() <<
            "global pos: " << event->globalPos() <<
            "screen pos: " << event->screenPos() <<
            qdebug;*/

    if (event->buttons() == Qt::NoButton) {
        QPointF pos = event->pos();
        QRectF rect = widget->frameGeometry();

        onTopEdge = pos.y() < EDGE_SIZE;
        onBottomEdge = rect.height() - pos.y() < EDGE_SIZE;
        onLeftEdge = pos.x() < EDGE_SIZE;
        onRightEdge = rect.width() - pos.x() < EDGE_SIZE;

        if ((onTopEdge && onLeftEdge) || (onBottomEdge && onRightEdge))
            widget->setCursor(Qt::SizeFDiagCursor);
        else if ((onTopEdge && onRightEdge) || (onBottomEdge && onLeftEdge))
            widget->setCursor(Qt::SizeBDiagCursor);
        else if (onLeftEdge || onRightEdge)
            widget->setCursor(Qt::SizeHorCursor);
        else if (onTopEdge || onBottomEdge)
            widget->setCursor(Qt::SizeVerCursor);
        else
            //widget->unsetCursor();
            widget->setCursor(Qt::ArrowCursor);
    } else if (event->buttons() == Qt::LeftButton) {
        QPointF pos = event->screenPos();
        QPointF offset = pos - dragPos;
        QSize parentSize = QtUtil::parentSize(widget);
        QRectF newRect = dragRect;

        if (onTopEdge || onLeftEdge || onBottomEdge || onRightEdge) { //resizing
            if (onTopEdge) {
                newRect.setTop(dragRect.top() + offset.y());
                if (newRect.y() < 0) newRect.setTop(0);
            }
            if (onLeftEdge) {
                newRect.setLeft(dragRect.left() + offset.x());
                if (newRect.x() < 0) newRect.setLeft(0);
            }
            if (onBottomEdge) {
                newRect.setBottom(dragRect.bottom() + offset.y());
                if (newRect.bottom() > parentSize.height()) newRect.setBottom(parentSize.height());
            }
            if (onRightEdge) {
                newRect.setRight(dragRect.right() + offset.x());
                if (newRect.right() > parentSize.width()) newRect.setRight(parentSize.width());
            }
        } else { // moving
            newRect.moveTo(dragRect.topLeft() + offset);

            if (newRect.x() < 0) newRect.moveLeft(0);
            if (newRect.y() < 0) newRect.moveTop(0);
            if (newRect.right() > parentSize.width()) newRect.moveRight(parentSize.width());
            if (newRect.bottom() > parentSize.height()) newRect.moveBottom(parentSize.height());
        }

        widget->setGeometry(newRect.toRect());
    }
}
