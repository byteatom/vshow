#include "hi_timer.h"

#include <util/win_util.h>

#include <cassert>

void CALLBACK timeout(UINT uTimerID,
                  UINT uMsg,
                  DWORD_PTR dwUser,
                  DWORD_PTR dw1,
                  DWORD_PTR dw2)
{
    HiTimer *hiTimer = (HiTimer *)dwUser;
    EnterCriticalSection((LPCRITICAL_SECTION)hiTimer->cs);
    if (hiTimer->id) {
        LeaveCriticalSection((LPCRITICAL_SECTION)hiTimer->cs);
        hiTimer->timeout(hiTimer->data);
    } else {
        LeaveCriticalSection((LPCRITICAL_SECTION)hiTimer->cs);
    }
}

long long HiTimer::elapse()
{
    LARGE_INTEGER counter, frequency;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&frequency);
    return counter.QuadPart * 1000 / frequency.QuadPart;
}

HiTimer::HiTimer(unsigned int interval, Timeout timeout, void *data) :
    periodChanged(false),
    id(0),
    interval(interval),
    timeout(timeout),
    data(data)
{
    TIMECAPS caps{0, 0};
    MMRESULT ret = timeGetDevCaps(&caps, sizeof(caps));
    assert(MMSYSERR_NOERROR == ret);
    if (interval < caps.wPeriodMin)
        interval = caps.wPeriodMin;
    if (interval > caps.wPeriodMax)
        interval = caps.wPeriodMax;
    if (interval < DEFAULT_PERIOD)
        periodChanged = (TIMERR_NOERROR == timeBeginPeriod(interval));
    cs = new CRITICAL_SECTION;
    InitializeCriticalSection((LPCRITICAL_SECTION)cs);
}

HiTimer::~HiTimer()
{
    stop();
    if (periodChanged) {
        timeEndPeriod(interval);
    }
    DeleteCriticalSection((LPCRITICAL_SECTION)cs);
    delete cs;
}

void HiTimer::start()
{
    assert(!id);
    id = timeSetEvent(interval, interval, &::timeout, (DWORD_PTR)this,
            TIME_PERIODIC | TIME_CALLBACK_FUNCTION /*| TIME_KILL_SYNCHRONOUS*/);
    assert(id);
}

void HiTimer::stop()
{
    if (id) {
        EnterCriticalSection((LPCRITICAL_SECTION)cs);
        timeKillEvent(id);
        id = 0;
        LeaveCriticalSection((LPCRITICAL_SECTION)cs);
    }
}

