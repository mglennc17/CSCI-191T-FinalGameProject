#include "_levelHandler.h"

_levelHandler::_levelHandler()
{
    //LEVEL 1
    /*road->parallaxInit("images/textures/road.png");
    road->xMax = 25;
    road->yMax = 2;

    ground->parallaxInit("images/textures/dirt.jpg");
    ground->xMax = 50;
    ground->yMax = 50;

    //LEVEL 2
    tunnel->loadOBJ("models/terrain/tunneltest.obj","models/terrain/tunneltest.mtl");
    tunnel->rot.y = 180;
    tunnel->pos.z = 0;
    tunnel->scale = 0.25;

    vector<char *> Texs;
    Texs.push_back("models/terrain/concrete.png");
    Texs.push_back("models/terrain/tunnel.png");
    Texs.push_back("models/terrain/emit.png");
    Texs.push_back("models/terrain/tunnel.png");
    Texs.push_back("models/terrain/concrete.png");
    Texs.push_back("models/terrain/tunnel.png");
    Texs.push_back("models/terrain/emit.png");
    Texs.push_back("models/terrain/concrete.png");
    Texs.push_back("models/terrain/tunnel.png");
    Texs.push_back("models/terrain/emit.png");
    Texs.push_back("models/terrain/concrete.png");
    Texs.push_back("models/terrain/tunnel.png");
    Texs.push_back("models/terrain/emit.png");
    tunnel->textureOBJ(Texs);

    Texs.clear();

    //LEVEL 3
    bridge->loadOBJ("models/terrain/bridge.obj","models/terrain/bridge.mtl");
    bridge->rot.y = 270;
    bridge->pos.z = -12.5;
    bridge->pos.x = -30;
    bridge->pos.y = -15.5;
    bridge->scale = 0.08;

    Texs.clear();
    Texs.push_back("models/terrain/bridge.png");
    Texs.push_back("models/terrain/bridge.png");
    Texs.push_back("models/terrain/bridge.png");
    Texs.push_back("models/terrain/bridge.png");
    Texs.push_back("models/terrain/bridge.png");
    Texs.push_back("models/terrain/bridge.png");
    bridge->textureOBJ(Texs);*/
}

_levelHandler::~_levelHandler()
{
    //dtor
}

void _levelHandler::setUpLevel(int lvl,int &numLanes,_player *plyr)
{
    level = lvl;
    switch (level) {
    case 1:
        //numLanes = 6;
        plyr->bounds = 0.9;
        fogDensity = fogDensOne;
        fogColor[0] = fogColorOne[0];
        fogColor[1] = fogColorOne[1];
        fogColor[2] = fogColorOne[2];
        fogColor[3] = fogColorOne[3];
        glFogf(GL_FOG_DENSITY,fogDensOne);
        glFogfv(GL_FOG_COLOR,fogColorOne);
        break;
    case 2:
        //numLanes = 4;
        fogDensity = fogDensTwo;
        fogColor[0] = fogColorTwo[0];
        fogColor[1] = fogColorTwo[1];
        fogColor[2] = fogColorTwo[2];
        fogColor[3] = fogColorTwo[3];
        plyr->bounds = 0.6;
        glFogf(GL_FOG_DENSITY,fogDensTwo);
        glFogfv(GL_FOG_COLOR,fogColorTwo);
        break;
    case 3:
        //numLanes = 4;
        fogDensity = fogDensThr;
        fogColor[0] = fogColorThr[0];
        fogColor[1] = fogColorThr[1];
        fogColor[2] = fogColorThr[2];
        fogColor[3] = fogColorThr[3];
        plyr->bounds = 0.6;
        glFogf(GL_FOG_DENSITY,fogDensThr);
        glFogfv(GL_FOG_COLOR,fogColorThr);
        break;
    }
}

void _levelHandler::updateLevel(float playerSpeed)
{
    switch (level) {
    case 1:
        ground->prlxScrollAuto("right",0.045 * playerSpeed);
        road->prlxScrollAuto("right", 0.045 * playerSpeed);
        break;
    case 2:
        tunnel->pos.z += (0.6 * playerSpeed);
        if (tunnel->pos.z >= 50) tunnel->pos.z = 10;
        break;
    case 3:
        bridge->pos.x -= (3.0 * playerSpeed);
        if (bridge->pos.x <= -190) bridge->pos.x = -30;
        break;
    }
}

void _levelHandler::drawLevel()
{
    switch(level) {
    case 1:
        glPushMatrix();
            glRotatef(90.0,0,0,1.0);
            glRotatef(90.0,0,1,0);
            glTranslatef(0,0,10);
            road->drawParallax(200.0,2.0);
            glTranslatef(1,-1,0);
            glScalef(1.0,200.0,1.0);
            glTranslatef(0,0,-0.05);
            ground->drawParallax(200.0,1.0);
        glPopMatrix();
        break;
    case 2:
        glPushMatrix();
            tunnel->drawOBJ();
        glPopMatrix();
        break;
    case 3:
        glPushMatrix();
            bridge->drawOBJ();
        glPopMatrix();
        break;
    }
}

