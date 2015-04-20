#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>
#include <QPixmap>

#include "mouse_select.h"

class Text;

class TextEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit TextEditor(Text *text);
    virtual ~TextEditor();

    QPixmap snapshot();

protected:
    /*
     * QTextEdit's mouse event directly called from viewport,
     * not even pass through self's event filter,
     * so override is the only method.
    */
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    Text *text;
};

#endif // TEXTEDITOR_H
