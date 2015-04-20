#include "animate_attr.h"

AnimateAttr::AnimateAttr() {
    style = MOVE_LEFT;
    pulses = 1;
    step = 1;
    stay = 1;
}

AnimateAttr::~AnimateAttr() {
}

void AnimateAttr::setSpeed(unsigned int speed) {
    if (speed < BASE_SPEED) {
        pulses = BASE_SPEED - speed + 1;
        step = 1;
    } else if (speed > BASE_SPEED) {
        pulses = 1;
        step = speed - BASE_SPEED + 1;
    } else {
        pulses = 1;
        step = 1;
    }
}

unsigned int AnimateAttr::getSpeed() {
    if (pulses != 1) {
        return BASE_SPEED - pulses + 1;
    } else if (step != 1) {
        return BASE_SPEED + step - 1;
    } else {
        return BASE_SPEED;
    }
}
