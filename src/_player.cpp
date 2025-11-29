#include "_player.h"

_player::_player()
{
    menuRot.x = 30; menuRot.y = 0; menuRot.z = 0;
    menuPos.x = 0; menuPos.y = 0.7; menuPos.z = -7.0;
    myTime->reset();
    pos = menuPos;
    rot = menuRot;
    scale = 1.0;
    speed = 0;
    maxSpeed = 1.0;
    accelerating = false;
    accel = 0.1;

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

        if (movement == right && pos.x >= -0.9) {
            if (rot.y > 0) rot.y -= 0.1 * speed;
            if (rot.y >= -30) rot.y -= 0.1 * speed;
        }
        else if (movement == left && pos.x <= 0.9) {
            if (rot.y < 0) rot.y += 0.1 * speed;
            if (rot.y <= 30) rot.y += 0.1 * speed;
        }
        else if (movement != menu) {
            if (rot.y > 0) rot.y -= 0.2;
            if (rot.y < 0) rot.y += 0.2;
            if (pos.x > 0.9) pos.x = 0.9;
            if (pos.x < -0.9) pos.x = -0.9;
        }
    }
    if (accelerating) {
        auto nw = chrono::system_clock::now();
        chrono::duration<float,milli>diff = nw - accelTmr;
        if (speed < maxSpeed) {
                accel = diff.count()/100000;
                accel *= (1 - ((speed) / (maxSpeed)));
                printf("%f\n",accel);
                speed += accel;// * (diff.count());// * //accelTmr->getTicks();
        }
    }
    else {
        if (speed > 0) speed -= 0.001;
    }

    if (speed < 0) speed = 0;
    if (speed > maxSpeed) speed = maxSpeed;

        if (pos.x > 0.9) pos.x = 0.9;
        if (pos.x < -0.9) pos.x = -0.9;

        if (movement != menu) {
            glTranslatef(0,0,5);
            pos.x += 0.1 * speed * rot.y / ( 270 );
            glTranslatef(0,0,-5);
        }
        glTranslatef(pos.x,pos.y,pos.z);

        glRotatef(rot.x,1.0,0,0);
        glRotatef(rot.y,0,1.0,0);
        glRotatef(rot.z,0,0,1.0);

        glScalef(scale,scale,scale);

        if(myTime->getTicks() >= 5) {
            myTime->reset();
            mdl->Actions();
            wpn->Actions();
        }
        mdl->Draw();
        wpn->Draw();
        body->drawOBJ();
        if (movement == left) whlsTurn->drawOBJ();
        else if (movement == right) {glScalef(-1,1,1); whlsTurn->drawOBJ();}
        else whls->drawOBJ();

    glPopMatrix();
}
