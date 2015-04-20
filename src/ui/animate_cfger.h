#ifndef ANIMATECFGER_H
#define ANIMATECFGER_H

#include "attr_cfger.h"
#include "data/animate_attr.h"

namespace Ui {
class AnimateCfger;
}

class AnimateCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit AnimateCfger();
    ~AnimateCfger();

    void show(AnimateAttr *attr);

private:
    void styleChanged(int style);
    void speedChanged(int speed);
    void stayChanged(int stay);

    AnimateAttr *attr;
    Ui::AnimateCfger *ui;
};

#endif    // ANIMATECFGER_H
