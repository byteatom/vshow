//#pragma  execution_character_set("utf-8")
#include "clock_view.h"
#include "clock.h"
#include <QDateTime>
#include <QPainter>
#include <QLocale>
#include <QStaticText>
#include "data/image_attr.h"

#define UNICODE_YEAR_CH 0x5e74
#define UNICODE_MONTH_CH 0x6708
#define UNICODE_DAY_CH 0x65e5
#define UNICODE_YEAR_TW UNICODE_YEAR_CH
#define UNICODE_MONTH_TW UNICODE_MONTH_CH
#define UNICODE_DAY_TW UNICODE_DAY_CH
#define UNICODE_HOUR_CH 0x65f6
#define UNICODE_MINUTE_CH 0x5206
#define UNICODE_SECOND_CH 0x79d2
#define UNICODE_HOUR_TW 0x6642
#define UNICODE_MINUTE_TW UNICODE_MINUTE_CH
#define UNICODE_SECOND_TW UNICODE_SECOND_CH

const double ClockView::tDwtSpace = 3;
const double ClockView::dwtSpace = 3;

ClockView::ClockView(Clock *clock) : clock(clock), mouseSelect{this, clock} {
    setObjectName("ClockView");
    // setWindowFlags(Qt::ToolTip); //for video overlay
    setMouseTracking(true);
    unsetCursor();    // use parent cursor
    clock->owner->view()->addWidget(this);
    clock->owner->view()->setCurrentWidget(this);
    // setAttribute(Qt::WA_OpaquePaintEvent, true);
}

ClockView::~ClockView() {
}

void ClockView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (ImageAttr::BG_COLOR_NONE != clock->attr()->bgColor) {
        painter.fillRect(rect(), QColor::fromRgba(clock->attr()->bgColor));
    }
    if (!clock->bgCache.isNull()) {
        painter.drawPixmap(Slice::POS_TOPLEFT, clock->bgCache);
    }
    if (!clock->cache->isNull())
        painter.drawPixmap(Slice::POS_TOPLEFT, *clock->cache);
}

void ClockView::resizeEvent(QResizeEvent *event) {
    clock->cacheView();
    QWidget::resizeEvent(event);
}

QPixmap *ClockView::drawClock() {
    if (!clock) {
        return nullptr;
    }
    QSize size(this->clock->owner->attr()->geo.width,
               this->clock->owner->attr()->geo.height);
    QPixmap *clock = new QPixmap(size);
    clock->fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
    QRectF region(Slice::POS_TOPLEFT, QSizeF(size));
    QPainter painter(clock);
    painter.setRenderHint(QPainter::Antialiasing);
    drawClock(painter, region);
    return clock;
}

void ClockView::drawClock(QPainter &painter, const QRectF &region) {
    ClockDisplayFormat displayFormat
        = this->clock->attr()->clock.value.clockTime.displayFormat;
    switch (displayFormat) {
    case CDF_DATE_WEEK_TIME:
        drawClockDWT(painter, region);
        break;
    case CDF_DATE_TIME_WEEK:
        drawClockDTW(painter, region);
        break;
    case CDF_WEEK_DATE_TIME:
        drawClockWDT(painter, region);
        break;
    case CDF_WEEK_TIME_DATE:
        drawClockWTD(painter, region);
        break;
    case CDF_TIME_DATE_WEEK:
        drawClockTDW(painter, region);
        break;
    case CDF_TIME_WEEK_DATE:
        drawClockTWD(painter, region);
        break;
    default:
        drawClockDWT(painter, region);
        break;
    }
}

ClockLineStyle ClockView::getClockLineStyle() {
    return this->clock->attr()->clock.value.clockTime.lineStyle;
}

QDateTime ClockView::getCurrentDateTime() {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int deltHour
        = this->clock->attr()->clock.value.clockTime.clockTimeDiff.deltHour;
    int deltMinute
        = this->clock->attr()->clock.value.clockTime.clockTimeDiff.deltMinute;
    int deltSecond
        = this->clock->attr()->clock.value.clockTime.clockTimeDiff.deltSecond;
    ClockDeltType deltType
        = this->clock->attr()->clock.value.clockTime.clockTimeDiff.deltType;
    if (CDT_POSITIVE == deltType) {
        currentDateTime = currentDateTime.addSecs(
            deltHour * 60 * 60 + deltMinute * 60 + deltSecond);
    } else if (CDT_NEGATIVE == deltType) {
        currentDateTime = currentDateTime.addSecs(
            -(deltHour * 60 * 60 + deltMinute * 60 + deltSecond));
    }
    return currentDateTime;
}

QString ClockView::getText() {
    return QString::fromStdWString(
        this->clock->attr()->clock.value.clockText.text);
}

QString ClockView::getDateFormatedString(QDateTime currentDateTime) {
    QDate currentDate = currentDateTime.date();
    ClockDateAlignFormat clockDateAlignFormat
        = this->clock->attr()->clock.value.clockTime.dateFormat.dateAlignFormat;
    ClockDateUintFormat clockDateUnitFormat
        = this->clock->attr()->clock.value.clockTime.dateFormat.dateUintFormat;
    QString dateFormatString;
    switch (clockDateAlignFormat) {
    case CDAF_YYYY_MM_DD:
        if (CDUF_SLASH == clockDateUnitFormat) {
            dateFormatString = "yyyy/MM/dd";
        } else if (CDUF_CHINESE == clockDateUnitFormat) {
            dateFormatString = "yyyy" + QString(QChar(UNICODE_YEAR_CH)) + "MM"
                               + QString(QChar(UNICODE_MONTH_CH)) + "dd"
                               + QString(QChar(UNICODE_DAY_CH));
        } else if (CDUF_CHINESE_TW == clockDateUnitFormat) {
            dateFormatString = "yyyy" + QString(QChar(UNICODE_YEAR_TW)) + "MM"
                               + QString(QChar(UNICODE_MONTH_TW)) + "dd"
                               + QString(QChar(UNICODE_DAY_TW));
        } else if (CDUF_COLON == clockDateUnitFormat) {
            dateFormatString = "yyyy:MM:dd";
        }
        break;
    case CDAF_YYYY_DD_MM:
        if (CDUF_SLASH == clockDateUnitFormat) {
            dateFormatString = "yyyy/dd/MM";
        } else if (CDUF_CHINESE == clockDateUnitFormat) {
            dateFormatString = "yyyy" + QString(QChar(UNICODE_YEAR_CH)) + "dd"
                               + QString(QChar(UNICODE_DAY_CH)) + "MM"
                               + QString(QChar(UNICODE_MONTH_CH));
        } else if (CDUF_CHINESE_TW == clockDateUnitFormat) {
            dateFormatString = "yyyy" + QString(QChar(UNICODE_YEAR_TW)) + "dd"
                               + QString(QChar(UNICODE_DAY_TW)) + "MM"
                               + QString(QChar(UNICODE_MONTH_TW));
        } else if (CDUF_COLON == clockDateUnitFormat) {
            dateFormatString = "yyyy:dd:MM";
        }
        break;
    case CDAF_MM_YYYY_DD:
        if (CDUF_SLASH == clockDateUnitFormat) {
            dateFormatString = "MM/yyyy/dd";
        } else if (CDUF_CHINESE == clockDateUnitFormat) {
            dateFormatString = "MM" + QString(QChar(UNICODE_MONTH_CH)) + "yyyy"
                               + QString(QChar(UNICODE_YEAR_CH)) + "dd"
                               + QString(QChar(UNICODE_DAY_CH));
        } else if (CDUF_CHINESE_TW == clockDateUnitFormat) {
            dateFormatString = "MM" + QString(QChar(UNICODE_MONTH_TW)) + "yyyy"
                               + QString(QChar(UNICODE_YEAR_TW)) + "dd"
                               + QString(QChar(UNICODE_DAY_TW));
        } else if (CDUF_COLON == clockDateUnitFormat) {
            dateFormatString = "MM:yyyy:dd";
        }
        break;
    case CDAF_MM_DD_YYYY:
        if (CDUF_SLASH == clockDateUnitFormat) {
            dateFormatString = "MM/dd/yyyy";
        } else if (CDUF_CHINESE == clockDateUnitFormat) {
            dateFormatString = "MM" + QString(QChar(UNICODE_MONTH_CH)) + "dd"
                               + QString(QChar(UNICODE_DAY_CH)) + "yyyy"
                               + QString(QChar(UNICODE_YEAR_CH));
        } else if (CDUF_CHINESE_TW == clockDateUnitFormat) {
            dateFormatString = "MM" + QString(QChar(UNICODE_MONTH_TW)) + "dd"
                               + QString(QChar(UNICODE_DAY_TW)) + "yyyy"
                               + QString(QChar(UNICODE_YEAR_TW));
        } else if (CDUF_COLON == clockDateUnitFormat) {
            dateFormatString = "MM:dd:yyyy";
        }
        break;
    case CDAF_DD_YYYY_MM:
        if (CDUF_SLASH == clockDateUnitFormat) {
            dateFormatString = "dd/yyyy/MM";
        } else if (CDUF_CHINESE == clockDateUnitFormat) {
            dateFormatString = "dd" + QString(QChar(UNICODE_DAY_CH)) + "yyyy"
                               + QString(QChar(UNICODE_YEAR_CH)) + "MM"
                               + QString(QChar(UNICODE_MONTH_CH));
        } else if (CDUF_CHINESE_TW == clockDateUnitFormat) {
            dateFormatString = "dd" + QString(QChar(UNICODE_DAY_TW)) + "yyyy"
                               + QString(QChar(UNICODE_YEAR_TW)) + "MM"
                               + QString(QChar(UNICODE_MONTH_TW));
        } else if (CDUF_COLON == clockDateUnitFormat) {
            dateFormatString = "dd:yyyy:MM";
        }
        break;
    case CDAF_DD_MM_YYYY:
        if (CDUF_SLASH == clockDateUnitFormat) {
            dateFormatString = "dd/MM/yyyy";
        } else if (CDUF_CHINESE == clockDateUnitFormat) {
            dateFormatString = "dd" + QString(QChar(UNICODE_DAY_CH)) + "MM"
                               + QString(QChar(UNICODE_MONTH_CH)) + "yyyy"
                               + QString(QChar(UNICODE_YEAR_CH));
        } else if (CDUF_CHINESE_TW == clockDateUnitFormat) {
            dateFormatString = "dd" + QString(QChar(UNICODE_DAY_TW)) + "MM"
                               + QString(QChar(UNICODE_MONTH_TW)) + "yyyy"
                               + QString(QChar(UNICODE_YEAR_TW));
        } else if (CDUF_COLON == clockDateUnitFormat) {
            dateFormatString = "dd:MM:yyyy";
        }
        break;
    default:
        if (CDUF_SLASH == clockDateUnitFormat) {
            dateFormatString = "yyyy/MM/dd";
        } else if (CDUF_CHINESE == clockDateUnitFormat) {
            dateFormatString = "yyyy" + QString(QChar(UNICODE_YEAR_CH)) + "MM"
                               + QString(QChar(UNICODE_MONTH_CH)) + "dd"
                               + QString(QChar(UNICODE_DAY_CH));
        } else if (CDUF_CHINESE_TW == clockDateUnitFormat) {
            dateFormatString = "yyyy" + QString(QChar(UNICODE_YEAR_TW)) + "MM"
                               + QString(QChar(UNICODE_MONTH_TW)) + "dd"
                               + QString(QChar(UNICODE_DAY_TW));
        } else if (CDUF_COLON == clockDateUnitFormat) {
            dateFormatString = "yyyy:MM:dd";
        }
        break;
    }
    return currentDate.toString(dateFormatString);
}

QString ClockView::getWeekFormatedString(QDateTime currentDateTime) {
    QLocale chinese(QLocale::Chinese, QLocale::China);
    QLocale chineseTW(QLocale::Chinese, QLocale::Taiwan);
    QLocale english(QLocale::English, QLocale::UnitedKingdom);
    int currentWeek = currentDateTime.date().dayOfWeek();
    QString weekFormatString;
    ClockWeekTextFormat clockWeekTextFormat
        = this->clock->attr()->clock.value.clockTime.weekFormat
              .clockWeekTextFormat;
    switch (clockWeekTextFormat) {
    case CWF_CHINESE:
        weekFormatString = chinese.dayName(currentWeek);
        break;
    case CWF_CHINESE_TW:
        weekFormatString = chineseTW.dayName(currentWeek);
        break;
    case CWF_ENGLISH:
        weekFormatString = english.dayName(currentWeek);
        break;
    default:
        weekFormatString = chinese.dayName(currentWeek);
        break;
    }
    return weekFormatString;
}

QString ClockView::getTimeFormatedString(QDateTime currentDateTime) {
    QTime currentTime = currentDateTime.time();
    QString timeFormatString;
    ClockTimeAlignFormat clockTimeAlignFormat
        = this->clock->attr()->clock.value.clockTime.timeFormat.timeAlignFormat;
    ClockTimeUintFormat clockTimeUintFormat
        = this->clock->attr()->clock.value.clockTime.timeFormat.timeUintFormat;
    switch (clockTimeAlignFormat) {
    case CTAF_HH_MM_SS:
        if (CTUF_COLON == clockTimeUintFormat) {
            timeFormatString = "hh:mm:ss";
        } else if (CTUF_CHINESE == clockTimeUintFormat) {
            timeFormatString = "hh" + QString(QChar(UNICODE_HOUR_CH)) + "mm"
                               + QString(QChar(UNICODE_MINUTE_CH)) + "ss"
                               + QString(QChar(UNICODE_SECOND_CH));
        } else if (CTUF_CHINESE_TW == clockTimeUintFormat) {
            timeFormatString = "hh" + QString(QChar(UNICODE_HOUR_TW)) + "mm"
                               + QString(QChar(UNICODE_MINUTE_TW)) + "ss"
                               + QString(QChar(UNICODE_SECOND_TW));
        }
        break;
    case CTAF_HH_MM:
        if (CTUF_COLON == clockTimeUintFormat) {
            timeFormatString = "hh:mm";
        } else if (CTUF_CHINESE == clockTimeUintFormat) {
            timeFormatString = "hh" + QString(QChar(UNICODE_HOUR_CH)) + "mm"
                               + QString(QChar(UNICODE_MINUTE_CH));
        } else if (CTUF_CHINESE_TW == clockTimeUintFormat) {
            timeFormatString = "hh" + QString(QChar(UNICODE_HOUR_TW)) + "mm"
                               + QString(QChar(UNICODE_MINUTE_TW));
        }
        break;
    default:
        if (CTUF_COLON == clockTimeUintFormat) {
            timeFormatString = "hh:mm:ss";
        } else if (CTUF_CHINESE == clockTimeUintFormat) {
            timeFormatString = "hh:mm:ss";
        } else if (CTUF_CHINESE_TW == clockTimeUintFormat) {
            timeFormatString = "hh:mm:ss";
        }
        break;
    }
    return currentTime.toString(timeFormatString);
}

QFont ClockView::getTextFont() {
    QFont textFont;
    QString textFontName;
    textFontName = QString::fromStdWString(
        this->clock->attr()->clock.value.clockText.textAttr.textFontName);
    if (textFontName.isEmpty()) {
        QFont textFont;
        textFontName = textFont.defaultFamily();
    }
    textFont.setFamily(textFontName);
    textFont.setPixelSize(
        this->clock->attr()->clock.value.clockText.textAttr.textFontSize);
    bool bold = this->clock->attr()->clock.value.clockText.textAttr.bold;
    bool italic = this->clock->attr()->clock.value.clockText.textAttr.italic;
    bool underLine
        = this->clock->attr()->clock.value.clockText.textAttr.underLine;
    textFont.setBold(bold);
    textFont.setItalic(italic);
    textFont.setUnderline(underLine);
    return textFont;
}

QFont ClockView::getDateFont() {
    QFont dateFont;
    QString dateFontName = QString::fromStdWString(
        this->clock->attr()->clock.value.clockTime.dateFormat.dateFontName);
    if (dateFontName.isEmpty()) {
        QFont dateFont;
        dateFontName = dateFont.defaultFamily();
    }
    dateFont.setFamily(dateFontName);
    dateFont.setPixelSize(
        this->clock->attr()->clock.value.clockTime.dateFormat.dateFontSize);
    return dateFont;
}

QFont ClockView::getWeekFont() {
    QFont weekFont;
    QString weekFontName;
    weekFontName = QString::fromStdWString(
        this->clock->attr()->clock.value.clockTime.weekFormat.weekFontName);
    if (weekFontName.isEmpty()) {
        QFont weekFont;
        weekFontName = weekFont.defaultFamily();
    }
    weekFont.setFamily(weekFontName);
    weekFont.setPixelSize(
        this->clock->attr()->clock.value.clockTime.weekFormat.weekFontSize);
    return weekFont;
}

QFont ClockView::getTimeFont() {
    QFont timeFont;
    QString timeFontName;
    timeFontName = QString::fromStdWString(
        this->clock->attr()->clock.value.clockTime.timeFormat.timeFontName);
    if (timeFontName.isEmpty()) {
        QFont timeFont;
        timeFontName = timeFont.defaultFamily();
    }
    timeFont.setFamily(timeFontName);
    timeFont.setPixelSize(
        this->clock->attr()->clock.value.clockTime.timeFormat.timeFontSize);
    return timeFont;
}

QColor ClockView::getTextColor() {
    return QColor::fromRgba(
        this->clock->attr()->clock.value.clockText.textAttr.textColor);
}

QColor ClockView::getDateColor() {
    return QColor::fromRgba(
        this->clock->attr()->clock.value.clockTime.dateFormat.dateColor);
}

QColor ClockView::getWeekColor() {
    return QColor::fromRgba(
        this->clock->attr()->clock.value.clockTime.weekFormat.weekColor);
}

QColor ClockView::getTimeColor() {
    return QColor::fromRgba(
        this->clock->attr()->clock.value.clockTime.timeFormat.timeColor);
}

QPen ClockView::getTextPen() {
    return QPen(getTextColor());
}

QPen ClockView::getDatePen() {
    return QPen(getDateColor());
}

QPen ClockView::getWeekPen() {
    return QPen(getWeekColor());
}

QPen ClockView::getTimePen() {
    return QPen(getTimeColor());
}

bool ClockView::isEnabledText() {
    return this->clock->attr()->clock.value.clockText.enabledText;
}

int ClockView::getLineSpace() {
    return this->clock->attr()->clock.value.clockTime.lineSpace;
}

void ClockView::drawClockDWT(QPainter &painter, const QRectF &region) {
    QDateTime currentDateTime = getCurrentDateTime();
    QString dateFormated = getDateFormatedString(currentDateTime);
    QString weekFormated = getWeekFormatedString(currentDateTime);
    QString timeFormated = getTimeFormatedString(currentDateTime);
    QFont dateFont = getDateFont();
    QFont weekFont = getWeekFont();
    QFont timeFont = getTimeFont();
    QFontMetricsF dateFontMetrics(dateFont);
    QFontMetricsF weekFontMetrics(weekFont);
    QFontMetricsF timeFontMetrics(timeFont);
    QRectF dateBoundRect = dateFontMetrics.boundingRect(dateFormated);
    QRectF weekBoundRect = weekFontMetrics.boundingRect(weekFormated);
    QRectF timeBoundRect = timeFontMetrics.boundingRect(timeFormated);
    double regionWidth = region.width();
    double regionHeight = region.height();
    double dateStrWidth = dateBoundRect.width();
    double dateStrHeight = dateBoundRect.height();
    double weekStrWidth = weekBoundRect.width();
    double weekStrHeight = weekBoundRect.height();
    double timeStrWidth = timeBoundRect.width();
    double timeStrHeight = timeBoundRect.height();
    QPointF datePaintPos;
    QPointF weekPaintPos;
    QPointF timePaintPos;
    QString text = getText();
    QFont textFont = getTextFont();
    QFontMetricsF textFontMetrics(textFont);
    QRectF textBoundRect = textFontMetrics.boundingRect(text);
    double textStrWidth = textBoundRect.width();
    double textStrHeight = textBoundRect.height();
    QPointF textPaintPos;
    bool enabledText = isEnabledText();
    int lineSpace = getLineSpace();
    ClockLineStyle clockLineStyle = getClockLineStyle();
    if (CLS_SINGLE == clockLineStyle) {
        double dwtWidth = dateStrWidth + weekStrWidth + timeStrWidth
                          + dwtSpace * 2 + lineSpace * 2;
        double dwtHeight = (dateStrHeight + weekStrHeight + timeStrHeight) / 3;
        QPointF clockStartPos;
        if (enabledText) {
            double totleWidth
                = (textStrWidth + tDwtSpace + dwtWidth + lineSpace);
            QPointF textStartPos((regionWidth - totleWidth) / 2,
                                 (regionHeight - textStrHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            clockStartPos.setX(textPaintPos.x() + textStrWidth + tDwtSpace
                               + lineSpace);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        } else {
            clockStartPos.setX((regionWidth - dwtWidth) / 2);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        }
        datePaintPos.setX(clockStartPos.x());
        datePaintPos.setY(clockStartPos.y());
        weekPaintPos.setX(datePaintPos.x() + dateStrWidth + dwtSpace
                          + lineSpace);
        weekPaintPos.setY(clockStartPos.y());
        timePaintPos.setX(weekPaintPos.x() + weekStrWidth + dwtSpace
                          + lineSpace);
        timePaintPos.setY(clockStartPos.y());
    } else {
        double dwtHeight = dateStrHeight + weekStrHeight + timeStrHeight
                           + dwtSpace * 2 + lineSpace * 2;
        if (enabledText) {
            double totleHeight = textStrHeight + tDwtSpace + dwtHeight
                                 + lineSpace;
            QPointF textStartPos((regionWidth - textStrWidth) / 2,
                                 (regionHeight - totleHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            datePaintPos.setX((regionWidth - dateStrWidth) / 2);
            datePaintPos.setY(textPaintPos.y() + textStrHeight + tDwtSpace
                              + lineSpace);
        } else {
            datePaintPos.setX((regionWidth - dateStrWidth) / 2);
            datePaintPos.setY((regionHeight - dwtHeight) / 2);
        }
        weekPaintPos.setX((regionWidth - weekStrWidth) / 2);
        weekPaintPos.setY(datePaintPos.y() + dateStrHeight + dwtSpace
                          + lineSpace);
        timePaintPos.setX((regionWidth - timeStrWidth) / 2);
        timePaintPos.setY(weekPaintPos.y() + weekStrHeight + dwtSpace
                          + lineSpace);
    }
    if (enabledText) {
        drawText(painter, text, textPaintPos, textFont, getTextPen());
    }
    drawText(painter, dateFormated, datePaintPos, dateFont, getDatePen());
    drawText(painter, weekFormated, weekPaintPos, weekFont, getWeekPen());
    drawText(painter, timeFormated, timePaintPos, timeFont, getTimePen());
}

void ClockView::drawClockDTW(QPainter &painter, const QRectF &region) {
    QDateTime currentDateTime = getCurrentDateTime();
    QString dateFormated = getDateFormatedString(currentDateTime);
    QString weekFormated = getWeekFormatedString(currentDateTime);
    QString timeFormated = getTimeFormatedString(currentDateTime);
    QFont dateFont = getDateFont();
    QFont weekFont = getWeekFont();
    QFont timeFont = getTimeFont();
    QFontMetricsF dateFontMetrics(dateFont);
    QFontMetricsF weekFontMetrics(weekFont);
    QFontMetricsF timeFontMetrics(timeFont);
    QRectF dateBoundRect = dateFontMetrics.boundingRect(dateFormated);
    QRectF weekBoundRect = weekFontMetrics.boundingRect(weekFormated);
    QRectF timeBoundRect = timeFontMetrics.boundingRect(timeFormated);
    double regionWidth = region.width();
    double regionHeight = region.height();
    double dateStrWidth = dateBoundRect.width();
    double dateStrHeight = dateBoundRect.height();
    double weekStrWidth = weekBoundRect.width();
    double weekStrHeight = weekBoundRect.height();
    double timeStrWidth = timeBoundRect.width();
    double timeStrHeight = timeBoundRect.height();
    QPointF datePaintPos;
    QPointF weekPaintPos;
    QPointF timePaintPos;
    QString text = getText();
    QFont textFont = getTextFont();
    QFontMetricsF textFontMetrics(textFont);
    QRectF textBoundRect = textFontMetrics.boundingRect(text);
    double textStrWidth = textBoundRect.width();
    double textStrHeight = textBoundRect.height();
    QPointF textPaintPos;
    bool enabledText = isEnabledText();
    int lineSpace = getLineSpace();
    ClockLineStyle clockLineStyle = getClockLineStyle();
    if (CLS_SINGLE == clockLineStyle) {
        double dwtWidth = dateStrWidth + weekStrWidth + timeStrWidth
                          + dwtSpace * 2 + lineSpace * 2;
        double dwtHeight = (dateStrHeight + weekStrHeight + timeStrHeight) / 3;
        QPointF clockStartPos;
        if (enabledText) {
            double totleWidth
                = (textStrWidth + tDwtSpace + dwtWidth + lineSpace);
            QPointF textStartPos((regionWidth - totleWidth) / 2,
                                 (regionHeight - textStrHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            clockStartPos.setX(textPaintPos.x() + textStrWidth + tDwtSpace
                               + lineSpace);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        } else {
            clockStartPos.setX((regionWidth - dwtWidth) / 2);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        }
        datePaintPos.setX(clockStartPos.x());
        datePaintPos.setY(clockStartPos.y());
        timePaintPos.setX(datePaintPos.x() + dateStrWidth + dwtSpace
                          + lineSpace);
        timePaintPos.setY(clockStartPos.y());
        weekPaintPos.setX(timePaintPos.x() + timeStrWidth + dwtSpace
                          + lineSpace);
        weekPaintPos.setY(clockStartPos.y());
    } else {
        double dwtHeight = dateStrHeight + weekStrHeight + timeStrHeight
                           + dwtSpace * 2 + lineSpace * 2;
        if (enabledText) {
            double totleHeight = textStrHeight + tDwtSpace + dwtHeight
                                 + lineSpace;
            QPointF textStartPos((regionWidth - textStrWidth) / 2,
                                 (regionHeight - totleHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            datePaintPos.setX((regionWidth - dateStrWidth) / 2);
            datePaintPos.setY(textPaintPos.y() + textStrHeight + tDwtSpace
                              + lineSpace);
        } else {
            datePaintPos.setX((regionWidth - dateStrWidth) / 2);
            datePaintPos.setY((regionHeight - dwtHeight) / 2);
        }
        timePaintPos.setX((regionWidth - timeStrWidth) / 2);
        timePaintPos.setY(datePaintPos.y() + dateStrHeight + dwtSpace
                          + lineSpace);
        weekPaintPos.setX((regionWidth - weekStrWidth) / 2);
        weekPaintPos.setY(timePaintPos.y() + timeStrHeight + dwtSpace
                          + lineSpace);
    }
    if (enabledText) {
        drawText(painter, text, textPaintPos, textFont, getTextPen());
    }
    drawText(painter, dateFormated, datePaintPos, dateFont, getDatePen());
    drawText(painter, weekFormated, weekPaintPos, weekFont, getWeekPen());
    drawText(painter, timeFormated, timePaintPos, timeFont, getTimePen());
}

void ClockView::drawClockWDT(QPainter &painter, const QRectF &region) {
    QDateTime currentDateTime = getCurrentDateTime();
    QString dateFormated = getDateFormatedString(currentDateTime);
    QString weekFormated = getWeekFormatedString(currentDateTime);
    QString timeFormated = getTimeFormatedString(currentDateTime);
    QFont dateFont = getDateFont();
    QFont weekFont = getWeekFont();
    QFont timeFont = getTimeFont();
    QFontMetricsF dateFontMetrics(dateFont);
    QFontMetricsF weekFontMetrics(weekFont);
    QFontMetricsF timeFontMetrics(timeFont);
    QRectF dateBoundRect = dateFontMetrics.boundingRect(dateFormated);
    QRectF weekBoundRect = weekFontMetrics.boundingRect(weekFormated);
    QRectF timeBoundRect = timeFontMetrics.boundingRect(timeFormated);
    double regionWidth = region.width();
    double regionHeight = region.height();
    double dateStrWidth = dateBoundRect.width();
    double dateStrHeight = dateBoundRect.height();
    double weekStrWidth = weekBoundRect.width();
    double weekStrHeight = weekBoundRect.height();
    double timeStrWidth = timeBoundRect.width();
    double timeStrHeight = timeBoundRect.height();
    QPointF datePaintPos;
    QPointF weekPaintPos;
    QPointF timePaintPos;
    QString text = getText();
    QFont textFont = getTextFont();
    QFontMetricsF textFontMetrics(textFont);
    QRectF textBoundRect = textFontMetrics.boundingRect(text);
    double textStrWidth = textBoundRect.width();
    double textStrHeight = textBoundRect.height();
    QPointF textPaintPos;
    bool enabledText = isEnabledText();
    int lineSpace = getLineSpace();
    ClockLineStyle clockLineStyle = getClockLineStyle();
    if (CLS_SINGLE == clockLineStyle) {
        double dwtWidth = dateStrWidth + weekStrWidth + timeStrWidth
                          + dwtSpace * 2 + lineSpace * 2;
        double dwtHeight = (dateStrHeight + weekStrHeight + timeStrHeight) / 3;
        QPointF clockStartPos;
        if (enabledText) {
            double totleWidth
                = (textStrWidth + tDwtSpace + dwtWidth + lineSpace);
            QPointF textStartPos((regionWidth - totleWidth) / 2,
                                 (regionHeight - textStrHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            clockStartPos.setX(textPaintPos.x() + textStrWidth + tDwtSpace
                               + lineSpace);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        } else {
            clockStartPos.setX((regionWidth - dwtWidth) / 2);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        }
        weekPaintPos.setX(clockStartPos.x());
        weekPaintPos.setY(clockStartPos.y());
        datePaintPos.setX(weekPaintPos.x() + weekStrWidth + dwtSpace
                          + lineSpace);
        datePaintPos.setY(clockStartPos.y());
        timePaintPos.setX(datePaintPos.x() + timeStrWidth + dwtSpace
                          + lineSpace);
        timePaintPos.setY(clockStartPos.y());
    } else {
        double dwtHeight = dateStrHeight + weekStrHeight + timeStrHeight
                           + dwtSpace * 2 + lineSpace * 2;
        if (enabledText) {
            double totleHeight = textStrHeight + tDwtSpace + dwtHeight
                                 + lineSpace;
            QPointF textStartPos((regionWidth - textStrWidth) / 2,
                                 (regionHeight - totleHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            weekPaintPos.setX((regionWidth - weekStrWidth) / 2);
            weekPaintPos.setY(textPaintPos.y() + textStrHeight + tDwtSpace
                              + lineSpace);
        } else {
            weekPaintPos.setX((regionWidth - weekStrWidth) / 2);
            weekPaintPos.setY((regionHeight - dwtHeight) / 2);
        }
        datePaintPos.setX((regionWidth - dateStrWidth) / 2);
        datePaintPos.setY(weekPaintPos.y() + weekStrHeight + dwtSpace
                          + lineSpace);
        timePaintPos.setX((regionWidth - timeStrWidth) / 2);
        timePaintPos.setY(datePaintPos.y() + dateStrHeight + dwtSpace
                          + lineSpace);
    }
    if (enabledText) {
        drawText(painter, text, textPaintPos, textFont, getTextPen());
    }
    drawText(painter, dateFormated, datePaintPos, dateFont, getDatePen());
    drawText(painter, weekFormated, weekPaintPos, weekFont, getWeekPen());
    drawText(painter, timeFormated, timePaintPos, timeFont, getTimePen());
}

void ClockView::drawClockWTD(QPainter &painter, const QRectF &region) {
    QDateTime currentDateTime = getCurrentDateTime();
    QString dateFormated = getDateFormatedString(currentDateTime);
    QString weekFormated = getWeekFormatedString(currentDateTime);
    QString timeFormated = getTimeFormatedString(currentDateTime);
    QFont dateFont = getDateFont();
    QFont weekFont = getWeekFont();
    QFont timeFont = getTimeFont();
    QFontMetricsF dateFontMetrics(dateFont);
    QFontMetricsF weekFontMetrics(weekFont);
    QFontMetricsF timeFontMetrics(timeFont);
    QRectF dateBoundRect = dateFontMetrics.boundingRect(dateFormated);
    QRectF weekBoundRect = weekFontMetrics.boundingRect(weekFormated);
    QRectF timeBoundRect = timeFontMetrics.boundingRect(timeFormated);
    double regionWidth = region.width();
    double regionHeight = region.height();
    double dateStrWidth = dateBoundRect.width();
    double dateStrHeight = dateBoundRect.height();
    double weekStrWidth = weekBoundRect.width();
    double weekStrHeight = weekBoundRect.height();
    double timeStrWidth = timeBoundRect.width();
    double timeStrHeight = timeBoundRect.height();
    QPointF datePaintPos;
    QPointF weekPaintPos;
    QPointF timePaintPos;
    QString text = getText();
    QFont textFont = getTextFont();
    QFontMetricsF textFontMetrics(textFont);
    QRectF textBoundRect = textFontMetrics.boundingRect(text);
    double textStrWidth = textBoundRect.width();
    double textStrHeight = textBoundRect.height();
    QPointF textPaintPos;
    bool enabledText = isEnabledText();
    int lineSpace = getLineSpace();
    ClockLineStyle clockLineStyle = getClockLineStyle();
    if (CLS_SINGLE == clockLineStyle) {
        double dwtWidth = dateStrWidth + weekStrWidth + timeStrWidth
                          + dwtSpace * 2 + lineSpace * 2;
        double dwtHeight = (dateStrHeight + weekStrHeight + timeStrHeight) / 3;
        QPointF clockStartPos;
        if (enabledText) {
            double totleWidth
                = (textStrWidth + tDwtSpace + dwtWidth + lineSpace);
            QPointF textStartPos((regionWidth - totleWidth) / 2,
                                 (regionHeight - textStrHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            clockStartPos.setX(textPaintPos.x() + textStrWidth + tDwtSpace
                               + lineSpace);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        } else {
            clockStartPos.setX((regionWidth - dwtWidth) / 2);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        }
        weekPaintPos.setX(clockStartPos.x());
        weekPaintPos.setY(clockStartPos.y());
        timePaintPos.setX(weekPaintPos.x() + weekStrWidth + dwtSpace
                          + lineSpace);
        timePaintPos.setY(clockStartPos.y());
        datePaintPos.setX(timePaintPos.x() + timeStrWidth + dwtSpace
                          + lineSpace);
        datePaintPos.setY(clockStartPos.y());
    } else {
        double dwtHeight = dateStrHeight + weekStrHeight + timeStrHeight
                           + dwtSpace * 2 + lineSpace * 2;
        if (enabledText) {
            double totleHeight = textStrHeight + tDwtSpace + dwtHeight
                                 + lineSpace;
            QPointF textStartPos((regionWidth - textStrWidth) / 2,
                                 (regionHeight - totleHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            weekPaintPos.setX((regionWidth - weekStrWidth) / 2);
            weekPaintPos.setY(textPaintPos.y() + textStrHeight + tDwtSpace
                              + lineSpace);
        } else {
            weekPaintPos.setX((regionWidth - weekStrWidth) / 2);
            weekPaintPos.setY((regionHeight - dwtHeight) / 2);
        }
        timePaintPos.setX((regionWidth - timeStrWidth) / 2);
        timePaintPos.setY(weekPaintPos.y() + weekStrHeight + dwtSpace
                          + lineSpace);
        datePaintPos.setX((regionWidth - dateStrWidth) / 2);
        datePaintPos.setY(timePaintPos.y() + timeStrHeight + dwtSpace
                          + lineSpace);
    }
    if (enabledText) {
        drawText(painter, text, textPaintPos, textFont, getTextPen());
    }
    drawText(painter, dateFormated, datePaintPos, dateFont, getDatePen());
    drawText(painter, weekFormated, weekPaintPos, weekFont, getWeekPen());
    drawText(painter, timeFormated, timePaintPos, timeFont, getTimePen());
}

void ClockView::drawClockTDW(QPainter &painter, const QRectF &region) {
    QDateTime currentDateTime = getCurrentDateTime();
    QString dateFormated = getDateFormatedString(currentDateTime);
    QString weekFormated = getWeekFormatedString(currentDateTime);
    QString timeFormated = getTimeFormatedString(currentDateTime);
    QFont dateFont = getDateFont();
    QFont weekFont = getWeekFont();
    QFont timeFont = getTimeFont();
    QFontMetricsF dateFontMetrics(dateFont);
    QFontMetricsF weekFontMetrics(weekFont);
    QFontMetricsF timeFontMetrics(timeFont);
    QRectF dateBoundRect = dateFontMetrics.boundingRect(dateFormated);
    QRectF weekBoundRect = weekFontMetrics.boundingRect(weekFormated);
    QRectF timeBoundRect = timeFontMetrics.boundingRect(timeFormated);
    double regionWidth = region.width();
    double regionHeight = region.height();
    double dateStrWidth = dateBoundRect.width();
    double dateStrHeight = dateBoundRect.height();
    double weekStrWidth = weekBoundRect.width();
    double weekStrHeight = weekBoundRect.height();
    double timeStrWidth = timeBoundRect.width();
    double timeStrHeight = timeBoundRect.height();
    QPointF datePaintPos;
    QPointF weekPaintPos;
    QPointF timePaintPos;
    QString text = getText();
    QFont textFont = getTextFont();
    QFontMetricsF textFontMetrics(textFont);
    QRectF textBoundRect = textFontMetrics.boundingRect(text);
    double textStrWidth = textBoundRect.width();
    double textStrHeight = textBoundRect.height();
    QPointF textPaintPos;
    bool enabledText = isEnabledText();
    int lineSpace = getLineSpace();
    ClockLineStyle clockLineStyle = getClockLineStyle();
    if (CLS_SINGLE == clockLineStyle) {
        double dwtWidth = dateStrWidth + weekStrWidth + timeStrWidth
                          + dwtSpace * 2 + lineSpace * 2;
        double dwtHeight = (dateStrHeight + weekStrHeight + timeStrHeight) / 3;
        QPointF clockStartPos;
        if (enabledText) {
            double totleWidth
                = (textStrWidth + tDwtSpace + dwtWidth + lineSpace);
            QPointF textStartPos((regionWidth - totleWidth) / 2,
                                 (regionHeight - textStrHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            clockStartPos.setX(textPaintPos.x() + textStrWidth + tDwtSpace
                               + lineSpace);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        } else {
            clockStartPos.setX((regionWidth - dwtWidth) / 2);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        }
        timePaintPos.setX(clockStartPos.x());
        timePaintPos.setY(clockStartPos.y());
        datePaintPos.setX(timePaintPos.x() + timeStrWidth + dwtSpace
                          + lineSpace);
        datePaintPos.setY(clockStartPos.y());
        weekPaintPos.setX(datePaintPos.x() + dateStrWidth + dwtSpace
                          + lineSpace);
        weekPaintPos.setY(clockStartPos.y());
    } else {
        double dwtHeight = dateStrHeight + weekStrHeight + timeStrHeight
                           + dwtSpace * 2 + lineSpace * 2;
        if (enabledText) {
            double totleHeight = textStrHeight + tDwtSpace + dwtHeight
                                 + lineSpace;
            QPointF textStartPos((regionWidth - textStrWidth) / 2,
                                 (regionHeight - totleHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            timePaintPos.setX((regionWidth - timeStrWidth) / 2);
            timePaintPos.setY(textPaintPos.y() + textStrHeight + tDwtSpace
                              + lineSpace);
        } else {
            timePaintPos.setX((regionWidth - timeStrWidth) / 2);
            timePaintPos.setY((regionHeight - dwtHeight) / 2);
        }
        datePaintPos.setX((regionWidth - dateStrWidth) / 2);
        datePaintPos.setY(timePaintPos.y() + timeStrHeight + dwtSpace
                          + lineSpace);
        weekPaintPos.setX((regionWidth - weekStrWidth) / 2);
        weekPaintPos.setY(datePaintPos.y() + dateStrHeight + dwtSpace
                          + lineSpace);
    }
    if (enabledText) {
        drawText(painter, text, textPaintPos, textFont, getTextPen());
    }
    drawText(painter, dateFormated, datePaintPos, dateFont, getDatePen());
    drawText(painter, weekFormated, weekPaintPos, weekFont, getWeekPen());
    drawText(painter, timeFormated, timePaintPos, timeFont, getTimePen());
}

void ClockView::drawClockTWD(QPainter &painter, const QRectF &region) {
    QDateTime currentDateTime = getCurrentDateTime();
    QString dateFormated = getDateFormatedString(currentDateTime);
    QString weekFormated = getWeekFormatedString(currentDateTime);
    QString timeFormated = getTimeFormatedString(currentDateTime);
    QFont dateFont = getDateFont();
    QFont weekFont = getWeekFont();
    QFont timeFont = getTimeFont();
    QFontMetricsF dateFontMetrics(dateFont);
    QFontMetricsF weekFontMetrics(weekFont);
    QFontMetricsF timeFontMetrics(timeFont);
    QRectF dateBoundRect = dateFontMetrics.boundingRect(dateFormated);
    QRectF weekBoundRect = weekFontMetrics.boundingRect(weekFormated);
    QRectF timeBoundRect = timeFontMetrics.boundingRect(timeFormated);
    double regionWidth = region.width();
    double regionHeight = region.height();
    double dateStrWidth = dateBoundRect.width();
    double dateStrHeight = dateBoundRect.height();
    double weekStrWidth = weekBoundRect.width();
    double weekStrHeight = weekBoundRect.height();
    double timeStrWidth = timeBoundRect.width();
    double timeStrHeight = timeBoundRect.height();
    QPointF datePaintPos;
    QPointF weekPaintPos;
    QPointF timePaintPos;
    QString text = getText();
    QFont textFont = getTextFont();
    QFontMetricsF textFontMetrics(textFont);
    QRectF textBoundRect = textFontMetrics.boundingRect(text);
    double textStrWidth = textBoundRect.width();
    double textStrHeight = textBoundRect.height();
    QPointF textPaintPos;
    bool enabledText = isEnabledText();
    int lineSpace = getLineSpace();
    ClockLineStyle clockLineStyle = getClockLineStyle();
    if (CLS_SINGLE == clockLineStyle) {
        double dwtWidth = dateStrWidth + weekStrWidth + timeStrWidth
                          + dwtSpace * 2 + lineSpace * 2;
        double dwtHeight = (dateStrHeight + weekStrHeight + timeStrHeight) / 3;
        QPointF clockStartPos;
        if (enabledText) {
            double totleWidth
                = (textStrWidth + tDwtSpace + dwtWidth + lineSpace);
            QPointF textStartPos((regionWidth - totleWidth) / 2,
                                 (regionHeight - textStrHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            clockStartPos.setX(textPaintPos.x() + textStrWidth + tDwtSpace
                               + lineSpace);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        } else {
            clockStartPos.setX((regionWidth - dwtWidth) / 2);
            clockStartPos.setY((regionHeight - dwtHeight) / 2);
        }
        timePaintPos.setX(clockStartPos.x());
        timePaintPos.setY(clockStartPos.y());
        weekPaintPos.setX(timePaintPos.x() + timeStrWidth + dwtSpace
                          + lineSpace);
        weekPaintPos.setY(clockStartPos.y());
        datePaintPos.setX(weekPaintPos.x() + weekStrWidth + dwtSpace
                          + lineSpace);
        datePaintPos.setY(clockStartPos.y());
    } else {
        double dwtHeight = dateStrHeight + weekStrHeight + timeStrHeight
                           + dwtSpace * 2 + lineSpace * 2;
        if (enabledText) {
            double totleHeight = textStrHeight + tDwtSpace + dwtHeight
                                 + lineSpace;
            QPointF textStartPos((regionWidth - textStrWidth) / 2,
                                 (regionHeight - totleHeight) / 2);
            textPaintPos.setX(textStartPos.x());
            textPaintPos.setY(textStartPos.y());
            timePaintPos.setX((regionWidth - timeStrWidth) / 2);
            timePaintPos.setY(textPaintPos.y() + textStrHeight + tDwtSpace
                              + lineSpace);
        } else {
            timePaintPos.setX((regionWidth - timeStrWidth) / 2);
            timePaintPos.setY((regionHeight - dwtHeight) / 2);
        }
        weekPaintPos.setX((regionWidth - weekStrWidth) / 2);
        weekPaintPos.setY(timePaintPos.y() + timeStrHeight + dwtSpace
                          + lineSpace);
        datePaintPos.setX((regionWidth - dateStrWidth) / 2);
        datePaintPos.setY(weekPaintPos.y() + weekStrHeight + dwtSpace
                          + lineSpace);
    }
    if (enabledText) {
        drawText(painter, text, textPaintPos, textFont, getTextPen());
    }
    drawText(painter, dateFormated, datePaintPos, dateFont, getDatePen());
    drawText(painter, weekFormated, weekPaintPos, weekFont, getWeekPen());
    drawText(painter, timeFormated, timePaintPos, timeFont, getTimePen());
}

void ClockView::drawText(QPainter &painter,
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
