#include "_obstacle.h"

_obstacle::_obstacle()
{
    //ctor

    // initialize position and rotation
    pos.x = pos.y = pos.z = 0;
    rot.x = rot.y = rot.z = 0;

    // base scale
    scale = 1.0;

    // Default model settings
    modelType = 0;
    modelScaleFix = 1.0f;
    modelYFix = 0.0f;
    modelYawFix = 0.0f;


}

_obstacle::~_obstacle()
{
    //dtor
}


void _obstacle::drawObstacle(_3DModelLoader *mdl)
{
    glPushMatrix();

        // Move obstacle into position
        // modelYFix helps fix models that float or sink
        //glTranslatef(pos.x,pos.y,pos.z);
        glTranslatef(pos.x, pos.y + modelYFix, pos.z);


        // Base rotations to align MD2 models with our world
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glRotatef(90,0,0,1);

        //  extra model rotation so different cars face forward correctly
        glRotatef(modelYawFix, 0, 0, 1);

        // runtime rotation (used for lane direction)
        glRotatef(rot.x,1,0,0);
        glRotatef(rot.y,0,1,0);
        glRotatef(rot.z,0,0,1);

        // apply base scale with any model specific adjustment
        glScalef(scale * modelScaleFix, scale * modelScaleFix, scale * modelScaleFix);

        mdl->Actions();
        glEnable(GL_TEXTURE_2D);
        mdl->Draw();

    glPopMatrix();
}

