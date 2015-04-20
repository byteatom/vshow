#include "video_attr.h"

VideoAttr::VideoAttr(RegionAttr *owner) :
    SegmentAttr(owner)
{
    type = VIDEO_SLICE;
}

VideoAttr::~VideoAttr()
{
}

void VideoAttr::load(Xml::Element *element)
{
    SegmentAttr::load(element);
    for (Xml::Element *child = element->getFirstElementChild();
         nullptr != child;
         child = child->getNextElementSibling()) {
        std::wstring tag{child->getTagName()};
        if (tag == L"file") {
            file = std::wstring(child->getTextContent());
        }
    }
}

void VideoAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
    Xml::Node *self = parent->appendChild(doc->createElement(L"video"));
    SegmentAttr::save(doc, self);
    self->appendChild(doc->createElement(L"file"))
            ->appendChild(doc->createTextNode(file.value.c_str()));
}
