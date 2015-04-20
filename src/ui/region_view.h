#ifndef REGIONVIEW_H
#define REGIONVIEW_H

#include <QStackedWidget>

#include "util/mouse_geo.h"
#include "mouse_select.h"

namespace Ui {
class RegionView;
}

class Region;

class RegionView : public QStackedWidget
{
    Q_OBJECT

public:
    static const QPen	BORDER_PEN_CURRENT;
    static const QPen	BORDER_PEN_DORMANT;

    explicit RegionView(Region *region);
    virtual ~RegionView();

    Region *region;
    MouseGeometry mouseGeo;
    MouseSelect mouseSelect;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void moveEvent(QMoveEvent * event) override;

private:
    void xChanged(int x);
    void yChanged(int y);
    void widthChanged(int width);
    void heightChanged(int height);
    void geometryEvent();

    Ui::RegionView *ui;
};

#endif // REGIONVIEW_H
