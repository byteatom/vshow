#ifndef SLICEATTR_H
#define SLICEATTR_H

#include <string>

#include "util/tree_node.h"
#include "attr.h"
#include "image_attr.h"
#include "util/xml_dom.h"

typedef enum {
    PROJECT_SLICE,
    SCREEN_SLICE,
    PROGRAM_SLICE,
    REGION_SLICE,
    SEGMENT_SLICE,
    TEXT_SLICE,
    PICTURE_SLICE,
    VIDEO_SLICE,
    WATCH_SLICE,
    CLOCK_SLICE,
    TIMER_SLICE,
    OFFICE_SLICE,
    MAX_SLICE = 512
}SliceTypeId;

class SliceAttr : public TreeNode
{
public:
    explicit SliceAttr(SliceAttr *owner);
    virtual ~SliceAttr();

    virtual void load(Xml::Element *element);
    virtual void save(Xml::Doc *doc, Xml::Node *parent);
    void saveOrgans(Xml::Doc *doc, Xml::Node *parent);

    SliceTypeId type;
    SliceAttr *owner;
    Attr<std::wstring> name;
    Attr<ImageAttr::Argb> bgColor;
    Attr<std::wstring> bgImage;
    Attr<ImageTool::Scale> bgScale;
    unsigned int nextChildNum;
};

#endif // SLICEATTR_H
