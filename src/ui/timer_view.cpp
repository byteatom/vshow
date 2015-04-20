#include "timer_view.h"
#include <QPainter>
#include <QDateTime>
#include <QStaticText>
#include <qmath.h>
#include "timer.h"
#include "data/image_attr.h"


#define UNICODE_UNIT_DAY_CH 0x5929
#define UNICODE_UNIT_HOUR_CH 0x65f6
#define UNICODE_UNIT_MINUTE_CH 0x5206
#define UNICODE_UNIT_SECOND_CH 0x79d2
#define UNICODE_UNIT_DAY_TW UNICODE_UNIT_DAY_CH
#define UNICODE_UNIT_HOUR_TW 0x6642
#define UNICODE_UNIT_MINUTE_TW UNICODE_UNIT_MINUTE_CH
#define UNICODE_UNIT_SECOND_TW UNICODE_UNIT_SECOND_CH

const QString TimerView::timerUint[3][4] = {
    {QString(QChar(UNICODE_UNIT_DAY_CH)),
     QString(QChar(UNICODE_UNIT_HOUR_CH)),
     QString(QChar(UNICODE_UNIT_MINUTE_CH)),
     QString(QChar(UNICODE_UNIT_SECOND_CH))},    // chinese
    {QString(QChar(UNICODE_UNIT_DAY_TW)),
     QString(QChar(UNICODE_UNIT_HOUR_TW)),
     QString(QChar(UNICODE_UNIT_MINUTE_TW)),
     QString(QChar(UNICODE_UNIT_SECOND_TW))},    // chinese-tw
    {"Day", "Hour", "Minute", "Second"}          // english
};
const double TimerView::numUnitSpace = 3;
const double TimerView::textTimeSpace = 3;

TimerView::TimerView(Timer *timer) : timer(timer), mouseSelect{this, timer} {
    setObjectName("TimerView");
    // setWindowFlags(Qt::ToolTip); //for video overlay
    setMouseTracking(true);
    unsetCursor();    // use parent cursor
    timer->owner->view()->addWidget(this);
    timer->owner->view()->setCurrentWidget(this);
    // setAttribute(Qt::WA_OpaquePaintEvent, true);
}

TimerView::~TimerView() {
}

void TimerView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (ImageAttr::BG_COLOR_NONE != timer->attr()->bgColor) {
        painter.fillRect(rect(), QColor::fromRgba(timer->attr()->bgColor));
    }
    if (!timer->bgCache.isNull()) {
        painter.drawPixmap(Slice::POS_TOPLEFT, timer->bgCache);
    }
    if (!timer->cache->isNull())
        painter.drawPixmap(Slice::POS_TOPLEFT, *timer->cache);
}

void TimerView::resizeEvent(QResizeEvent *event) {
    timer->cacheView();
    QWidget::resizeEvent(event);
}

QPixmap *TimerView::drawTimer() {
    if (!timer) {
        return nullptr;
    }
    QSize size(this->timer->owner->attr()->geo.width,
               this->timer->owner->attr()->geo.height);
    QPixmap *timer = new QPixmap(size);
    timer->fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
    QRectF region(Slice::POS_TOPLEFT, QSizeF(size));
    QPainter painter(timer);
    painter.setRenderHint(QPainter::Antialiasing);
    drawTimer(painter, region);
    return timer;
}


void TimerView::drawTimer(QPainter &painter, const QRectF &region) {
    bool displayDay = isDisplayDay();
    bool displayHour = isDisplayHour();
    bool displayMinute = isDisplayMinute();
    bool displaySecond = isDisplaySecond();
    if (displayDay) {
        if (displayHour) {
            if (displayMinute) {
                if (displaySecond) {
                    drawTimer_DHMS(painter, region);
                } else {
                    drawTimer_DHM(painter, region);
                }
            } else {
                if (displaySecond) {
                    drawTimer_DHS(painter, region);
                } else {
                    drawTimer_DH(painter, region);
                }
            }
        } else {
            if (displayMinute) {
                if (displaySecond) {
                    drawTimer_DMS(painter, region);
                } else {
                    drawTimer_DM(painter, region);
                }
            } else {
                if (displaySecond) {
                    drawTimer_DS(painter, region);
                } else {
                    drawTimer_D(painter, region);
                }
            }
        }
    } else {
        if (displayHour) {
            if (displayMinute) {
                if (displaySecond) {
                    drawTimer_HMS(painter, region);
                } else {
                    drawTimer_HM(painter, region);
                }
            } else {
                if (displaySecond) {
                    drawTimer_HS(painter, region);
                } else {
                    drawTimer_H(painter, region);
                }
            }
        } else {
            if (displayMinute) {
                if (displaySecond) {
                    drawTimer_MS(painter, region);
                } else {
                    drawTimer_M(painter, region);
                }
            } else {
                if (displaySecond) {
                    drawTimer_S(painter, region);
                } else {
                    drawTimer_None(painter, region);
                }
            }
        }
    }
}

void TimerView::drawTimer_DHMS(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    qint64 seconds = getSeconds(targetDateTime, currentDateTime);
    QString daysUnitStr = getDaysUnitStr();
    QString hoursUnitStr = getHoursUnitStr();
    QString minutesUnitStr = getMinutesUnitStr();
    QString secondsUnitStr = getSecondsUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    QString daysStr = getDaysStr(days);
    QString hoursStr = getHMSStr(hours);
    QString minutesStr = getHMSStr(minutes);
    QString secondsStr = getHMSStr(seconds);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF daysRect = timeMetrics.boundingRect(daysStr);
    QRectF daysUnitRect = timeMetrics.boundingRect(daysUnitStr);
    QRectF hoursRect = timeMetrics.boundingRect(hoursStr);
    QRectF hoursUnitRect = timeMetrics.boundingRect(hoursUnitStr);
    QRectF minutesRect = timeMetrics.boundingRect(minutesStr);
    QRectF minutesUnitRect = timeMetrics.boundingRect(minutesUnitStr);
    QRectF secondsRect = timeMetrics.boundingRect(secondsStr);
    QRectF secondsUnitRect = timeMetrics.boundingRect(secondsUnitStr);
    double timeToltleWidth = daysRect.width() + daysUnitRect.width()
                             + hoursRect.width() + hoursUnitRect.width()
                             + minutesRect.width() + minutesUnitRect.width()
                             + secondsRect.width() + secondsUnitRect.width()
                             + numUnitSpace * 7;
    double timeToltleHeight
        = (daysRect.height() + daysUnitRect.height() + hoursRect.height()
           + hoursUnitRect.height() + minutesRect.height()
           + minutesUnitRect.height() + secondsRect.height()
           + secondsUnitRect.height()) / 8;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + minutesUnitRect.width()
                          + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + minutesUnitRect.width()
                          + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, daysUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
        drawText(painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, hoursUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, minutesStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 minutesUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
        paintPos.setX(paintPos.x() + minutesUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, secondsStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 secondsUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_DHM(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    QString daysUnitStr = getDaysUnitStr();
    QString hoursUnitStr = getHoursUnitStr();
    QString minutesUnitStr = getMinutesUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    QString daysStr = getDaysStr(days);
    QString hoursStr = getHMSStr(hours);
    QString minutesStr = getHMSStr(minutes);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF daysRect = timeMetrics.boundingRect(daysStr);
    QRectF daysUnitRect = timeMetrics.boundingRect(daysUnitStr);
    QRectF hoursRect = timeMetrics.boundingRect(hoursStr);
    QRectF hoursUnitRect = timeMetrics.boundingRect(hoursUnitStr);
    QRectF minutesRect = timeMetrics.boundingRect(minutesStr);
    QRectF minutesUnitRect = timeMetrics.boundingRect(minutesUnitStr);
    double timeToltleWidth = daysRect.width() + daysUnitRect.width()
                             + hoursRect.width() + hoursUnitRect.width()
                             + minutesRect.width() + minutesUnitRect.width()
                             + numUnitSpace * 5;
    double timeToltleHeight
        = (daysRect.height() + daysUnitRect.height() + hoursRect.height()
           + hoursUnitRect.height() + minutesRect.height()
           + minutesUnitRect.height()) / 6;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, daysUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
        drawText(painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, hoursUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, minutesStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 minutesUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_DHS(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    qint64 seconds = getSeconds(targetDateTime, currentDateTime);
    QString daysUnitStr = getDaysUnitStr();
    QString hoursUnitStr = getHoursUnitStr();
    QString secondsUnitStr = getSecondsUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    QString daysStr = getDaysStr(days);
    QString hoursStr = getHMSStr(hours);
    seconds = minutes * 60 + seconds;
    QString secondsStr = getHMSStr(seconds);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF daysRect = timeMetrics.boundingRect(daysStr);
    QRectF daysUnitRect = timeMetrics.boundingRect(daysUnitStr);
    QRectF hoursRect = timeMetrics.boundingRect(hoursStr);
    QRectF hoursUnitRect = timeMetrics.boundingRect(hoursUnitStr);
    QRectF secondsRect = timeMetrics.boundingRect(secondsStr);
    QRectF secondsUnitRect = timeMetrics.boundingRect(secondsUnitStr);
    double timeToltleWidth = daysRect.width() + daysUnitRect.width()
                             + hoursRect.width() + hoursUnitRect.width()
                             + secondsRect.width() + secondsUnitRect.width()
                             + numUnitSpace * 5;
    double timeToltleHeight
        = (daysRect.height() + daysUnitRect.height() + hoursRect.height()
           + hoursUnitRect.height() + secondsRect.height()
           + secondsUnitRect.height()) / 6;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);

            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, daysUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
        drawText(painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, hoursUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, secondsStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 secondsUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_DH(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    QString daysUnitStr = getDaysUnitStr();
    QString hoursUnitStr = getHoursUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    QString daysStr = getDaysStr(days);
    QString hoursStr = getHMSStr(hours);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF daysRect = timeMetrics.boundingRect(daysStr);
    QRectF daysUnitRect = timeMetrics.boundingRect(daysUnitStr);
    QRectF hoursRect = timeMetrics.boundingRect(hoursStr);
    QRectF hoursUnitRect = timeMetrics.boundingRect(hoursUnitStr);
    double timeToltleWidth = daysRect.width() + daysUnitRect.width()
                             + hoursRect.width() + hoursUnitRect.width()
                             + numUnitSpace * 3;
    double timeToltleHeight = (daysRect.height() + daysUnitRect.height()
                               + hoursRect.height() + hoursUnitRect.height())
                              / 4;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, daysUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
        drawText(painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, hoursUnitStr, timerTimeFont, timerTimeUnitColor);
    }
}

void TimerView::drawTimer_DMS(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    qint64 seconds = getSeconds(targetDateTime, currentDateTime);
    QString daysUnitStr = getDaysUnitStr();
    QString minutesUnitStr = getMinutesUnitStr();
    QString secondsUnitStr = getSecondsUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    QString daysStr = getDaysStr(days);
    minutes = hours * 60 + minutes;
    QString minutesStr = getHMSStr(minutes);
    QString secondsStr = getHMSStr(seconds);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF daysRect = timeMetrics.boundingRect(daysStr);
    QRectF daysUnitRect = timeMetrics.boundingRect(daysUnitStr);
    QRectF minutesRect = timeMetrics.boundingRect(minutesStr);
    QRectF minutesUnitRect = timeMetrics.boundingRect(minutesUnitStr);
    QRectF secondsRect = timeMetrics.boundingRect(secondsStr);
    QRectF secondsUnitRect = timeMetrics.boundingRect(secondsUnitStr);
    double timeToltleWidth = daysRect.width() + daysUnitRect.width()
                             + minutesRect.width() + minutesUnitRect.width()
                             + secondsRect.width() + secondsUnitRect.width()
                             + numUnitSpace * 5;
    double timeToltleHeight
        = (daysRect.height() + daysUnitRect.height() + minutesRect.height()
           + minutesUnitRect.height() + secondsRect.height()
           + secondsUnitRect.height()) / 6;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + minutesUnitRect.width()
                          + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);

            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + minutesUnitRect.width()
                          + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, daysUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, minutesStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 minutesUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
        paintPos.setX(paintPos.x() + minutesUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, secondsStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 secondsUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_DM(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    QString daysUnitStr = getDaysUnitStr();
    QString minutesUnitStr = getMinutesUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    QString daysStr = getDaysStr(days);
    minutes = hours * 60 + minutes;
    QString minutesStr = getHMSStr(minutes);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF daysRect = timeMetrics.boundingRect(daysStr);
    QRectF daysUnitRect = timeMetrics.boundingRect(daysUnitStr);
    QRectF minutesRect = timeMetrics.boundingRect(minutesStr);
    QRectF minutesUnitRect = timeMetrics.boundingRect(minutesUnitStr);
    double timeToltleWidth = daysRect.width() + daysUnitRect.width()
                             + minutesRect.width() + minutesUnitRect.width()
                             + numUnitSpace * 3;
    double timeToltleHeight
        = (daysRect.height() + daysUnitRect.height() + minutesRect.height()
           + minutesUnitRect.height()) / 4;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, daysUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, minutesStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 minutesUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_DS(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    qint64 seconds = getSeconds(targetDateTime, currentDateTime);
    QString daysUnitStr = getDaysUnitStr();
    QString secondsUnitStr = getSecondsUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    QString daysStr = getDaysStr(days);
    seconds = hours * 60 * 60 + minutes * 60 + seconds;
    QString secondsStr = getHMSStr(seconds);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF daysRect = timeMetrics.boundingRect(daysStr);
    QRectF daysUnitRect = timeMetrics.boundingRect(daysUnitStr);
    QRectF secondsRect = timeMetrics.boundingRect(secondsStr);
    QRectF secondsUnitRect = timeMetrics.boundingRect(secondsUnitStr);
    double timeToltleWidth = daysRect.width() + daysUnitRect.width()
                             + secondsRect.width() + secondsUnitRect.width()
                             + numUnitSpace * 3;
    double timeToltleHeight
        = (daysRect.height() + daysUnitRect.height() + secondsRect.height()
           + secondsUnitRect.height()) / 4;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(xTopLeft + daysRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, daysUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + daysUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, secondsStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 secondsUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_D(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    QString daysUnitStr = getDaysUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    QString daysStr = getDaysStr(days);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF daysRect = timeMetrics.boundingRect(daysStr);
    QRectF daysUnitRect = timeMetrics.boundingRect(daysUnitStr);
    double timeToltleWidth = daysRect.width() + daysUnitRect.width()
                             + numUnitSpace * 1;
    double timeToltleHeight = (daysRect.height() + daysUnitRect.height()) / 2;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + daysRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     daysUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, daysStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(xTopLeft + daysRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, daysUnitStr, timerTimeFont, timerTimeUnitColor);
    }
}

void TimerView::drawTimer_HMS(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    qint64 seconds = getSeconds(targetDateTime, currentDateTime);
    QString hoursUnitStr = getHoursUnitStr();
    QString minutesUnitStr = getMinutesUnitStr();
    QString secondsUnitStr = getSecondsUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    hours = days * 24 + hours;
    QString hoursStr = getHMSStr(hours);
    QString minutesStr = getHMSStr(minutes);
    QString secondsStr = getHMSStr(seconds);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF hoursRect = timeMetrics.boundingRect(hoursStr);
    QRectF hoursUnitRect = timeMetrics.boundingRect(hoursUnitStr);
    QRectF minutesRect = timeMetrics.boundingRect(minutesStr);
    QRectF minutesUnitRect = timeMetrics.boundingRect(minutesUnitStr);
    QRectF secondsRect = timeMetrics.boundingRect(secondsStr);
    QRectF secondsUnitRect = timeMetrics.boundingRect(secondsUnitStr);
    double timeToltleWidth = hoursRect.width() + hoursUnitRect.width()
                             + minutesRect.width() + minutesUnitRect.width()
                             + secondsRect.width() + secondsUnitRect.width()
                             + numUnitSpace * 5;
    double timeToltleHeight
        = (hoursRect.height() + hoursUnitRect.height() + minutesRect.height()
           + minutesUnitRect.height() + secondsRect.height()
           + secondsUnitRect.height()) / 6;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + minutesUnitRect.width()
                          + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + minutesUnitRect.width()
                          + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, hoursUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, minutesStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 minutesUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
        paintPos.setX(paintPos.x() + minutesUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, secondsStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 secondsUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_HM(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    QString hoursUnitStr = getHoursUnitStr();
    QString minutesUnitStr = getMinutesUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    hours = days * 24 + hours;
    QString hoursStr = getHMSStr(hours);
    QString minutesStr = getHMSStr(minutes);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF hoursRect = timeMetrics.boundingRect(hoursStr);
    QRectF hoursUnitRect = timeMetrics.boundingRect(hoursUnitStr);
    QRectF minutesRect = timeMetrics.boundingRect(minutesStr);
    QRectF minutesUnitRect = timeMetrics.boundingRect(minutesUnitStr);
    double timeToltleWidth = hoursRect.width() + hoursUnitRect.width()
                             + minutesRect.width() + minutesUnitRect.width()
                             + numUnitSpace * 3;
    double timeToltleHeight
        = (hoursRect.height() + hoursUnitRect.height() + minutesRect.height()
           + minutesUnitRect.height()) / 4;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, hoursUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, minutesStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 minutesUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_HS(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    qint64 seconds = getSeconds(targetDateTime, currentDateTime);
    QString hoursUnitStr = getHoursUnitStr();
    QString secondsUnitStr = getSecondsUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    hours = days * 24 + hours;
    QString hoursStr = getHMSStr(hours);
    seconds = minutes * 60 + seconds;
    QString secondsStr = getHMSStr(seconds);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF hoursRect = timeMetrics.boundingRect(hoursStr);
    QRectF hoursUnitRect = timeMetrics.boundingRect(hoursUnitStr);
    QRectF secondsRect = timeMetrics.boundingRect(secondsStr);
    QRectF secondsUnitRect = timeMetrics.boundingRect(secondsUnitStr);
    double timeToltleWidth = hoursRect.width() + hoursUnitRect.width()
                             + secondsRect.width() + secondsUnitRect.width()
                             + numUnitSpace * 3;
    double timeToltleHeight
        = (hoursRect.height() + hoursUnitRect.height() + secondsRect.height()
           + secondsUnitRect.height()) / 4;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, hoursUnitStr, timerTimeFont, timerTimeUnitColor);
        paintPos.setX(paintPos.x() + hoursUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, secondsStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 secondsUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_H(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    QString hoursUnitStr = getHoursUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    hours = days * 24 + hours;
    QString hoursStr = getHMSStr(hours);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF hoursRect = timeMetrics.boundingRect(hoursStr);
    QRectF hoursUnitRect = timeMetrics.boundingRect(hoursUnitStr);
    double timeToltleWidth = hoursRect.width() + hoursUnitRect.width()
                             + numUnitSpace * 1;
    double timeToltleHeight = (hoursRect.height() + hoursUnitRect.height()) / 2;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(
                painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
            paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     hoursUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(painter, paintPos, hoursStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + hoursRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, hoursUnitStr, timerTimeFont, timerTimeUnitColor);
    }
}

void TimerView::drawTimer_MS(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    qint64 seconds = getSeconds(targetDateTime, currentDateTime);
    QString minutesUnitStr = getMinutesUnitStr();
    QString secondsUnitStr = getSecondsUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    minutes = days * 24 * 60 + hours * 60 + minutes;
    QString minutesStr = getHMSStr(minutes);
    QString secondsStr = getHMSStr(seconds);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF minutesRect = timeMetrics.boundingRect(minutesStr);
    QRectF minutesUnitRect = timeMetrics.boundingRect(minutesUnitStr);
    QRectF secondsRect = timeMetrics.boundingRect(secondsStr);
    QRectF secondsUnitRect = timeMetrics.boundingRect(secondsUnitStr);
    double timeToltleWidth = minutesRect.width() + minutesUnitRect.width()
                             + secondsRect.width() + secondsUnitRect.width()
                             + numUnitSpace * 3;
    double timeToltleHeight
        = (minutesRect.height() + minutesUnitRect.height()
           + secondsRect.height() + secondsUnitRect.height()) / 4;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + minutesUnitRect.width()
                          + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
            paintPos.setX(paintPos.x() + minutesUnitRect.width()
                          + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(
            painter, paintPos, minutesStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 minutesUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
        paintPos.setX(paintPos.x() + minutesUnitRect.width() + numUnitSpace);
        drawText(
            painter, paintPos, secondsStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 secondsUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_M(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    QString minutesUnitStr = getMinutesUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    minutes = days * 24 * 60 + hours * 60 + minutes;
    QString minutesStr = getHMSStr(minutes);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF minutesRect = timeMetrics.boundingRect(minutesStr);
    QRectF minutesUnitRect = timeMetrics.boundingRect(minutesUnitStr);
    double timeToltleWidth = minutesRect.width() + minutesUnitRect.width()
                             + numUnitSpace * 1;
    double timeToltleHeight = (minutesRect.height() + minutesUnitRect.height())
                              / 2;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(painter,
                     paintPos,
                     minutesStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     minutesUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(
            painter, paintPos, minutesStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + minutesRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 minutesUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}
void TimerView::drawTimer_S(QPainter &painter, const QRectF &region) {
    QDateTime targetDateTime = getTargetDateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 days = getDays(targetDateTime, currentDateTime);
    qint64 hours = getHours(targetDateTime, currentDateTime);
    qint64 minutes = getMinutes(targetDateTime, currentDateTime);
    qint64 seconds = getSeconds(targetDateTime, currentDateTime);
    QString secondsUnitStr = getSecondsUnitStr();
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    QFont timerTimeFont = getTimeFont();
    QColor timerTimeNumColor = getTimeNumColor();
    QColor timerTimeUnitColor = getTimeUnitColor();
    bool enabledText = isEnabledText();
    seconds = days * 24 * 60 * 60 + hours * 60 * 60 + minutes * 60 + seconds;
    QString secondsStr = getHMSStr(seconds);
    QFontMetricsF timeMetrics(timerTimeFont);
    QRectF secondsRect = timeMetrics.boundingRect(secondsStr);
    QRectF secondsUnitRect = timeMetrics.boundingRect(secondsUnitStr);
    double timeToltleWidth = secondsRect.width() + secondsUnitRect.width()
                             + numUnitSpace * 1;
    double timeToltleHeight = (secondsRect.height() + secondsUnitRect.height())
                              / 2;
    double xTopLeft = (regionWidth - timeToltleWidth) / 2;
    double yTopLeft = (regionHeight - timeToltleHeight) / 2;
    if (enabledText) {
        bool newline = isNewline();
        QString timerText = getText();
        QFont timerTextFont = getTextFont();
        QColor timerTextColor = getTextColor();
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        if (newline) {
            QPointF textPaintPos(
                (regionWidth - textRect.width()) / 2,
                (regionHeight - textRect.height() - timeToltleHeight) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setY(textPaintPos.y() + textRect.height() + textTimeSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        } else {
            QPointF textPaintPos((regionWidth - textRect.width() - textTimeSpace
                                  - timeToltleWidth) / 2,
                                 (regionHeight - textRect.height()) / 2);
            drawText(painter,
                     textPaintPos,
                     timerText,
                     timerTextFont,
                     timerTextColor);
            QPointF paintPos(xTopLeft, yTopLeft);
            paintPos.setX(textPaintPos.x() + textRect.width() + textTimeSpace);
            drawText(painter,
                     paintPos,
                     secondsStr,
                     timerTimeFont,
                     timerTimeNumColor);
            paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
            drawText(painter,
                     paintPos,
                     secondsUnitStr,
                     timerTimeFont,
                     timerTimeUnitColor);
        }
    } else {
        QPointF paintPos(xTopLeft, yTopLeft);
        drawText(
            painter, paintPos, secondsStr, timerTimeFont, timerTimeNumColor);
        paintPos.setX(paintPos.x() + secondsRect.width() + numUnitSpace);
        drawText(painter,
                 paintPos,
                 secondsUnitStr,
                 timerTimeFont,
                 timerTimeUnitColor);
    }
}

void TimerView::drawTimer_None(QPainter &painter, const QRectF &region) {
    qreal regionWidth = region.width();
    qreal regionHeight = region.height();
    bool enabledText = isEnabledText();
    QString timerText = getText();
    QFont timerTextFont = getTextFont();
    QColor timerTextColor = getTextColor();
    if (enabledText) {
        QFontMetricsF textMetrics(timerTextFont);
        QRectF textRect = textMetrics.boundingRect(timerText);
        QPointF textPaintPos((regionWidth - textRect.width()) / 2,
                             (regionHeight - textRect.height()) / 2);
        drawText(
            painter, textPaintPos, timerText, timerTextFont, timerTextColor);
    }
}

QDateTime TimerView::getTargetDateTime() {
    int targetYear
        = this->timer->attr()->timer.value.timerTime.targetDateTime.date.year;
    int targetMonth
        = this->timer->attr()->timer.value.timerTime.targetDateTime.date.month;
    int targetDay
        = this->timer->attr()->timer.value.timerTime.targetDateTime.date.day;
    int targetHour
        = this->timer->attr()->timer.value.timerTime.targetDateTime.time.hour;
    int targetMinute
        = this->timer->attr()->timer.value.timerTime.targetDateTime.time.minute;
    int targetSecond
        = this->timer->attr()->timer.value.timerTime.targetDateTime.time.second;
    QDate targetDate(targetYear, targetMonth, targetDay);
    QTime targetTime(targetHour, targetMinute, targetSecond);
    QDateTime targetDateTime(targetDate, targetTime);
    return targetDateTime;
}

bool TimerView::isDisplayDay() {
    return this->timer->attr()->timer.value.timerTime.displayFormat.displayDay;
}

bool TimerView::isDisplayHour() {
    return this->timer->attr()->timer.value.timerTime.displayFormat.displayHour;
}

bool TimerView::isDisplayMinute() {
    return this->timer->attr()->timer.value.timerTime.displayFormat.displayMinute;
}

bool TimerView::isDisplaySecond() {
    return this->timer->attr()->timer.value.timerTime.displayFormat.displaySecond;
}

qint64 TimerView::getDays(const QDateTime &targetDateTime,
                          const QDateTime &currentDateTime) {
    qint64 days;
    QDateTime startDateTime, endDateTime;
    TimerType timerType = this->timer->attr()->timer.value.timerTime.timerType;
    if (TIMER_UP == timerType) {
        startDateTime = targetDateTime;
        endDateTime = currentDateTime;
    } else {
        startDateTime = currentDateTime;
        endDateTime = targetDateTime;
    }
    if (endDateTime < startDateTime) {
        days = 0;
    } else {
        days = startDateTime.daysTo(endDateTime) - 1;
        if (days < 0) {
            days = 0;
        }
    }
    return days;
}

qint64 TimerView::getHours(const QDateTime &targetDateTime,
                           const QDateTime &currentDateTime) {
    qint64 days, hours;
    QDateTime startDateTime, endDateTime;
    TimerType timerType = this->timer->attr()->timer.value.timerTime.timerType;
    if (TIMER_UP == timerType) {
        startDateTime = targetDateTime;
        endDateTime = currentDateTime;
    } else {
        startDateTime = currentDateTime;
        endDateTime = targetDateTime;
    }
    if (endDateTime < startDateTime) {
        days = 0;
        hours = 0;
    } else {
        days = startDateTime.daysTo(endDateTime) - 1;
        if (days < 0) {
            days = 0;
        }
        qint64 totleSeconds = startDateTime.secsTo(endDateTime);
        qint64 remainderSeconds = totleSeconds - days * 24 * 60 * 60;
        hours = (remainderSeconds - remainderSeconds % (60 * 60)) / (60 * 60);
    }
    return hours;
}

qint64 TimerView::getMinutes(const QDateTime &targetDateTime,
                             const QDateTime &currentDateTime) {
    qint64 days, hours, minutes;
    QDateTime startDateTime, endDateTime;
    TimerType timerType = this->timer->attr()->timer.value.timerTime.timerType;
    if (TIMER_UP == timerType) {
        startDateTime = targetDateTime;
        endDateTime = currentDateTime;
    } else {
        startDateTime = currentDateTime;
        endDateTime = targetDateTime;
    }
    if (endDateTime < startDateTime) {
        days = 0;
        hours = 0;
        minutes = 0;
    } else {
        days = startDateTime.daysTo(endDateTime) - 1;
        if (days < 0) {
            days = 0;
        }
        qint64 totleSeconds = startDateTime.secsTo(endDateTime);
        qint64 remainderSeconds = totleSeconds - days * 24 * 60 * 60;
        hours = (remainderSeconds - remainderSeconds % (60 * 60)) / (60 * 60);
        remainderSeconds = remainderSeconds - hours * 60 * 60;
        minutes = (remainderSeconds - remainderSeconds % 60) / 60;
    }
    return minutes;
}

qint64 TimerView::getSeconds(const QDateTime &targetDateTime,
                             const QDateTime &currentDateTime) {
    qint64 days, hours, minutes, seconds;
    QDateTime startDateTime, endDateTime;
    TimerType timerType = this->timer->attr()->timer.value.timerTime.timerType;
    if (TIMER_UP == timerType) {
        startDateTime = targetDateTime;
        endDateTime = currentDateTime;
    } else {
        startDateTime = currentDateTime;
        endDateTime = targetDateTime;
    }
    if (endDateTime < startDateTime) {
        days = 0;
        hours = 0;
        minutes = 0;
        seconds = 0;
    } else {
        days = startDateTime.daysTo(endDateTime) - 1;
        if (days < 0) {
            days = 0;
        }
        qint64 totleSeconds = startDateTime.secsTo(endDateTime);
        qint64 remainderSeconds = totleSeconds - days * 24 * 60 * 60;
        hours = (remainderSeconds - remainderSeconds % (60 * 60)) / (60 * 60);
        remainderSeconds = remainderSeconds - hours * 60 * 60;
        minutes = (remainderSeconds - remainderSeconds % 60) / 60;
        remainderSeconds = remainderSeconds - minutes * 60;
        seconds = remainderSeconds;
    }
    return seconds;
}

QString TimerView::getDaysUnitStr() {
    return tr("Day");
}

QString TimerView::getHoursUnitStr() {
    return tr("Hour");
}

QString TimerView::getMinutesUnitStr() {
    return tr("Minute");
}

QString TimerView::getSecondsUnitStr() {
    return tr("Second");
}

QFont TimerView::getTimeFont() {
    QString timerTimeFontNeme = QString::fromStdWString(
        this->timer->attr()->timer.value.timerTime.timeFont.timeFontName);
    if (timerTimeFontNeme.isEmpty()) {
        QFont timerTimeFont;
        timerTimeFontNeme = timerTimeFont.defaultFamily();
    }
    int timerTimeFontSize
        = this->timer->attr()->timer.value.timerTime.timeFont.timeFontSize;
    QFont timerTimeFont;
    timerTimeFont.setFamily(timerTimeFontNeme);
    timerTimeFont.setPixelSize(timerTimeFontSize);
    return timerTimeFont;
}

QColor TimerView::getTimeNumColor() {
    return QColor::fromRgba(
        this->timer->attr()->timer.value.timerTime.timeColor.numColor);
}

QColor TimerView::getTimeUnitColor() {
    return QColor::fromRgba(
        this->timer->attr()->timer.value.timerTime.timeColor.unitColor);
}

bool TimerView::isEnabledText() {
    return this->timer->attr()->timer.value.timerText.enabledText;
}

bool TimerView::isNewline() {
    return this->timer->attr()->timer.value.timerText.newline;
}

QString TimerView::getText() {
    return QString::fromStdWString(
        this->timer->attr()->timer.value.timerText.text);
}

QFont TimerView::getTextFont() {
    QString timerTextFontName = QString::fromStdWString(
        this->timer->attr()->timer.value.timerText.textAttr.textFontName);
    if (timerTextFontName.isEmpty()) {
        QFont timerTextFont;
        timerTextFontName = timerTextFont.defaultFamily();
    }
    int timerTextFontSize
        = this->timer->attr()->timer.value.timerText.textAttr.textFontSize;
    bool bold = this->timer->attr()->timer.value.timerText.textAttr.bold;
    bool italic = this->timer->attr()->timer.value.timerText.textAttr.italic;
    bool underLine
        = this->timer->attr()->timer.value.timerText.textAttr.underLine;
    QFont timerTextFont;
    timerTextFont.setFamily(timerTextFontName);
    timerTextFont.setPixelSize(timerTextFontSize);
    timerTextFont.setBold(bold);
    timerTextFont.setItalic(italic);
    timerTextFont.setUnderline(underLine);
    return timerTextFont;
}

QColor TimerView::getTextColor() {
    return QColor::fromRgba(
        this->timer->attr()->timer.value.timerText.textAttr.textColor);
}

QString TimerView::getDaysStr(qint64 days) {
    QString daysStr;
    daysStr.setNum(days);
    if (100 <= days && days < 1000) {
        daysStr = "0" + daysStr;
    } else if (10 < days && days < 100) {
        daysStr = "00" + daysStr;
    } else if (0 <= days && days < 10) {
        daysStr = "000" + daysStr;
    }
    return daysStr;
}

QString TimerView::getHMSStr(qint64 value) {
    QString hmsStr;
    hmsStr.setNum(value);
    if (0 <= value && value < 10) {
        hmsStr = "0" + hmsStr;
    }
    return hmsStr;
}

void TimerView::drawText(QPainter &painter,
                         const QPointF &posation,
                         const QString &text,
                         const QFont &font,
                         const QColor &color) {
    painter.save();
    painter.setFont(font);
    painter.setPen(QPen(color));
    QStaticText staticText(text);
    QTextOption textOption;
    textOption.setAlignment(Qt::AlignCenter);
    staticText.setTextOption(textOption);
    painter.drawStaticText(posation, staticText);
    painter.restore();
}
