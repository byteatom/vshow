#include "mouse_select.h"

#include "main_ui.h"

MouseSelect::MouseSelect(QWidget *widget, Slice *slice) :
    widget(widget),
    slice(slice)
{
    widget->installEventFilter(this);
}

MouseSelect::~MouseSelect()
{
    widget->removeEventFilter(this);
}

bool MouseSelect::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != widget || !widget->isEnabled()) return false;

    QMouseEvent *e = static_cast<QMouseEvent *>(event);

    switch (event->type()) {
    case QEvent::MouseButtonPress:
        /*qlog << "obj: " << obj->objectName() <<
                " widget:" << widget->objectName() <<
                " type: " << event->type() <<
                " spontaneous: " << event->spontaneous() <<
                qdebug;*/
        if (e->button() == Qt::LeftButton &&
            (e->spontaneous() || slice->organs.size() == 0)) {
            slice->treeItem->treeWidget()->setCurrentItem(slice->treeItem);
        }
        break;
    default:
        break;
    }

    return false;
}
