#include "program_attr.h"

#include "region_attr.h"

ProgramAttr::ProgramAttr(ScreenAttr *owner) :
    SliceAttr{owner},
    owner{owner}
{
    type = PROGRAM_SLICE;
}

ProgramAttr::~ProgramAttr()
{
}

void ProgramAttr::load(Xml::Element *element)
{
    SliceAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getTagName()};
        if (tag == L"region") {
            RegionAttr *attr = new RegionAttr(this);
            attr->load(child);
        }
    }
}

void ProgramAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
    Xml::Node *self = parent->appendChild(doc->createElement(L"program"));
    SliceAttr::save(doc, self);
    SliceAttr::saveOrgans(doc, self);
}
