#ifndef PROGRAMATTR_H
#define PROGRAMATTR_H

#include "slice_attr.h"

#include "screen_attr.h"

class ProgramAttr : public SliceAttr
{
public:
    explicit ProgramAttr(ScreenAttr *owner);
    virtual ~ProgramAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    ScreenAttr *owner;
};

#endif // PROGRAMATTR_H
