#ifndef TEXTATTR_H
#define TEXTATTR_H

#include "segment_attr.h"
#include "animate_attr.h"

class TextAttr : public SegmentAttr
{
public:
    explicit TextAttr(RegionAttr *owner);
    virtual ~TextAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    std::wstring html;
    bool wrap;
    AnimateAttr animateAttr;
};

#endif // TEXTATTR_H
