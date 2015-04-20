#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include "mouse_select.h"
#include "data/clock_attr.h"

class Clock;

class ClockView : public QWidget {
public:
    static const double tDwtSpace;
    static const double dwtSpace;
    explicit ClockView(Clock *clock);
    virtual ~ClockView();
    QPixmap *drawClock();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    ClockLineStyle getClockLineStyle();
    QDateTime getCurrentDateTime();
    QString getText();
    QString getDateFormatedString(QDateTime currentDateTime);
    QString getWeekFormatedString(QDateTime currentDateTime);
    QString getTimeFormatedString(QDateTime currentDateTime);
    QFont getTextFont();
    QFont getDateFont();
    QFont getWeekFont();
    QFont getTimeFont();
    QColor getTextColor();
    QColor getDateColor();
    QColor getWeekColor();
    QColor getTimeColor();
    QPen getTextPen();
    QPen getDatePen();
    QPen getWeekPen();
    QPen getTimePen();
    bool isEnabledText();
    int getLineSpace();
    void drawClock(QPainter &painter, const QRectF &region);
    void drawClockDWT(QPainter &painter, const QRectF &region);
    void drawClockDTW(QPainter &painter, const QRectF &region);
    void drawClockWDT(QPainter &painter, const QRectF &region);
    void drawClockWTD(QPainter &painter, const QRectF &region);
    void drawClockTDW(QPainter &painter, const QRectF &region);
    void drawClockTWD(QPainter &painter, const QRectF &region);
    void drawText(QPainter &painter,
                  const QString &text,
                  const QPointF &paintPos,
                  const QFont &font,
                  const QPen &pen);

    Clock *clock;
    MouseSelect mouseSelect;
};

#endif    // CLOCKVIEW_H
