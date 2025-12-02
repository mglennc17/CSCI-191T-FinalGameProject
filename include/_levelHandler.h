#ifndef _LEVELHANDLER_H
#define _LEVELHANDLER_H

#include <_common.h>
#include <_objLoader.h>
#include <_parallax.h>
#include <_player.h>

class _levelHandler
{
    public:
        _levelHandler();
        virtual ~_levelHandler();

        _parallax *ground = new _parallax();
        _parallax *road = new _parallax();

        _objLoader *tunnel = new _objLoader();
        _objLoader *bridge = new _objLoader();
        int level;

        void setUpLevel(int,int &,_player *);
        void updateLevel(float);
        void drawLevel();

    protected:

    private:

    float fogColorOne[4] = {0.4,0.4,0.43,1.0};
    float fogColorTwo[4] = {0,0,0.01,1.0};
    float fogColorThr[4] = {0.41,0.33,0.41,1.0};

    float fogDensOne = 0.01;
    float fogDensTwo = 0.4;
    float fogDensThr = 0.1;
};

#endif // _LEVELHANDLER_H
