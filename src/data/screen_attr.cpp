#include "screen_attr.h"

#include "program_attr.h"

ScreenAttr::ScreenAttr(ProjectAttr *owner) :
    SliceAttr{owner},
    owner{owner}
{
    type = SCREEN_SLICE;
    bgColor = ImageAttr::BG_COLOR_DEFAULT;

    geo.x = 0;
    geo.y = 60;
    geo.width = 640;
    geo.height = 480;
}

ScreenAttr::~ScreenAttr()
{
}

void ScreenAttr::load(Xml::Element *element)
{
    SliceAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getTagName()};
        if (tag == L"x") {
            geo.x = std::stoi(child->getTextContent());
        } else if (tag == L"y") {
            geo.y = std::stoi(child->getTextContent());
        } else if (tag == L"width") {
            geo.width = std::stoi(child->getTextContent());
        } else if (tag == L"height") {
            geo.height = std::stoi(child->getTextContent());
        } else if (tag == L"lock") {
            lock = static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"top") {
            top =  static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"program") {
            ProgramAttr *attr = new ProgramAttr(this);
            attr->load(child);
        }
    }
}

void ScreenAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
    Xml::Node *self = parent->appendChild(doc->createElement(L"screen"));
    SliceAttr::save(doc, self);
    self->appendChild(doc->createElement(L"x"))
            ->appendChild(doc->createTextNode(std::to_wstring(geo.x).c_str()));
    self->appendChild(doc->createElement(L"y"))
            ->appendChild(doc->createTextNode(std::to_wstring(geo.y).c_str()));
    self->appendChild(doc->createElement(L"width"))
            ->appendChild(doc->createTextNode(std::to_wstring(geo.width).c_str()));
    self->appendChild(doc->createElement(L"height"))
            ->appendChild(doc->createTextNode(std::to_wstring(geo.height).c_str()));
    self->appendChild(doc->createElement(L"lock"))
            ->appendChild(doc->createTextNode(std::to_wstring(lock).c_str()));
    self->appendChild(doc->createElement(L"top"))
            ->appendChild(doc->createTextNode(std::to_wstring(top).c_str()));
    SliceAttr::saveOrgans(doc, self);
}
