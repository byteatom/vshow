#include "task_runner.h"

#include <qlog/qlog.h>

TaskRunner::TaskRunner()
{

}

TaskRunner::~TaskRunner()
{
    requestInterruption();
    mutex.lock();
    cond.wakeAll();
    mutex.unlock();
    wait();
}

void TaskRunner::addTask(Task *task)
{
    //qlog << "TaskThread::addTask" << qdebug;
    mutex.lock();
    queue.enqueue(task);
    cond.wakeAll();
    mutex.unlock();
}

void TaskRunner::clear()
{
    mutex.lock();
    queue.clear();
    mutex.unlock();
}

void TaskRunner::run()
{
    while (!isInterruptionRequested()) {
        Task *task = nullptr;
        mutex.lock();
        if (queue.empty())
            cond.wait(&mutex);
        if (!queue.empty())
            task = queue.dequeue();
        mutex.unlock();
        if (task)
            (*task)();
    }
}
