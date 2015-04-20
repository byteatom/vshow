#ifndef SCREENATTR_H
#define SCREENATTR_H

#include "slice_attr.h"

#include "project_attr.h"
#include "attr.h"
#include "geo_attr.h"

class ScreenAttr : public SliceAttr
{
public:
    explicit ScreenAttr(ProjectAttr *owner);
    virtual ~ScreenAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    ProjectAttr *owner;
    GeoAttr geo;
    Attr<bool> lock;
    Attr<bool> top;
};

#endif // SCREENATTR_H
