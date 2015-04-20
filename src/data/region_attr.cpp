#include "region_attr.h"

#include "text_attr.h"
#include "picture_attr.h"
#include "video_attr.h"
#include "watch_attr.h"
#include "clock_attr.h"
#include "timer_attr.h"
#include "office_attr.h"

RegionAttr::RegionAttr(ProgramAttr *owner) :
    SliceAttr{owner},
    owner{owner}
{
    type = REGION_SLICE;
    GeoAttr *creenGeo = &owner->owner->geo;
    geo.x = creenGeo->width / 4;
    geo.y = creenGeo->height / 4;;
    geo.width = creenGeo->width / 2;
    geo.height = creenGeo->height / 2;
    geo.setConstraint(creenGeo->width, creenGeo->height);
}

RegionAttr::~RegionAttr()
{
}

void RegionAttr::load(Xml::Element *element)
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
        } else if (tag == L"text") {
            TextAttr *attr = new TextAttr(this);
            attr->load(child);
        } else if (tag == L"picture") {
            PictureAttr *attr = new PictureAttr(this);
            attr->load(child);
        } else if (tag == L"video") {
            VideoAttr *attr = new VideoAttr(this);
            attr->load(child);
        } else if (tag == L"watch") {
            WatchAttr *attr = new WatchAttr(this);
            attr->load(child);
        } else if (tag == L"clock") {
            ClockAttr *attr = new ClockAttr(this);
            attr->load(child);
        } else if (tag == L"timer") {
            TimerAttr *attr = new TimerAttr(this);
            attr->load(child);
        }else if (tag == L"office") {
            OfficeAttr *attr = new OfficeAttr(this);
            attr->load(child);
        }

    }
}

void RegionAttr::save(Xml::Doc *doc, Xml::Node *parent)
{
    Xml::Node *self = parent->appendChild(doc->createElement(L"region"));
    SliceAttr::save(doc, self);
    self->appendChild(doc->createElement(L"x"))
            ->appendChild(doc->createTextNode(std::to_wstring(geo.x).c_str()));
    self->appendChild(doc->createElement(L"y"))
            ->appendChild(doc->createTextNode(std::to_wstring(geo.y).c_str()));
    self->appendChild(doc->createElement(L"width"))
            ->appendChild(doc->createTextNode(std::to_wstring(geo.width).c_str()));
    self->appendChild(doc->createElement(L"height"))
            ->appendChild(doc->createTextNode(std::to_wstring(geo.height).c_str()));
    SliceAttr::saveOrgans(doc, self);
}
