#include "screen_cfger.h"
#include "ui_screen_cfger.h"

#include <QApplication>
#include <QDesktopWidget>

ScreenCfger::ScreenCfger() : ui(new Ui::ScreenCfger), attr(nullptr) {
    ui->setupUi(this);

    QRect rect = QApplication::desktop()->screenGeometry();

    connect(ui->lock, &QCheckBox::toggled, this, &ScreenCfger::lockChanged);
    connect(ui->top, &QCheckBox::toggled, this, &ScreenCfger::topChanged);
}

ScreenCfger::~ScreenCfger() {
    delete ui;
}

void ScreenCfger::show(ScreenAttr *attr) {
    this->attr = attr;
    AttrCfger::show();

    ui->lock->blockSignals(true);
    ui->top->blockSignals(true);

    ui->lock->setChecked(attr->lock);
    ui->top->setChecked(attr->top);

    ui->lock->blockSignals(false);
    ui->top->blockSignals(false);
}

void ScreenCfger::lockChanged(bool checked) {
    attr->lock = checked;
}

void ScreenCfger::topChanged(bool checked) {
    attr->top = checked;
}
