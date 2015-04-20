#include "region_view.h"
#include "ui_region_view.h"

#include <QMouseEvent>
#include <QPainter>

#include "region.h"
#include "main_ui.h"

const QPen RegionView::BORDER_PEN_CURRENT{QBrush{Qt::red}, 2};
const QPen RegionView::BORDER_PEN_DORMANT{QBrush{Qt::gray}, 2};

RegionView::RegionView(Region *region) :
    QStackedWidget(region->owner->view()),
    ui(new Ui::RegionView),
    region{region},
    mouseGeo{this},
    mouseSelect(this, region)
{
    ui->setupUi(this);

    //setAttribute(Qt::WA_OpaquePaintEvent, true);
    /*QPalette pal(palette());
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);*/

    setGeometry(region->attr()->geo.x,
                region->attr()->geo.y,
                region->attr()->geo.width,
                region->attr()->geo.height);

    region->attr()->geo.x.addObserver(std::bind(&RegionView::xChanged, this, std::placeholders::_1));
    region->attr()->geo.y.addObserver(std::bind(&RegionView::yChanged, this, std::placeholders::_1));
    region->attr()->geo.width.addObserver(std::bind(&RegionView::widthChanged, this, std::placeholders::_1));
    region->attr()->geo.height.addObserver(std::bind(&RegionView::heightChanged, this, std::placeholders::_1));
}

RegionView::~RegionView()
{
    delete ui;
}

void RegionView::paintEvent(QPaintEvent *event)
{
    //qlog << "RegionView::paintEvent " << region->attr()->name.value << qdebug;
    QPainter painter(this);
    //painter.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
    QRect box = rect();
    if (!region->playing) {
        painter.setPen(region->isActive() ? BORDER_PEN_CURRENT : BORDER_PEN_DORMANT);
        painter.drawRect(box);
        box.adjust(1, 1, -1, -1);
    }
    if (ImageAttr::BG_COLOR_NONE != region->attr()->bgColor) {
        painter.fillRect(box, QColor::fromRgba(region->attr()->bgColor));
    }
    if (!region->bgCache.isNull()) {
        painter.drawPixmap(Slice::POS_TOPLEFT, region->bgCache);
    }
}

void RegionView::resizeEvent(QResizeEvent * event)
{
    geometryEvent();
    QStackedWidget::resizeEvent(event);
}

void RegionView::moveEvent(QMoveEvent * event)
{
    geometryEvent();
    QStackedWidget::moveEvent(event);
}

void RegionView::xChanged(int x)
{
    QRect rect = frameGeometry();
    rect.moveLeft(x);
    setGeometry(rect);
}

void RegionView::yChanged(int y)
{
    QRect rect = frameGeometry();
    rect.moveTop(y);
    setGeometry(rect);
}

void RegionView::widthChanged(int width)
{
    QRect rect = frameGeometry();
    rect.setWidth(width);
    setGeometry(rect);
}

void RegionView::heightChanged(int height)
{
    QRect rect = frameGeometry();
    rect.setHeight(height);
    setGeometry(rect);
}

void RegionView::geometryEvent()
{
    QRect rect = frameGeometry();
    region->attr()->geo.x.value = rect.x();
    region->attr()->geo.y.value = rect.y();
    region->attr()->geo.width.value = rect.width();
    region->attr()->geo.height.value = rect.height();

    if (region->isCurrent())
        region->showCfgers();
    region->cacheView();
}
