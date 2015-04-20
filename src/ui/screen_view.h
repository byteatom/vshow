#ifndef SCREEN_VIEWER_H
#define SCREEN_VIEWER_H

#include <QDialog>
#include <QPen>
#include <QStackedLayout>

#include "util/mouse_geo.h"
#include "mouse_select.h"

namespace Ui {
class ScreenView;
}

class Screen;

class ScreenView : public QDialog
{
    Q_OBJECT

public:
    static const QPen	BORDER_PEN;

    explicit ScreenView(Screen *screen);
    virtual ~ScreenView();

    Screen *screen;
    QStackedLayout layout;
    MouseGeometry mouseGeo;
    MouseSelect mouseSelect;

protected:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void moveEvent(QMoveEvent * event) override;
    virtual void enterEvent(QEvent *) override;
    virtual void leaveEvent(QEvent *) override;


    void nameChanged(std::wstring &name);
    void xChanged(int x);
    void yChanged(int y);
    void widthChanged(int width);
    void heightChanged(int height);
    void lockChanged(bool lock);
    void topChanged(bool top);
    void geometryEvent();

    Ui::ScreenView *ui;
};

#endif // SCREEN_VIEWER_H
