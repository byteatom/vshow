#ifndef TIMERVIEW_H
#define TIMERVIEW_H

#include "mouse_select.h"

class Timer;

class TimerView : public QWidget {
    Q_DECLARE_TR_FUNCTIONS(TimerView)
public:
    static const QString timerUint[3][4];
    static const double numUnitSpace;
    static const double textTimeSpace;
    explicit TimerView(Timer *timer);
    virtual ~TimerView();
    QPixmap *drawTimer();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void drawTimer(QPainter &painter, const QRectF &region);
    void drawTimer_DHMS(QPainter &painter, const QRectF &region);
    void drawTimer_DHM(QPainter &painter, const QRectF &region);
    void drawTimer_DHS(QPainter &painter, const QRectF &region);
    void drawTimer_DH(QPainter &painter, const QRectF &region);
    void drawTimer_DMS(QPainter &painter, const QRectF &region);
    void drawTimer_DM(QPainter &painter, const QRectF &region);
    void drawTimer_DS(QPainter &painter, const QRectF &region);
    void drawTimer_D(QPainter &painter, const QRectF &region);
    void drawTimer_HMS(QPainter &painter, const QRectF &region);
    void drawTimer_HM(QPainter &painter, const QRectF &region);
    void drawTimer_HS(QPainter &painter, const QRectF &region);
    void drawTimer_H(QPainter &painter, const QRectF &region);
    void drawTimer_MS(QPainter &painter, const QRectF &region);
    void drawTimer_M(QPainter &painter, const QRectF &region);
    void drawTimer_S(QPainter &painter, const QRectF &region);
    void drawTimer_None(QPainter &painter, const QRectF &region);
    QDateTime getTargetDateTime();
    bool isDisplayDay();
    bool isDisplayHour();
    bool isDisplayMinute();
    bool isDisplaySecond();
    qint64 getDays(const QDateTime &targetDateTime,
                   const QDateTime &currentDateTime);
    qint64 getHours(const QDateTime &targetDateTime,
                    const QDateTime &currentDateTime);
    qint64 getMinutes(const QDateTime &targetDateTime,
                      const QDateTime &currentDateTime);
    qint64 getSeconds(const QDateTime &targetDateTime,
                      const QDateTime &currentDateTime);
    QString getDaysUnitStr();
    QString getHoursUnitStr();
    QString getMinutesUnitStr();
    QString getSecondsUnitStr();
    QFont getTimeFont();
    QColor getTimeNumColor();
    QColor getTimeUnitColor();
    bool isEnabledText();
    bool isNewline();
    QString getText();
    QFont getTextFont();
    QColor getTextColor();
    QString getDaysStr(qint64 days);
    QString getHMSStr(qint64 value);
    void drawText(QPainter &painter,
                  const QPointF &posation,
                  const QString &text,
                  const QFont &font,
                  const QColor &color);

    Timer *timer;
    MouseSelect mouseSelect;
};

#endif    // TIMERVIEW_H
