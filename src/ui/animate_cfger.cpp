#include "animate_cfger.h"
#include "ui_animate_cfger.h"
#include "animator.h"


AnimateCfger::AnimateCfger() : ui(new Ui::AnimateCfger), attr(nullptr) {
    ui->setupUi(this);

    for (int i = 0; i < AnimateAttr::STYLE_MAX; i++) {
        ui->style->addItem(Animator::actions[i].text);
    }
    ui->speed->setMaximum(AnimateAttr::MAX_SPEED);
    ui->stay->setMaximum(AnimateAttr::MAX_STAY);

    connect(
        ui->style,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this,
        &AnimateCfger::styleChanged);
    connect(ui->speed,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &AnimateCfger::speedChanged);
    connect(ui->stay,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &AnimateCfger::stayChanged);
}

AnimateCfger::~AnimateCfger() {
    delete ui;
}

void AnimateCfger::show(AnimateAttr *attr) {
    this->attr = attr;
    ui->style->setCurrentIndex(attr->style);
    ui->speed->setValue(attr->getSpeed());
    ui->stay->setValue(attr->stay);
    AttrCfger::show();
}

void AnimateCfger::styleChanged(int style) {
    attr->style = AnimateAttr::Style(style);
}

void AnimateCfger::speedChanged(int speed) {
    attr->setSpeed(speed);
}

void AnimateCfger::stayChanged(int stay) {
    attr->stay = stay;
}
