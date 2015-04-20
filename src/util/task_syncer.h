#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <functional>

#include <QObject>

#include "task.h"

class TaskSyncer : public QObject
{
    Q_OBJECT
public:
    explicit TaskSyncer();
    virtual ~TaskSyncer();

    void operator()(Task *task);

    bool enabled;
signals:
    void sign(Task *task);
private:
    void slot(Task *task);
};

#endif // EXECUTOR_H
