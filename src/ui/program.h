#ifndef PROGRAM_H
#define PROGRAM_H

#include <QWidget>

#include "data/program_attr.h"
#include "slice.h"
#include "screen.h"
#include "program_view.h"

class Region;

class Program : public Slice {
    Q_DECLARE_TR_FUNCTIONS(Program)

public:
    typedef Screen OwnerType;
    static const SliceTypeId typeId;

    Program(Screen *owner, QTreeWidgetItem *treeItem, ProgramAttr *attr);
    Program(Screen *owner, QTreeWidgetItem *treeItem);
    virtual ~Program();

    virtual void showCfgers() override;
    virtual void hideCfgers() override;
    virtual void raiseView() override;
    ProgramAttr *attr();
    ProgramView *view();

    Screen *owner;
};

#endif    // PROGRAM_H
