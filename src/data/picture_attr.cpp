#include "picture_attr.h"


PictureAttr::PictureAttr(RegionAttr *owner) :
    SegmentAttr(owner)
{
    type = PICTURE_SLICE;
    scale = ImageTool::STRECH;
}

PictureAttr::~PictureAttr()
{
}

void PictureAttr::load(Xml::Element *element)
{
    SegmentAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getTagName()};
        if (tag == L"file") {
            file = std::wstring(child->getTextContent());
        } else if (tag == L"scale") {
            scale = static_cast<ImageTool::Scale>(std::stoi(child->getTextContent()));
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

void PictureAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
    Xml::Node *self = parent->appendChild(doc->createElement(L"picture"));
    SegmentAttr::save(doc, self);
    self->appendChild(doc->createElement(L"file"))
            ->appendChild(doc->createTextNode(file.value.c_str()));
    self->appendChild(doc->createElement(L"scale"))
            ->appendChild(doc->createTextNode(std::to_wstring(scale.value).c_str()));
    self->appendChild(doc->createElement(L"style"))
            ->appendChild(doc->createTextNode(std::to_wstring(animateAttr.style).c_str()));
    self->appendChild(doc->createElement(L"pulses"))
            ->appendChild(doc->createTextNode(std::to_wstring(animateAttr.pulses).c_str()));
    self->appendChild(doc->createElement(L"step"))
            ->appendChild(doc->createTextNode(std::to_wstring(animateAttr.step).c_str()));
    self->appendChild(doc->createElement(L"stay"))
            ->appendChild(doc->createTextNode(std::to_wstring(animateAttr.stay).c_str()));
}
