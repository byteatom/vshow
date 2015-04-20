#ifndef REGION_H
#define REGION_H

#include "data/region_attr.h"
#include "slice.h"
#include "program.h"
#include "region_view.h"

class Segment;

class Region : public Slice {
    Q_DECLARE_TR_FUNCTIONS(Region)

public:
    typedef Program OwnerType;
    static const SliceTypeId typeId;

    Region(Program *owner, QTreeWidgetItem *treeItem, RegionAttr *attr);
    Region(Program *owner, QTreeWidgetItem *treeItem);
    virtual ~Region();

    virtual void showCfgers() override;
    virtual void hideCfgers() override;

    virtual void start() override;
    virtual void stop() override;
    RegionAttr *attr();
    RegionView *view();

    Program *owner;
};

#endif    // REGION_H
