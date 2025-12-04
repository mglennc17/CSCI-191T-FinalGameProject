#include "_player.h"

_player::_player()
{
    menuRot.x = 30; menuRot.y = 0; menuRot.z = 0;
    menuPos.x = 0; menuPos.y = 0.7; menuPos.z = -7.0;
    pos = menuPos;
    rot = menuRot;
    scale = 1.0;
    speed = 0;
    maxSpeed = 1.0;
    accelerating = false;
    accel = 0.1;
    animTmr = chrono::system_clock::now();
    wheelTmr = chrono::system_clock::now();
    prevRot = 0;
    //ctor
}

_player::~_player()
{
    //dtor
}

void _player::drawPlayer()
{
    glPushMatrix();

    nw = chrono::system_clock::now();
    diff = nw - animTmr;

    if(diff.count() > 16 && !crashed && movement != menu) {

        if (movement == right && pos.x >= -bounds + 0.01) {
            if (rot.y > 0) rot.y -= 0.2 * speed * diff.count()/16;
            if (rot.y >= -30) rot.y -= 0.2 * speed * diff.count()/16;
        }
        else if (movement == left && pos.x <= bounds -0.01) {
            if (rot.y < 0) rot.y += 0.2 * speed * diff.count()/16;
            if (rot.y <= 30) rot.y += 0.2 * speed * diff.count()/16;
        }
        else if (movement != menu) {
            //if (rot.y > 0) rot.y -= 0.4;
            //if (rot.y < 0) rot.y += 0.4;
            rot.y *= 0.95;
            if (pos.x > 0.9) pos.x = 0.9;
            if (pos.x < -0.9) pos.x = -0.9;
        }
    if (accelerating) {
        diff = nw - accelTmr;
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

    diff = nw - animTmr;

    if (speed < 0) speed = 0;
    if (speed > maxSpeed) speed = maxSpeed;

            glTranslatef(0,0,-10);
            rot.z *= 0.8;
            rot.z += 5 * (rot.y - prevRot);
            if (rot.z > 7) rot.z = 7;
            else if (rot.z < -7) rot.z = -7;
            glTranslatef(0,0,10);
            prevRot = rot.y;
            animTmr = chrono::system_clock::now();
    }

    diff = nw - wheelTmr;

    if(diff.count() * (16 * speed) > 16 && !crashed && movement != menu) {
        frameCount++; wheelTmr = chrono::system_clock::now();
        pos.x += (0.4 * rot.y / ( 270 )) * (diff.count()/16);
    }

    if (pos.x > bounds) pos.x = bounds;
    if (pos.x < -bounds) pos.x = -bounds;

    glTranslatef(pos.x,pos.y,pos.z);

    glRotatef(rot.x,1.0,0,0);
    glRotatef(rot.y,0,1.0,0);
    glRotatef(rot.z,0,0,1.0);

    glScalef(scale,scale,scale);

    mdl->Draw();
    wpn->Draw();
    body->drawOBJ();

    glRotatef(-rot.z,0,0,1.0);

    if (movement == right) {glScalef(-1,1,1),turnFrames[frameCount % 7].drawOBJ(); }
    else if (movement == left) {turnFrames[frameCount % 7].drawOBJ(); }
    else driveFrames[frameCount % 7].drawOBJ();

    glPopMatrix();
}
