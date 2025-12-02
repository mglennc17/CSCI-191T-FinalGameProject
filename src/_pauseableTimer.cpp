#include "_pauseableTimer.h"

_pauseableTimer::_pauseableTimer()
{
    //ctor
}

_pauseableTimer::~_pauseableTimer()
{
    //dtor
}

void _pauseableTimer::start()
{
    startTime = chrono::system_clock::now();
}

void _pauseableTimer::pause()
{
    pauseTime = chrono::system_clock::now();
}

void _pauseableTimer::resume()
{
    nw = chrono::system_clock::now();
    t = nw - pauseTime;
    pauseDuration += t;
}

void _pauseableTimer::finish()
{

}

int _pauseableTimer::getDuration()
{
    nw = chrono::system_clock::now();
    timerDuration = nw - startTime;
    printf("%f\n",timerDuration.count());
    return timerDuration.count() - pauseDuration.count();
}
