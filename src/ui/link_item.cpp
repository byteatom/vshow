#include "link_item.h"
#include "ui_link_item.h"

#include <QMouseEvent>

#include "link_ui.h"
#include "qlog/qlog.h"

LinkItem::LinkItem(int row, int col, LinkUi *linkUi)
    : QFrame(nullptr),
      ui(new Ui::LinkItem),
      row{row},
      col{col},
      linkUi{linkUi},
      focusIn{false} {
    ui->setupUi(this);

    index = ui->index;
    width = ui->width;
    height = ui->height;

    setStyleSheet("background-color:transparent;");
    ui->multiply->setStyleSheet("background-color:transparent;");
    index->setStyleSheet("background-color:transparent;");
    width->setStyleSheet("background-color:transparent;");
    height->setStyleSheet("background-color:transparent;");

    catchEvent(this);

    connect(
        ui->width, &QSpinBox::editingFinished, this, &LinkItem::widthChanged);
    connect(
        ui->height, &QSpinBox::editingFinished, this, &LinkItem::heightChanged);
}

LinkItem::~LinkItem() {
    delete ui;
}

void LinkItem::catchEvent(QObject *widget) {
    widget->installEventFilter(this);
    /*QObjectList children = widget->children();
    for (auto child: children) {
        catchEvent(child);
    }*/
}

bool LinkItem::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress
        || event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        qlog << "LinkItem::eventFilter obj: " << obj->objectName()
             << ", type: " << e->type() << ", spontaneous: " << e->spontaneous()
             << "pos: " << e->pos() << "window pos: " << e->windowPos()
             << "local pos: " << e->localPos()
             << "global pos: " << e->globalPos()
             << "screen pos: " << e->screenPos() << qdebug;
    } else if (event->type() != QEvent::Paint) {
        qlog << "LinkItem::eventFilter obj: " << obj->objectName()
             << ", type: " << event->type() << qdebug;
    }

    if (event->type() == QEvent::MouseButtonPress) {
        // setFocus(Qt::MouseFocusReason);
    }

    if (event->type() == QEvent::FocusIn) {
        // setStyleSheet("background-color:#3399FF;");
        // ui->multiply->setStyleSheet("background-color:#3399FF;");
        if (!focusIn) {
            focusIn = true;
            // sink(focusIn, row, col);
        }
    } else if (event->type() == QEvent::FocusOut) {
        // setStyleSheet("");
        // ui->multiply->setStyleSheet("");
        if (focusIn) {
            focusIn = false;
            // sink(focusIn, row, col);
        }
    }
    return false;
}

bool LinkItem::event(QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress
        || event->type() == QEvent::MouseButtonRelease) {
        qlog << "LinkItem::event"
             << ", type: " << event->type()
             << ", spontaneous: " << event->spontaneous() << qdebug;
        // QSpinBox::event(event);
    }

    return QFrame::event(event);
}

void LinkItem::mousePressEvent(QMouseEvent *event) {
    QFrame::mousePressEvent(event);
    qlog << "LinkItem::mousePressEvent"
         << "type: " << event->type()
         << ", spontaneous: " << event->spontaneous() << qdebug;
    // event->ignore();
}

void LinkItem::widthChanged() {
    linkUi->widthChanged(this, ui->width->value());
}

void LinkItem::heightChanged() {
    linkUi->heightChanged(this, ui->height->value());
}
