#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#include <_3dmodelloader.h>


class _obstacle
{
    public:
        _obstacle();
        virtual ~_obstacle();

        void drawObstacle(_3DModelLoader *);

        vec3 pos;
        vec3 rot;
        float scale;

    protected:

    private:
};

#endif // _OBSTACLE_H
