#include "video_cfger.h"
#include "ui_video_cfger.h"

#include <cassert>

VideoCfger::VideoCfger() : ui(new Ui::VideoCfger), video(nullptr) {
    ui->setupUi(this);

    controlBtns.addButton(ui->play);
    controlBtns.addButton(ui->pause);
    controlBtns.addButton(ui->stop);

    connect(ui->play, &QPushButton::clicked, this, &VideoCfger::play);
    connect(ui->pause, &QPushButton::clicked, this, &VideoCfger::pause);
    connect(ui->stop, &QPushButton::clicked, this, &VideoCfger::stop);
}

VideoCfger::~VideoCfger() {
    delete ui;
}

void VideoCfger::show(Video *video) {
    this->video = video;
    AttrCfger::show();

    ui->play->blockSignals(true);
    ui->pause->blockSignals(true);
    ui->stop->blockSignals(true);

    if (Vlc::Playing == video->videoState()) {
        ui->play->setChecked(true);
    } else if (Vlc::Paused == video->videoState()) {
        ui->pause->setChecked(true);
    } else {
        ui->stop->setChecked(true);
    }

    ui->play->blockSignals(false);
    ui->pause->blockSignals(false);
    ui->stop->blockSignals(false);
}



void VideoCfger::play() {
    assert(video);
    video->videoPlay();
}

void VideoCfger::pause() {
    assert(video);
    video->videoPause();
}

void VideoCfger::stop() {
    assert(video);
    video->videoStop();
}
