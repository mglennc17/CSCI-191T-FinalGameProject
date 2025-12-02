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

        chrono::duration<float,milli>t{0};
        chrono::duration<float,milli>pauseDuration{0};
        chrono::duration<float,milli>timerDuration{0};
        chrono::time_point<chrono::system_clock> nw;

};

#endif // _PAUSEABLETIMER_H
