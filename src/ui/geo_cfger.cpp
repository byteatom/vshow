#include "geo_cfger.h"
#include "ui_geo_cfger.h"

#include <qlog/qlog.h>

GeoCfger::GeoCfger() : ui(new Ui::GeoCfger), attr(nullptr) {
    ui->setupUi(this);

    connect(ui->x,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &GeoCfger::xChanged);
    connect(ui->y,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &GeoCfger::yChanged);
    connect(ui->width,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &GeoCfger::widthChanged);
    connect(ui->height,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &GeoCfger::heightChanged);
}

GeoCfger::~GeoCfger() {
    delete ui;
}

void GeoCfger::show(GeoAttr *attr) {
    this->attr = attr;
    AttrCfger::show();

    ui->x->blockSignals(true);
    ui->y->blockSignals(true);
    ui->width->blockSignals(true);
    ui->height->blockSignals(true);

    ui->x->setValue(attr->x);
    ui->y->setValue(attr->y);
    ui->width->setValue(attr->width);
    ui->height->setValue(attr->height);

    ui->x->setMaximum(attr->horizontalMax);
    ui->y->setMaximum(attr->verticalMax);
    ui->width->setMaximum(attr->horizontalMax);
    ui->height->setMaximum(attr->verticalMax);

    ui->x->blockSignals(false);
    ui->y->blockSignals(false);
    ui->width->blockSignals(false);
    ui->height->blockSignals(false);
}

void GeoCfger::xChanged(int x) {
    attr->x = x;
}

void GeoCfger::yChanged(int y) {
    attr->y = y;
}

void GeoCfger::widthChanged(int width) {
    attr->width = width;
}

void GeoCfger::heightChanged(int height) {
    attr->height = height;
}
