#include "slice_attr.h"

SliceAttr::SliceAttr(SliceAttr *owner) :
    TreeNode{owner},
    type{MAX_SLICE},
    owner{owner},
    nextChildNum{0}
{
    if (owner) owner->nextChildNum++;
}

SliceAttr::~SliceAttr()
{
    freeOrgans();
}

void SliceAttr::load(Xml::Element *element)
{
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getTagName()};
        if (tag == L"name") {
            name = std::wstring(child->getTextContent());
        } else if (tag == L"bgColor") {
            bgColor = std::stoul(child->getTextContent());
        } else if (tag == L"bgImage") {
            bgImage = std::wstring(child->getTextContent());
        } else if (tag == L"bgScale") {
            bgScale = static_cast<ImageTool::Scale>(std::stoi(child->getTextContent()));
        }
    }
}

void SliceAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
    parent->appendChild(doc->createElement(L"name"))
            ->appendChild(doc->createTextNode(name.value.c_str()));
    parent->appendChild(doc->createElement(L"bgColor"))
            ->appendChild(doc->createTextNode(std::to_wstring(bgColor.value).c_str()));
    parent->appendChild(doc->createElement(L"bgImage"))
            ->appendChild(doc->createTextNode(bgImage.value.c_str()));
    parent->appendChild(doc->createElement(L"bgScale"))
            ->appendChild(doc->createTextNode(std::to_wstring(bgScale.value).c_str()));
}

void SliceAttr::saveOrgans(Xml::Doc *doc, Xml::Node *parent)
{
    for (auto attr: organs) {
        static_cast<SliceAttr *>(attr)->save(doc, parent);
    }
}
