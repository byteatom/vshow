#ifndef SEGMENTATTR_H
#define SEGMENTATTR_H

#include "slice_attr.h"

#include <string>

#include "region_attr.h"

class SegmentAttr : public SliceAttr
{
public:
    explicit SegmentAttr(RegionAttr *owner);
    virtual ~SegmentAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    RegionAttr *owner;
};

#endif // SEGMENTATTR_H
