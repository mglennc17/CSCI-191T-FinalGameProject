#include "_player.h"

_player::_player()
{
    myTime->reset();
    pos.x = pos.y = pos.z = 0;
    rot.x = rot.y = rot.z = 0;
    scale = 1.0;
    //ctor
}

_player::~_player()
{
    //dtor
}

void _player::drawPlayer()
{
    glPushMatrix();

    if(myTime->getTicks() >= 5) {
        //glTranslatef(mdl->pos.x,mdl->pos.y,mdl->pos.z);

        if (movement == left && pos.x >= -0.9) {
            if (rot.z > 270) rot.z -= 0.1;
            if (rot.z >= 260) rot.z -= 0.1;
        }
        else if (movement == right && pos.x <= 0.9) {
            if (rot.z < 270) rot.z += 0.1;
            if (rot.z <= 280) rot.z += 0.1;
        }
        else if (movement != menu) {
            if (rot.z > 270) rot.z -= 0.2;
            if (rot.z < 270) rot.z += 0.2;
            if (pos.x > 0.9) pos.x = 0.9;
            if (pos.x < -0.9) pos.x = -0.9;
        }
    }

        if (pos.x > 0.9) pos.x = 0.9;
        if (pos.x < -0.9) pos.x = -0.9;
        glTranslatef(pos.x,pos.y,pos.z);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glRotatef(90,0,0,1);

        if (movement != menu) {
            pos.x += 0.1 * ( rot.z - 270 ) / ( 270 );
            glTranslatef(0,0,-5);
        }

        glRotatef(rot.x,1.0,0,0);
        glRotatef(rot.y,0,1.0,0);
        glRotatef(rot.z,0,0,1.0);

        if (movement != menu) glTranslatef(0,0,5);

        glScalef(scale,scale,scale);
        if(myTime->getTicks() >= 5) {
            myTime->reset();
            mdl->Actions();
            wpn->Actions();
        }
        mdl->Draw();
        wpn->Draw();
    glPopMatrix();
}
