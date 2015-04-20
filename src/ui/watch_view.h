#ifndef WATCHVIEW_H
#define WATCHVIEW_H

#include "mouse_select.h"
#include "data/watch_attr.h"

class Watch;

class WatchView : public QWidget {
public:
    static const QString digital[12];
    static const double maxRegulateDistance;
    static const double lineScaleWHRatio;
    static const double hourHandRatio;
    static const double minuteHandRatio;
    static const double secondHandRatio;
    static const qreal windmillRadians;
    explicit WatchView(Watch *watch);
    virtual ~WatchView();
    QPixmap *drawWatch();

protected:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    DialStyle getWatchDialStyle();
    bool isEnabledTsnDotNumber();
    DialNumStyle getTsnDotNumberStyle();
    QRgb getTsnDotNumberColor();
    QString getTsnDotNumberFamily();
    int getTsnDotNumberSize();
    bool isEnabledHourDotNumber();
    DialNumStyle getHourDotNumberStyle();
    QRgb getHourDotNumberColor();
    QString getHourDotNumberFamily();
    int getHourDotNumberSize();
    DialPointStyle getTsnDotStyle();
    QRgb getTsnDotColor();
    int getTsnDotSize();
    DialPointStyle getHourDotStyle();
    QRgb getHourDotColor();
    int getHourDotSize();
    DialPointStyle getMinuteDotStyle();
    QRgb getMinuteDotColor();
    int getMinuteDotSize();
    DialHandStyle getHourHandStyle();
    QRgb getHourHandColor();
    int getHourHandSize();
    DialHandStyle getMinuteHandStyle();
    QRgb getMinuteHandColor();
    int getMinuteHandSize();
    DialHandStyle getSecondHandStyle();
    QRgb getSecondHandColor();
    int getSecondHandSize();
    DialDeltType getDeltType();
    int getDeltHour();
    int getDeltMinute();
    int getDeltSecond();
    void drawWatch(QPainter &painter, const QRectF &region);
    void drawCircleWatch(QPainter &painter, const QRectF &region);
    void drawRoundRectWatch(QPainter &painter, const QRectF &region);
    void drawEllipseWatch(QPainter &painter, const QRectF &region);
    void drawRhombusWatch(QPainter &painter, const QRectF &region);
    void drawWindmillWatch(QPainter &painter, const QRectF &region);
    void drawCircleWatchDial(QPainter &painter,
                             const QRectF &region,
                             double maxRegulateOffset);
    void drawRoundRectWatchDial(QPainter &painter,
                                const QRectF &region,
                                double maxRegulateOffset);
    void drawEllipseWatchDial(QPainter &painter,
                              const QRectF &region,
                              double maxRegulateOffset);
    void drawRhombusWatchDial(QPainter &painter,
                              const QRectF &region,
                              double maxRegulateOffset);
    void drawWindmillWatchDial(QPainter &painter,
                               const QRectF &region,
                               double maxRegulateOffset);
    void drawTsnScale(QPainter &painter, double regulateRedius);
    void drawHourScale(QPainter &painter, double regulateRedius);
    void drawMinuteScale(QPainter &painter, double regulateRedius);
    void drawScale(QPainter &painter,
                   DialPointStyle scaleStyle,
                   const QColor &scaleColor,
                   double radius,
                   double scaleRadius);
    void drawCircleScale(QPainter &painter,
                         const QColor &scaleColor,
                         double radius,
                         double scaleRadius);
    void drawSquareScale(QPainter &painter,
                         const QColor &scaleColor,
                         double radius,
                         double scaleRadius);
    void drawLineScale(QPainter &painter,
                       const QColor &scaleColor,
                       double radius,
                       double scaleRadius);
    void drawCircleWatchDigitals(QPainter &painter,
                                 const QRectF &region,
                                 double maxRegulateOffset);
    void drawRoundRectWatchDigitals(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset);
    void drawEllipseWatchDigitals(QPainter &painter,
                                  const QRectF &region,
                                  double maxRegulateOffset);
    void drawRhombusWatchDigitals(QPainter &painter,
                                  const QRectF &region,
                                  double maxRegulateOffset);
    void drawWindmillWatchDigitals(QPainter &painter,
                                   const QRectF &region,
                                   double maxRegulateOffset);
    QPointF getDigitalRegulatedPaintPoint(QPointF original,
                                          int i,
                                          double digitalStringWidth,
                                          double digitalStringHeight,
                                          double Pos3Regulate = 0,
                                          double Pos6Regulate = 0,
                                          double Pos9Regulate = 0,
                                          double Pos12Regulate = 0);
    void drawWatchHand(QPainter &painter,
                       const QRectF &region,
                       double maxRegulateOffset);
    void drawWatchHourHand(QPainter &painter,
                           const QRectF &region,
                           double maxRegulateOffset);
    void drawCircleWatchHourHand(QPainter &painter,
                                 const QRectF &region,
                                 double maxRegulateOffset);
    void drawRoundRectWatchHourHand(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset);
    void drawEllipseWatchHourHand(QPainter &painter,
                                  const QRectF &region,
                                  double maxRegulateOffset);
    void drawRhombusWatchHourHand(QPainter &painter,
                                  const QRectF &region,
                                  double maxRegulateOffset);
    void drawWindmillWatchHourHand(QPainter &painter,
                                   const QRectF &region,
                                   double maxRegulateOffset);
    void drawWatchMinuteHand(QPainter &painter,
                             const QRectF &region,
                             double maxRegulateOffset);
    void drawCircleWatchMinuteHand(QPainter &painter,
                                   const QRectF &region,
                                   double maxRegulateOffset);
    void drawRoundRectWatchMinuteHand(QPainter &painter,
                                      const QRectF &region,
                                      double maxRegulateOffset);
    void drawEllipseWatchMinuteHand(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset);
    void drawRhombusWatchMinuteHand(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset);
    void drawWindmillWatchMinuteHand(QPainter &painter,
                                     const QRectF &region,
                                     double maxRegulateOffset);
    void drawWatchSecondHand(QPainter &painter,
                             const QRectF &region,
                             double maxRegulateOffset);
    void drawCircleWatchSecondHand(QPainter &painter,
                                   const QRectF &region,
                                   double maxRegulateOffset);
    void drawRoundRectWatchSecondHand(QPainter &painter,
                                      const QRectF &region,
                                      double maxRegulateOffset);
    void drawEllipseWatchSecondHand(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset);
    void drawRhombusWatchSecondHand(QPainter &painter,
                                    const QRectF &region,
                                    double maxRegulateOffset);
    void drawWindmillWatchSecondHand(QPainter &painter,
                                     const QRectF &region,
                                     double maxRegulateOffset);
    void drawLineHand(QPainter &painter,
                      QRgb lineHandColor,
                      double lineHandLength,
                      double lineHandWidth);
    void drawRhombusHand(QPainter &painter,
                         QRgb rhombusHandColor,
                         double RhombusHandLength,
                         double RhombusHandWidth);
    double regulateEllipseRedius(const QRectF &region,
                                 double maxRegulateOffset,
                                 double degrees);
    double regulateRhombusRedius(const QRectF &region,
                                 double maxRegulateOffset,
                                 double degrees);
    double regulateWindmillRedius(const QRectF &region,
                                  double maxRegulateOffset,
                                  double degrees);
    QTime getRegulatedTime();
    double getMaxScaleRedius();
    bool isEnabledText();
    QString getText();
    QPointF getPaintPos();
    QFont getTextFont();
    QColor getTextColor();
    QPen getTextPen();
    void drawText(QPainter &painter, const QRectF &region);
    void drawText(QPainter &painter,
                  const QRectF &region,
                  const QString &text,
                  const QPointF &paintPos,
                  const QFont &font,
                  const QPen &pen);

    Watch *watch;
    MouseSelect mouseSelect;
};

#endif    // WATCHVIEW_H
