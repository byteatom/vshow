#include "segment_cfger.h"
#include "ui_segment_cfger.h"

SegmentCfger::SegmentCfger() : ui(new Ui::SegmentCfger), attr(nullptr) {
    ui->setupUi(this);
}

SegmentCfger::~SegmentCfger() {
    delete ui;
}

void SegmentCfger::show(SegmentAttr *attr) {
    this->attr = attr;
    AttrCfger::show();
}
