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

        int modelType;          // 0 = Focus, 1 = Jeep 2 = Humvee
        float modelScaleFix;    // extra per-model scaling multiplier
        float modelYFix;        // extra y offset if needed
        float modelYawFix;   // extra rotate to make this model face forward



    protected:

    private:
};

#endif // _OBSTACLE_H
