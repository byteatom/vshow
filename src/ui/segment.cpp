#include "segment.h"

#include "main_ui.h"

Segment::Segment(Region *owner, QTreeWidgetItem *treeItem)
    : Slice{owner, treeItem}, owner{owner} {
    mode = SELF;
}

Segment::~Segment() {
    hideCfgers();
}

void Segment::showCfgers() {
    Slice::showCfgers();
    MainUi::Obj->segmentCfger->show(attr());
}

void Segment::hideCfgers() {
    Slice::hideCfgers();
    MainUi::Obj->segmentCfger->hide();
}

void Segment::raiseView() {
    Slice::raiseView();
    owner->view()->setCurrentWidget(view());
}

SegmentAttr *Segment::attr() {
    return static_cast<SegmentAttr *>(attr_);
}
