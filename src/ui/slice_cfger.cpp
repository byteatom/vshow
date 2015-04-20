#include "slice_cfger.h"
#include "ui_slice_cfger.h"

#include <cassert>

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>

#include "picture.h"

SliceCfger::SliceCfger() : ui(new Ui::SliceCfger), attr(nullptr) {
    ui->setupUi(this);

    connect(ui->name, &QLineEdit::textChanged, this, &SliceCfger::nameChanged);
    connect(ui->colorSwitch,
            &QCheckBox::stateChanged,
            this,
            &SliceCfger::colorSwitch);
    connect(
        ui->colorSelect, &QPushButton::clicked, this, &SliceCfger::colorSelect);
}

SliceCfger::~SliceCfger() {
    delete ui;
}

void SliceCfger::show(SliceAttr *attr) {
    this->attr = attr;
    AttrCfger::show();

    ui->name->blockSignals(true);
    ui->colorSwitch->blockSignals(true);

    ui->name->setText(QString::fromStdWString(attr->name));
    ui->colorSwitch->setCheckState(ImageAttr::BG_COLOR_NONE == attr->bgColor
                                       ? Qt::Unchecked
                                       : Qt::Checked);

    ui->name->blockSignals(false);
    ui->colorSwitch->blockSignals(false);
}

void SliceCfger::nameChanged(const QString &name) {
    attr->name = name.toStdWString();
}

void SliceCfger::colorSwitch(int state) {
    if (Qt::Checked == state)
        attr->bgColor = ImageAttr::BG_COLOR_DEFAULT;
    else if (Qt::Unchecked == state)
        attr->bgColor = ImageAttr::BG_COLOR_NONE;
}

void SliceCfger::colorSelect() {
    QColor color = QColorDialog::getColor(QColor::fromRgba(attr->bgColor),
                                          this,
                                          tr("Select Color"),
                                          QColorDialog::ShowAlphaChannel);
    if (!color.isValid()) {
        return;
    }

    attr->bgColor = color.rgba();
    ui->colorSwitch->setCheckState(Qt::Checked);
}
