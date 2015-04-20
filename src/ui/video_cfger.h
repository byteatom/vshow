#ifndef VIDEOCFGER_H
#define VIDEOCFGER_H

#include <QButtonGroup>

#include "attr_cfger.h"
#include "video.h"

namespace Ui {
class VideoCfger;
}

class VideoCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit VideoCfger();
    ~VideoCfger();

    void show(Video *video);


private:
    void play();
    void pause();
    void stop();

    Video *video;
    QButtonGroup controlBtns;
    Ui::VideoCfger *ui;
};

#endif    // VIDEOCFGER_H
