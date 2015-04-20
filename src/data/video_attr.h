#ifndef VIDEOATTR_H
#define VIDEOATTR_H

#include "segment_attr.h"
#include "attr.h"

class VideoAttr : public SegmentAttr
{
public:
    explicit VideoAttr(RegionAttr *owner);
    virtual ~VideoAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    Attr<std::wstring> file;
};

#endif // VIDEOATTR_H
