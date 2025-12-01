#ifndef _PLAYER_H
#define _PLAYER_H

#include <_3dmodelloader.h>
#include <_objLoader.h>
#include <_timer.h>
#include <_common.h>

class _player
{
    public:
        _player();
        virtual ~_player();

        void drawPlayer();
        _timer *myTime = new _timer();
        //_timer *accelTmr = new _timer();
        _timer *iFrames = new _timer();

        _3DModelLoader *mdl = new _3DModelLoader();
        _3DModelLoader *wpn = new _3DModelLoader();

        _objLoader *body = new _objLoader();
        _objLoader *whls = new _objLoader();
        _objLoader driveFrames[7];
        _objLoader turnFrames[7];
        _objLoader *whlsTurn = new _objLoader();

        int health = 5;
        vec3 pos;
        vec3 rot;
        float scale;
        bool crashed = false;
        bool wheelsFlipped = false;

        int movement;
        enum {menu,none,left,right};

        float speed;
        float maxSpeed;
        float accel;

        bool accelerating;
        chrono::time_point<chrono::system_clock> wheelTmr;
        chrono::time_point<chrono::system_clock> accelTmr;
        chrono::time_point<chrono::system_clock> animTmr;
        chrono::time_point<chrono::system_clock> nw;

        chrono::duration<float,milli>diff;

        vec3 menuPos;
        vec3 menuRot;
        float prevRot;

        int actions;
        enum {idle,drive,turn};

        float bounds = 0.9;

        int frameCount = 0;

    protected:

    private:
};

#endif // _PLAYER_H
