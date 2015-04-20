#include "sender.h"

#include <cassert>

#include "qlog.h"

Sender sender;

Sender::Sender() : stream(&bytes, QIODevice::WriteOnly)
{
}

Sender::~Sender()
{
    requestInterruption();
    cond.wakeAll();
    wait();
    //mutex.unlock();
}

void Sender::push(QLog *log)
{
    mutex.lock();
    if (queue.size() > 40960) {
        delete queue.dequeue();
    }
    queue.enqueue(log);
    cond.wakeAll();
    mutex.unlock();
}

void Sender::run()
{
    QTcpSocket client;
    while (!isInterruptionRequested()) {
        if (client.state() != client.ConnectedState) {
            client.connectToHost("127.0.0.1", 34567);
            if (!client.waitForConnected(3000)) {
                continue;
            }
        }
        QLog *log = nullptr;
        mutex.lock();
        if (queue.empty())
            cond.wait(&mutex);
        if (!queue.empty())
            log = queue.dequeue();
        mutex.unlock();
        if (log) {
            log->write(stream, bytes);
            client.write(bytes);
            client.flush();
        }
    }
}
