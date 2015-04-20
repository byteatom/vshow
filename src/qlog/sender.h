#ifndef SENDER_H
#define SENDER_H

#include <QThread>
#include <QTcpSocket>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDataStream>

class QLog;

class Sender : public QThread
{
public:
    Sender();
    virtual ~Sender();

    void push(QLog *log);
protected:
    virtual void run() override;

private:
    QQueue<QLog *> queue;
    QMutex mutex;
    QWaitCondition cond;
    QByteArray bytes;
    QDataStream stream;
};

extern Sender sender;

#endif // SENDER_H
