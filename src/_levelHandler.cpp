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
        glFogf(GL_FOG_DENSITY,fogDensOne);
        glFogfv(GL_FOG_COLOR,fogColorOne);
        break;
    case 2:
        //numLanes = 4;
        plyr->bounds = 0.6;
        glFogf(GL_FOG_DENSITY,fogDensTwo);
        glFogfv(GL_FOG_COLOR,fogColorTwo);
        break;
    case 3:
        //numLanes = 4;
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
        ground->prlxScrollAuto("right",0.03 * playerSpeed);
        road->prlxScrollAuto("right", 0.03 * playerSpeed);
        break;
    case 2:
        tunnel->pos.z += (0.50 * playerSpeed);
        if (tunnel->pos.z >= 50) tunnel->pos.z = 10;
        break;
    case 3:
        bridge->pos.x -= (2.0 * playerSpeed);
        if (bridge->pos.x <= -190) bridge->pos.x = -30;
        break;
    }
}

void _levelHandler::drawLevel()
{
    switch(level) {
    case 1:
        glEnable(GL_FOG);
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
        glDisable(GL_FOG);
        break;
    case 2:
        //glEnable(GL_FOG);
        glPushMatrix();
            tunnel->drawOBJ();
        glPopMatrix();
        //glDisable(GL_FOG);
        break;
    case 3:
        glEnable(GL_FOG);
        glPushMatrix();
            bridge->drawOBJ();
        glPopMatrix();
        glDisable(GL_FOG);
        break;
    }
}

