#include "leak_spinbox.h"

#include <QCoreApplication>

#include "qlog/qlog.h"

LeakSpinBox::LeakSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    catchEvent(this);
}

LeakSpinBox::~LeakSpinBox()
{

}

void LeakSpinBox::catchEvent(QObject *widget)
{
    widget->installEventFilter(this);
    QObjectList children = widget->children();
    for (auto child: children) {
        catchEvent(child);
    }
}

bool LeakSpinBox::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease) {
        qlog << "LeakSpinBox eventFilter obj: " << obj->objectName() <<
                    ", type: " << event->type() <<
                    ", spontaneous: " << event->spontaneous() <<
                    qdebug;
        this->event(event);
    }

    return false;
}

bool LeakSpinBox::event(QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease) {
        qlog << "LeakSpinBox::event" <<
                    ", type: " << event->type() <<
                    ", spontaneous: " << event->spontaneous() <<
                    qdebug;
        //QSpinBox::event(event);
    }

    return QSpinBox::event(event);
}

void LeakSpinBox::mousePressEvent(QMouseEvent *event)
{
    QSpinBox::mousePressEvent(event);
    qlog <<  "LeakSpinBox::mousePressEvent" <<
                "type: " << event->type() <<
                ", spontaneous: " << event->spontaneous() <<
                qdebug;
    //event->ignore();
}

void LeakSpinBox::mouseReleaseEvent(QMouseEvent *event)
{
    QSpinBox::mouseReleaseEvent(event);
    qlog <<  "LeakSpinBox::mouseReleaseEvent" <<
                "type: " << event->type() <<
                ", spontaneous: " << event->spontaneous() <<
                qdebug;
    event->ignore();
}
