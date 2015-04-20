#include "text_view.h"

#include <QMouseEvent>
#include <QPainter>

#include "qlog/qlog.h"
#include "text.h"
#include "main_ui.h"
#include "region.h"

TextView::TextView(Text *text) :
    text(text),
    editor{new TextEditor(text)},
    mouseSelect{this, text}
{
    setObjectName("TextView");

    //setWindowFlags(Qt::ToolTip); //for video overlay
    setMouseTracking(true);
    addWidget(editor);
    setCurrentWidget(editor);
    text->owner->view()->addWidget(this);
    text->owner->view()->setCurrentWidget(this);

    //setAttribute(Qt::WA_OpaquePaintEvent, true);
}

TextView::~TextView()
{
}

void TextView::paintEvent(QPaintEvent *event)
{
    //qlog << "TextView::paintEvent " << QString::fromStdWString(program->attr->name) << qdebug;
    QPainter painter(this);
    if (ImageAttr::BG_COLOR_NONE != text->attr()->bgColor) {
        painter.fillRect(rect(), QColor::fromRgba(text->attr()->bgColor));
    }
    if (!text->bgCache.isNull()) {
        painter.drawPixmap(Slice::POS_TOPLEFT, text->bgCache);
    }
    if (!text->cache.isNull()) {
        painter.drawPixmap(Slice::POS_TOPLEFT, text->cache);
    }
}

void TextView::resizeEvent(QResizeEvent * event)
{
    text->cacheView();
    QStackedWidget::resizeEvent(event);
}
