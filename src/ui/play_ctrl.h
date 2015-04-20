#ifndef PLAYCTRL_H
#define PLAYCTRL_H

#include <QDialog>

namespace Ui {
class PlayCtrl;
}

class PlayCtrl : public QDialog {
    Q_OBJECT

public:
    explicit PlayCtrl(QWidget *parent);
    ~PlayCtrl();

protected:
    virtual void leaveEvent(QEvent *) override;

private slots:
    void prev(bool);
    void stop(bool);
    void play(bool);
    void next(bool);
    void silence(bool);
    void voice(int);

private:
    Ui::PlayCtrl *ui;
};

#endif    // PLAYCTRL_H
