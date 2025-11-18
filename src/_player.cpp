#include "_player.h"

_player::_player()
{
    myTime->startTime = clock();
    rot.x = rot.y = rot.z = 0;
    pos.x = pos.y = pos.z = 0;
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

        glTranslatef(mdl->pos.x,mdl->pos.y,mdl->pos.z);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);

        glRotatef(rot.x,1.0,0,0);
        glRotatef(rot.y,0,1.0,0);
        glRotatef(rot.z,0,0,1.0);

        glScalef(scale,scale,scale);

        //glScalef(0.012,0.012,0.012);
        if(myTime->getTicks() > 10) {
            myTime->reset();
            mdl->Actions();
            wpn->Actions();
        }
        mdl->Draw();
        wpn->Draw();
    glPopMatrix();

}
