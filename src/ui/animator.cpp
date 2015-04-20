#include "animator.h"
#include <QtMath>
#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>
#include <cassert>
#include <QTime>
#include "data/image_attr.h"
#include "qlog/qlog.h"


const int Animator::pageTurnRange = 60;
const int Animator::fanRange = 90;
const int Animator::spinRange = 180;
const double Animator::PI = 3.1415926;
const Animator::StyleAction Animator::actions[AnimateAttr::STYLE_MAX] = {
    {AnimateAttr::RANDOM, &Animator::random, QT_TR_NOOP("Random")},
    {AnimateAttr::MOTIONLESS, &Animator::motionless, QT_TR_NOOP("Static")},
    {AnimateAttr::MOVE_UP, &Animator::moveUp, QT_TR_NOOP("Move Up")},
    {AnimateAttr::MOVE_DOWN, &Animator::moveDown, QT_TR_NOOP("Move Down")},
    {AnimateAttr::MOVE_LEFT, &Animator::moveLeft, QT_TR_NOOP("Move Left")},
    {AnimateAttr::MOVE_RIGHT, &Animator::moveRight, QT_TR_NOOP("Move Right")},
    {AnimateAttr::OVERLAY_UP, &Animator::overlayUp, QT_TR_NOOP("Overlay Up")},
    {AnimateAttr::OVERLAY_DOWN,
     &Animator::overlayDown,
     QT_TR_NOOP("Overlay Down")},
    {AnimateAttr::OVERLAY_LEFT,
     &Animator::overlayLeft,
     QT_TR_NOOP("Overlay Left")},
    {AnimateAttr::OVERLAY_RIGHT,
     &Animator::overlayRight,
     QT_TR_NOOP("Overlay Right")},
    {AnimateAttr::HORIZONTAL_INAUGURATE,
     &Animator::horizontalInaugurate,
     QT_TR_NOOP("Horizontal Inauguate")},
    {AnimateAttr::HORIZONTAL_CONCLUDE,
     &Animator::horizontalConclude,
     QT_TR_NOOP("Horizontal Conclude")},
    {AnimateAttr::VERTICAL_INAUGURATE,
     &Animator::verticalInaugurate,
     QT_TR_NOOP("Vertical Inauguate")},
    {AnimateAttr::VERTICAL_CONCLUDE,
     &Animator::verticalConclude,
     QT_TR_NOOP("Vertical Conclude")},
    {AnimateAttr::SHUTTER_UP, &Animator::shutterUp, QT_TR_NOOP("Shutter Up")},
    {AnimateAttr::SHUTTER_DOWN,
     &Animator::shutterDown,
     QT_TR_NOOP("Shutter Down")},
    {AnimateAttr::SHUTTER_LEFT,
     &Animator::shutterLeft,
     QT_TR_NOOP("Shutter Left")},
    {AnimateAttr::SHUTTER_RIGHT,
     &Animator::shutterRight,
     QT_TR_NOOP("Shutter Right")},
    {AnimateAttr::BLINK, &Animator::blink, QT_TR_NOOP("Blink")},
    {AnimateAttr::BRUSH_UP, &Animator::brushUp, QT_TR_NOOP("Brush Up")},
    {AnimateAttr::BRUSH_DOWN, &Animator::brushDown, QT_TR_NOOP("Brush Down")},
    {AnimateAttr::BRUSH_LEFT, &Animator::brushLeft, QT_TR_NOOP("Brush Left")},
    {AnimateAttr::BRUSH_RIGHT,
     &Animator::brushRight,
     QT_TR_NOOP("Brush Right")},
    {AnimateAttr::PAGE_LEFT, &Animator::pageLeft, QT_TR_NOOP("Page Left")},
    {AnimateAttr::PAGE_RIGHT, &Animator::pageRight, QT_TR_NOOP("Page Right")},
    {AnimateAttr::FAN_INAUGURATE,
     &Animator::fanInaugurate,
     QT_TR_NOOP("Fan Inauguate")},
    {AnimateAttr::FAN_CONCLUDE,
     &Animator::fanConclude,
     QT_TR_NOOP("Fan Conclude")},
    {AnimateAttr::SPIN_LEFT, &Animator::spinLeft, QT_TR_NOOP("Spin Left")},
    {AnimateAttr::SPIN_RIGHT, &Animator::spinRight, QT_TR_NOOP("Spin Right")},
    {AnimateAttr::RECTANGLE_INAUGURATE,
     &Animator::rectangleInaugurate,
     QT_TR_NOOP("Rectangle Inaugurate")},
    {AnimateAttr::RECTANGLE_CONCLUDE,
     &Animator::rectangleConclude,
     QT_TR_NOOP("Rectangle Conclude")},
    {AnimateAttr::CURTAIN_DOWN,
     &Animator::curtainDown,
     QT_TR_NOOP("Curtain Down")},
    {AnimateAttr::CURTAIN_RIGHT,
     &Animator::curtainRight,
     QT_TR_NOOP("Curtain Right")},
    {AnimateAttr::RECTANGLE_TOP_LEFT,
     &Animator::rectangleTopLeft,
     QT_TR_NOOP("Rectangle TopLeft")},
    {AnimateAttr::RECTANGLE_TOP_RIGHT,
     &Animator::rectangleTopRight,
     QT_TR_NOOP("Rectangle TopRight")},
    {AnimateAttr::RECTANGLE_BOTTOM_LEFT,
     &Animator::rectangleBottomLeft,
     QT_TR_NOOP("Rectangle BottomLeft")},
    {AnimateAttr::RECTANGLE_BOTTOM_RIGHT,
     &Animator::rectangleBottomRight,
     QT_TR_NOOP("Rectangle BottomRight")},
    {AnimateAttr::DIAGONAL_TOP_LEFT,
     &Animator::diagonalTopLeft,
     QT_TR_NOOP("Diagonal TopLeft")},
    {AnimateAttr::DIAGONAL_TOP_RIGHT,
     &Animator::diagonalTopRight,
     QT_TR_NOOP("Diagonal TopRight")},
    {AnimateAttr::DIAGONAL_BOTTOM_LEFT,
     &Animator::diagonalBottomLeft,
     QT_TR_NOOP("Diagonal BottomLeft")},
    {AnimateAttr::DIAGONAL_BOTTOM_RIGHT,
     &Animator::diagonalBottomRight,
     QT_TR_NOOP("Diagonal BottomRight")},
    {AnimateAttr::CROSS_EXPAND,
     &Animator::crossExpand,
     QT_TR_NOOP("Cross Expand")},
    {AnimateAttr::CROSS_SHRINKAGE,
     &Animator::crossShrinkage,
     QT_TR_NOOP("Cross Shrinkage")},
    {AnimateAttr::MOVE_LEFT_CONTINUOUS,
     &Animator::moveLeftContinuous,
     QT_TR_NOOP("Move Left Continuous")},
    {AnimateAttr::MOVE_RIGHT_CONTINUOUS,
     &Animator::moveRightContinuous,
     QT_TR_NOOP("Move Right Continuous")}};

void Animator::check() {
	for (int i = 0; i < AnimateAttr::STYLE_MAX; i++) {
		if (actions[i].style != i) {
			assert(0);
			return;
		}
	}
}

Animator::Animator() {
	sourcePixmapWidth = 0;
	sourcePixmapHeight = 0;
	xPageCount = 1;
	yPageCount = 1;
	pageCount = 1;
	currentPage = 1;
	pageCountCycleComplete = false;
	currentStepLocation = 0;
	blinkCount = 0;
	curtainMoveIndex = 0;
    brushVeil = false;
}

Animator::~Animator() {
}

void Animator::build(QRectF region, QPixmap source, AnimateAttr *attr) {
	assert(attr->style < AnimateAttr::STYLE_MAX);
	this->region = region;
	this->source = source;
	this->attr = attr;
	sourcePixmapWidth = this->source.width();
	sourcePixmapHeight = this->source.height();
	xPageCount = qCeil(((qreal)sourcePixmapWidth) / region.width());
	yPageCount = qCeil(((qreal)sourcePixmapHeight) / region.height());
	pageCount = xPageCount * yPageCount;
	currentPage = 1;
	pageCountCycleComplete = false;
	currentStepLocation = 0;
	randomStyle = produceRandomStyle();
	blinkCount = 0;
}

QPixmap Animator::frame() {
	return (this->*(actions[attr->style].action))();
}

AnimateAttr::Style Animator::produceRandomStyle() {
	int randomInt = 0;
    QTime currentTime = QTime::currentTime();
    qsrand(currentTime.second());
	randomInt = qrand();
	assert((AnimateAttr::Style)(randomInt % (AnimateAttr::STYLE_MAX))
           < AnimateAttr::STYLE_MAX);
	return (AnimateAttr::Style)(randomInt % (AnimateAttr::STYLE_MAX));
}

QPixmap Animator::getCurrentPagePixmap() {
	int xCurrentPagePixmapLocation = (currentPage - 1) % xPageCount;
	int yCurrentPagePixmapLocation = (currentPage - 1) / yPageCount;
	QRect currentPageRect;
	int currentPagePixmapLeft = xCurrentPagePixmapLocation * region.width();
	currentPageRect.setLeft(currentPagePixmapLeft);
	int currentPagePixmapRight = 0;
	if ((currentPagePixmapLeft + region.width()) < sourcePixmapWidth) {
		currentPagePixmapRight = currentPagePixmapLeft + region.width();
	} else {
		currentPagePixmapRight = sourcePixmapWidth;
	}
	currentPageRect.setRight(currentPagePixmapRight);
	int currentPagePixmapTop = yCurrentPagePixmapLocation * region.height();
	currentPageRect.setTop(currentPagePixmapTop);
	int currentPagePixmapBottom = 0;
	if ((currentPagePixmapTop + region.height()) < sourcePixmapHeight) {
		currentPagePixmapBottom = currentPagePixmapTop + region.height();
	} else {
		currentPagePixmapBottom = sourcePixmapHeight;
	}
	currentPageRect.setBottom(currentPagePixmapBottom);
	QPixmap currentPagePixmap(region.width(), region.height());
	currentPagePixmap.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
	QPainter painter;
	painter.begin(&currentPagePixmap);
	painter.drawPixmap(QPoint(0, 0), source, currentPageRect);
	painter.end();
	return currentPagePixmap;
}

QPixmap Animator::random() {
    if (AnimateAttr::RANDOM == randomStyle) {
        randomStyle = produceRandomStyle();
        while (AnimateAttr::RANDOM == randomStyle) {
            randomStyle = produceRandomStyle();
        }
    }
	return (this->*(actions[randomStyle].action))();
}

QPixmap Animator::motionless() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
        pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintpoint = region.topLeft();
		painter.drawPixmap(currentPageStepPixmapPaintpoint, currentPagePixmap);
		painter.end();
		currentPage = currentPage + 1;
		if (currentPage > pageCount) {
			currentPage = 1;
			pageCountCycleComplete = true;
			randomStyle = produceRandomStyle();
		}
		return pix;
	}
}

QPixmap Animator::moveUp() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int moveRange = region.height();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > moveRange) {
			currentStepLocation = moveRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapRect.topLeft());
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(
            currentPagePixmapRect.bottomRight().x());
		currentPagePixmapStepRectBottomRight.setY(currentStepLocation);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintpoint(
            region.x(), region.height() - currentStepLocation);
		painter.drawPixmap(currentPageStepPixmapPaintpoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == moveRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::moveDown() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmapPixmap = getCurrentPagePixmap();
		int moveRange = region.height();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > moveRange) {
			currentStepLocation = moveRange;
		}
		QRect currentPagePixmapRect = currentPagePixmapPixmap.rect();
		QRect currentPagePixmapStepRect;
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapRect.bottomRight());
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(
            currentPagePixmapRect.topLeft().x());
		currentPagePixmapStepRectTopLeft.setY(currentPagePixmapRect.height()
                                              - currentStepLocation);
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		QPixmap currentPageStepPixmap
            = currentPagePixmapPixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint = region.topLeft();
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == moveRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::moveLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int moveRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > moveRange) {
			currentStepLocation = moveRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapRect.topLeft());
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(currentStepLocation);
		currentPagePixmapStepRectBottomRight.setY(
            currentPagePixmapRect.bottomRight().y());
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(
            region.width() - currentStepLocation, region.y());
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == moveRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::moveRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int moveRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > moveRange) {
			currentStepLocation = moveRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(currentPagePixmapRect.width()
                                              - currentStepLocation);
		currentPagePixmapStepRectTopLeft.setY(
            currentPagePixmapRect.topLeft().y());
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapRect.bottomRight());
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint = region.topLeft();
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == moveRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::overlayUp() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int overlayRange = region.height();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > overlayRange) {
			currentStepLocation = overlayRange;
		}
		QRect currentPagePixmapRect;
		currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(
            currentPagePixmapRect.topLeft().x());
		currentPagePixmapStepRectTopLeft.setY(currentPagePixmapRect.height()
                                              - currentStepLocation);
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapRect.bottomRight());
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(
            region.x(), region.height() - currentStepLocation);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == overlayRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::overlayDown() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int overlayRange = region.height();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > overlayRange) {
			currentStepLocation = overlayRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapRect.topLeft());
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(
            currentPagePixmapRect.bottomRight().x());
		currentPagePixmapStepRectBottomRight.setY(currentStepLocation);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint = region.topLeft();
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == overlayRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::overlayLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int overlayRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > overlayRange) {
			currentStepLocation = overlayRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(currentPagePixmapRect.width()
                                              - currentStepLocation);
		currentPagePixmapStepRectTopLeft.setY(
            currentPagePixmapRect.topLeft().y());
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapRect.bottomRight());
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(
            region.width() - currentStepLocation, region.y());
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == overlayRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::overlayRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int overlayRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > overlayRange) {
			currentStepLocation = overlayRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapRect.topLeft());
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(currentStepLocation);
		currentPagePixmapStepRectBottomRight.setY(
            currentPagePixmapRect.bottomRight().y());
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint = region.topLeft();
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == overlayRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::horizontalInaugurate() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int horizontalRange = region.width() / 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > horizontalRange) {
			currentStepLocation = horizontalRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(currentPagePixmapRect.width() / 2
                                              - currentStepLocation);
		currentPagePixmapStepRectTopLeft.setY(
            currentPagePixmapRect.topLeft().y());
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(currentPagePixmapRect.width()
                                                  / 2 + currentStepLocation);
		currentPagePixmapStepRectBottomRight.setY(
            currentPagePixmapRect.bottomRight().y());
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(
            region.width() / 2 - currentStepLocation, region.y());
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == horizontalRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::horizontalConclude() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int horizontalRange = region.width() / 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > horizontalRange) {
			currentStepLocation = horizontalRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepLeftRect;
		QPoint currentPagePixmapStepLeftRectTopLeft;
		QPoint currentPagePixmapStepLeftRectBottomRight;
		currentPagePixmapStepLeftRectTopLeft.setX(
            currentPagePixmapRect.topLeft().x());
		currentPagePixmapStepLeftRectTopLeft.setY(
            currentPagePixmapRect.topLeft().y());
		currentPagePixmapStepLeftRectBottomRight.setX(currentStepLocation);
		currentPagePixmapStepLeftRectBottomRight.setY(
            currentPagePixmapRect.bottomLeft().y());
		currentPagePixmapStepLeftRect.setTopLeft(
            currentPagePixmapStepLeftRectTopLeft);
		currentPagePixmapStepLeftRect.setBottomRight(
            currentPagePixmapStepLeftRectBottomRight);
		QRect currentPagePixmapStepRightRect;
		QPoint currentPagePixmapStepRightRectTopLeft;
		QPoint currentPagePixmapStepRightRectBottomRight;
		currentPagePixmapStepRightRectTopLeft.setX(currentPagePixmapRect.width()
                                                   - currentStepLocation);
		currentPagePixmapStepRightRectTopLeft.setY(
            currentPagePixmapRect.topRight().y());
		currentPagePixmapStepRightRectBottomRight.setX(
            currentPagePixmapRect.bottomRight().x());
		currentPagePixmapStepRightRectBottomRight.setY(
            currentPagePixmapRect.bottomRight().y());
		currentPagePixmapStepRightRect.setTopLeft(
            currentPagePixmapStepRightRectTopLeft);
		currentPagePixmapStepRightRect.setBottomRight(
            currentPagePixmapStepRightRectBottomRight);
		QPixmap currentPageStepPixmapLeft
            = currentPagePixmap.copy(currentPagePixmapStepLeftRect);
		QPixmap currentPageStepPixmapRight
            = currentPagePixmap.copy(currentPagePixmapStepRightRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapLeftRectPaintPoint;
		QPointF currentPageStepPixmapRightRectPaintPoint;
		currentPageStepPixmapLeftRectPaintPoint.setX(region.topLeft().x());
		currentPageStepPixmapLeftRectPaintPoint.setY(region.topLeft().y());
		currentPageStepPixmapRightRectPaintPoint.setX(region.width()
                                                      - currentStepLocation);
		currentPageStepPixmapRightRectPaintPoint.setY(region.topRight().y());
		painter.drawPixmap(currentPageStepPixmapLeftRectPaintPoint,
                           currentPageStepPixmapLeft);
		painter.drawPixmap(currentPageStepPixmapRightRectPaintPoint,
                           currentPageStepPixmapRight);
		painter.end();
		if (currentStepLocation == horizontalRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::verticalInaugurate() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int verticalRange = region.height() / 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > verticalRange) {
			currentStepLocation = verticalRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(
            currentPagePixmapRect.topLeft().x());
		currentPagePixmapStepRectTopLeft.setY(currentPagePixmapRect.height() / 2
                                              - currentStepLocation);
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(
            currentPagePixmapRect.bottomRight().x());
		currentPagePixmapStepRectBottomRight.setY(currentPagePixmapRect.height()
                                                  / 2 + currentStepLocation);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(
            region.x(), region.height() / 2 - currentStepLocation);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == verticalRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::verticalConclude() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int verticalRange = region.height() / 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > verticalRange) {
			currentStepLocation = verticalRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepTopRect;
		QPoint currentPagePixmapStepTopRectTopLeft;
		QPoint currentPagePixmapStepTopRectBottomRight;
		currentPagePixmapStepTopRectTopLeft.setX(
            currentPagePixmapRect.topLeft().x());
		currentPagePixmapStepTopRectTopLeft.setY(
            currentPagePixmapRect.topLeft().y());
		currentPagePixmapStepTopRectBottomRight.setX(
            currentPagePixmapRect.bottomRight().x());
		currentPagePixmapStepTopRectBottomRight.setY(currentStepLocation);
		currentPagePixmapStepTopRect.setTopLeft(
            currentPagePixmapStepTopRectTopLeft);
		currentPagePixmapStepTopRect.setBottomRight(
            currentPagePixmapStepTopRectBottomRight);
		QRect currentPagePixmapStepBottomRect;
		QPoint currentPagePixmapStepBottomRectTopLeft;
		QPoint currentPagePixmapStepBottomRectBottomRight;
		currentPagePixmapStepBottomRectTopLeft.setX(
            currentPagePixmapRect.topLeft().x());
		currentPagePixmapStepBottomRectTopLeft.setY(
            currentPagePixmapRect.height() - currentStepLocation);
		currentPagePixmapStepBottomRectBottomRight.setX(
            currentPagePixmapRect.bottomRight().x());
		currentPagePixmapStepBottomRectBottomRight.setY(
            currentPagePixmapRect.bottomRight().y());
		currentPagePixmapStepBottomRect.setTopLeft(
            currentPagePixmapStepBottomRectTopLeft);
		currentPagePixmapStepBottomRect.setBottomRight(
            currentPagePixmapStepBottomRectBottomRight);
		QPixmap currentPageStepPixmapTop
            = currentPagePixmap.copy(currentPagePixmapStepTopRect);
		QPixmap currentPageStepPixmapBottom
            = currentPagePixmap.copy(currentPagePixmapStepBottomRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapTopRectPaintPoint;
		QPointF currentPageStepPixmapBottomRectPaintPoint;
		currentPageStepPixmapTopRectPaintPoint.setX(region.topLeft().x());
		currentPageStepPixmapTopRectPaintPoint.setY(region.topLeft().y());
		currentPageStepPixmapBottomRectPaintPoint.setX(region.bottomLeft().x());
		currentPageStepPixmapBottomRectPaintPoint.setY(region.height()
                                                       - currentStepLocation);
		painter.drawPixmap(currentPageStepPixmapTopRectPaintPoint,
                           currentPageStepPixmapTop);
		painter.drawPixmap(currentPageStepPixmapBottomRectPaintPoint,
                           currentPageStepPixmapBottom);
		painter.end();
		if (currentStepLocation == verticalRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::shutterUp() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		int singleShutterWidth = 16;
		int shutterCount = qCeil(region.height() / singleShutterWidth);
		if (currentStepLocation > singleShutterWidth) {
			currentStepLocation = singleShutterWidth;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect singleShutterRect;
		QPoint singleShutterRectTopLeft;
		QPoint singleShutterRectBottomRight;
		int xSingleShutterRectTopLeft = 0, ySingleShutterRectTopLeft = 0;
		int xSingleShutterRectBottomRight = 0,
            ySingleShutterRectBottomRight = 0;
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		for (int shutterIndex = 0; shutterIndex < shutterCount;
             shutterIndex++) {
			xSingleShutterRectBottomRight
                = currentPagePixmapRect.bottomRight().x();
			ySingleShutterRectBottomRight = currentPagePixmapRect.height()
                                            - shutterIndex * singleShutterWidth;
			xSingleShutterRectTopLeft = currentPagePixmapRect.topLeft().x();
			ySingleShutterRectTopLeft
                = currentPagePixmapRect.height()
                  - (shutterIndex * singleShutterWidth + currentStepLocation);
			singleShutterRectTopLeft.setX(xSingleShutterRectTopLeft);
			singleShutterRectTopLeft.setY(ySingleShutterRectTopLeft);
			singleShutterRectBottomRight.setX(xSingleShutterRectBottomRight);
			singleShutterRectBottomRight.setY(ySingleShutterRectBottomRight);
			singleShutterRect.setTopLeft(singleShutterRectTopLeft);
			singleShutterRect.setBottomRight(singleShutterRectBottomRight);
			QPixmap singleShutterRectStepPixmap
                = currentPagePixmap.copy(singleShutterRect);
			QPointF singleShutterStepPixmapPaintPoint
                = singleShutterRectTopLeft;
			painter.drawPixmap(singleShutterStepPixmapPaintPoint,
                               singleShutterRectStepPixmap);
		}
		painter.end();
		if (currentStepLocation == singleShutterWidth) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::shutterDown() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		int singleShutterWidth = 16;
		int shutterCount = qCeil(region.height() / singleShutterWidth);
		if (currentStepLocation > singleShutterWidth) {
			currentStepLocation = singleShutterWidth;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect singleShutterRect;
		QPoint singleShutterRectTopLeft;
		QPoint singleShutterRectBottomRight;
		int xSingleShutterRectTopLeft = 0, ySingleShutterRectTopLeft = 0;
		int xSingleShutterRectBottomRight = 0,
            ySingleShutterRectBottomRight = 0;
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		for (int shutterIndex = 0; shutterIndex < shutterCount;
             shutterIndex++) {
			xSingleShutterRectTopLeft = currentPagePixmapRect.topLeft().x();
			ySingleShutterRectTopLeft = shutterIndex * singleShutterWidth;
			xSingleShutterRectBottomRight
                = currentPagePixmapRect.bottomRight().x();
			ySingleShutterRectBottomRight = shutterIndex * singleShutterWidth
                                            + currentStepLocation;
			singleShutterRectTopLeft.setX(xSingleShutterRectTopLeft);
			singleShutterRectTopLeft.setY(ySingleShutterRectTopLeft);
			singleShutterRectBottomRight.setX(xSingleShutterRectBottomRight);
			singleShutterRectBottomRight.setY(ySingleShutterRectBottomRight);
			singleShutterRect.setTopLeft(singleShutterRectTopLeft);
			singleShutterRect.setBottomRight(singleShutterRectBottomRight);
			QPixmap singleShutterRectStepPixmap
                = currentPagePixmap.copy(singleShutterRect);
			QPointF singleShutterStepPixmapPaintPoint
                = singleShutterRectTopLeft;
			painter.drawPixmap(singleShutterStepPixmapPaintPoint,
                               singleShutterRectStepPixmap);
		}
		painter.end();
		if (currentStepLocation == singleShutterWidth) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::shutterLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		int singleShutterWidth = 16;
		int shutterCount = qCeil(region.width() / singleShutterWidth);
		if (currentStepLocation > singleShutterWidth) {
			currentStepLocation = singleShutterWidth;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect singleShutterRect;
		QPoint singleShutterRectTopLeft;
		QPoint singleShutterRectBottomRight;
		int xSingleShutterRectTopLeft = 0, ySingleShutterRectTopLeft = 0;
		int xSingleShutterRectBottomRight = 0,
            ySingleShutterRectBottomRight = 0;
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		for (int shutterIndex = 0; shutterIndex < shutterCount;
             shutterIndex++) {
			xSingleShutterRectTopLeft
                = currentPagePixmapRect.width()
                  - (shutterIndex * singleShutterWidth + currentStepLocation);
			ySingleShutterRectTopLeft = currentPagePixmapRect.topLeft().y();
			xSingleShutterRectBottomRight = currentPagePixmapRect.width()
                                            - shutterIndex * singleShutterWidth;
			ySingleShutterRectBottomRight
                = currentPagePixmapRect.bottomRight().y();
			singleShutterRectTopLeft.setX(xSingleShutterRectTopLeft);
			singleShutterRectTopLeft.setY(ySingleShutterRectTopLeft);
			singleShutterRectBottomRight.setX(xSingleShutterRectBottomRight);
			singleShutterRectBottomRight.setY(ySingleShutterRectBottomRight);
			singleShutterRect.setTopLeft(singleShutterRectTopLeft);
			singleShutterRect.setBottomRight(singleShutterRectBottomRight);
			QPixmap singleShutterRectStepPixmap
                = currentPagePixmap.copy(singleShutterRect);
			QPointF singleShutterStepPixmapPaintPoint
                = singleShutterRectTopLeft;
			painter.drawPixmap(singleShutterStepPixmapPaintPoint,
                               singleShutterRectStepPixmap);
		}
		painter.end();
		if (currentStepLocation == singleShutterWidth) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::shutterRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		int singleShutterWidth = 16;
		int shutterCount = qCeil(region.width() / singleShutterWidth);
		if (currentStepLocation > singleShutterWidth) {
			currentStepLocation = singleShutterWidth;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect singleShutterRect;
		QPoint singleShutterRectTopLeft;
		QPoint singleShutterRectBottomRight;
		int xSingleShutterRectTopLeft = 0, ySingleShutterRectTopLeft = 0;
		int xSingleShutterRectBottomRight = 0,
            ySingleShutterRectBottomRight = 0;
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		for (int shutterIndex = 0; shutterIndex < shutterCount;
             shutterIndex++) {
			xSingleShutterRectTopLeft = shutterIndex * singleShutterWidth;
			ySingleShutterRectTopLeft = currentPagePixmapRect.topLeft().y();
			xSingleShutterRectBottomRight = shutterIndex * singleShutterWidth
                                            + currentStepLocation;
			ySingleShutterRectBottomRight
                = currentPagePixmapRect.bottomRight().y();
			singleShutterRectTopLeft.setX(xSingleShutterRectTopLeft);
			singleShutterRectTopLeft.setY(ySingleShutterRectTopLeft);
			singleShutterRectBottomRight.setX(xSingleShutterRectBottomRight);
			singleShutterRectBottomRight.setY(ySingleShutterRectBottomRight);
			singleShutterRect.setTopLeft(singleShutterRectTopLeft);
			singleShutterRect.setBottomRight(singleShutterRectBottomRight);
			QPixmap singleShutterRectStepPixmap
                = currentPagePixmap.copy(singleShutterRect);
			QPointF singleShutterStepPixmapPaintPoint
                = singleShutterRectTopLeft;
			painter.drawPixmap(singleShutterStepPixmapPaintPoint,
                               singleShutterRectStepPixmap);
		}
		painter.end();
		if (currentStepLocation == singleShutterWidth) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::blink() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
        if (blinkCount < 5 * attr->step) {
			QPixmap pix(region.width(), region.height());
			pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
			QPixmap currentPagePixmap;
			if (0 == blinkCount % 2) {
                currentPagePixmap = QPixmap(region.width(), region.height());
                currentPagePixmap.fill(
                    QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
                QRect fillRectBlink(QPoint(0, 0),
                                    QPoint(region.width(), region.height()));
                QColor colorBlink(125, 125, 125);
                QBrush brushBlink(colorBlink);
                QPainter painterBlink;
                painterBlink.begin(&currentPagePixmap);
                painterBlink.fillRect(fillRectBlink, brushBlink);
                painterBlink.end();
			} else {
                currentPagePixmap = getCurrentPagePixmap();
			}
			QPainter painter;
			painter.begin(&pix);
			painter.setRenderHint(QPainter::Antialiasing);
			QPointF currentPageStepPixmapPaintpoint = region.topLeft();
			painter.drawPixmap(currentPageStepPixmapPaintpoint,
                               currentPagePixmap);
			painter.end();
			blinkCount++;
			return pix;
		} else {
			blinkCount = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
			QPixmap pix(region.width(), region.height());
			pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
            QPixmap currentPagePixmap = getCurrentPagePixmap();
            QPainter painter;
            painter.begin(&pix);
            painter.setRenderHint(QPainter::Antialiasing);
            QPointF currentPageStepPixmapPaintpoint = region.topLeft();
            painter.drawPixmap(currentPageStepPixmapPaintpoint,
                               currentPagePixmap);
            painter.end();
			return pix;
		}
	}
}

QPixmap Animator::brushUp() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
        QPainter painter;
        painter.begin(&pix);
        painter.setRenderHint(QPainter::Antialiasing);
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int brushRange = region.height();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > brushRange) {
			currentStepLocation = brushRange;
		}
        if (brushVeil) {
            painter.drawPixmap(region.topLeft(), currentPagePixmap);
        } else {
            QRect currentPagePixmapRect = currentPagePixmap.rect();
            QRect currentPagePixmapStepRect;
            QPoint currentPagePixmapStepRectTopLeft;
            currentPagePixmapStepRectTopLeft.setX(
                currentPagePixmapRect.topLeft().x());
            currentPagePixmapStepRectTopLeft.setY(currentPagePixmapRect.height()
                                                  - currentStepLocation);
            currentPagePixmapStepRect.setTopLeft(
                currentPagePixmapStepRectTopLeft);
            currentPagePixmapStepRect.setBottomRight(
                currentPagePixmapRect.bottomRight());
            QPixmap currentPageStepPixmap
                = currentPagePixmap.copy(currentPagePixmapStepRect);
            QPointF currentPageStepPixmapPaintPoint(
                region.x(), region.height() - currentStepLocation);
            painter.drawPixmap(currentPageStepPixmapPaintPoint,
                               currentPageStepPixmap);
        }
		QImage pixImage = pix.toImage();
		for (int xPos = 0; xPos < pixImage.width(); xPos++) {
			int pixPosX = xPos;
			int pixPosY = pixImage.height() - currentStepLocation;
			if (pixImage.valid(pixPosX, pixPosY)) {
				QRgb pixRgb = pixImage.pixel(pixPosX, pixPosY);
                painter.setPen(QColor::fromRgba(pixRgb));
				painter.drawLine(pixPosX, pixPosY, pixPosX, 0);
			}
		}
		painter.end();
		if (currentStepLocation == brushRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::brushDown() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
        QPainter painter;
        painter.begin(&pix);
        painter.setRenderHint(QPainter::Antialiasing);
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int brushRange = region.height();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > brushRange) {
			currentStepLocation = brushRange;
		}
        if (brushVeil) {
            painter.drawPixmap(region.topLeft(), currentPagePixmap);
        } else {
            QRect currentPagePixmapRect = currentPagePixmap.rect();
            QRect currentPagePixmapStepRect;
            currentPagePixmapStepRect.setTopLeft(
                currentPagePixmapRect.topLeft());
            QPoint currentPagePixmapStepRectBottomRight;
            currentPagePixmapStepRectBottomRight.setX(
                currentPagePixmapRect.bottomRight().x());
            currentPagePixmapStepRectBottomRight.setY(currentStepLocation);
            currentPagePixmapStepRect.setBottomRight(
                currentPagePixmapStepRectBottomRight);
            QPixmap currentPageStepPixmap
                = currentPagePixmap.copy(currentPagePixmapStepRect);
            QPointF currentPageStepPixmapPaintPoint = region.topLeft();
            painter.drawPixmap(currentPageStepPixmapPaintPoint,
                               currentPageStepPixmap);
        }
		QImage pixImage = pix.toImage();
		for (int xPos = 0; xPos < pixImage.width(); xPos++) {
			int pixPosX = xPos;
			int pixPosY = currentStepLocation;
			if (pixImage.valid(pixPosX, pixPosY)) {
				QRgb pixRgb = pixImage.pixel(pixPosX, pixPosY);
                painter.setPen(QColor::fromRgba(pixRgb));
				painter.drawLine(pixPosX, pixPosY, pixPosX, pixImage.height());
			}
		}
		painter.end();
		if (currentStepLocation == brushRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::brushLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
        pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
        QPainter painter;
        painter.begin(&pix);
        painter.setRenderHint(QPainter::Antialiasing);
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int brushRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > brushRange) {
			currentStepLocation = brushRange;
		}
        if (brushVeil) {
            painter.drawPixmap(region.topLeft(), currentPagePixmap);
        } else {
            QRect currentPagePixmapRect = currentPagePixmap.rect();
            QRect currentPagePixmapStepRect;
            QPoint currentPagePixmapStepRectTopLeft;
            currentPagePixmapStepRectTopLeft.setX(currentPagePixmapRect.width()
                                                  - currentStepLocation);
            currentPagePixmapStepRectTopLeft.setY(
                currentPagePixmapRect.topLeft().y());
            currentPagePixmapStepRect.setTopLeft(
                currentPagePixmapStepRectTopLeft);
            currentPagePixmapStepRect.setBottomRight(
                currentPagePixmapRect.bottomRight());
            QPixmap currentPageStepPixmap
                = currentPagePixmap.copy(currentPagePixmapStepRect);
            QPointF currentPageStepPixmapPaintPoint(
                region.width() - currentStepLocation, region.y());
            painter.drawPixmap(currentPageStepPixmapPaintPoint,
                               currentPageStepPixmap);
        }
		QImage pixImage = pix.toImage();
		for (int yPos = 0; yPos < pixImage.height(); yPos++) {
			int pixPosX = pixImage.width() - currentStepLocation;
			int pixPosY = yPos;
			if (pixImage.valid(pixPosX, pixPosY)) {
				QRgb pixRgb = pixImage.pixel(pixPosX, pixPosY);
                painter.setPen(QColor::fromRgba(pixRgb));
				painter.drawLine(pixPosX, pixPosY, 0, pixPosY);
			}
		}
		painter.end();
		if (currentStepLocation == brushRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::brushRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
        QPainter painter;
        painter.begin(&pix);
        painter.setRenderHint(QPainter::Antialiasing);
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int brushRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > brushRange) {
			currentStepLocation = brushRange;
		}
        if (brushVeil) {
            painter.drawPixmap(region.topLeft(), currentPagePixmap);
        } else {
            QRect currentPagePixmapRect = currentPagePixmap.rect();
            QRect currentPagePixmapStepRect;
            currentPagePixmapStepRect.setTopLeft(
                currentPagePixmapRect.topLeft());
            QPoint currentPagePixmapStepRectBottomRight(
                currentStepLocation, currentPagePixmapRect.bottomRight().y());
            currentPagePixmapStepRect.setBottomRight(
                currentPagePixmapStepRectBottomRight);
            QPixmap currentPageStepPixmap
                = currentPagePixmap.copy(currentPagePixmapStepRect);
            QPointF currentPageStepPixmapPaintPoint = region.topLeft();
            painter.drawPixmap(currentPageStepPixmapPaintPoint,
                               currentPageStepPixmap);
        }
		QImage pixImage = pix.toImage();
		for (int yPos = 0; yPos < pixImage.height(); yPos++) {
			int pixPosX = currentStepLocation;
			int pixPosY = yPos;
			if (pixImage.valid(pixPosX, pixPosY)) {
				QRgb pixRgb = pixImage.pixel(pixPosX, pixPosY);
                painter.setPen(QColor::fromRgba(pixRgb));
				painter.drawLine(pixPosX, pixPosY, pix.width(), pixPosY);
			}
		}
		painter.end();
		if (currentStepLocation == brushRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::pageLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > pageTurnRange) {
			currentStepLocation = pageTurnRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect = currentPagePixmapRect;
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal slopeRadian
            = (PI / 2 - (PI / 3 - currentStepLocation * PI / 180));
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal interceptY = currentPagePixmapHeight
                           + currentPagePixmapWidth * 1.72 / 3.0;
		qreal interceptX = currentPagePixmapWidth;
		QVector<QPointF> pathPoints;
		if (slopeRadian < PI / 2.0) {
			qreal slope = -qTan(slopeRadian);
			qreal interceptX_Y = slope * interceptX + interceptY;
			if (interceptX_Y > 0) {
				QPointF point_1(0, currentPagePixmapHeight);
				QPointF point_2(0, 0);
				QPointF point_3(currentPagePixmapWidth, 0);
				QPointF point_4(currentPagePixmapWidth, interceptX_Y);
				QPointF point_5((currentPagePixmapHeight - interceptY) / slope,
                                currentPagePixmapHeight);
				pathPoints.append(point_1);
				pathPoints.append(point_2);
				pathPoints.append(point_3);
				pathPoints.append(point_4);
				pathPoints.append(point_5);
			} else {
				QPointF point_1(0, currentPagePixmapHeight);
				QPointF point_2(0, 0);
				QPointF point_3(-interceptY / slope, 0);
				QPointF point_4((currentPagePixmapHeight - interceptY) / slope,
                                currentPagePixmapHeight);
				pathPoints.append(point_1);
				pathPoints.append(point_2);
				pathPoints.append(point_3);
				pathPoints.append(point_4);
			}
			QPolygonF pathPologon(pathPoints);
			QPainterPath fillPath;
			fillPath.addPolygon(pathPologon);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillPath(fillPath,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		}
		painter.end();
		if (currentStepLocation == pageTurnRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::pageRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > pageTurnRange) {
			currentStepLocation = pageTurnRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect = currentPagePixmapRect;
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal slopeRadian
            = (PI / 2 - (PI / 3 - currentStepLocation * PI / 180));
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal pointY = currentPagePixmapHeight
                       + currentPagePixmapWidth * 1.72 / 3.0;
		qreal pointX = currentPagePixmapWidth;
		QVector<QPointF> pathPoints;
		if (slopeRadian < PI / 2.0) {
			qreal slope = qTan(slopeRadian);
			qreal interceptY_Y = -slope * currentPagePixmapWidth + pointY;
			if (interceptY_Y > 0) {
				QPointF point_1(currentPagePixmapWidth,
                                currentPagePixmapHeight);
				QPointF point_2(currentPagePixmapWidth, 0);
				QPointF point_3(0, 0);
				QPointF point_4(0, interceptY_Y);
				QPointF point_5(
                    (currentPagePixmapHeight - pointY) / slope + pointX,
                    currentPagePixmapHeight);
				pathPoints.append(point_1);
				pathPoints.append(point_2);
				pathPoints.append(point_3);
				pathPoints.append(point_4);
				pathPoints.append(point_5);
			} else {
				QPointF point_1(currentPagePixmapWidth,
                                currentPagePixmapHeight);
				QPointF point_2(currentPagePixmapWidth, 0);
				QPointF point_3(-pointY / slope + pointX, 0);
				QPointF point_4(
                    (currentPagePixmapHeight - pointY) / slope + pointX,
                    currentPagePixmapHeight);
				pathPoints.append(point_1);
				pathPoints.append(point_2);
				pathPoints.append(point_3);
				pathPoints.append(point_4);
			}
			QPolygonF pathPologon(pathPoints);
			QPainterPath fillPath;
			fillPath.addPolygon(pathPologon);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillPath(fillPath,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		}
		painter.end();
		if (currentStepLocation == pageTurnRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::fanInaugurate() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > fanRange) {
			currentStepLocation = fanRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect = currentPagePixmapRect;
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal slopeRadian = (PI / 2 - currentStepLocation * PI / 180);
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal interceptY = currentPagePixmapHeight;
		qreal interceptX = currentPagePixmapWidth / 2;
		painter.translate(interceptX, 0);
		QVector<QPointF> pathPoints;
		if (slopeRadian > 0 && slopeRadian < PI / 2) {
			qreal slope = -qTan(slopeRadian);
			qreal interceptX_Y = slope * interceptX + interceptY;
			QPainterPath fillPath;
			if (interceptX_Y > 0) {
				QPointF point_R1(0, currentPagePixmapHeight);
				QPointF point_R2(interceptX, interceptX_Y);
				QPointF point_R3(interceptX, interceptY);
				pathPoints.append(point_R1);
				pathPoints.append(point_R2);
				pathPoints.append(point_R3);
				QPolygonF pathPologonR(pathPoints);
				fillPath.addPolygon(pathPologonR);
				pathPoints.erase(pathPoints.begin(), pathPoints.end());
				QPointF point_L1(0, currentPagePixmapHeight);
				QPointF point_L2(-interceptX, interceptX_Y);
				QPointF point_L3(-interceptX, interceptY);
				pathPoints.append(point_L1);
				pathPoints.append(point_L2);
				pathPoints.append(point_L3);
				QPolygonF pathPologonL(pathPoints);
				fillPath.addPolygon(pathPologonL);
			} else {
				QPointF point_R1(0, currentPagePixmapHeight);
				QPointF point_R2(-interceptY / slope, 0);
				QPointF point_R3(interceptX, 0);
				QPointF point_R4(interceptX, interceptY);
				pathPoints.append(point_R1);
				pathPoints.append(point_R2);
				pathPoints.append(point_R3);
				pathPoints.append(point_R4);
				QPolygonF pathPologonR(pathPoints);
				fillPath.addPolygon(pathPologonR);
				pathPoints.erase(pathPoints.begin(), pathPoints.end());
				QPointF point_L1(0, currentPagePixmapHeight);
				QPointF point_L2(interceptY / slope, 0);
				QPointF point_L3(-interceptX, 0);
				QPointF point_L4(-interceptX, interceptY);
				pathPoints.append(point_L1);
				pathPoints.append(point_L2);
				pathPoints.append(point_L3);
				pathPoints.append(point_L4);
				QPolygonF pathPologonL(pathPoints);
				fillPath.addPolygon(pathPologonL);
			}
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillPath(fillPath,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		}
		painter.end();
		if (currentStepLocation == fanRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::fanConclude() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > fanRange) {
			currentStepLocation = fanRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect = currentPagePixmapRect;
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal slopeRadian = currentStepLocation * PI / 180;
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal interceptY = currentPagePixmapHeight;
		qreal interceptX = currentPagePixmapWidth / 2;
		painter.translate(interceptX, 0);
		QVector<QPointF> pathPoints;
		if (slopeRadian > 0 && slopeRadian < PI / 2) {
			qreal slope = -qTan(slopeRadian);
			qreal interceptX_Y = slope * interceptX + interceptY;
			QPainterPath fillPath;
			if (interceptX_Y > 0) {
				QPointF point_1(0, currentPagePixmapHeight);
				QPointF point_2(-interceptX, interceptX_Y);
				QPointF point_3(-interceptX, 0);
				QPointF point_4(interceptX, 0);
				QPointF point_5(interceptX, interceptX_Y);
				pathPoints.append(point_1);
				pathPoints.append(point_2);
				pathPoints.append(point_3);
				pathPoints.append(point_4);
				pathPoints.append(point_5);
				QPolygonF pathPologon(pathPoints);
				fillPath.addPolygon(pathPologon);
			} else {
				QPointF point_1(0, currentPagePixmapHeight);
				QPointF point_2(interceptY / slope, 0);
				QPointF point_3(-interceptY / slope, 0);
				pathPoints.append(point_1);
				pathPoints.append(point_2);
				pathPoints.append(point_3);
				QPolygonF pathPologon(pathPoints);
				fillPath.addPolygon(pathPologon);
			}
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillPath(fillPath,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		}
		painter.end();
		if (currentStepLocation == fanRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::spinLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > spinRange) {
			currentStepLocation = spinRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect = currentPagePixmapRect;
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal slopeRadian = PI * 3 / 2 - currentStepLocation * PI / 180;
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal interceptY = currentPagePixmapHeight / 2;
		qreal interceptX = currentPagePixmapWidth / 2;
		painter.translate(interceptX, interceptY);
		QVector<QPointF> pathPoints;
		if (slopeRadian >= (PI / 2 + PI / 180) && slopeRadian < PI * 3 / 2) {
			qreal slope = qTan(slopeRadian);
			qreal interceptX_Y = slope * interceptX;
			QPainterPath fillPath;
			if (interceptX_Y >= 0) {
				if (interceptX_Y < interceptY) {
					QPointF point_1_1(0, 0);
					QPointF point_1_2(0, -interceptY);
					QPointF point_1_3(interceptX, -interceptY);
					QPointF point_1_4(interceptX, slope * interceptX);
					pathPoints.append(point_1_1);
					pathPoints.append(point_1_2);
					pathPoints.append(point_1_3);
					pathPoints.append(point_1_4);
					QPolygonF pathPologon1(pathPoints);
					fillPath.addPolygon(pathPologon1);
					pathPoints.erase(pathPoints.begin(), pathPoints.end());
					QPointF point_3_1(0, 0);
					QPointF point_3_2(0, interceptY);
					QPointF point_3_3(-interceptX, interceptY);
					QPointF point_3_4(-interceptX, -slope * interceptX);
					pathPoints.append(point_3_1);
					pathPoints.append(point_3_2);
					pathPoints.append(point_3_3);
					pathPoints.append(point_3_4);
					QPolygonF pathPologon3(pathPoints);
					fillPath.addPolygon(pathPologon3);
				} else {
					QPointF point_1_1(0, 0);
					QPointF point_1_2(0, -interceptY);
					QPointF point_1_3(interceptX, -interceptY);
					QPointF point_1_4(interceptX, interceptY);
					QPointF point_1_5(interceptY / slope, interceptY);
					pathPoints.append(point_1_1);
					pathPoints.append(point_1_2);
					pathPoints.append(point_1_3);
					pathPoints.append(point_1_4);
					pathPoints.append(point_1_5);
					QPolygonF pathPologon1(pathPoints);
					fillPath.addPolygon(pathPologon1);
					pathPoints.erase(pathPoints.begin(), pathPoints.end());
					QPointF point_3_1(0, 0);
					QPointF point_3_2(0, interceptY);
					QPointF point_3_3(-interceptX, interceptY);
					QPointF point_3_4(-interceptX, -interceptY);
					QPointF point_3_5(-interceptY / slope, -interceptY);
					pathPoints.append(point_3_1);
					pathPoints.append(point_3_2);
					pathPoints.append(point_3_3);
					pathPoints.append(point_3_4);
					pathPoints.append(point_3_5);
					QPolygonF pathPologon3(pathPoints);
					fillPath.addPolygon(pathPologon3);
				}
			} else {
				if (interceptX_Y < -interceptY) {
					QPointF point_4_1(0, 0);
					QPointF point_4_2(0, -interceptY);
					QPointF point_4_3(-interceptY / slope, -interceptY);
					pathPoints.append(point_4_1);
					pathPoints.append(point_4_2);
					pathPoints.append(point_4_3);
					QPolygonF pathPologon4(pathPoints);
					fillPath.addPolygon(pathPologon4);
					pathPoints.erase(pathPoints.begin(), pathPoints.end());
					QPointF point_2_1(0, 0);
					QPointF point_2_2(0, interceptY);
					QPointF point_2_3(interceptY / slope, interceptY);
					pathPoints.append(point_2_1);
					pathPoints.append(point_2_2);
					pathPoints.append(point_2_3);
					QPolygonF pathPologon2(pathPoints);
					fillPath.addPolygon(pathPologon2);
				} else {
					QPointF point_4_1(0, 0);
					QPointF point_4_2(0, -interceptY);
					QPointF point_4_3(interceptX, -interceptY);
					QPointF point_4_4(interceptX, slope * interceptX);
					pathPoints.append(point_4_1);
					pathPoints.append(point_4_2);
					pathPoints.append(point_4_3);
					pathPoints.append(point_4_4);
					QPolygonF pathPologon4(pathPoints);
					fillPath.addPolygon(pathPologon4);
					pathPoints.erase(pathPoints.begin(), pathPoints.end());
					QPointF point_2_1(0, 0);
					QPointF point_2_2(0, interceptY);
					QPointF point_2_3(-interceptX, interceptY);
					QPointF point_2_4(-interceptX, -slope * interceptX);
					pathPoints.append(point_2_1);
					pathPoints.append(point_2_2);
					pathPoints.append(point_2_3);
					pathPoints.append(point_2_4);
					QPolygonF pathPologon2(pathPoints);
					fillPath.addPolygon(pathPologon2);
				}
			}
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillPath(fillPath,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		}
		painter.end();
		if (currentStepLocation == spinRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::spinRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > spinRange) {
			currentStepLocation = spinRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect currentPagePixmapStepRect = currentPagePixmapRect;
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal slopeRadian = PI / 2 + currentStepLocation * PI / 180;
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal interceptY = currentPagePixmapHeight / 2;
		qreal interceptX = currentPagePixmapWidth / 2;
		painter.translate(interceptX, interceptY);
		QVector<QPointF> pathPoints;
		if (slopeRadian > PI / 2 && slopeRadian < PI * 3 / 2) {
			qreal slope = qTan(slopeRadian);
			qreal interceptX_Y = slope * interceptX;
			QPainterPath fillPath;
			if (interceptX_Y >= 0) {
				if (interceptX_Y < interceptY) {
					QPointF point_1_1(0, 0);
					QPointF point_1_2(interceptX, interceptX_Y);
					QPointF point_1_3(interceptX, interceptY);
					QPointF point_1_4(0, interceptY);
					pathPoints.append(point_1_1);
					pathPoints.append(point_1_2);
					pathPoints.append(point_1_3);
					pathPoints.append(point_1_4);
					QPolygonF pathPologon1(pathPoints);
					fillPath.addPolygon(pathPologon1);
					pathPoints.erase(pathPoints.begin(), pathPoints.end());
					QPointF point_3_1(0, 0);
					QPointF point_3_2(-interceptX, -interceptX_Y);
					QPointF point_3_3(-interceptX, -interceptY);
					QPointF point_3_4(0, -interceptY);
					pathPoints.append(point_3_1);
					pathPoints.append(point_3_2);
					pathPoints.append(point_3_3);
					pathPoints.append(point_3_4);
					QPolygonF pathPologon3(pathPoints);
					fillPath.addPolygon(pathPologon3);
				} else {
					QPointF point_1_1(0, 0);
					QPointF point_1_2(interceptY / slope, interceptY);
					QPointF point_1_3(0, interceptY);
					pathPoints.append(point_1_1);
					pathPoints.append(point_1_2);
					pathPoints.append(point_1_3);
					QPolygonF pathPologon1(pathPoints);
					fillPath.addPolygon(pathPologon1);
					pathPoints.erase(pathPoints.begin(), pathPoints.end());
					QPointF point_3_1(0, 0);
					QPointF point_3_2(-interceptY / slope, -interceptY);
					QPointF point_3_3(0, -interceptY);
					pathPoints.append(point_3_1);
					pathPoints.append(point_3_2);
					pathPoints.append(point_3_3);
					QPolygonF pathPologon3(pathPoints);
					fillPath.addPolygon(pathPologon3);
				}
			} else {
				if (interceptX_Y < -interceptY) {
					QPointF point_4_1(0, 0);
					QPointF point_4_2(-interceptY / slope, -interceptY);
					QPointF point_4_3(interceptX, -interceptY);
					QPointF point_4_4(interceptX, interceptY);
					QPointF point_4_5(0, interceptY);
					pathPoints.append(point_4_1);
					pathPoints.append(point_4_2);
					pathPoints.append(point_4_3);
					pathPoints.append(point_4_4);
					pathPoints.append(point_4_5);
					QPolygonF pathPologon4(pathPoints);
					fillPath.addPolygon(pathPologon4);
					pathPoints.erase(pathPoints.begin(), pathPoints.end());
					QPointF point_2_1(0, 0);
					QPointF point_2_2(interceptY / slope, interceptY);
					QPointF point_2_3(-interceptX, interceptY);
					QPointF point_2_4(-interceptX, -interceptY);
					QPointF point_2_5(0, -interceptY);
					pathPoints.append(point_2_1);
					pathPoints.append(point_2_2);
					pathPoints.append(point_2_3);
					pathPoints.append(point_2_4);
					pathPoints.append(point_2_5);
					QPolygonF pathPologon2(pathPoints);
					fillPath.addPolygon(pathPologon2);
				} else {
					QPointF point_4_1(0, 0);
					QPointF point_4_2(interceptX, slope * interceptX);
					QPointF point_4_3(interceptX, interceptY);
					QPointF point_4_4(0, interceptY);
					pathPoints.append(point_4_1);
					pathPoints.append(point_4_2);
					pathPoints.append(point_4_3);
					pathPoints.append(point_4_4);
					QPolygonF pathPologon4(pathPoints);
					fillPath.addPolygon(pathPologon4);
					pathPoints.erase(pathPoints.begin(), pathPoints.end());
					QPointF point_2_1(0, 0);
					QPointF point_2_2(-interceptX, -slope * interceptX);
					QPointF point_2_3(-interceptX, -interceptY);
					QPointF point_2_4(0, -interceptY);
					pathPoints.append(point_2_1);
					pathPoints.append(point_2_2);
					pathPoints.append(point_2_3);
					pathPoints.append(point_2_4);
					QPolygonF pathPologon2(pathPoints);
					fillPath.addPolygon(pathPologon2);
				}
			}
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillPath(fillPath,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		}
		painter.end();
		if (currentStepLocation == spinRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::rectangleInaugurate() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int rectangleRange = region.width() / 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > rectangleRange) {
			currentStepLocation = rectangleRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal slope = currentPagePixmapHeight / currentPagePixmapWidth;
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(currentPagePixmapWidth / 2
                                              - currentStepLocation);
		currentPagePixmapStepRectTopLeft.setY(
            slope * (currentPagePixmapWidth / 2 - currentStepLocation));
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(currentPagePixmapWidth / 2
                                                  + currentStepLocation);
		currentPagePixmapStepRectBottomRight.setY(
            slope * (currentPagePixmapWidth / 2 + currentStepLocation));
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint
            = currentPagePixmapStepRectTopLeft;
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == rectangleRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::rectangleConclude() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int rectangleRange = region.width() / 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > rectangleRange) {
			currentStepLocation = rectangleRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal slope = currentPagePixmapHeight / currentPagePixmapWidth;
		QPoint currentPagePixmapStepRectTopLeft(currentStepLocation,
                                                slope * currentStepLocation);
		QPoint currentPagePixmapStepRectBottomRight(
            currentPagePixmapWidth - currentStepLocation,
            slope * (currentPagePixmapWidth - currentStepLocation));
		QRect currentPagePixmapStepRect(currentPagePixmapStepRectTopLeft,
                                        currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawPixmap(QPoint(0, 0), currentPageStepPixmap);
        if (currentPagePixmapStepRect.width() > 2) {
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillRect(currentPagePixmapStepRect,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
        }
		painter.end();
		if (currentStepLocation == rectangleRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::curtainDown() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		int singleShutterWidth = 16;
		int shutterCount = qCeil(region.height() / singleShutterWidth);
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect singleShutterRect;
		QPoint singleShutterRectTopLeft;
		QPoint singleShutterRectBottomRight;
		int xSingleShutterRectTopLeft = 0, ySingleShutterRectTopLeft = 0;
		int xSingleShutterRectBottomRight = 0,
            ySingleShutterRectBottomRight = 0;
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		if (currentStepLocation <= singleShutterWidth) {
			for (int shutterIndex = 0; shutterIndex < shutterCount;
                 shutterIndex++) {
				xSingleShutterRectTopLeft = currentPagePixmapRect.topLeft().x();
				ySingleShutterRectTopLeft = shutterIndex * singleShutterWidth;
				xSingleShutterRectBottomRight
                    = currentPagePixmapRect.bottomRight().x();
				ySingleShutterRectBottomRight
                    = shutterIndex * singleShutterWidth
                      + (int)(shutterCount - shutterIndex) / (qreal)shutterCount
                        * currentStepLocation;
				singleShutterRectTopLeft.setX(xSingleShutterRectTopLeft);
				singleShutterRectTopLeft.setY(ySingleShutterRectTopLeft);
				singleShutterRectBottomRight.setX(
                    xSingleShutterRectBottomRight);
				singleShutterRectBottomRight.setY(
                    ySingleShutterRectBottomRight);
				singleShutterRect.setTopLeft(singleShutterRectTopLeft);
				singleShutterRect.setBottomRight(singleShutterRectBottomRight);
				QPixmap singleShutterRectStepPixmap
                    = currentPagePixmap.copy(singleShutterRect);
				QPointF singleShutterStepPixmapPaintPoint
                    = singleShutterRectTopLeft;
				painter.drawPixmap(singleShutterStepPixmapPaintPoint,
                                   singleShutterRectStepPixmap);
			}
		} else {
			curtainMoveIndex++;
			QPoint topRectTopLeft(0, 0);
			QPoint topRectBottomRight(currentPagePixmapRect.bottomRight().x(),
                                      curtainMoveIndex * singleShutterWidth);
			QRect topRect(topRectTopLeft, topRectBottomRight);
			QPoint bottomRectTopLeft(currentPagePixmapRect.topLeft().x(),
                                     curtainMoveIndex * singleShutterWidth);
			QPoint bottomRectBottomRight = currentPagePixmapRect.bottomRight();
			QRect bottomRect(bottomRectTopLeft, bottomRectBottomRight);
			QPixmap topPix = currentPagePixmap.copy(topRect);
			painter.drawPixmap(topRect.topLeft(), topPix);
			QPixmap bottomPix(region.width(), region.height());
			bottomPix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
			QPainter bottomPainter;
			bottomPainter.begin(&bottomPix);
			QRect currentPageMovePixmapRect = bottomRect;
			int moveShutterCount
                = qCeil(currentPageMovePixmapRect.width() / singleShutterWidth);
			QPixmap currentPageMovePixmap
                = currentPagePixmap.copy(currentPageMovePixmapRect);
			for (int shutterIndex = 0; shutterIndex < moveShutterCount;
                 shutterIndex++) {
				xSingleShutterRectTopLeft
                    = currentPageMovePixmapRect.topLeft().x();
				ySingleShutterRectTopLeft = shutterIndex * singleShutterWidth;
				xSingleShutterRectBottomRight
                    = currentPageMovePixmapRect.bottomRight().x();
				ySingleShutterRectBottomRight
                    = shutterIndex * singleShutterWidth
                      + (int)(moveShutterCount - shutterIndex)
                        / (qreal)moveShutterCount * singleShutterWidth;
				singleShutterRectTopLeft.setX(xSingleShutterRectTopLeft);
				singleShutterRectTopLeft.setY(ySingleShutterRectTopLeft);
				singleShutterRectBottomRight.setX(
                    xSingleShutterRectBottomRight);
				singleShutterRectBottomRight.setY(
                    ySingleShutterRectBottomRight);
				singleShutterRect.setTopLeft(singleShutterRectTopLeft);
				singleShutterRect.setBottomRight(singleShutterRectBottomRight);
				QPixmap singleShutterRectStepPixmap
                    = currentPageMovePixmap.copy(singleShutterRect);
				QPointF singleShutterStepPixmapPaintPoint
                    = singleShutterRectTopLeft;
				bottomPainter.drawPixmap(singleShutterStepPixmapPaintPoint,
                                         singleShutterRectStepPixmap);
			}
			bottomPainter.end();
			painter.drawPixmap(bottomRect.topLeft(), bottomPix);
		}
		painter.end();
		if (curtainMoveIndex > shutterCount) {
			curtainMoveIndex = 0;
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::curtainRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		int singleShutterWidth = 16;
		int shutterCount = qCeil(region.width() / singleShutterWidth);
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QRect singleShutterRect;
		QPoint singleShutterRectTopLeft;
		QPoint singleShutterRectBottomRight;
		int xSingleShutterRectTopLeft = 0, ySingleShutterRectTopLeft = 0;
		int xSingleShutterRectBottomRight = 0,
            ySingleShutterRectBottomRight = 0;
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		if (currentStepLocation <= singleShutterWidth) {
			for (int shutterIndex = 0; shutterIndex < shutterCount;
                 shutterIndex++) {
				xSingleShutterRectTopLeft = shutterIndex * singleShutterWidth;
				ySingleShutterRectTopLeft = currentPagePixmapRect.topLeft().y();
				xSingleShutterRectBottomRight
                    = shutterIndex * singleShutterWidth
                      + (int)(shutterCount - shutterIndex) / (qreal)shutterCount
                        * currentStepLocation;
				ySingleShutterRectBottomRight
                    = currentPagePixmapRect.bottomRight().y();
				singleShutterRectTopLeft.setX(xSingleShutterRectTopLeft);
				singleShutterRectTopLeft.setY(ySingleShutterRectTopLeft);
				singleShutterRectBottomRight.setX(
                    xSingleShutterRectBottomRight);
				singleShutterRectBottomRight.setY(
                    ySingleShutterRectBottomRight);
				singleShutterRect.setTopLeft(singleShutterRectTopLeft);
				singleShutterRect.setBottomRight(singleShutterRectBottomRight);
				QPixmap singleShutterRectStepPixmap
                    = currentPagePixmap.copy(singleShutterRect);
				QPointF singleShutterStepPixmapPaintPoint
                    = singleShutterRectTopLeft;
				painter.drawPixmap(singleShutterStepPixmapPaintPoint,
                                   singleShutterRectStepPixmap);
			}
		} else {
			curtainMoveIndex++;
			QPoint leftRectTopLeft(0, 0);
			QPoint leftRectBottomRight(curtainMoveIndex * singleShutterWidth,
                                       currentPagePixmapRect.bottomRight().y());
			QRect leftRect(leftRectTopLeft, leftRectBottomRight);
			QPoint rightRectTopLeft(curtainMoveIndex * singleShutterWidth, 0);
			QPoint rightRectBottomRight = currentPagePixmapRect.bottomRight();
			QRect rightRect(rightRectTopLeft, rightRectBottomRight);
			QPixmap leftPix = currentPagePixmap.copy(leftRect);
			painter.drawPixmap(leftRect.topLeft(), leftPix);
			QPixmap rightPix(region.width(), region.height());
			rightPix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
			QPainter rightPainter;
			rightPainter.begin(&rightPix);
			QRect currentPageMovePixmapRect = rightRect;
			int moveShutterCount
                = qCeil(currentPageMovePixmapRect.width() / singleShutterWidth);
			QPixmap currentPageMovePixmap
                = currentPagePixmap.copy(currentPageMovePixmapRect);
			for (int shutterIndex = 0; shutterIndex < moveShutterCount;
                 shutterIndex++) {
				xSingleShutterRectTopLeft = shutterIndex * singleShutterWidth;
				ySingleShutterRectTopLeft
                    = currentPageMovePixmapRect.topLeft().y();
				xSingleShutterRectBottomRight
                    = shutterIndex * singleShutterWidth
                      + (int)(moveShutterCount - shutterIndex)
                        / (qreal)moveShutterCount * singleShutterWidth;
				ySingleShutterRectBottomRight
                    = currentPageMovePixmapRect.bottomRight().y();
				singleShutterRectTopLeft.setX(xSingleShutterRectTopLeft);
				singleShutterRectTopLeft.setY(ySingleShutterRectTopLeft);
				singleShutterRectBottomRight.setX(
                    xSingleShutterRectBottomRight);
				singleShutterRectBottomRight.setY(
                    ySingleShutterRectBottomRight);
				singleShutterRect.setTopLeft(singleShutterRectTopLeft);
				singleShutterRect.setBottomRight(singleShutterRectBottomRight);
				QPixmap singleShutterRectStepPixmap
                    = currentPageMovePixmap.copy(singleShutterRect);
				QPointF singleShutterStepPixmapPaintPoint
                    = singleShutterRectTopLeft;
				rightPainter.drawPixmap(singleShutterStepPixmapPaintPoint,
                                        singleShutterRectStepPixmap);
			}
			rightPainter.end();
			painter.drawPixmap(rightRect.topLeft(), rightPix);
		}
		painter.end();
		if (curtainMoveIndex > shutterCount) {
			curtainMoveIndex = 0;
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::rectangleTopLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int rectangleRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > rectangleRange) {
			currentStepLocation = rectangleRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal slope = currentPagePixmapHeight / currentPagePixmapWidth;
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(currentPagePixmapWidth
                                              - currentStepLocation);
		currentPagePixmapStepRectTopLeft.setY(
            slope * (currentPagePixmapWidth - currentStepLocation));
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(currentPagePixmapWidth);
		currentPagePixmapStepRectBottomRight.setY(currentPagePixmapHeight);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint;
		qreal xPos = currentPagePixmapStepRectTopLeft.x();
		qreal yPos = currentPagePixmapStepRectTopLeft.y();
		currentPageStepPixmapPaintPoint.setX(xPos);
		currentPageStepPixmapPaintPoint.setY(yPos);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == rectangleRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::rectangleTopRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int rectangleRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > rectangleRange) {
			currentStepLocation = rectangleRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal slope = -currentPagePixmapHeight / currentPagePixmapWidth;
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(0);
		currentPagePixmapStepRectTopLeft.setY(slope * currentStepLocation
                                              + currentPagePixmapHeight);
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(currentStepLocation);
		currentPagePixmapStepRectBottomRight.setY(currentPagePixmapHeight);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint;
		qreal xPos = currentPagePixmapStepRectTopLeft.x();
		qreal yPos = currentPagePixmapStepRectTopLeft.y();
		currentPageStepPixmapPaintPoint.setX(xPos);
		currentPageStepPixmapPaintPoint.setY(yPos);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == rectangleRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::rectangleBottomLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int rectangleRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > rectangleRange) {
			currentStepLocation = rectangleRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal slope = -currentPagePixmapHeight / currentPagePixmapWidth;
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(currentPagePixmapWidth
                                              - currentStepLocation);
		currentPagePixmapStepRectTopLeft.setY(0);
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(currentPagePixmapWidth);
		currentPagePixmapStepRectBottomRight.setY(
            slope * (currentPagePixmapWidth - currentStepLocation)
            + currentPagePixmapHeight);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint;
		qreal xPos = currentPagePixmapStepRectTopLeft.x();
		qreal yPos = currentPagePixmapStepRectTopLeft.y();
		currentPageStepPixmapPaintPoint.setX(xPos);
		currentPageStepPixmapPaintPoint.setY(yPos);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == rectangleRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::rectangleBottomRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int rectangleRange = region.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > rectangleRange) {
			currentStepLocation = rectangleRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal slope = currentPagePixmapHeight / currentPagePixmapWidth;
		QRect currentPagePixmapStepRect;
		QPoint currentPagePixmapStepRectTopLeft;
		currentPagePixmapStepRectTopLeft.setX(0);
		currentPagePixmapStepRectTopLeft.setY(0);
		currentPagePixmapStepRect.setTopLeft(currentPagePixmapStepRectTopLeft);
		QPoint currentPagePixmapStepRectBottomRight;
		currentPagePixmapStepRectBottomRight.setX(currentStepLocation);
		currentPagePixmapStepRectBottomRight.setY(slope * currentStepLocation);
		currentPagePixmapStepRect.setBottomRight(
            currentPagePixmapStepRectBottomRight);
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapStepRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint;
		qreal xPos = currentPagePixmapStepRectTopLeft.x();
		qreal yPos = currentPagePixmapStepRectTopLeft.y();
		currentPageStepPixmapPaintPoint.setX(xPos);
		currentPageStepPixmapPaintPoint.setY(yPos);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		painter.end();
		if (currentStepLocation == rectangleRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::diagonalTopLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		int diagonalRange = region.height() * 2;
		if (currentStepLocation > diagonalRange) {
			currentStepLocation = diagonalRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal slope = -currentPagePixmapHeight / currentPagePixmapWidth;
		qreal interceptY = currentPagePixmapHeight;
		qreal interceptX = currentPagePixmapWidth;
		QVector<QPointF> pathPoints;
		QPainterPath fillPath;
		if ((diagonalRange - currentStepLocation) > 0
            && (diagonalRange - currentStepLocation) < interceptY) {
			QPointF point_1(0, 0);
			QPointF point_2(-(diagonalRange - currentStepLocation) / slope, 0);
			QPointF point_3(0, diagonalRange - currentStepLocation);
			pathPoints.append(point_1);
			pathPoints.append(point_2);
			pathPoints.append(point_3);
			QPolygonF pathPologon(pathPoints);
			fillPath.addPolygon(pathPologon);
		} else if ((diagonalRange - currentStepLocation) < diagonalRange) {
			QPointF point_1(0, 0);
			QPointF point_2(interceptX, 0);
			QPointF point_3(
                interceptX,
                slope * interceptX + diagonalRange - currentStepLocation);
			QPointF point_4(
                (interceptY - (diagonalRange - currentStepLocation)) / slope,
                interceptY);
			QPointF point_5(0, interceptY);
			pathPoints.append(point_1);
			pathPoints.append(point_2);
			pathPoints.append(point_3);
			pathPoints.append(point_4);
			pathPoints.append(point_5);
			QPolygonF pathPologon(pathPoints);
			fillPath.addPolygon(pathPologon);
		}
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillPath(fillPath, QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		painter.end();
		if (currentStepLocation == diagonalRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::diagonalTopRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int diagonalRange = region.height() * 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > diagonalRange) {
			currentStepLocation = diagonalRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal slope = currentPagePixmapHeight / currentPagePixmapWidth;
		qreal interceptY = currentPagePixmapHeight;
		qreal interceptX = currentPagePixmapWidth;
		QVector<QPointF> pathPoints;
		QPainterPath fillPath;
		if ((currentPagePixmapHeight - currentStepLocation) > 0
            && (currentPagePixmapHeight - currentStepLocation) < interceptY) {
			QPointF point_1(0, 0);
			QPointF point_2(interceptX, 0);
			QPointF point_3(interceptX, interceptY);
			QPointF point_4((interceptY - (currentPagePixmapHeight
                                           - currentStepLocation)) / slope,
                            interceptY);
			QPointF point_5(0, currentPagePixmapHeight - currentStepLocation);
			pathPoints.append(point_1);
			pathPoints.append(point_2);
			pathPoints.append(point_3);
			pathPoints.append(point_4);
			pathPoints.append(point_5);
			QPolygonF pathPologon(pathPoints);
			fillPath.addPolygon(pathPologon);
		} else if ((currentPagePixmapHeight - currentStepLocation) <= 0) {
			QPointF point_1(
                -(currentPagePixmapHeight - currentStepLocation) / slope, 0);
			QPointF point_2(interceptX, 0);
			QPointF point_3(interceptX,
                            slope * interceptX + currentPagePixmapHeight
                            - currentStepLocation);
			pathPoints.append(point_1);
			pathPoints.append(point_2);
			pathPoints.append(point_3);
			QPolygonF pathPologon(pathPoints);
			fillPath.addPolygon(pathPologon);
		}
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillPath(fillPath, QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		painter.end();
		if (currentStepLocation == diagonalRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::diagonalBottomLeft() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int diagonalRange = region.height() * 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > diagonalRange) {
			currentStepLocation = diagonalRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal slope = currentPagePixmapHeight / currentPagePixmapWidth;
		qreal interceptY = currentPagePixmapHeight;
		qreal interceptX = currentPagePixmapWidth;
		QVector<QPointF> pathPoints;
		QPainterPath fillPath;
		if ((currentStepLocation - currentPagePixmapHeight) > 0
            && (currentStepLocation - currentPagePixmapHeight) < interceptY) {
			QPointF point_1(0, currentStepLocation - currentPagePixmapHeight);
			QPointF point_2((interceptY - (currentStepLocation
                                           - currentPagePixmapHeight)) / slope,
                            interceptY);
			QPointF point_3(0, interceptY);
			pathPoints.append(point_1);
			pathPoints.append(point_2);
			pathPoints.append(point_3);
			QPolygonF pathPologon(pathPoints);
			fillPath.addPolygon(pathPologon);
		} else if ((currentStepLocation - currentPagePixmapHeight) <= 0) {
			QPointF point_1(0, 0);
			QPointF point_2(
                -(currentStepLocation - currentPagePixmapHeight) / slope, 0);
			QPointF point_3(interceptX,
                            slope * interceptX
                            + (currentStepLocation - currentPagePixmapHeight));
			QPointF point_4(interceptX, interceptY);
			QPointF point_5(0, interceptY);
			pathPoints.append(point_1);
			pathPoints.append(point_2);
			pathPoints.append(point_3);
			pathPoints.append(point_4);
			pathPoints.append(point_5);
			QPolygonF pathPologon(pathPoints);
			fillPath.addPolygon(pathPologon);
		}
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillPath(fillPath, QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		painter.end();
		if (currentStepLocation == diagonalRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::diagonalBottomRight() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int diagonalRange = region.height() * 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > diagonalRange) {
			currentStepLocation = diagonalRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		QPointF currentPageStepPixmapPaintPoint(0, 0);
		painter.drawPixmap(currentPageStepPixmapPaintPoint,
                           currentPageStepPixmap);
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal slope = -currentPagePixmapHeight / currentPagePixmapWidth;
		qreal interceptY = currentPagePixmapHeight;
		qreal interceptX = currentPagePixmapWidth;
		QVector<QPointF> pathPoints;
		QPainterPath fillPath;
		if (currentStepLocation > 0 && currentStepLocation < interceptY) {
			QPointF point_1(0, currentStepLocation);
			QPointF point_2(-currentStepLocation / slope, 0);
			QPointF point_3(interceptX, 0);
			QPointF point_4(interceptX, interceptY);
			QPointF point_5(0, interceptY);
			pathPoints.append(point_1);
			pathPoints.append(point_2);
			pathPoints.append(point_3);
			pathPoints.append(point_4);
			pathPoints.append(point_5);
			QPolygonF pathPologon(pathPoints);
			fillPath.addPolygon(pathPologon);
		} else if (currentStepLocation <= diagonalRange) {
			QPointF point_1((interceptY - currentStepLocation) / slope,
                            interceptY);
			QPointF point_2(interceptX,
                            slope * interceptX + currentStepLocation);
			QPointF point_3(interceptX, interceptY);

			pathPoints.append(point_1);
			pathPoints.append(point_2);
			pathPoints.append(point_3);

			QPolygonF pathPologon(pathPoints);
			fillPath.addPolygon(pathPologon);
		}
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillPath(fillPath, QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		painter.end();
		if (currentStepLocation == diagonalRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::crossExpand() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		currentStepLocation = currentStepLocation + attr->step;
		int crossRange = region.width() / 2;
		if (currentStepLocation > crossRange) {
			currentStepLocation = crossRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal slope = currentPagePixmapHeight / currentPagePixmapWidth;
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawPixmap(QPoint(0, 0), currentPageStepPixmap);
		painter.translate(currentPagePixmapWidth / 2,
                          currentPagePixmapHeight / 2);
		QPainterPath fillPath;
		if (currentStepLocation < currentPagePixmapWidth / 2) {
			QPointF point_1_1(currentStepLocation, slope * currentStepLocation);
			QPointF point_1_3(currentPagePixmapWidth / 2,
                              currentPagePixmapHeight / 2);
			if ((point_1_3.x() - point_1_1.x()) > 1.0) {
				fillPath.addRect(QRectF(point_1_1, point_1_3));
			}
			QPointF point_2_2(-currentPagePixmapWidth / 2,
                              slope * currentStepLocation);
			QPointF point_2_4(-currentStepLocation,
                              currentPagePixmapHeight / 2);
			if ((point_2_4.x() - point_2_2.x()) > 1.0) {
				fillPath.addRect(QRectF(point_2_2, point_2_4));
			}
			QPointF point_3_1(-currentStepLocation,
                              -slope * currentStepLocation);
			QPointF point_3_3(-currentPagePixmapWidth / 2,
                              -currentPagePixmapHeight / 2);
			if ((point_3_1.x() - point_3_3.x()) > 1.0) {
				fillPath.addRect(QRectF(point_3_3, point_3_1));
			}
			QPointF point_4_2(currentPagePixmapWidth / 2,
                              -slope * currentStepLocation);
			QPointF point_4_4(currentStepLocation,
                              -currentPagePixmapHeight / 2);
            if ((point_4_2.x() - point_4_4.x()) > 2.0) {
				fillPath.addRect(QRectF(point_4_4, point_4_2));
			}
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillPath(fillPath,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		}
		painter.end();
		if (currentStepLocation == crossRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::crossShrinkage() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		QPixmap currentPagePixmap = getCurrentPagePixmap();
		int crossRange = region.width() / 2;
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > crossRange) {
			currentStepLocation = crossRange;
		}
		QRect currentPagePixmapRect = currentPagePixmap.rect();
		qreal currentPagePixmapHeight = currentPagePixmapRect.height();
		qreal currentPagePixmapWidth = currentPagePixmapRect.width();
		qreal interceptY = currentPagePixmapHeight / 2;
		qreal interceptX = currentPagePixmapWidth / 2;
		qreal slope = interceptY / interceptX;
		QPixmap currentPageStepPixmap
            = currentPagePixmap.copy(currentPagePixmapRect);
		QPainter painter;
		painter.begin(&pix);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawPixmap(QPoint(0, 0), currentPageStepPixmap);
		painter.translate(interceptX, interceptY);
		QPainterPath fillPath;
		if ((interceptX - currentStepLocation) > 0) {
			QPointF point_1_2(interceptX,
                              slope * (interceptX - currentStepLocation));
			QPointF point_1_4((interceptX - currentStepLocation), interceptY);
			QPointF point_2_1(-(interceptX - currentStepLocation),
                              slope * (interceptX - currentStepLocation));
			QPointF point_3_2(-interceptX,
                              -slope * (interceptX - currentStepLocation));
			QPointF point_3_4(-(interceptX - currentStepLocation), -interceptY);
			QPointF point_4_1((interceptX - currentStepLocation),
                              -slope * (interceptX - currentStepLocation));
            if ((point_1_4.x() - point_2_1.x()) > 2.0) {
				fillPath.addRect(QRectF(point_2_1, point_1_4));
			}
            if ((point_1_2.x() - point_3_2.x()) > 2.0
                && (point_1_2.y() - point_3_2.y()) > 2.0) {
				fillPath.addRect(QRectF(point_3_2, point_1_2));
			}
            if ((point_4_1.x() - point_3_4.x()) > 2.0) {
				fillPath.addRect(QRectF(point_3_4, point_4_1));
			}
            painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.fillPath(fillPath,
                             QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		}
		painter.end();
		if (currentStepLocation == crossRange) {
			currentStepLocation = 0;
			currentPage = currentPage + 1;
			if (currentPage > pageCount) {
				currentPage = 1;
				pageCountCycleComplete = true;
				randomStyle = produceRandomStyle();
			}
		}
		return pix;
	}
}

QPixmap Animator::moveLeftContinuous() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		int moveRange = region.width() + source.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > moveRange) {
			currentStepLocation = moveRange;
		}
		QRect currentViewport;
		QPoint currentViewportPaintPos;
		if (currentStepLocation <= region.width()) {
			currentViewport.setTopLeft(QPoint(0, 0));
			currentViewport.setBottomRight(
                QPoint(currentStepLocation, region.height()));
			currentViewportPaintPos.setX(region.width() - currentStepLocation);
		} else if (currentStepLocation <= source.width()) {
			currentViewport.setTopLeft(
                QPoint(currentStepLocation - region.width(), 0));
			currentViewport.setBottomRight(
                QPoint(currentStepLocation, source.height()));
			currentViewportPaintPos.setX(0);
		} else if (currentStepLocation <= moveRange) {
			currentViewport.setTopLeft(
                QPoint(source.width() - (moveRange - currentStepLocation), 0));
			currentViewport.setBottomRight(
                QPoint(source.width(), source.height()));
			currentViewportPaintPos.setX(0);
		}
		currentViewportPaintPos.setY(0);
		QPixmap currentViewportPix;
		if (currentViewport.width() > 1)
			currentViewportPix = source.copy(currentViewport);
		QPainter painter;
		painter.begin(&pix);
		painter.drawPixmap(currentViewportPaintPos, currentViewportPix);
		painter.end();
		if (currentStepLocation == moveRange) {
			currentStepLocation = 0;
			pageCountCycleComplete = true;
			randomStyle = produceRandomStyle();
		}
		return pix;
	}
}

QPixmap Animator::moveRightContinuous() {
	if (pageCountCycleComplete) {
		pageCountCycleComplete = false;
		QPixmap pix;
		return pix;
	} else {
		QPixmap pix(region.width(), region.height());
		pix.fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
		int moveRange = region.width() + source.width();
		currentStepLocation = currentStepLocation + attr->step;
		if (currentStepLocation > moveRange) {
			currentStepLocation = moveRange;
		}
		QRect currentViewport;
		if (currentStepLocation <= region.width()) {
			currentViewport.setTopLeft(
                QPoint(source.width() - currentStepLocation, 0));
			currentViewport.setBottomRight(QPoint(source.rect().bottomRight()));
		} else if (currentStepLocation <= source.width()) {
			currentViewport.setTopLeft(
                QPoint(source.width() - currentStepLocation, 0));
			currentViewport.setBottomRight(
                QPoint(source.width() - currentStepLocation + region.width(),
                       source.height()));
		} else if (currentStepLocation <= moveRange) {
			currentViewport.setTopLeft(QPoint(0, 0));
			currentViewport.setBottomRight(
                QPoint(region.width() - (currentStepLocation - source.width()),
                       source.height()));
		}
		QPoint currentViewportPaintPos;
		if (currentStepLocation <= source.width()) {
			currentViewportPaintPos.setX(0);

		} else {
			currentViewportPaintPos.setX(currentStepLocation - source.width());
		}
		currentViewportPaintPos.setY(0);
		QPixmap currentViewportPix;
		if (currentViewport.width() > 1)
			currentViewportPix = source.copy(currentViewport);
		QPainter painter;
		painter.begin(&pix);
		painter.drawPixmap(currentViewportPaintPos, currentViewportPix);
		painter.end();
		if (currentStepLocation == moveRange) {
			currentStepLocation = 0;
			pageCountCycleComplete = true;
			randomStyle = produceRandomStyle();
		}
		return pix;
	}
}
