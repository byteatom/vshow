#ifndef PICTUREATTR_H
#define PICTUREATTR_H

#include "segment_attr.h"
#include "attr.h"
#include "image_attr.h"
#include "animate_attr.h"

class PictureAttr : public SegmentAttr
{
public:
    explicit PictureAttr(RegionAttr *owner);
    virtual ~PictureAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    Attr<std::wstring> file;
    Attr<ImageTool::Scale> scale;
    AnimateAttr animateAttr;
};

#endif // PICTUREATTR_H
