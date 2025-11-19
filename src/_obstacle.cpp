#include "_obstacle.h"

_obstacle::_obstacle()
{
    //ctor
    pos.x = pos.y = pos.z = 0;
    rot.x = rot.y = rot.z = 0;
    scale = 1.0;
}

_obstacle::~_obstacle()
{
    //dtor
}

void _obstacle::drawObstacle(_3DModelLoader *mdl)
{
    glPushMatrix();

        glTranslatef(pos.x,pos.y,pos.z);

        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glRotatef(90,0,0,1);

        glRotatef(rot.x,1.0,0,0);
        glRotatef(rot.y,0,1.0,0);
        glRotatef(rot.z,0,0,1.0);

        glScalef(scale,scale,scale);
        mdl->Actions();
        mdl->Draw();

    glPopMatrix();
}
