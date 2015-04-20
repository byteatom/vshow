#ifndef MOUSE_SELECT_H
#define MOUSE_SELECT_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>

#include "ui/slice.h"

class MouseSelect : public QObject
{
    Q_OBJECT
public:
    MouseSelect(QWidget *widget, Slice *slice);
    virtual ~MouseSelect();

protected:
    virtual bool eventFilter(QObject* obj, QEvent *event) override;

private:
    Slice *slice;
    QWidget *widget;
};

#endif // MOUSE_SELECT_H
