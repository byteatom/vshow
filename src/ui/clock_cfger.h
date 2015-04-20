#ifndef CLOCK_CFGER_H
#define CLOCK_CFGER_H

#include <QFrame>
#include <QButtonGroup>
#include "attr_cfger.h"
#include "clock.h"

namespace Ui {
class ClockCfger;
}

class Clock;

class ClockCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit ClockCfger();
    ~ClockCfger();
    void show(Clock *clock);
    void hide();
private slots:
    void wrapChanged(bool checked);
    void displayFormat(const QString &);
    void lineSpace(int);
    void dateFormat(const QString &);
    void dateColor(bool checked = false);
    void dateFont(const QFont &);
    void dateFontSize(int);

    void weekFormat(const QString &);
    void weekColor(bool checked = false);
    void weekFont(const QFont &f);
    void weekFontSize(int);

    void timeFormat(const QString &);
    void timeColor(bool checked = false);
    void timeFont(const QFont &f);
    void timeFontSize(int);

    void dateUnitFormat(const QString &);
    void timeUnitFormat(const QString &);

    void deltType(const QString &);
    void deltTime(const QTime &time);

    void enableText(bool checked);
    void lineEdit(const QString &);
    void textFont(const QFont &f);
    void textFontSize(int);
    void textColor(bool checked);
    void bold(bool checked);
    void italic(bool checked);
    void underLine(bool checked);

private:
    Clock *clock;
    Ui::ClockCfger *ui;
};

#endif    // CLOCK_CFGER_H
