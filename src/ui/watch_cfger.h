#ifndef WATCH_CFGER_H
#define WATCH_CFGER_H

#include <QFrame>
#include "attr_cfger.h"
#include "watch.h"

namespace Ui {
class WatchCfger;
}

class Watch;

class WatchCfger : public AttrCfger {
    Q_OBJECT

public:
    static const double step;
    explicit WatchCfger();
    ~WatchCfger();
    void show(Watch *watch);
    void hide();

private slots:
    void dialStyle(int index);

    void hourNumStyle(const QString &);
    void hourNumColor(bool checked);
    void hourNumFont(const QFont &f);
    void hourNumFontSize(int);
    void hourNumDisplay(bool checked);

    void tsnNumStyle(const QString &);
    void tsnNumColor(bool checked);
    void tsnNumFont(const QFont &f);
    void tsnNumFontSize(int);
    void tsnNumDisplay(bool checked);

    void hourPointSize(int);
    void hourPointColor(bool checked);
    void hourPointStyle(const QString &);
    void tsnPointSize(int);
    void tsnPointColor(bool checked);
    void tsnPointStyle(const QString &);
    void minutePointSize(int);
    void minutePointColor(bool checked);
    void minutePointStyle(const QString &);

    void hourHandSize(int);
    void hourHandColor(bool checked);
    void hourHandStyle(const QString &);
    void minuteHandSize(int);
    void minuteHandColor(bool checked);
    void minuteHandStyle(const QString &);
    void secondHandSize(int);
    void secondHandColor(bool checked);
    void secondHandStyle(const QString &);

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
    void left(bool checked);
    void up(bool checked);
    void center(bool checked);
    void down(bool checked);
    void right(bool checked);

private:
    bool isEnabledText();
    QString getText();
    QFont getTextFont();

private:
    Watch *watch;
    Ui::WatchCfger *ui;
};

#endif    // WATCH_CFGER_H
