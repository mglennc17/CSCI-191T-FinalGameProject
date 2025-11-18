#ifndef _CAR_H
#define _CAR_H

#include "_common.h"
#include "_3dmodelloader.h"

using namespace std;

class _car
{
    public:
        _car();
        virtual ~_car();

        _3DModelLoader *carMdl = new _3DModelLoader();

        _car(int,int,int);
        void drawCar();

        int lane;
        enum {FORWARD, BACKWARD};
        int dir;
        float speed;

    protected:

    private:
};

#endif // _CAR_H
