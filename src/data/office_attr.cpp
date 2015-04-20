#include "office_attr.h"

OfficeAttr::OfficeAttr(RegionAttr *owner) : SegmentAttr(owner) {
    type = OFFICE_SLICE;
    file = L"";
    scale = ImageTool::STRECH;
}

OfficeAttr::~OfficeAttr() {
}

void OfficeAttr::load(Xml::Element *element) {
    SegmentAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getNodeName()};
        if (tag == L"file") {
            file = std::wstring(child->getTextContent());
        } else if (tag == L"scale") {
            scale = static_cast<ImageTool::Scale>(
                std::stoi(child->getTextContent()));
        }
    }
}

void OfficeAttr::save(Xml::Doc *doc, Xml::Node *parent) {
    Xml::Node *self = parent->appendChild(doc->createElement(L"office"));
    SegmentAttr::save(doc, self);
    self->appendChild(doc->createElement(L"file"))
        ->appendChild(doc->createTextNode(std::wstring(file).c_str()));
    self->appendChild(doc->createElement(L"scale"))->appendChild(
        doc->createTextNode(std::to_wstring(scale.value).c_str()));
}
