#ifndef TIMER_CFGER_H
#define TIMER_CFGER_H

#include <QFrame>
#include <QButtonGroup>
#include "attr_cfger.h"
#include "timer.h"

namespace Ui {
class TimerCfger;
}

class Timer;

class TimerCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit TimerCfger();
    virtual ~TimerCfger();
    void show(Timer *timer);
    void hide();
private slots:
    void upTimer(bool checked);
    void downTimer(bool checked);
    void targetDate(const QDate &date);
    void targetTime(const QTime &time);
    void day(bool checked);
    void hour(bool checked);
    void minute(bool checked);
    void second(bool checked);
    void timeFont(const QFont &f);
    void timeFontSize(int);
    void timeNumColor(bool checked = false);
    void timeUnitColor(bool checked = false);
    void enabledText(bool checked);
    void newline(bool checked);
    void lineText(const QString &);
    void textFont(const QFont &f);
    void textFontSize(int);
    void textColor(bool checked = false);
    void bold(bool checked = false);
    void italic(bool checked = false);
    void underLine(bool checked = false);

private:
    Timer *timer;
    QButtonGroup timerTypeBtns;
    Ui::TimerCfger *ui;
};

#endif    // TIMER_CFGER_H
