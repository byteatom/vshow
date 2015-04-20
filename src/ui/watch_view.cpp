#include "watch_view.h"
#include <QtGlobal>
#include <QtMath>
#include <QTime>
#include <QStaticText>
#include <QPainter>
#include "watch.h"
#include "data/image_attr.h"
#include "qlog/qlog.h"

#define PI 3.1415926


const QString WatchView::digital[12] = {
    "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII"};
const double WatchView::maxRegulateDistance = 2;
const double WatchView::lineScaleWHRatio = 10;
const double WatchView::hourHandRatio = 0.618;
const double WatchView::minuteHandRatio = 0.816;
const double WatchView::secondHandRatio = 0.985;
const qreal WatchView::windmillRadians = 48.0;

WatchView::WatchView(Watch *watch) : watch(watch), mouseSelect{this, watch} {
	setObjectName("WatchView");
	// setWindowFlags(Qt::ToolTip); //for video overlay
	setMouseTracking(true);
	unsetCursor();    // use parent cursor
    watch->owner->view()->addWidget(this);
    watch->owner->view()->setCurrentWidget(this);
	// setAttribute(Qt::WA_OpaquePaintEvent, true);
}

WatchView::~WatchView() {
}

void WatchView::paintEvent(QPaintEvent *) {
	QPainter painter(this);
    if (ImageAttr::BG_COLOR_NONE != watch->attr()->bgColor) {
        painter.fillRect(rect(), QColor::fromRgba(watch->attr()->bgColor));
	}
	if (!watch->bgCache.isNull()) {
		painter.drawPixmap(Slice::POS_TOPLEFT, watch->bgCache);
	}
	if (!watch->cache->isNull())
        painter.drawPixmap(Slice::POS_TOPLEFT, *watch->cache);
}

void WatchView::resizeEvent(QResizeEvent *event) {
	watch->cacheView();
	QWidget::resizeEvent(event);
}

QPixmap *WatchView::drawWatch() {
	if (!watch) {
		return nullptr;
	}
    QSize size(this->watch->owner->attr()->geo.width,
               this->watch->owner->attr()->geo.height);
	QPixmap *watch = new QPixmap(size);
	watch->fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
    QRectF region(Slice::POS_TOPLEFT, QSizeF(size));
	QPainter painter(watch);
	painter.setRenderHint(QPainter::Antialiasing);
	drawWatch(painter, region);
	return watch;
}

DialStyle WatchView::getWatchDialStyle() {
    return watch->attr()->dial.value.dialStyle;
}

bool WatchView::isEnabledTsnDotNumber() {
    return watch->attr()->dial.value.dialTime.dialNum.tsnNum.displayType;
}

DialNumStyle WatchView::getTsnDotNumberStyle() {
    return watch->attr()->dial.value.dialTime.dialNum.tsnNum.numStyle;
}

QRgb WatchView::getTsnDotNumberColor() {
    return watch->attr()->dial.value.dialTime.dialNum.tsnNum.numColor;
}

QString WatchView::getTsnDotNumberFamily() {
	return QString::fromStdWString(
        watch->attr()->dial.value.dialTime.dialNum.tsnNum.fontName);
}

int WatchView::getTsnDotNumberSize() {
    return watch->attr()->dial.value.dialTime.dialNum.tsnNum.fontSize;
}

bool WatchView::isEnabledHourDotNumber() {
    return watch->attr()->dial.value.dialTime.dialNum.hourNum.displayType;
}

DialNumStyle WatchView::getHourDotNumberStyle() {
    return watch->attr()->dial.value.dialTime.dialNum.hourNum.numStyle;
}

QRgb WatchView::getHourDotNumberColor() {
    return watch->attr()->dial.value.dialTime.dialNum.hourNum.numColor;
}

QString WatchView::getHourDotNumberFamily() {
	return QString::fromStdWString(
        watch->attr()->dial.value.dialTime.dialNum.hourNum.fontName);
}

int WatchView::getHourDotNumberSize() {
    return watch->attr()->dial.value.dialTime.dialNum.hourNum.fontSize;
}

DialPointStyle WatchView::getTsnDotStyle() {
    return watch->attr()->dial.value.dialTime.dialPoint.tsnPoint.pointStyle;
}

QRgb WatchView::getTsnDotColor() {
    return watch->attr()->dial.value.dialTime.dialPoint.tsnPoint.pointColor;
}

int WatchView::getTsnDotSize() {
    return watch->attr()->dial.value.dialTime.dialPoint.tsnPoint.pointSize;
}

DialPointStyle WatchView::getHourDotStyle() {
    return watch->attr()->dial.value.dialTime.dialPoint.hourPoint.pointStyle;
}

QRgb WatchView::getHourDotColor() {
    return watch->attr()->dial.value.dialTime.dialPoint.hourPoint.pointColor;
}

int WatchView::getHourDotSize() {
    return watch->attr()->dial.value.dialTime.dialPoint.hourPoint.pointSize;
}

DialPointStyle WatchView::getMinuteDotStyle() {
    return watch->attr()->dial.value.dialTime.dialPoint.minutePoint.pointStyle;
}

QRgb WatchView::getMinuteDotColor() {
    return watch->attr()->dial.value.dialTime.dialPoint.minutePoint.pointColor;
}

int WatchView::getMinuteDotSize() {
    return watch->attr()->dial.value.dialTime.dialPoint.minutePoint.pointSize;
}

DialHandStyle WatchView::getHourHandStyle() {
    return watch->attr()->dial.value.dialTime.dialHand.hourHand.handStyle;
}

QRgb WatchView::getHourHandColor() {
    return watch->attr()->dial.value.dialTime.dialHand.hourHand.handColor;
}

int WatchView::getHourHandSize() {
    return watch->attr()->dial.value.dialTime.dialHand.hourHand.handSize;
}

DialHandStyle WatchView::getMinuteHandStyle() {
    return watch->attr()->dial.value.dialTime.dialHand.minuteHand.handStyle;
}

QRgb WatchView::getMinuteHandColor() {
    return watch->attr()->dial.value.dialTime.dialHand.minuteHand.handColor;
}

int WatchView::getMinuteHandSize() {
    return watch->attr()->dial.value.dialTime.dialHand.minuteHand.handSize;
}

DialHandStyle WatchView::getSecondHandStyle() {
    return watch->attr()->dial.value.dialTime.dialHand.secondHand.handStyle;
}

QRgb WatchView::getSecondHandColor() {
    return watch->attr()->dial.value.dialTime.dialHand.secondHand.handColor;
}

int WatchView::getSecondHandSize() {
    return watch->attr()->dial.value.dialTime.dialHand.secondHand.handSize;
}

DialDeltType WatchView::getDeltType() {
    return watch->attr()->dial.value.dialTime.dialTimeDiff.deltType;
}

int WatchView::getDeltHour() {
    return watch->attr()->dial.value.dialTime.dialTimeDiff.deltHour;
}

int WatchView::getDeltMinute() {
    return watch->attr()->dial.value.dialTime.dialTimeDiff.deltMinute;
}

int WatchView::getDeltSecond() {
    return watch->attr()->dial.value.dialTime.dialTimeDiff.deltSecond;
}

void WatchView::drawWatch(QPainter &painter, const QRectF &region) {
	DialStyle dialStyle = getWatchDialStyle();
	switch (dialStyle) {
	case DS_CIRCLE:
		drawCircleWatch(painter, region);
		break;
	case DS_ROUNDRECT:
		drawRoundRectWatch(painter, region);
		break;
	case DS_ELLIPSE:
		drawEllipseWatch(painter, region);
		break;
	case DS_RHOMBUS:
		drawRhombusWatch(painter, region);
		break;
	case DS_WINDMILL:
		drawWindmillWatch(painter, region);
		break;
	default:
		drawCircleWatch(painter, region);
		break;
	}
}

void WatchView::drawCircleWatch(QPainter &painter, const QRectF &region) {
	drawCircleWatchDial(painter, region, maxRegulateDistance);
	drawCircleWatchDigitals(painter, region, maxRegulateDistance);
	drawText(painter, region);
	drawWatchHand(painter, region, maxRegulateDistance);
}

void WatchView::drawRoundRectWatch(QPainter &painter, const QRectF &region) {
	drawRoundRectWatchDial(painter, region, maxRegulateDistance);
	drawRoundRectWatchDigitals(painter, region, maxRegulateDistance);
	drawText(painter, region);
	drawWatchHand(painter, region, maxRegulateDistance);
}

void WatchView::drawEllipseWatch(QPainter &painter, const QRectF &region) {
	drawEllipseWatchDial(painter, region, maxRegulateDistance);
	drawEllipseWatchDigitals(painter, region, maxRegulateDistance);
	drawText(painter, region);
	drawWatchHand(painter, region, maxRegulateDistance);
}

void WatchView::drawRhombusWatch(QPainter &painter, const QRectF &region) {
	drawRhombusWatchDial(painter, region, maxRegulateDistance);
	drawRhombusWatchDigitals(painter, region, maxRegulateDistance);
	drawText(painter, region);
	drawWatchHand(painter, region, maxRegulateDistance);
}

void WatchView::drawWindmillWatch(QPainter &painter, const QRectF &region) {
	drawWindmillWatchDial(painter, region, maxRegulateDistance);
	drawWindmillWatchDigitals(painter, region, maxRegulateDistance);
	drawText(painter, region);
	drawWatchHand(painter, region, maxRegulateDistance);
}

void WatchView::drawCircleWatchDial(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset) {
	painter.save();
    double maxWatchRedius
        = static_cast<double>(qMin(region.width() / 2, region.height() / 2))
          - maxRegulateOffset;
	painter.translate(region.width() / 2, region.height() / 2);
	for (int i = 0; i < 60; ++i) {
		if (0 == i % 5) {
			if (0 == i % 15) {
				drawTsnScale(painter, maxWatchRedius);
			} else {
				drawHourScale(painter, maxWatchRedius);
			}
		} else {
			drawMinuteScale(painter, maxWatchRedius);
		}
		painter.rotate(6.0);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRoundRectWatchDial(QPainter &painter,
                                       const QRectF &region,
                                       double maxRegulateOffset) {
	painter.save();
    double maxWatchRedius
        = static_cast<double>(qMin(region.width() / 2, region.height() / 2))
          - maxRegulateOffset;
	painter.translate(region.width() / 2, region.height() / 2);
	double regulateRedius = 0;
	for (int quadrant = 1; quadrant <= 4; quadrant++) {
		for (int i = 0; i <= 14; i++) {
			if (i <= 5) {
				regulateRedius = maxWatchRedius / qCos(i * PI / 30);
			} else if (i <= 10) {
				regulateRedius = maxWatchRedius / qCos(5 * PI / 30);
			} else if (i <= 14) {
				regulateRedius = maxWatchRedius / qCos((5 - i % 10) * PI / 30);
			}
			if (i % 15 == 0) {
				drawTsnScale(painter, regulateRedius);
			} else {
				if (i % 5 == 0) {
					drawHourScale(painter, regulateRedius);
				} else {
					drawMinuteScale(painter, regulateRedius);
				}
			}
			painter.rotate(6.0);
		}
		painter.resetTransform();
		painter.translate(region.width() / 2, region.height() / 2);
		painter.rotate(quadrant * 90.0);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawEllipseWatchDial(QPainter &painter,
                                     const QRectF &region,
                                     double maxRegulateOffset) {
	painter.save();
    double a = (region.width() - 2 * maxRegulateOffset) / 2;
    double b = (region.height() - 2 * maxRegulateOffset) / 2;
	painter.translate(region.width() / 2, region.height() / 2);
	qreal radians = 0;
	double regulateRedius = 0;
	for (int quadrant = 1; quadrant <= 2; quadrant++) {
		for (int i = 0; i <= 29; i++) {
			radians = i * PI / 30;
			regulateRedius = a * b
                             / qSqrt(a * a * qSin(radians) * qSin(radians)
                                     + b * b * qCos(radians) * qCos(radians));
			if (i % 15 == 0) {
				drawTsnScale(painter, regulateRedius);
			} else {
				if (i % 5 == 0) {
					drawHourScale(painter, regulateRedius);
				} else {
					drawMinuteScale(painter, regulateRedius);
				}
			}
			painter.rotate(6.0);
		}
		painter.resetTransform();
		painter.translate(region.width() / 2, region.height() / 2);
		painter.rotate(quadrant * 180.0);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRhombusWatchDial(QPainter &painter,
                                     const QRectF &region,
                                     double maxRegulateOffset) {
	painter.save();
    double a = (region.width() - 2 * maxRegulateOffset) / 2;
    double b = (region.height() - 2 * maxRegulateOffset) / 2;
	painter.translate(region.width() / 2, region.height() / 2);
	qreal radians = 0;
	double regulateRedius = 0;
	for (int quadrant = 1; quadrant <= 2; quadrant++) {
		for (int i = 0; i <= 29; i++) {
			radians = i * PI / 30;
			if (i <= 14) {
				regulateRedius = a * b
                                 / (a * qSin(radians) + b * qCos(radians));
			} else {
				regulateRedius = a * b
                                 / (a * qSin(radians) - b * qCos(radians));
			}
			if (i % 15 == 0) {
				drawTsnScale(painter, regulateRedius);
			} else {
				if (i % 5 == 0) {
					drawHourScale(painter, regulateRedius);
				} else {
					drawMinuteScale(painter, regulateRedius);
				}
			}
			painter.rotate(6.0);
		}
		painter.resetTransform();
		painter.translate(region.width() / 2, region.height() / 2);
		painter.rotate(quadrant * 180.0);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawWindmillWatchDial(QPainter &painter,
                                      const QRectF &region,
                                      double maxRegulateOffset) {
	painter.save();
    double maxWatchRedius
        = static_cast<double>(qMin(region.width() / 2, region.height() / 2))
          - maxRegulateOffset;
	painter.translate(region.width() / 2, region.height() / 2);
	double regulateRedius = 0;
	for (int quadrant = 1; quadrant <= 4; quadrant++) {
		for (int i = 0; i <= 14; i++) {
			if (i < 5) {
                regulateRedius = maxWatchRedius * qTan(windmillRadians)
                                 / (qTan(windmillRadians) * qCos(i * PI / 30)
                                    + qSin(i * PI / 30));
            } else if (i < 10) {
                regulateRedius
                    = maxWatchRedius * qTan(windmillRadians)
                      / (qTan(windmillRadians) * qCos((i - 5) * PI / 30)
                         + qSin((i - 5) * PI / 30));
			} else if (i <= 14) {
                regulateRedius
                    = maxWatchRedius * qTan(windmillRadians)
                      / (qTan(windmillRadians) * qCos((i - 10) * PI / 30)
                         + qSin((i - 10) * PI / 30));
			}
			if (i % 15 == 0) {
				drawTsnScale(painter, regulateRedius);
			} else {
				if (i % 5 == 0) {
					drawHourScale(painter, regulateRedius);
				} else {
					drawMinuteScale(painter, regulateRedius);
				}
			}
			painter.rotate(6.0);
		}
		painter.resetTransform();
		painter.translate(region.width() / 2, region.height() / 2);
		painter.rotate(quadrant * 90.0);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawTsnScale(QPainter &painter, double regulateRedius) {
	painter.save();
	int tsnScaleRedius = getTsnDotSize();
	if (tsnScaleRedius > 0) {
		drawScale(painter,
                  getTsnDotStyle(),
                  QColor::fromRgba(getTsnDotColor()),
                  regulateRedius,
                  tsnScaleRedius);
	}
	painter.restore();
}

void WatchView::drawHourScale(QPainter &painter, double regulateRedius) {
	painter.save();
	int hourScaleRedius = getHourDotSize();
	if (hourScaleRedius > 0) {
		drawScale(painter,
                  getHourDotStyle(),
                  QColor::fromRgba(getHourDotColor()),
                  regulateRedius,
                  hourScaleRedius);
	}
	painter.restore();
}

void WatchView::drawMinuteScale(QPainter &painter, double regulateRedius) {
	painter.save();
	int minuteScaleRedius = getMinuteDotSize();
	if (minuteScaleRedius > 0) {
		drawScale(painter,
                  getMinuteDotStyle(),
                  QColor::fromRgba(getMinuteDotColor()),
                  regulateRedius,
                  minuteScaleRedius);
	}
	painter.restore();
}

void WatchView::drawScale(QPainter &painter,
                          DialPointStyle scaleStyle,
                          const QColor &scaleColor,
                          double radius,
                          double scaleRadius) {
	painter.save();
	painter.setPen(Qt::NoPen);
	if (DPS_CIRCLE == scaleStyle) {
		drawCircleScale(painter, scaleColor, radius, scaleRadius);
	} else if (DPS_SQUARE == scaleStyle) {
		drawSquareScale(painter, scaleColor, radius, scaleRadius);
	} else if (DPS_LINE == scaleStyle) {
		drawLineScale(painter, scaleColor, radius, scaleRadius);
	}
	painter.restore();
}

void WatchView::drawCircleScale(QPainter &painter,
                                const QColor &scaleColor,
                                double radius,
                                double scaleRadius) {
	QRectF scaleBoundRect(QPointF(radius - 2 * scaleRadius, -scaleRadius),
                          QPointF(radius, scaleRadius));
	QPainterPath scaleCirclePath;
	scaleCirclePath.addEllipse(scaleBoundRect);
	painter.fillPath(scaleCirclePath, QBrush(scaleColor));
}

void WatchView::drawSquareScale(QPainter &painter,
                                const QColor &scaleColor,
                                double radius,
                                double scaleRadius) {
	QRectF scaleBoundRect(QPointF(radius - 2 * scaleRadius, -scaleRadius),
                          QPointF(radius, scaleRadius));
	QPainterPath scaleSquarePath;
	scaleSquarePath.addRect(scaleBoundRect);
	painter.fillPath(scaleSquarePath, QBrush(scaleColor));
}

void WatchView::drawLineScale(QPainter &painter,
                              const QColor &scaleColor,
                              double radius,
                              double scaleRadius) {
	QRectF scaleLineRect(
        QPointF(radius - 2 * scaleRadius, -scaleRadius / lineScaleWHRatio),
        QPointF(radius, scaleRadius / lineScaleWHRatio));
	QPainterPath scaleLinePath;
	scaleLinePath.addRect(scaleLineRect);
	painter.fillPath(scaleLinePath, QBrush(scaleColor));
}

void WatchView::drawCircleWatchDigitals(QPainter &painter,
                                        const QRectF &region,
                                        double maxRegulateOffset) {
	double maxScaleRedius = getMaxScaleRedius();
    double realInnerRadius
        = (static_cast<double>(qMin(region.width() / 2, region.height() / 2))
           - maxRegulateOffset) - 3 * maxScaleRedius;
	QRgb digitalRgb;
	QColor digitalColor;
	QPen digitalPen;
	QString digitalString;
	QString digitalFontFamily;
	int digitalFontSize;
	QFont digitalFont;
	bool useScaleColor = false;
	painter.save();
	painter.resetTransform();
	painter.translate(region.width() / 2, region.height() / 2);
	for (int i = 1; i <= 12; i++) {
		if (0 == i % 3) {
			DialNumStyle tsnNumStyle = getTsnDotNumberStyle();
			if (DNS_ARABIC == tsnNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == tsnNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getTsnDotColor();
			} else {
				digitalRgb = getTsnDotNumberColor();
			}
			QString tempFontFamily = getTsnDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getTsnDotNumberSize();
		} else {
			DialNumStyle hourNumStyle = getHourDotNumberStyle();
			if (DNS_ARABIC == hourNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == hourNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getHourDotColor();
			} else {
				digitalRgb = getHourDotNumberColor();
			}
			QString tempFontFamily = getHourDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getHourDotNumberSize();
		}
		digitalColor.setRgba(digitalRgb);
		digitalFont.setFamily(digitalFontFamily);
		digitalFont.setPixelSize(digitalFontSize);
		QFontMetricsF digitalMetrics(digitalFont);
		QRectF digitalBoundRect = digitalMetrics.boundingRect(digitalString);
		double digitalStringWidth = digitalBoundRect.width();
		double digitalStringHeight = digitalBoundRect.height();
		QPointF paintTextPoint;
		paintTextPoint.setX(qCos(PI / 3 - (i - 1) * PI / 6) * realInnerRadius);
		paintTextPoint.setY(-qSin(PI / 3 - (i - 1) * PI / 6) * realInnerRadius);
		paintTextPoint = getDigitalRegulatedPaintPoint(
            paintTextPoint, i, digitalStringWidth, digitalStringHeight);
		QStaticText digitalStaticText(digitalString);
		QTextOption digitalTextOption;
		digitalTextOption.setAlignment(Qt::AlignCenter);
		digitalStaticText.setTextOption(digitalTextOption);
		painter.setFont(digitalFont);
		digitalPen.setColor(digitalColor);
		painter.setPen(digitalPen);
		if (0 == i % 3 && DDT_YES == isEnabledTsnDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		} else if (0 != i % 3 && DDT_YES == isEnabledHourDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		}
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRoundRectWatchDigitals(QPainter &painter,
                                           const QRectF &region,
                                           double maxRegulateOffset) {
	double maxScaleRedius = getMaxScaleRedius();
    double realInnerRadius
        = (static_cast<double>(qMin(region.width() / 2, region.height() / 2))
           - maxRegulateOffset) - 2 * maxScaleRedius;
	double regulateRadius = 0;
	QRgb digitalRgb;
	QColor digitalColor;
	QPen digitalPen;
	QString digitalString;
	QString digitalFontFamily;
	int digitalFontSize;
	QFont digitalFont;
	bool useScaleColor = false;
	painter.save();
	painter.resetTransform();
	painter.translate(region.width() / 2, region.height() / 2);
	for (int i = 1; i <= 12; i++) {
		if (0 == i % 3) {
			DialNumStyle tsnNumStyle = getTsnDotNumberStyle();
			if (DNS_ARABIC == tsnNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == tsnNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getTsnDotColor();
			} else {
				digitalRgb = getTsnDotNumberColor();
			}
			QString tempFontFamily = getTsnDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getTsnDotNumberSize();
			regulateRadius = realInnerRadius;
		} else {
			DialNumStyle hourNumStyle = getHourDotNumberStyle();
			if (DNS_ARABIC == hourNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == hourNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getHourDotColor();
			} else {
				digitalRgb = getHourDotNumberColor();
			}
			QString tempFontFamily = getHourDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getHourDotNumberSize();
			regulateRadius = realInnerRadius / qCos(PI / 6);
		}
		digitalColor.setRgba(digitalRgb);
		digitalFont.setFamily(digitalFontFamily);
		digitalFont.setPixelSize(digitalFontSize);
		QFontMetricsF digitalMetrics(digitalFont);
		QRectF digitalBoundRect = digitalMetrics.boundingRect(digitalString);
		double digitalStringWidth = digitalBoundRect.width();
		double digitalStringHeight = digitalBoundRect.height();
		QPointF paintTextPoint;
		paintTextPoint.setX(qCos(PI / 3 - (i - 1) * PI / 6) * regulateRadius);
		paintTextPoint.setY(-qSin(PI / 3 - (i - 1) * PI / 6) * regulateRadius);
		paintTextPoint = getDigitalRegulatedPaintPoint(
            paintTextPoint, i, digitalStringWidth, digitalStringHeight);
		QStaticText digitalStaticText(digitalString);
		QTextOption digitalTextOption;
		digitalTextOption.setAlignment(Qt::AlignCenter);
		digitalStaticText.setTextOption(digitalTextOption);
		painter.setFont(digitalFont);
		digitalPen.setColor(digitalColor);
		painter.setPen(digitalPen);
		if (0 == i % 3 && DDT_YES == isEnabledTsnDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		} else if (0 != i % 3 && DDT_YES == isEnabledHourDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		}
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawEllipseWatchDigitals(QPainter &painter,
                                         const QRectF &region,
                                         double maxRegulateOffset) {
	double maxScaleRedius = getMaxScaleRedius();
    double a = (region.height() - 2 * maxRegulateOffset - 3 * maxScaleRedius)
               / 2;
    double b = (region.width() - 2 * maxRegulateOffset - 3 * maxScaleRedius)
               / 2;
	double regulateRadius = 0;
	QRgb digitalRgb;
	QColor digitalColor;
	QPen digitalPen;
	QString digitalString;
	QString digitalFontFamily;
	int digitalFontSize;
	QFont digitalFont;
	bool useScaleColor = false;
	painter.save();
	painter.resetTransform();
	painter.translate(region.width() / 2, region.height() / 2);
	for (int i = 1; i <= 12; i++) {
		if (0 == i % 3) {
			DialNumStyle tsnNumStyle = getTsnDotNumberStyle();
			if (DNS_ARABIC == tsnNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == tsnNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getTsnDotColor();
			} else {
				digitalRgb = getTsnDotNumberColor();
			}
			QString tempFontFamily = getTsnDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getTsnDotNumberSize();
		} else {
			DialNumStyle hourNumStyle = getHourDotNumberStyle();
			if (DNS_ARABIC == hourNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == hourNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getHourDotColor();
			} else {
				digitalRgb = getHourDotNumberColor();
			}
			QString tempFontFamily = getHourDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getHourDotNumberSize();
		}
		digitalColor.setRgba(digitalRgb);
		digitalFont.setFamily(digitalFontFamily);
		digitalFont.setPixelSize(digitalFontSize);
		QFontMetricsF digitalMetrics(digitalFont);
		QRectF digitalBoundRect = digitalMetrics.boundingRect(digitalString);
		double digitalStringWidth = digitalBoundRect.width();
		double digitalStringHeight = digitalBoundRect.height();
		qreal radians = i * PI / 6;
		regulateRadius = a * b / qSqrt(a * a * qSin(radians) * qSin(radians)
                                       + b * b * qCos(radians) * qCos(radians));
		QPointF paintTextPoint;
		paintTextPoint.setX(qCos(PI / 3 - (i - 1) * PI / 6) * regulateRadius);
		paintTextPoint.setY(-qSin(PI / 3 - (i - 1) * PI / 6) * regulateRadius);
		paintTextPoint = getDigitalRegulatedPaintPoint(paintTextPoint,
                                                       i,
                                                       digitalStringWidth,
                                                       digitalStringHeight,
                                                       0.5,
                                                       0.2,
                                                       0.5,
                                                       0.2);
		QStaticText digitalStaticText(digitalString);
		QTextOption digitalTextOption;
		digitalTextOption.setAlignment(Qt::AlignCenter);
		digitalStaticText.setTextOption(digitalTextOption);
		painter.setFont(digitalFont);
		digitalPen.setColor(digitalColor);
		painter.setPen(digitalPen);
		if (0 == i % 3 && DDT_YES == isEnabledTsnDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		} else if (0 != i % 3 && DDT_YES == isEnabledHourDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		}
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRhombusWatchDigitals(QPainter &painter,
                                         const QRectF &region,
                                         double maxRegulateOffset) {
	double maxScaleRedius = getMaxScaleRedius();
    double a = (region.height() - 2 * maxRegulateOffset - 3 * maxScaleRedius)
               / 2;
    double b = (region.width() - 2 * maxRegulateOffset - 3 * maxScaleRedius)
               / 2;
	double regulateRadius = 0;
	QRgb digitalRgb;
	QColor digitalColor;
	QPen digitalPen;
	QString digitalString;
	QString digitalFontFamily;
	int digitalFontSize;
	QFont digitalFont;
	bool useScaleColor = false;
	painter.save();
	painter.resetTransform();
	painter.translate(region.width() / 2, region.height() / 2);
	for (int i = 1; i <= 12; i++) {
		if (0 == i % 3) {
			DialNumStyle tsnNumStyle = getTsnDotNumberStyle();
			if (DNS_ARABIC == tsnNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == tsnNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getTsnDotColor();
			} else {
				digitalRgb = getTsnDotNumberColor();
			}
			QString tempFontFamily = getTsnDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getTsnDotNumberSize();
		} else {
			DialNumStyle hourNumStyle = getHourDotNumberStyle();
			if (DNS_ARABIC == hourNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == hourNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getHourDotColor();
			} else {
				digitalRgb = getHourDotNumberColor();
			}
			QString tempFontFamily = getHourDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getHourDotNumberSize();
		}
		digitalColor.setRgba(digitalRgb);
		digitalFont.setFamily(digitalFontFamily);
		digitalFont.setPixelSize(digitalFontSize);
		QFontMetricsF digitalMetrics(digitalFont);
		QRectF digitalBoundRect = digitalMetrics.boundingRect(digitalString);
		double digitalStringWidth = digitalBoundRect.width();
		double digitalStringHeight = digitalBoundRect.height();
		qreal radians = i * PI / 6;
		if (i >= 1 && i <= 3) {
			regulateRadius = a * b / (a * qSin(radians) + b * qCos(radians));
		} else if (i <= 6) {
			regulateRadius = a * b / (a * qSin(radians) - b * qCos(radians));
		} else if (i <= 9) {
			regulateRadius = a * b / (a * qCos(3 * PI / 2 - radians)
                                      + b * qSin(3 * PI / 2 - radians));
		} else if (i <= 12) {
			regulateRadius = a * b / (b * qCos(radians) - a * qSin(radians));
		}
		QPointF paintTextPoint;
		paintTextPoint.setX(qCos(PI / 3 - (i - 1) * PI / 6) * regulateRadius);
		paintTextPoint.setY(-qSin(PI / 3 - (i - 1) * PI / 6) * regulateRadius);
		paintTextPoint = getDigitalRegulatedPaintPoint(paintTextPoint,
                                                       i,
                                                       digitalStringWidth,
                                                       digitalStringHeight,
                                                       0.5,
                                                       0.1,
                                                       0.5,
                                                       0.1);
		QStaticText digitalStaticText(digitalString);
		QTextOption digitalTextOption;
		digitalTextOption.setAlignment(Qt::AlignCenter);
		digitalStaticText.setTextOption(digitalTextOption);
		painter.setFont(digitalFont);
		digitalPen.setColor(digitalColor);
		painter.setPen(digitalPen);
		if (0 == i % 3 && DDT_YES == isEnabledTsnDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		} else if (0 != i % 3 && DDT_YES == isEnabledHourDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		}
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawWindmillWatchDigitals(QPainter &painter,
                                          const QRectF &region,
                                          double maxRegulateOffset) {
	double maxScaleRedius = getMaxScaleRedius();
    double realInnerRadius
        = (static_cast<double>(qMin(region.width() / 2, region.height() / 2))
           - maxRegulateOffset) - 3 * maxScaleRedius;
	QRgb digitalRgb;
	QColor digitalColor;
	QPen digitalPen;
	QString digitalString;
	QString digitalFontFamily;
	int digitalFontSize;
	QFont digitalFont;
	bool useScaleColor = false;
	painter.save();
	painter.resetTransform();
	painter.translate(region.width() / 2, region.height() / 2);
	for (int i = 1; i <= 12; i++) {
		if (0 == i % 3) {
			DialNumStyle tsnNumStyle = getTsnDotNumberStyle();
			if (DNS_ARABIC == tsnNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == tsnNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getTsnDotColor();
			} else {
				digitalRgb = getTsnDotNumberColor();
			}
			QString tempFontFamily = getTsnDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getTsnDotNumberSize();
		} else {
			DialNumStyle hourNumStyle = getHourDotNumberStyle();
			if (DNS_ARABIC == hourNumStyle) {
				digitalString.setNum(i);
			} else if (DNS_ROAMER == hourNumStyle) {
				digitalString = digital[i - 1];
			}
			if (useScaleColor) {
				digitalRgb = getHourDotColor();
			} else {
				digitalRgb = getHourDotNumberColor();
			}
			QString tempFontFamily = getHourDotNumberFamily();
			if (tempFontFamily.isEmpty()) {
				digitalFontFamily = digitalFont.defaultFamily();
			} else {
				digitalFontFamily = tempFontFamily;
			}
			digitalFontSize = getHourDotNumberSize();
		}
		digitalColor.setRgba(digitalRgb);
		digitalFont.setFamily(digitalFontFamily);
		digitalFont.setPixelSize(digitalFontSize);
		QFontMetricsF digitalMetrics(digitalFont);
		QRectF digitalBoundRect = digitalMetrics.boundingRect(digitalString);
		double digitalStringWidth = digitalBoundRect.width();
		double digitalStringHeight = digitalBoundRect.height();
		QPointF paintTextPoint;
		paintTextPoint.setX(qCos(PI / 3 - (i - 1) * PI / 6) * realInnerRadius);
		paintTextPoint.setY(-qSin(PI / 3 - (i - 1) * PI / 6) * realInnerRadius);
		paintTextPoint = getDigitalRegulatedPaintPoint(
            paintTextPoint, i, digitalStringWidth, digitalStringHeight);
		QStaticText digitalStaticText(digitalString);
		QTextOption digitalTextOption;
		digitalTextOption.setAlignment(Qt::AlignCenter);
		digitalStaticText.setTextOption(digitalTextOption);
		painter.setFont(digitalFont);
		digitalPen.setColor(digitalColor);
		painter.setPen(digitalPen);
		if (0 == i % 3 && DDT_YES == isEnabledTsnDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		} else if (0 != i % 3 && DDT_YES == isEnabledHourDotNumber()) {
			painter.drawStaticText(paintTextPoint, digitalStaticText);
		}
	}
	painter.restore();
	painter.resetTransform();
}

QPointF WatchView::getDigitalRegulatedPaintPoint(QPointF original,
                                                 int i,
                                                 double digitalStringWidth,
                                                 double digitalStringHeight,
                                                 double Pos3Regulate,
                                                 double Pos6Regulate,
                                                 double Pos9Regulate,
                                                 double Pos12Regulate) {
	QPointF paintPos = original;
	if (i >= 0 && i < 3) {
		paintPos.setX(original.x() - digitalStringWidth);
	}
	if (i == 3) {
		paintPos.setX(original.x() - (1 + Pos3Regulate) * digitalStringWidth);
		paintPos.setY(original.y() - digitalStringHeight / 2);
	}
	if (i > 3 && i < 6) {
		paintPos.setX(original.x() - digitalStringWidth);
		paintPos.setY(original.y() - digitalStringHeight);
	}
	if (i == 6) {
		paintPos.setX(original.x() - digitalStringWidth / 2);
		paintPos.setY(original.y() - (1 + Pos6Regulate) * digitalStringHeight);
	}
	if (i > 6 && i < 9) {
		paintPos.setY(original.y() - digitalStringHeight);
	}
	if (i == 9) {
		paintPos.setX(original.x() + Pos9Regulate * digitalStringWidth);
		paintPos.setY(original.y() - digitalStringHeight / 2);
	}
	if (i > 9 && i < 12) {
		// do nothing
	}
	if (i == 12) {
		paintPos.setX(original.x() - digitalStringWidth / 2);
		paintPos.setY(original.y() + Pos12Regulate * digitalStringHeight);
	}
	return paintPos;
}

void WatchView::drawWatchHand(QPainter &painter,
                              const QRectF &region,
                              double maxRegulateOffset) {
	drawWatchHourHand(painter, region, maxRegulateOffset);
	drawWatchMinuteHand(painter, region, maxRegulateOffset);
	drawWatchSecondHand(painter, region, maxRegulateOffset);
}

void WatchView::drawWatchHourHand(QPainter &painter,
                                  const QRectF &region,
                                  double maxRegulateOffset) {
	DialStyle dialStyle = getWatchDialStyle();
	switch (dialStyle) {
	case DS_CIRCLE:
		drawCircleWatchHourHand(painter, region, maxRegulateOffset);
		break;
	case DS_ROUNDRECT:
		drawRoundRectWatchHourHand(painter, region, maxRegulateOffset);
		break;
	case DS_ELLIPSE:
		drawEllipseWatchHourHand(painter, region, maxRegulateOffset);
		break;
	case DS_RHOMBUS:
		drawRhombusWatchHourHand(painter, region, maxRegulateOffset);
		break;
	case DS_WINDMILL:
		drawWindmillWatchHourHand(painter, region, maxRegulateOffset);
		break;
	default:
		drawCircleWatchHourHand(painter, region, maxRegulateOffset);
		break;
	}
}

void WatchView::drawCircleWatchHourHand(QPainter &painter,
                                        const QRectF &region,
                                        double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	double maxScaleRedius = getMaxScaleRedius();
	double maxWatchRedius
        = static_cast<double>(qMin(region.width() / 2, region.height() / 2))
          - maxRegulateOffset;
	double hourHandLength = (maxWatchRedius - 2 * maxScaleRedius)
                            * hourHandRatio;
	double hourHandWidth = getHourHandSize();
	QRgb rgbHourHandColor = getHourHandColor();
	QTime regulatedTime = getRegulatedTime();
	painter.rotate(30.0
                   * ((regulatedTime.hour() + regulatedTime.minute() / 60.0)));
	DialHandStyle hourHandStyle = getHourHandStyle();
	if (DHS_LINE == hourHandStyle) {
		drawLineHand(painter, rgbHourHandColor, hourHandLength, hourHandWidth);
	} else if (DHS_RHOMBUS == hourHandStyle) {
		drawRhombusHand(
            painter, rgbHourHandColor, hourHandLength, hourHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRoundRectWatchHourHand(QPainter &painter,
                                           const QRectF &region,
                                           double maxRegulateOffset) {
	drawCircleWatchHourHand(painter, region, maxRegulateOffset);
}

void WatchView::drawEllipseWatchHourHand(QPainter &painter,
                                         const QRectF &region,
                                         double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 30.0
                     * ((regulatedTime.hour() + regulatedTime.minute() / 60.0));
    double regulateRedius
        = regulateEllipseRedius(region, maxRegulateOffset, degrees);
	double hourHandLength = (regulateRedius - 2 * maxScaleRedius)
                            * hourHandRatio;
	double hourHandWidth = getHourHandSize();
	QRgb rgbHourHandColor = getHourHandColor();
	painter.rotate(degrees);
	DialHandStyle hourHandStyle = getHourHandStyle();
	if (DHS_LINE == hourHandStyle) {
		drawLineHand(painter, rgbHourHandColor, hourHandLength, hourHandWidth);
	} else if (DHS_RHOMBUS == hourHandStyle) {
		drawRhombusHand(
            painter, rgbHourHandColor, hourHandLength, hourHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRhombusWatchHourHand(QPainter &painter,
                                         const QRectF &region,
                                         double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 30.0
                     * ((regulatedTime.hour() + regulatedTime.minute() / 60.0));
    double regulateRedius
        = regulateRhombusRedius(region, maxRegulateOffset, degrees);
	double hourHandLength = (regulateRedius - 2 * maxScaleRedius)
                            * hourHandRatio;
	double hourHandWidth = getHourHandSize();
	QRgb rgbHourHandColor = getHourHandColor();
	painter.rotate(degrees);
	DialHandStyle hourHandStyle = getHourHandStyle();
	if (DHS_LINE == hourHandStyle) {
		drawLineHand(painter, rgbHourHandColor, hourHandLength, hourHandWidth);
	} else if (DHS_RHOMBUS == hourHandStyle) {
		drawRhombusHand(
            painter, rgbHourHandColor, hourHandLength, hourHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawWindmillWatchHourHand(QPainter &painter,
                                          const QRectF &region,
                                          double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 30.0
                     * ((regulatedTime.hour() + regulatedTime.minute() / 60.0));
    double regulateRedius
        = regulateWindmillRedius(region, maxRegulateOffset, degrees);
	double hourHandLength = (regulateRedius - 2 * maxScaleRedius)
                            * hourHandRatio;
	double hourHandWidth = getHourHandSize();
	QRgb rgbHourHandColor = getHourHandColor();
	painter.rotate(degrees);
	DialHandStyle hourHandStyle = getHourHandStyle();
	if (DHS_LINE == hourHandStyle) {
		drawLineHand(painter, rgbHourHandColor, hourHandLength, hourHandWidth);
	} else if (DHS_RHOMBUS == hourHandStyle) {
		drawRhombusHand(
            painter, rgbHourHandColor, hourHandLength, hourHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawWatchMinuteHand(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset) {
	DialStyle dialStyle = getWatchDialStyle();
	switch (dialStyle) {
	case DS_CIRCLE:
		drawCircleWatchMinuteHand(painter, region, maxRegulateOffset);
		break;
	case DS_ROUNDRECT:
		drawRoundRectWatchMinuteHand(painter, region, maxRegulateOffset);
		break;
	case DS_ELLIPSE:
		drawEllipseWatchMinuteHand(painter, region, maxRegulateOffset);
		break;
	case DS_RHOMBUS:
		drawRhombusWatchMinuteHand(painter, region, maxRegulateOffset);
		break;
	case DS_WINDMILL:
		drawWindmillWatchMinuteHand(painter, region, maxRegulateOffset);
		break;
	default:
		drawCircleWatchMinuteHand(painter, region, maxRegulateOffset);
		break;
	}
}

void WatchView::drawCircleWatchMinuteHand(QPainter &painter,
                                          const QRectF &region,
                                          double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	double maxScaleRedius = getMaxScaleRedius();
	double maxWatchRedius
        = static_cast<double>(qMin(region.width() / 2, region.height() / 2))
          - maxRegulateOffset;
	double minuteHandLength = (maxWatchRedius - 2 * maxScaleRedius)
                              * minuteHandRatio;
	double minuteHandWidth = getMinuteHandSize();
	QRgb rgbMinuteHandColor = getMinuteHandColor();
	QTime regulatedTime = getRegulatedTime();
	painter.rotate(6.0
                   * (regulatedTime.minute() + regulatedTime.second() / 60.0));
	DialHandStyle minuteHandStyle = getMinuteHandStyle();
	if (DHS_LINE == minuteHandStyle) {
		drawLineHand(
            painter, rgbMinuteHandColor, minuteHandLength, minuteHandWidth);
	} else if (DHS_RHOMBUS == minuteHandStyle) {
		drawRhombusHand(
            painter, rgbMinuteHandColor, minuteHandLength, minuteHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRoundRectWatchMinuteHand(QPainter &painter,
                                             const QRectF &region,
                                             double maxRegulateOffset) {
	drawCircleWatchMinuteHand(painter, region, maxRegulateOffset);
}

void WatchView::drawEllipseWatchMinuteHand(QPainter &painter,
                                           const QRectF &region,
                                           double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 6.0
                     * (regulatedTime.minute() + regulatedTime.second() / 60.0);
    double regulateRedius
        = regulateEllipseRedius(region, maxRegulateOffset, degrees);
	double minuteHandLength = (regulateRedius - 2 * maxScaleRedius)
                              * minuteHandRatio;
	double minuteHandWidth = getMinuteHandSize();
	QRgb rgbMinuteHandColor = getMinuteHandColor();
	painter.rotate(degrees);
	DialHandStyle minuteHandStyle = getMinuteHandStyle();
	if (DHS_LINE == minuteHandStyle) {
		drawLineHand(
            painter, rgbMinuteHandColor, minuteHandLength, minuteHandWidth);
	} else if (DHS_RHOMBUS == minuteHandStyle) {
		drawRhombusHand(
            painter, rgbMinuteHandColor, minuteHandLength, minuteHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRhombusWatchMinuteHand(QPainter &painter,
                                           const QRectF &region,
                                           double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 6.0
                     * (regulatedTime.minute() + regulatedTime.second() / 60.0);
    double regulateRedius
        = regulateRhombusRedius(region, maxRegulateOffset, degrees);
	double minuteHandLength = (regulateRedius - 2 * maxScaleRedius)
                              * minuteHandRatio;
	double minuteHandWidth = getMinuteHandSize();
	QRgb rgbMinuteHandColor = getMinuteHandColor();
	painter.rotate(degrees);
	DialHandStyle minuteHandStyle = getMinuteHandStyle();
	if (DHS_LINE == minuteHandStyle) {
		drawLineHand(
            painter, rgbMinuteHandColor, minuteHandLength, minuteHandWidth);
	} else if (DHS_RHOMBUS == minuteHandStyle) {
		drawRhombusHand(
            painter, rgbMinuteHandColor, minuteHandLength, minuteHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawWindmillWatchMinuteHand(QPainter &painter,
                                            const QRectF &region,
                                            double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 6.0
                     * (regulatedTime.minute() + regulatedTime.second() / 60.0);
    double regulateRedius
        = regulateWindmillRedius(region, maxRegulateOffset, degrees);
	double minuteHandLength = (regulateRedius - 2 * maxScaleRedius)
                              * minuteHandRatio;
	double minuteHandWidth = getMinuteHandSize();
	QRgb rgbMinuteHandColor = getMinuteHandColor();
	painter.rotate(degrees);
	DialHandStyle minuteHandStyle = getMinuteHandStyle();
	if (DHS_LINE == minuteHandStyle) {
		drawLineHand(
            painter, rgbMinuteHandColor, minuteHandLength, minuteHandWidth);
	} else if (DHS_RHOMBUS == minuteHandStyle) {
		drawRhombusHand(
            painter, rgbMinuteHandColor, minuteHandLength, minuteHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawWatchSecondHand(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset) {
	DialStyle dialStyle = getWatchDialStyle();
	switch (dialStyle) {
	case DS_CIRCLE:
		drawCircleWatchSecondHand(painter, region, maxRegulateOffset);
		break;
	case DS_ROUNDRECT:
		drawRoundRectWatchSecondHand(painter, region, maxRegulateOffset);
		break;
	case DS_ELLIPSE:
		drawEllipseWatchSecondHand(painter, region, maxRegulateOffset);
		break;
	case DS_RHOMBUS:
		drawRhombusWatchSecondHand(painter, region, maxRegulateOffset);
		break;
	case DS_WINDMILL:
		drawWindmillWatchSecondHand(painter, region, maxRegulateOffset);
		break;
	default:
		drawCircleWatchSecondHand(painter, region, maxRegulateOffset);
		break;
	}
}

void WatchView::drawCircleWatchSecondHand(QPainter &painter,
                                          const QRectF &region,
                                          double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	double maxScaleRedius = getMaxScaleRedius();
	double maxWatchRedius
        = static_cast<double>(qMin(region.width() / 2, region.height() / 2))
          - maxRegulateOffset;
	double secondHandLength = (maxWatchRedius - 2 * maxScaleRedius)
                              * secondHandRatio;
	double secondHandWidth = getSecondHandSize();
	QRgb rgbSecondColor = getSecondHandColor();
	QTime regulatedTime = getRegulatedTime();
	painter.rotate(6.0 * regulatedTime.second());
	DialHandStyle secondHandStyle = getSecondHandStyle();
	if (DHS_LINE == secondHandStyle) {
		drawLineHand(
            painter, rgbSecondColor, secondHandLength, secondHandWidth);
	} else if (DHS_RHOMBUS == secondHandStyle) {
		drawRhombusHand(
            painter, rgbSecondColor, secondHandLength, secondHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRoundRectWatchSecondHand(QPainter &painter,
                                             const QRectF &region,
                                             double maxRegulateOffset) {
	drawCircleWatchSecondHand(painter, region, maxRegulateOffset);
}

void WatchView::drawEllipseWatchSecondHand(QPainter &painter,
                                           const QRectF &region,
                                           double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 6.0 * regulatedTime.second();
    double regulateRedius
        = regulateEllipseRedius(region, maxRegulateOffset, degrees);
	double secondHandLength = (regulateRedius - 2 * maxScaleRedius)
                              * secondHandRatio;
	double secondHandWidth = getSecondHandSize();
	QRgb rgbSecondHandColor = getSecondHandColor();
	painter.rotate(degrees);
	DialHandStyle secondHandStyle = getSecondHandStyle();
	if (DHS_LINE == secondHandStyle) {
		drawLineHand(
            painter, rgbSecondHandColor, secondHandLength, secondHandWidth);
	} else if (DHS_RHOMBUS == secondHandStyle) {
		drawRhombusHand(
            painter, rgbSecondHandColor, secondHandLength, secondHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawRhombusWatchSecondHand(QPainter &painter,
                                           const QRectF &region,
                                           double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 6.0 * regulatedTime.second();
    double regulateRedius
        = regulateRhombusRedius(region, maxRegulateOffset, degrees);
	double secondHandLength = (regulateRedius - 2 * maxScaleRedius)
                              * secondHandRatio;
	double secondHandWidth = getSecondHandSize();
	QRgb rgbSecondHandColor = getSecondHandColor();
	painter.rotate(degrees);
	DialHandStyle secondHandStyle = getSecondHandStyle();
	if (DHS_LINE == secondHandStyle) {
		drawLineHand(
            painter, rgbSecondHandColor, secondHandLength, secondHandWidth);
	} else if (DHS_RHOMBUS == secondHandStyle) {
		drawRhombusHand(
            painter, rgbSecondHandColor, secondHandLength, secondHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawWindmillWatchSecondHand(QPainter &painter,
                                            const QRectF &region,
                                            double maxRegulateOffset) {
	painter.save();
	painter.translate(region.width() / 2, region.height() / 2);
	painter.rotate(270.0);
	QTime regulatedTime = getRegulatedTime();
	double maxScaleRedius = getMaxScaleRedius();
	double degrees = 6.0 * regulatedTime.second();
    double regulateRedius
        = regulateWindmillRedius(region, maxRegulateOffset, degrees);
	double secondHandLength = (regulateRedius - 2 * maxScaleRedius)
                              * secondHandRatio;
	double secondHandWidth = getSecondHandSize();
	QRgb rgbSecondHandColor = getSecondHandColor();
	painter.rotate(degrees);
	DialHandStyle secondHandStyle = getSecondHandStyle();
	if (DHS_LINE == secondHandStyle) {
		drawLineHand(
            painter, rgbSecondHandColor, secondHandLength, secondHandWidth);
	} else if (DHS_RHOMBUS == secondHandStyle) {
		drawRhombusHand(
            painter, rgbSecondHandColor, secondHandLength, secondHandWidth);
	}
	painter.restore();
	painter.resetTransform();
}

void WatchView::drawLineHand(QPainter &painter,
                             QRgb lineHandColor,
                             double lineHandLength,
                             double lineHandWidth) {
	painter.save();
	if (lineHandWidth < 1.0) {
		lineHandWidth = 1.0;
	}
	QPointF lineHandLeft(-lineHandLength / 5, 0);
	QPointF lineHandRight(lineHandLength, 0);
	QPen lineHandPen(QColor::fromRgba(lineHandColor));
	lineHandPen.setWidth(lineHandWidth);
	painter.setPen(lineHandPen);
	painter.drawLine(lineHandLeft, lineHandRight);
	painter.restore();
}

void WatchView::drawRhombusHand(QPainter &painter,
                                QRgb rhombusHandColor,
                                double RhombusHandLength,
                                double RhombusHandWidth) {
	painter.save();
	if (RhombusHandWidth < 1.0) {
		RhombusHandWidth = 1.0;
	}
	QPointF rhombusHandLeft(-qTan(PI / 3) * 2 * RhombusHandWidth, 0);
	QPointF rhombusHandTop(0, -RhombusHandWidth);
	QPointF rhombusHandRight(RhombusHandLength, 0);
	QPointF rhombusHandBottom(0, RhombusHandWidth);
	QPolygonF rhombusHandPolygon;
	rhombusHandPolygon.append(rhombusHandLeft);
	rhombusHandPolygon.append(rhombusHandTop);
	rhombusHandPolygon.append(rhombusHandRight);
	rhombusHandPolygon.append(rhombusHandBottom);
	rhombusHandPolygon.append(rhombusHandLeft);
	QPainterPath rhombusHandPath;
	rhombusHandPath.addPolygon(rhombusHandPolygon);
	painter.setPen(Qt::NoPen);
	QBrush secondHandBrush(QColor::fromRgba(rhombusHandColor));
	painter.fillPath(rhombusHandPath, secondHandBrush);
	painter.restore();
}

double WatchView::regulateEllipseRedius(const QRectF &region,
                                        double maxRegulateOffset,
                                        double degrees) {
    double a = (region.height() - 2 * maxRegulateOffset) / 2;
    double b = (region.width() - 2 * maxRegulateOffset) / 2;
    double radians = qDegreesToRadians(degrees);
    double regulateRedius = a * b
                            / qSqrt(a * a * qSin(radians) * qSin(radians)
                                    + b * b * qCos(radians) * qCos(radians));
    return regulateRedius;
}

double WatchView::regulateRhombusRedius(const QRectF &region,
                                        double maxRegulateOffset,
                                        double degrees) {
    double a = (region.height() - 2 * maxRegulateOffset) / 2;
    double b = (region.width() - 2 * maxRegulateOffset) / 2;
    double radians
        = qDegreesToRadians(degrees - qFloor(degrees / 360.0) * 360.0);
    double regulateRedius = 0;
    if (radians <= PI / 2) {
        regulateRedius = a * b / (a * qSin(radians) + b * qCos(radians));
    } else if (radians <= PI) {
        regulateRedius = a * b / (a * qSin(radians) - b * qCos(radians));
    } else if (radians <= 3 * PI / 2) {
        regulateRedius = a * b
                         / (a * qSin(radians - PI) + b * qCos(radians - PI));
    } else if (radians <= 2 * PI) {
        regulateRedius = a * b
                         / (a * qSin(radians - PI) - b * qCos(radians - PI));
    }
    return regulateRedius;
}

double WatchView::regulateWindmillRedius(const QRectF &region,
                                         double maxRegulateOffset,
                                         double degrees) {
    double maxWatchRedius
        = static_cast<double>(qMin(region.width() / 2, region.height() / 2))
          - maxRegulateOffset;
    double radians = qDegreesToRadians(degrees - qFloor(degrees / 90.0) * 90.0);
    double regulateRedius = 0;
    if (radians <= PI / 6) {
        regulateRedius
            = maxWatchRedius * qTan(windmillRadians)
              / (qTan(windmillRadians) * qCos(radians) + qSin(radians));
    } else if (radians <= PI / 3) {
        regulateRedius = maxWatchRedius * qTan(windmillRadians)
                         / (qTan(windmillRadians) * qCos(radians - PI / 6)
                            + qSin(radians - PI / 6));
    } else if (radians <= PI / 2) {
        regulateRedius = maxWatchRedius * qTan(windmillRadians)
                         / (qTan(windmillRadians) * qCos(radians - PI / 3)
                            + qSin(radians - PI / 3));
    }
    return regulateRedius;
}

QTime WatchView::getRegulatedTime() {
	QTime time = QTime::currentTime();
	QTime regulatedTime;
	int deltHour = getDeltHour();
	int deltMinute = getDeltMinute();
	int deltSecond = getDeltSecond();
	DialDeltType watchDeltType = getDeltType();
	if (DDT_POSITIVE == watchDeltType) {
		regulatedTime
            = time.addSecs(deltHour * 60 * 60 + deltMinute * 60 + deltSecond);
	} else {
		regulatedTime = time.addSecs(
            -(deltHour * 60 * 60 + deltMinute * 60 + deltSecond));
	}
	return regulatedTime;
}

double WatchView::getMaxScaleRedius() {
	return static_cast<double>(
        qMax(qMax(getHourDotSize(), getTsnDotSize()), getMinuteDotSize()));
}

bool WatchView::isEnabledText() {
    return watch->attr()->dial.value.dialText.enabledText;
}

QString WatchView::getText() {
    return QString::fromStdWString(this->watch->attr()->dial.value.dialText.text);
}

QPointF WatchView::getPaintPos() {
	QFontMetricsF textFontMetrics(getTextFont());
	QRectF textRect = textFontMetrics.boundingRect(getText());
	double xPosRatio
        = this->watch->attr()->dial.value.dialText.textAttr.xPosRatio;
	double yPosRatio
        = this->watch->attr()->dial.value.dialText.textAttr.yPosRatio;
	double viewWidth = this->rect().width();
	double viewHeight = this->rect().height();
	double watchDiameter = qMin(viewWidth, viewHeight);
	double xPos = (viewWidth - watchDiameter) / 2
                  + xPosRatio * (watchDiameter - textRect.width());
	double yPos = (viewHeight - watchDiameter) / 2
                  + yPosRatio * (watchDiameter - textRect.height());
	return QPointF(xPos, yPos);
}

QFont WatchView::getTextFont() {
	QFont textFont;
	QString textFontName = QString::fromStdWString(
        this->watch->attr()->dial.value.dialText.textAttr.textFontName);
	if (textFontName.isEmpty()) {
		QFont textFont;
		textFontName = textFont.defaultFamily();
	}
	int textFontSize
        = this->watch->attr()->dial.value.dialText.textAttr.textFontSize;
    bool bold = this->watch->attr()->dial.value.dialText.textAttr.bold;
    bool italic = this->watch->attr()->dial.value.dialText.textAttr.italic;
    bool underLine = this->watch->attr()->dial.value.dialText.textAttr.underLine;
	textFont.setFamily(textFontName);
	textFont.setPixelSize(textFontSize);
	textFont.setBold(bold);
	textFont.setItalic(italic);
	textFont.setUnderline(underLine);
	return textFont;
}

QColor WatchView::getTextColor() {
	return QColor::fromRgba(
        this->watch->attr()->dial.value.dialText.textAttr.textColor);
}

QPen WatchView::getTextPen() {
	return QPen(getTextColor());
}

void WatchView::drawText(QPainter &painter, const QRectF &region) {
	if (isEnabledText()) {
		drawText(painter,
                 region,
                 getText(),
                 getPaintPos(),
                 getTextFont(),
                 getTextPen());
	}
}

void WatchView::drawText(QPainter &painter,
                         const QRectF &region,
                         const QString &text,
                         const QPointF &paintPos,
                         const QFont &font,
                         const QPen &pen) {
	painter.save();
	painter.setFont(font);
	painter.setPen(pen);
	QStaticText staticText(text);
	QTextOption textOption;
	textOption.setAlignment(Qt::AlignCenter);
	staticText.setTextOption(textOption);
	painter.drawStaticText(paintPos, staticText);
	painter.restore();
}
