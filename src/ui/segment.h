#ifndef SEGMENT_H
#define SEGMENT_H

#include "region.h"
#include "data/segment_attr.h"

class Segment : public Slice {
public:
    typedef Region OwnerType;

    Segment(Region *owner, QTreeWidgetItem *treeItem);
    virtual ~Segment();

    virtual void showCfgers() override;
    virtual void hideCfgers() override;
    virtual void raiseView() override;

    Region *owner;
    SegmentAttr *attr();
};


#endif    // SEGMENT_H
