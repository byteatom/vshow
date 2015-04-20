#include "text_editor.h"

#include <QMouseEvent>
#include <QPainter>

#include "qlog/qlog.h"
#include "text.h"
#include "main_ui.h"
#include "region.h"

TextEditor::TextEditor(Text *text) :
    text(text)
{
    setObjectName("TextEditor");
    setFrameShape(QFrame::NoFrame);
    viewport()->setAutoFillBackground(false);
    setStyleSheet("color : white"); //default cursor color
    setTextColor(Qt::white);    //default text color for drawContents
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setUndoRedoEnabled(true);
    setCursorWidth(2);
    //setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

TextEditor::~TextEditor()
{
}


void TextEditor::mouseDoubleClickEvent(QMouseEvent *event)
{
    //qlog << "TextCfgeror double click" << qdebug;
    setReadOnly(!isReadOnly());
    setMouseTracking(isReadOnly()); //for edge cursor
    if (isReadOnly()) {
        viewport()->unsetCursor();  //use parent's cursor
    } else {
        viewport()->setCursor(Qt::IBeamCursor); //QTextCfger defualt cursor
    }
    event->accept();
}


QPixmap TextEditor::snapshot()
{
    QTextDocument *doc = document();
    assert(doc->size().width() > 0 && doc->size().height() > 0);
    QPixmap pix(doc->size().toSize());
    pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);
    doc->drawContents(&painter);
    //qlog << "snapshot " << pix.width() << qdebug;

    return pix;
}

void TextEditor::mousePressEvent(QMouseEvent *event)
{
    //qlog << "TextCfgeror press" << qdebug;
    QTextEdit::mousePressEvent(event);
    event->ignore();
}


void TextEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (isReadOnly())
        event->ignore();    //bypass to parent region
    else {
        QTextEdit::mouseMoveEvent(event);
        //event->accept();
    }
}
