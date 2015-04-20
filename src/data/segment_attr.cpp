#include "segment_attr.h"

SegmentAttr::SegmentAttr(RegionAttr *owner) :
    SliceAttr{owner},
    owner{owner}
{
    type = SEGMENT_SLICE;
}

SegmentAttr::~SegmentAttr()
{
}

void SegmentAttr::load(Xml::Element *element)
{
    SliceAttr::load(element);
}

void SegmentAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
    SliceAttr::save(doc, parent);
}
