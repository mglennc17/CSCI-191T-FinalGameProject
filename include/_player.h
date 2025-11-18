#ifndef _PLAYER_H
#define _PLAYER_H

#include <_3dmodelloader.h>
#include <_timer.h>

class _player
{
    public:
        _player();
        virtual ~_player();

        void drawPlayer();
        _timer *myTime = new _timer();
        _timer *iFrames = new _timer();

        _3DModelLoader *mdl = new _3DModelLoader();
        _3DModelLoader *wpn = new _3DModelLoader();

        int health = 5;
        vec3 pos;
        vec3 rot;
        float scale;
        bool invinsible = false;

    protected:

    private:
};

#endif // _PLAYER_H
