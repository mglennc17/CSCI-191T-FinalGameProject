#ifndef _PAUSEABLETIMER_H
#define _PAUSEABLETIMER_H

#include <_common.h>


class _pauseableTimer
{
    public:
        _pauseableTimer();
        virtual ~_pauseableTimer();

        void start();
        void pause();
        void resume();
        void finish();

        int getDuration();

    protected:

    private:
        chrono::time_point<chrono::system_clock> startTime;
        chrono::time_point<chrono::system_clock> pauseTime;

        chrono::duration<float,milli>t;
        chrono::duration<float,milli>pauseDuration;
        chrono::duration<float,milli>timerDuration;
        chrono::time_point<chrono::system_clock> nw;

};

#endif // _PAUSEABLETIMER_H
