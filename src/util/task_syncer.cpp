#include "task_syncer.h"

#include "qlog/qlog.h"

TaskSyncer::TaskSyncer() : enabled(true)
{
    connect(this, &TaskSyncer::sign, this, &TaskSyncer::slot, Qt::QueuedConnection);
}

TaskSyncer::~TaskSyncer()
{
    disconnect(this, &TaskSyncer::sign, this, &TaskSyncer::slot);
}

void TaskSyncer::operator()(Task *task)
{
    if (enabled)
        emit sign(task);
    else
        delete task;
}

void TaskSyncer::slot(Task *task)
{
    //qlog << "TaskSyncer::slot" << qdebug;
    if (*task) (*task)();
    delete task;
}
