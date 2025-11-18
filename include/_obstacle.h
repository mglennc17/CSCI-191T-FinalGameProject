#ifndef _OBSTACLE_H
#define _OBSTACLE_H
#include <_3dmodelloader.h>

class _obstacle
{
    public:
        _obstacle();
        virtual ~_obstacle();

        _3DModelLoader *mdl = new _3DModelLoader();
        vec3 pos;

        bool collided = false;

    protected:

    private:
};

#endif // _OBSTACLE_H
