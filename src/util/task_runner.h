#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

#include "task.h"

class TaskRunner : public QThread
{
public:
    TaskRunner();
    virtual ~TaskRunner();

    void addTask(Task *task);
    void clear();

protected:
    virtual void run() override;

private:
    QQueue<Task *> queue;
    QMutex mutex;
    QWaitCondition cond;
};

#endif // TASKTHREAD_H
