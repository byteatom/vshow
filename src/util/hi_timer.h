#ifndef HITIMER_H
#define HITIMER_H

class HiTimer
{
public:
    static const int DEFAULT_PERIOD = 16;
    typedef void (*Timeout)(void *data);
    static long long elapse();

    HiTimer(unsigned int interval, Timeout timeout, void *data);
    ~HiTimer();

    void start();
    void stop();

    bool periodChanged;
    void *cs;
    unsigned int id;
    unsigned int interval;
    Timeout timeout;
    void *data;
};

#endif // HITIMER_H
