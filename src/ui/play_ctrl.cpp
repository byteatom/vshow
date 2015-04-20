#include "play_ctrl.h"
#include "ui_play_ctrl.h"
#include "main_ui.h"

PlayCtrl::PlayCtrl(QWidget *parent) : QDialog(parent), ui(new Ui::PlayCtrl) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    // setAttribute(Qt::WA_TranslucentBackground, true);
    // setWindowOpacity(0.5);
    connect(ui->prev, SIGNAL(clicked(bool)), this, SLOT(prev(bool)));
    connect(ui->stop, SIGNAL(clicked(bool)), this, SLOT(stop(bool)));
    connect(ui->play, SIGNAL(clicked(bool)), this, SLOT(play(bool)));
    connect(ui->next, SIGNAL(clicked(bool)), this, SLOT(next(bool)));
    connect(ui->silence, SIGNAL(clicked(bool)), this, SLOT(silence(bool)));
    connect(ui->voice, SIGNAL(valueChanged(int)), this, SLOT(voice(int)));
}

PlayCtrl::~PlayCtrl() {
    delete ui;
}

void PlayCtrl::leaveEvent(QEvent *) {
    hide();
    static_cast<ScreenView *>(this->parent())->screen->programList->hide();
}

void PlayCtrl::prev(bool) {
}

void PlayCtrl::stop(bool) {

    Screen *screen = static_cast<ScreenView *>(this->parent())->screen;
    if (!screen->playing) {
        return;
    }
    screen->owner->stopScreen(screen);
}

void PlayCtrl::play(bool) {
    Screen *screen = static_cast<ScreenView *>(this->parent())->screen;
    if (screen->playing) {
        return;
    }
    screen->owner->startScreen(screen);
}

void PlayCtrl::next(bool) {
}

void PlayCtrl::silence(bool) {
}

void PlayCtrl::voice(int) {
}
