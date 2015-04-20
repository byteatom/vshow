#include "text_attr.h"

TextAttr::TextAttr(RegionAttr *owner) :
    SegmentAttr(owner),
    wrap(true)
{
    type = TEXT_SLICE;
}

TextAttr::~TextAttr()
{
}

void TextAttr::load(Xml::Element *element)
{
    SegmentAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {

        std::wstring tag{child->getTagName()};
        if (tag == L"html") {
            html = std::wstring(child->getTextContent());
        } else if (tag == L"wrap") {
            wrap =  static_cast<bool>(std::stoi(child->getTextContent()));
        } else if (tag == L"style") {
            animateAttr.style = static_cast<AnimateAttr::Style>(std::stoi(child->getTextContent()));
        } else if (tag == L"pulses") {
            animateAttr.pulses = std::stoi(child->getTextContent());
        } else if (tag == L"step") {
            animateAttr.step = std::stoi(child->getTextContent());
        } else if (tag == L"stay") {
            animateAttr.stay = std::stoi(child->getTextContent());
        }
    }
}

void TextAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
    Xml::Node *self = parent->appendChild(doc->createElement(L"text"));
    SegmentAttr::save(doc, self);
    self->appendChild(doc->createElement(L"html"))
            ->appendChild(doc->createCDATASection(html.c_str()));
    self->appendChild(doc->createElement(L"wrap"))
            ->appendChild(doc->createTextNode(std::to_wstring(wrap).c_str()));
    self->appendChild(doc->createElement(L"style"))
            ->appendChild(doc->createTextNode(std::to_wstring(animateAttr.style).c_str()));
    self->appendChild(doc->createElement(L"pulses"))
            ->appendChild(doc->createTextNode(std::to_wstring(animateAttr.pulses).c_str()));
    self->appendChild(doc->createElement(L"step"))
            ->appendChild(doc->createTextNode(std::to_wstring(animateAttr.step).c_str()));
    self->appendChild(doc->createElement(L"stay"))
            ->appendChild(doc->createTextNode(std::to_wstring(animateAttr.stay).c_str()));
}
