#ifndef SCREEN_UI_H
#define SCREEN_UI_H

#include <QTreeWidget>

#include "data/screen_attr.h"
#include "slice.h"
#include "project.h"
#include "screen_view.h"
#include "play_ctrl.h"
#include "program_list.h"

class Screen : public Slice {
    Q_DECLARE_TR_FUNCTIONS(Screen)

public:
    typedef Project OwnerType;
    static const SliceTypeId typeId;

    Screen(Project *owner, QTreeWidgetItem *treeItem, ScreenAttr *attr);
    Screen(Project *owner, QTreeWidgetItem *treeItem);
    virtual ~Screen();

    virtual void showCfgers() override;
    virtual void raiseView() override;
    virtual void hideCfgers() override;
    virtual void inflate() override;

    virtual void start() override;
    virtual void pulse() override;
    virtual void organEnd(Slice *organ) override;
    virtual void stop() override;
    ScreenAttr *attr();
    ScreenView *view();

    Project *owner;
    PlayCtrl *playCtrl;
    ProgramList *programList;

    std::list<TreeNode *>::iterator playingProgram;

private:
    void lockChanged(bool lock);
};

#endif    // SCREEN_UI_H
