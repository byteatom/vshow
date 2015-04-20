#include "program_cfger.h"
#include "ui_program_cfger.h"

ProgramCfger::ProgramCfger() : ui(new Ui::ProgramCfger), attr(nullptr) {
    ui->setupUi(this);
}

ProgramCfger::~ProgramCfger() {
    delete ui;
}

void ProgramCfger::show(ProgramAttr *attr) {
    this->attr = attr;
    AttrCfger::show();
}
