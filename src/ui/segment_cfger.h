#ifndef SEGMENTCFGER_H
#define SEGMENTCFGER_H

#include "attr_cfger.h"
#include "data/segment_attr.h"

namespace Ui {
class SegmentCfger;
}

class SegmentCfger : public AttrCfger {
    Q_OBJECT

public:
    explicit SegmentCfger();
    virtual ~SegmentCfger();

    void show(SegmentAttr *attr);

private:
    SegmentAttr *attr;
    Ui::SegmentCfger *ui;
};

#endif    // SEGMENTCFGER_H
