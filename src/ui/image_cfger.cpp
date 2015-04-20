#include "image_cfger.h"
#include "ui_image_cfger.h"

#include <cassert>

ImageCfger::ImageCfger() : ui(new Ui::ImageCfger), scale(nullptr) {
    ui->setupUi(this);

    scaleBtns.addButton(ui->strech);
    scaleBtns.addButton(ui->fill);
    scaleBtns.addButton(ui->fit);
    scaleBtns.addButton(ui->tile);
    scaleBtns.addButton(ui->center);
    scaleBtns.setId(ui->strech, ImageTool::STRECH);
    scaleBtns.setId(ui->fill, ImageTool::FILL);
    scaleBtns.setId(ui->fit, ImageTool::FIT);
    scaleBtns.setId(ui->tile, ImageTool::TILE);
    scaleBtns.setId(ui->center, ImageTool::CENTER);

    connect(&scaleBtns,
            static_cast<void (QButtonGroup::*)(int id)>(
                &QButtonGroup::buttonClicked),
            this,
            &ImageCfger::scaleChanged);
}

ImageCfger::~ImageCfger() {
    delete ui;
}

void ImageCfger::show(Attr<ImageTool::Scale> *scale) {
    this->scale = scale;
    AttrCfger::show();

    scaleBtns.blockSignals(true);
    scaleBtns.button(*scale)->setChecked(true);
    scaleBtns.blockSignals(false);
}

void ImageCfger::scaleChanged(int index) {
    *scale = static_cast<ImageTool::Scale>(index);
}
