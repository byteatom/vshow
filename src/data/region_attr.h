#ifndef REGIONATTR_H
#define REGIONATTR_H

#include "slice_attr.h"

#include "program_attr.h"
#include "geo_attr.h"

class RegionAttr : public SliceAttr
{
public:
    explicit RegionAttr(ProgramAttr *owner);
    virtual ~RegionAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    ProgramAttr *owner;
    GeoAttr geo;
};

#endif // REGIONATTR_H
