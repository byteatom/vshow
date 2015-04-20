#ifndef PROJECT_H
#define PROJECT_H

#include "data/project_attr.h"
#include "slice.h"
#include "util/hi_timer.h"
#include "util/task_runner.h"


class Project : public Slice {
    Q_OBJECT
public:
    typedef Project OwnerType;
    static const SliceTypeId typeId;
    static void atomPulse(void *data);

    Project(QTreeWidgetItem *treeItem, ProjectAttr *attr);
    Project(QTreeWidgetItem *treeItem);
    virtual ~Project();

    virtual bool event(QEvent *e) override;

    virtual void start() override;
    virtual void stop() override;

    void startScreen(Slice *screen);
    void stopScreen(Slice *screen);
    void playScreen(Slice *screen);
    ProjectAttr *attr();

    HiTimer pulser;
    TaskRunner tasker;
};

#endif    // PROJECT_H
