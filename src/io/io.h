#ifndef IOER_H
#define IOER_H

#include <stdint.h>
#include <functional>

#include "util/task_syncer.h"

class Io {
public:
    static Io *obj;

    Io();
    virtual ~Io();

    void start();
    void stop();

    TaskSyncer syncer;
};

#endif // IOER_H
