#ifndef TEXT_VIEWER_H
#define TEXT_VIEWER_H

#include <list>

#include <QStackedWidget>
#include <QPixmap>

#include "mouse_select.h"
#include "text_editor.h"

class Text;

class TextView : public QStackedWidget
{
    Q_OBJECT

public:
    explicit TextView(Text *text);
    virtual ~TextView();

    TextEditor *editor;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent * event) override;

private:
    Text *text;
    MouseSelect mouseSelect;
};

#endif // TEXT_VIEWER_H
