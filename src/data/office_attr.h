#ifndef OFFICEATTR_H
#define OFFICEATTR_H

#include "segment_attr.h"


struct OfficeInfo {
    std::wstring file;
    ImageTool::Scale scale;

public:
    OfficeInfo operator=(const OfficeInfo &newValue) {
        file = newValue.file;
        scale = newValue.scale;
        return *this;
    }
    bool operator!=(const OfficeInfo &newValue) {
        if (file != newValue.file || scale != newValue.scale) {
            return true;
        } else {
            return false;
        }
    }
};

class OfficeAttr : public SegmentAttr {
public:
    explicit OfficeAttr(RegionAttr *owner);
    virtual ~OfficeAttr();

    virtual void load(Xml::Element *element) override;
    virtual void save(Xml::Doc *doc, Xml::Node *parent) override;

    Attr<std::wstring> file;
    Attr<ImageTool::Scale> scale;
};

#endif    // OFFICEATTR_H
