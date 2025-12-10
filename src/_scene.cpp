#include "_scene.h"
#include <cmath>
#include <cstdio>

_Scene::_Scene()
{
    //ctor
    rng = mt19937(dev());
    rand100 = uniform_int_distribution<mt19937::result_type>(1,100);
    rand6 = uniform_int_distribution<mt19937::result_type>(1,6);
    rand4 = uniform_int_distribution<mt19937::result_type>(1,4);
    rand2 = uniform_int_distribution<mt19937::result_type>(1,2);
    animationTimer->startTime = clock();

    // init collectibles
    for(int i = 0; i < MAX_COINS; ++i){
        coins[i].active = false;
        coins[i].size = 0.3;
    }
    for(int i = 0; i < MAX_DOLLARS; ++i){
        dollars[i].active = false;
        dollars[i].size = 0.3;
    }

    coinScore = 0;
    dollarScore = 0;
    totalScore = 0;

    collectableSpawnTimer = 0.0;
    pickupTextTimer = 0.0;
    lastPickupValue = 0;

    showInGameHelp = false;

    camOrbitAngle = 0.0;

}

_Scene::~_Scene()
{
    //dtor
}

void _Scene::reSizeScene(int width, int height)
{
    float aspectRatio = (float)width/(float)height;// keep track of the ratio
    glViewport(0,0,width,height); // adjust my viewport

    //glMatrixMode(GL_PROJECTION);  // To setup ptrojection
    //glLoadIdentity();             // calling identity matrix
    //gluPerspective(45, aspectRatio,0.1,1000.0); // setting perspective projection

    this->width = GetSystemMetrics(SM_CXSCREEN);
    this->height= GetSystemMetrics(SM_CYSCREEN);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             // calling identity matrix
}

void _Scene::resetObstacles()
{
    for (int i = 0; i < 10; i++) {
        obstcls[i].rot.x = obstcls[i].rot.y = 0;
        obstcls[i].rot.z = 270;
        obstcls[i].scale = 0.1;
        obstcls[i].pos.y = 0.11;
        if (numLanes == 6) obstcls[i].pos.x = 1.2 - (rand6(rng) * (2.0/numLanes));
        if (numLanes == 4) obstcls[i].pos.x = 0.9 - (rand4(rng) * (1.4/numLanes));
        //if (i % 3) obstcls[i].pos.x = -0.7;
        //else if (i % 2) obstcls[i].pos.x = 0.7;
        obstcls[i].pos.z = 15.0 + i * 4.0;
        if (level != 3) { obstcls[i].rot.z = 270; continue; }
        if (obstcls[i].pos.x < 0) { obstcls[i].rot.z = 270; continue; }
        obstcls[i].rot.z = 90;
    }
}

void _Scene::initGL()
{

    numLanes = 4;

    gameState = mainMenu;
    frameCount = 0;

    glShadeModel(GL_SMOOTH); // to handle GPU shaders
    glClearColor(0.0f,0.0f,0.0f,0.0f); // black background color
    glClearDepth(2.0f);         //depth test for layers
    glEnable(GL_DEPTH_TEST);    //activate depth test
    glDepthFunc(GL_LEQUAL);     // depth function type

    glEnable(GL_TEXTURE_2D);

    myLight->setLight(GL_LIGHT0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glFogf(GL_FOG_DENSITY,0.01);
    glFogfv(GL_FOG_COLOR,fogColor);

    obstacleMdl->initModel("models/car/Ford_Focus.md2","models/car/Tex.png");
    obstacleMdl->actionTrigger = obstacleMdl->STAND;

    daySky->skyBoxInit();
    daySky->tex[0] = daySky->textures->loadTexture("images/skybox/dusk/back.png");
    daySky->tex[1] = daySky->textures->loadTexture("images/skybox/dusk/front.png");
    daySky->tex[2] = daySky->textures->loadTexture("images/skybox/dusk/top.png");
    daySky->tex[3] = daySky->textures->loadTexture("images/skybox/dusk/bottom.png");
    daySky->tex[4] = daySky->textures->loadTexture("images/skybox/dusk/right.png");
    daySky->tex[5] = daySky->textures->loadTexture("images/skybox/dusk/left.png");

    landingPage->parallaxInit("images/menu/landingPage.png");
    menuBackground.parallaxInit("images/menu/menuBackground.png");
    menuLogo.parallaxInit("images/menu/menuLogo.png");
    mainMenuElements[newGame].initButton("images/menu/newGame.png",-4.0,-2.5);
    mainMenuElements[help].initButton("images/menu/help.png",0,-2.5);
    mainMenuElements[exit].initButton("images/menu/exit.png",4.0,-2.5);

    pauseMenuBackground.parallaxInit("images/menu/pauseMenuBackground.png");
    pauseMenuElements[pauseResume].initButton("images/menu/pauseMenuReturn.png",-2.0,-2.5);
    pauseMenuElements[pauseExit].initButton("images/menu/pauseMenuExit.png",2.0,-2.5);

    helpMenuBackground.parallaxInit("images/menu/helpMenuBackground.png");
    helpMenuReturn.initButton("images/menu/pauseMenuReturn.png",0,-2.5);

    levelSelectButtons[0].initButton("images/menu/levelOneButton.png",-5.0,-2.5);
    levelSelectButtons[1].initButton("images/menu/levelTwoButton.png",-1.66,-2.5);
    levelSelectButtons[2].initButton("images/menu/levelThreeButton.png",1.66,-2.5);
    levelSelectButtons[3].initButton("images/menu/PauseMenuReturn.png",5.0,-2.5);
    for (int i = 0; i < 4; i++) levelSelectButtons[i].scale = 0.8;

    goText = new _textDisplay();
    goText->textInit("images/fonts/upper.png",8,4);
    gameOverButtons[goPlayAgain].initButton("images/menu/pauseMenuReturn.png", -2.0f, -2.5f);
    gameOverButtons[goMainMenu].initButton("images/menu/pauseMenuExit.png",   2.0f, -2.5f);

    levels->road->parallaxInit("images/textures/road.png");
    levels->road->xMax = 25;
    levels->road->yMax = 2;

    levels->ground->parallaxInit("images/textures/dirt.jpg");
    levels->ground->xMax = 50;
    levels->ground->yMax = 50;

    //plyr->mdl->initModel("models/car/Ford_Focus.md2","models/car/Tex.png");//,"models/car/Tex.png");
    //plyr->mdl->actionTrigger = plyr->mdl->STAND;
    plyr->rot = plyr->menuRot;
    plyr->pos = plyr->menuPos;

    resetObstacles();
    myCam->camInit();

    menuMsc->initSounds();
    menuMsc->playMusic("sounds/04 GARAGE TALK.mp3");

    textUpper->textInit("images/fonts/upper.png",9,3);
    textUpperWhite->textInit("images/fonts/upperWhite.png",9,3);
    textLower->textInit("images/fonts/lower.png",9,3);
    textNum->textInit("images/fonts/numEtc.png",9,3);
    textNumWhite->textInit("images/fonts/numEtcWhite.png",9,3);

    plyrScore->resetScore();

    obj->loadOBJ("models/car/nissan.obj","models/car/nissan.mtl");
    plyr->body->loadOBJ("models/car/nissanTest.obj","models/car/nissanTest.mtl");
    plyr->whls->loadOBJ("models/car/nissanWheels.obj","models/car/nissanWheels.mtl");
    plyr->whlsTurn->loadOBJ("models/car/drive1.obj","models/car/drive1.mtl");

    plyr->driveFrames[0].loadOBJ("models/car/drive1.obj","models/car/nissanWheels.mtl");
    plyr->driveFrames[1].loadOBJ("models/car/drive2.obj","models/car/nissanWheels.mtl");
    plyr->driveFrames[2].loadOBJ("models/car/drive3.obj","models/car/nissanWheels.mtl");
    plyr->driveFrames[3].loadOBJ("models/car/drive4.obj","models/car/nissanWheels.mtl");
    plyr->driveFrames[4].loadOBJ("models/car/drive5.obj","models/car/nissanWheels.mtl");
    plyr->driveFrames[5].loadOBJ("models/car/drive6.obj","models/car/nissanWheels.mtl");
    plyr->driveFrames[6].loadOBJ("models/car/drive7.obj","models/car/nissanWheels.mtl");

    plyr->turnFrames[0].loadOBJ("models/car/turn1.obj","models/car/nissanWheels.mtl");
    plyr->turnFrames[1].loadOBJ("models/car/turn2.obj","models/car/nissanWheels.mtl");
    plyr->turnFrames[2].loadOBJ("models/car/turn3.obj","models/car/nissanWheels.mtl");
    plyr->turnFrames[3].loadOBJ("models/car/turn4.obj","models/car/nissanWheels.mtl");
    plyr->turnFrames[4].loadOBJ("models/car/turn5.obj","models/car/nissanWheels.mtl");
    plyr->turnFrames[5].loadOBJ("models/car/turn6.obj","models/car/nissanWheels.mtl");
    plyr->turnFrames[6].loadOBJ("models/car/turn7.obj","models/car/nissanWheels.mtl");

    levels->tunnel->loadOBJ("models/terrain/tunneltest.obj","models/terrain/tunneltest.mtl");
    levels->tunnel->rot.y = 180;
    levels->tunnel->pos.z = 20;
    levels->tunnel->scale = 0.25;

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
    levels->tunnel->textureOBJ(Texs);

    levels->bridge->loadOBJ("models/terrain/bridge.obj","models/terrain/bridge.mtl");
    levels->bridge->rot.y = 270;
    levels->bridge->pos.z = -12.5;
    levels->bridge->pos.x = -30;
    levels->bridge->pos.y = -15.5;
    levels->bridge->scale = 0.08;

    Texs.clear();
    Texs.push_back("models/terrain/bridge.png");
    Texs.push_back("models/terrain/bridge.png");
    Texs.push_back("models/terrain/bridge.png");
    levels->bridge->textureOBJ(Texs);


    //myTexture->loadTexture("images/skybox.png");
    //tex = myTexture->textID;
    //sky->loadOBJ("models/sky/testSky.obj","models/sky/testSky.mtl");
    //vector<char *> Texs;
    //Texs.push_back("models/sky/sky_water_landscape.jpg");
    //sky->textureOBJ(Texs);
    //bool res = obj->loadOBJ("models/cube.obj",vertices,uvs,normals);

    plyr->bounds = 0.6;

    level = 3;
    timeLimit = 60000;


}

void _Scene::resetScoreAndCollectibles()
{
    coinScore = 0;
    dollarScore = 0;
    totalScore = 0;

    pickupTextTimer = 0.0;
    lastPickupValue = 0;

    collectableSpawnTimer = 0.0;

    for(int i = 0; i < MAX_COINS; ++i){
        coins[i].active = false;
    }

    for(int i = 0; i < MAX_DOLLARS; ++i){
        dollars[i].active = false;
    }
    if(plyrScore){
        plyrScore->resetScore();
    }
}
void _Scene::spawnCollectibles()
{
    if(coinScore >= MAX_COINS && dollarScore >= MAX_DOLLARS)
        return;

    //adding a 1:7 ratio (dollars are rare). rolls 1 through 8. 1 = dollar and 2-8 = coins
    int roll = (int)rand8(rng);
    bool spawnDollar = (roll <= 3);

    float laneX = 0;
    float zStart = 0;

    if(spawnDollar && dollarScore < MAX_DOLLARS){
        //spawn in active dollar (dollar that haven't got collected)
        for(int i = 0; i < MAX_DOLLARS; ++i){
            if(!dollars[i].active){
                if (numLanes == 6) laneX = 1.05 - (rand6(rng) * 0.30);
                else if (numLanes == 4) laneX = 0.9 - (rand4(rng) * 1.4 / numLanes);
                zStart = 20.0 + (float)(rand6(rng) * 5.0);

                dollars[i].init(laneX, 0.2, zStart, 0.08, (char*)"images/collectibles/dollar.png");

                break;
            }
        }
    }else{
        if(coinScore >=MAX_COINS)return;

        for(int i = 0; i < MAX_COINS; ++i){
            if(!coins[i].active){
                if (numLanes == 6) laneX = 1.05 - (rand6(rng) * 0.30);
                else if (numLanes == 4) laneX = 0.9 - (rand4(rng) * 1.4 / numLanes);

                zStart = 20.0 + (float)(rand6(rng) * 5.0);

                coins[i].init(laneX, 0.2, zStart, 0.08, (char*)"images/collectibles/coin.png");
                break;
            }
        }
    }
}

void _Scene::checkCollectibleCollisions()
{
     if(gameState != inGame) return;

    float px = plyr->pos.x;
    float pz = plyr->pos.z;
    float pr = 0.3;   // player radius

    // coins
    for(int i = 0; i < MAX_COINS; ++i){
        if(!coins[i].active) continue;

        float dx = px - coins[i].x;
        float dz = pz - coins[i].z;
        float dist2 = dx*dx + dz*dz;
        float rSum = pr + coins[i].size;

        if(dist2 <= rSum/10){
            coins[i].deactivate();
            coinScore++;
            totalScore += 2;

            timeLimit += 2000;

            lastPickupValue = 2;
            pickupTextTimer = 0.8;  // shows points float after collecting coin

            lastPickupPos.x = coins[i].x;
            lastPickupPos.y = coins[i].y + 0.3;
            lastPickupPos.z = coins[i].z;

            // to add a sound do it here
            menuMsc->playSounds("sounds/coinDing.mp3");
        }
    }

    //dollars
    for(int i = 0; i < MAX_DOLLARS; ++i){
        if(!dollars[i].active) continue;

        float dx = px - dollars[i].x;
        float dz = pz - dollars[i].z;
        float dist2 = dx*dx + dz*dz;
        float rSum = pr + dollars[i].size;

        if(dist2 <= rSum * rSum){
            dollars[i].deactivate();
            dollarScore++;
            totalScore += 4;

            timeLimit += 4000;

            lastPickupValue = 4;
            pickupTextTimer = 0.8;

            lastPickupPos.x = dollars[i].x;
            lastPickupPos.y = dollars[i].y + 0.3;
            lastPickupPos.z = dollars[i].z;

            //sound
            menuMsc->playSounds("sounds/cashDing.mp3");
        }
    }

}

void _Scene::updateCollectibles(float dt)
{
    //adding spinning animation to all collectibles
    for(int i = 0; i < MAX_COINS; ++i){
        if(!coins[i].active) continue;
        coins[i].rotZ += coins[i].spinSpeed * dt;
        if(coins[i].rotZ > 360.0) coins[i].rotZ -= 360.0;
    }

    for(int i = 0; i < MAX_DOLLARS; ++i){
        if(!dollars[i].active) continue;
        dollars[i].rotZ += dollars[i].spinSpeed * dt;
        if(dollars[i].rotZ > 360.0) dollars[i].rotZ -= 360.0;
    }


    //spawning timer
    collectableSpawnTimer += dt;

    // spawn something ever 5 seconds
    if(collectableSpawnTimer >= 5.0){
        collectableSpawnTimer = 0.0;

        if(rand2(rng) == 1){
            spawnCollectibles();
        }
    }

    // move active collectibles with the road
    if(animationTimer->getTicks() >= 10){
        for(int i = 0; i < MAX_COINS; ++i){
            if(!coins[i].active) continue;
            coins[i].z -= (0.1 * plyr->speed);
            if(coins[i].z <= -20.0){
                coins[i].active = false;
            }
        }
        for(int i = 0; i < MAX_DOLLARS; ++i){
            if(!dollars[i].active)continue;
            dollars[i].z -= (0.1 * plyr->speed);
            if(dollars[i].z <= -20.0){
                dollars[i].active = false;
            }
        }
    }
}

bool _Scene::isSafeCollectibleSpawn(float lanx, float zStart)
{
    const float safeZ = 5.0;  // the minimum space allowed for collectibles to spawn near a car
    for(int i = 0; i < 10; ++i){  // 10 is the amount of cars we have
        float dz = fabs(zStart - obstcls[i].pos.z);

        if(dz < safeZ){
            return false;
        }
    }
    return true;
}

void _Scene::drawLevel()
{
    switch(level) {
    case 1:
        glPushMatrix();
            tunnel->drawOBJ();
        glPopMatrix();
        break;
    case 2:
        glPushMatrix();
            glDisable(GL_FOG);
            daySky->drawSkyBox();
            glEnable(GL_FOG);
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
    case 3:
        glPushMatrix();
            bridge->drawOBJ();
            glDisable(GL_FOG);
            daySky->drawSkyBox();
            glEnable(GL_FOG);
        glPopMatrix();
        break;
    }
}

void _Scene::drawScene()
{

   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear bits in each itteration
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45,(float)width/(float)height,0.1,1000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   updateInGame();

   switch (gameState) {
   case inGame:
       if (!paused) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            myCam->fov = 45 * (plyr->speed/plyr->maxSpeed + 1);
            gluPerspective(myCam->fov,(float)width/(float)height,0.1,1000.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            switch(camMode)
            {
            case 0: //defualt camera.. mode 0
            {
                const float orbitSpeed = 1.5; // rotation speed
                float maxOrbit = 75.0; // max degrees it can rotate
                const float returnSpeed = 2.0; // speed of spring back

                if(level == 2){
                    maxOrbit = 20.0;
                }

                if(rotateLeft) camOrbitAngle += orbitSpeed;
                if(rotateRight) camOrbitAngle -= orbitSpeed;

                if(!rotateLeft && !rotateRight){
                    if(camOrbitAngle > 0.0){
                        camOrbitAngle -= returnSpeed;
                        if(camOrbitAngle < 0.0) camOrbitAngle = 0.0;
                    }else if(camOrbitAngle < 0.0){
                    camOrbitAngle += returnSpeed;
                    if(camOrbitAngle > 0.0) camOrbitAngle = 0.0;
                    }
                }

                if(camOrbitAngle > maxOrbit) camOrbitAngle = maxOrbit;
                if(camOrbitAngle < -maxOrbit) camOrbitAngle = -maxOrbit;


                myCam->des = plyr->pos;
                myCam->des.y += 0.5;
                myCam->rotAngle.x = 180 + camOrbitAngle;
                myCam->rotAngle.y = 0.0;
                myCam->distance = 4.0 / (1 +  2 * (plyr->speed/plyr->maxSpeed));
                myCam->rotateXY();
                break;
            }
            case 1: // driver pov...mode 1
                myCam->eye = plyr->pos;
                myCam->eye.y += 0.25; //above car's center
                myCam->eye.z -= 0.10; // inside car

                // look at road
                myCam->des = myCam->eye;
                myCam->des.z += 1.0;
                myCam->up.x = 0.0;
                myCam->up.y = 1.0;
                myCam->up.z = 0.0;
                break;
            case 2: // hood view..mode 2
                myCam->eye = plyr->pos; // in front of car and a bit lower
                myCam->eye.y += 0.15;   // hood height
                myCam->eye.z += 0.60;  // front of car

                //look ahead
                myCam->des = myCam->eye;
                myCam->des.z += 1.5;

                myCam->up.x = 0.0;
                myCam->up.y = 1.0;
                myCam->up.z = 0.0;
                break;
            }
            if (playerHealth.isFlashing()) {// && animationTimer->getTicks() >= 9) {
                // how strong the shake feels – tweak if too weak/strong
                const float strength = 0.08;


                // rand100(rng) gives [1..100], normalize to [-1..1]
                float nx = ((float)rand100(rng) / 50.0) - 1.0;
                float ny = ((float)rand100(rng) / 50.0) - 1.0;
                float nz = ((float)rand100(rng) / 50.0) - 1.0;
                myCam->eye.x += nx * strength;
                myCam->eye.y += ny * strength * 0.5;  // smaller vertical shake
                myCam->eye.z += nz * strength;
            }
            myCam->setUpCamera();

            //myCam->setUpCamera();
            glPushMatrix();
                if (playerHealth.isFlashing()) {
                    glColor3f(1.0, 0.2, 0.2);   // red tint when recently hit
                } else {
                    glColor3f(1.0, 1.0, 1.0);   // normal color
                }
                plyr->rot.x = 0;
                plyr->drawPlayer();
            glPopMatrix();

                //adding break lights
                glPushMatrix();
                    glTranslatef(plyr->pos.x, plyr->pos.y, plyr->pos.z);
                    glRotatef(plyr->rot.x, 1, 0, 0);
                    glRotatef(plyr->rot.x, 0, 1, 0);
                    glRotatef(plyr->rot.x, 0, 0, 1);
                    glScalef(plyr->scale, plyr->scale, plyr->scale);

                    glDisable(GL_TEXTURE_2D);
                    glDisable(GL_LIGHTING);

                    if(plyr->braking){
                        glColor3f(1.0,0.0,0.0);

                        float bw = 0.06;
                        float bh = 0.03;
                        float zBack = -0.55;
                        float xOffB = 0.15;
                        float yOffB = 0.05;

                        glBegin(GL_QUADS);
                        //right break light
                            glVertex3f(xOffB - bw, yOffB - bh, zBack);
                            glVertex3f(xOffB + bw, yOffB - bh, zBack);
                            glVertex3f(xOffB + bw, yOffB + bh, zBack);
                            glVertex3f(xOffB - bw, yOffB + bh, zBack);
                        // left brake light
                            glVertex3f(-xOffB - bw, yOffB - bh, zBack);
                            glVertex3f(-xOffB + bw, yOffB - bh, zBack);
                            glVertex3f(-xOffB + bw, yOffB + bh, zBack);
                            glVertex3f(-xOffB - bw, yOffB + bh, zBack);
                        glEnd();
                    }
                    glEnable(GL_LIGHTING);
                    glEnable(GL_TEXTURE_2D);
                glPopMatrix();

            glColor3f(1.0,1.0,1.0);

            //drawLevel();
            if (playerHealth.isDead()) glEnable(GL_FOG);
            if (level != 2) daySky->drawSkyBox();
            if (!playerHealth.isDead()) glEnable(GL_FOG);
            levels->drawLevel();

            if (animationTimer->getTicks()>= 10 && !playerHealth.isDead()) {
                for (int i = 0; i < 10; i++) {
                    if (level != 3 || obstcls[i].pos.x < 0) obstcls[i].pos.z -= (0.2 * plyr->speed) - 0.05;
                    else obstcls[i].pos.z -= (0.2 * plyr->speed) + 0.05;
                    if(obstcls[i].pos.z <= -20) {
                        obstcls[i].pos.z = 40;
                        if (numLanes == 6) obstcls[i].pos.x = 1.2 - (rand6(rng) * (2.0/numLanes));
                        if (numLanes == 4) {
                            obstcls[i].pos.x = 0.9 - (rand4(rng) * (1.4/numLanes));
                            if (obstcls[i].pos.x > 0 && level == 3) obstcls[i].rot.z = 90;
                            else obstcls[i].rot.z = 270;
                        }
                    for (int j = 0;j < 10; j++) {
                        if (j != i && myCol->isSphereCol(obstcls[i].pos,obstcls[j].pos,.1,.05,0.01)) {
                            obstcls[i].pos.z  = -20;
                            break;
                        }
                    }
                    }
                }
                levels->updateLevel(plyr->speed);
                animationTimer->reset();
            }
            for (int i = 0; i < 10; i++){
                glPushMatrix();
                obstcls[i].drawObstacle(obstacleMdl);
                glPopMatrix();
            }

            //drawing coin and dollar
            for(int i = 0; i < MAX_COINS; ++i)
                coins[i].draw();
            for(int i = 0; i < MAX_DOLLARS; ++i)
                dollars[i].draw();
            //placing score top left
            //char scoreStr[16];
            //sprintf(scoreStr, "%d", totalScore);
            glDisable(GL_FOG);
            glColor3f(1.0,1.0,1.0);


            //plyrScore->updateScore(plyr->speed);
            plyrScore->playerScore = totalScore;
            plyrScore->updateScore(1);
            glDisable(GL_DEPTH_TEST);
            glPushMatrix();
                glTranslatef(myCam->eye.x,myCam->eye.y,myCam->eye.z);
                glTranslatef(1.30,1.8,-3.0 * plyr->speed);
                glRotatef(180,1,0,0);
                glRotatef(180,0,0,1);
                if (level != 2) textNum->drawText(plyrScore->strScore,0.28);
                else textNumWhite->drawText(plyrScore->strScore,0.28);
            glPopMatrix();

            // showing the floating +1 and +4
            if(pickupTextTimer > 0.0){
                char popupStr[8];
                if(lastPickupValue == 4) strcpy(popupStr, "Z4");
                else                     strcpy(popupStr, "Z2");

                float t = pickupTextTimer / 0.8;
                float lift = (1.0 - t) * 0.5;

                glColor3f(1.0, 1.0, 1.0);

                glPushMatrix();

                    glTranslatef(lastPickupPos.x + 0.4, lastPickupPos.y + lift, lastPickupPos.z);

                    glRotatef(180,1,0,0);
                    glRotatef(180,0,0,1);
                    if (level != 2) textNum->drawText(popupStr, 0.3);
                    else textNumWhite->drawText(popupStr,0.3);

                glPopMatrix();
            }

            dur = inGameTimer->getDuration();
            dur = timeLimit - dur;
            if (dur <= 0) gameState = gameOver;
            m = dur/60000;
            s = (dur % 60000) / 1000;
            ms = (dur % 1000) / 10;
            //sprintf(timerStr, "%d%c%d%c%d", m,'N',s,'N',ms);
            glPushMatrix();
                glTranslatef(myCam->eye.x,myCam->eye.y,myCam->eye.z);
                glTranslatef(-0.10,1.8,-3.0 * plyr->speed);
                glRotatef(180,1,0,0);
                glRotatef(180,0,0,1);
                if (level != 2) textNum->drawTime(m,s,ms,0.3);
                else textNumWhite->drawTime(m,s,ms,0.3);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(myCam->eye.x, myCam->eye.y, myCam->eye.z);
                glTranslatef(1.30, 1.55, -3.0 * plyr->speed);
                glRotatef(180,1,0,0);
                glRotatef(180,0,0,1);
                if(level != 2){
                    textUpper->drawText((char*)"PRESS AND HOLD H TO LEARN HOW TO PLAY", 0.12);
                }
                else{
                    textUpperWhite->drawText((char*)"PRESS AND HOLD H TO LEARN HOW TO PLAY", 0.12);
                }
            glPopMatrix();
            if(showInGameHelp){
                glPushMatrix();
                    glDisable(GL_TEXTURE_2D);
                    glColor4f(0.0, 0.0, 0.0, 0.5);

                    glBegin(GL_QUADS);
                        glVertex3f(-1.9, 1.3, -2.5);
                        glVertex3f( 1.9, 1.3, -2.5);
                        glVertex3f( 1.9, -0.5, -2.5);
                        glVertex3f(-1.9, -0.5, -2.5);
                    glEnd();

                    glEnable(GL_TEXTURE_2D);
                glPopMatrix();

                //drawing bullet points for help menu
                glColor3f(1.0,1.0,1.0);

                glPushMatrix();
                    glTranslatef(myCam->eye.x, myCam->eye.y, myCam->eye.z);
                    glTranslatef(-0.80, 1.3, -3.0 * plyr->speed);
                    glRotatef(180, 1, 0, 0);
                    glRotatef(180, 0, 0, 1);

                    _textDisplay* helpFont = (level != 2) ? textUpper : textUpperWhite;

                    helpFont->drawText((char*)"HOW TO PLAY", 0.15);
                    glTranslatef(0.0, -0.12, 0.0);

                    helpFont->drawText((char*)"- W TO ACCELERATE", 0.10);
                    glTranslatef(0.0, -0.12, 0.0);

                    helpFont->drawText((char*)"- A TO TURN LEFT", 0.10);
                    glTranslatef(0.0, -0.12, 0.0);

                    helpFont->drawText((char*)"- D TO TURN RIGHT ", 0.10);
                    glTranslatef(0.0, -0.12, 0.0);

                    helpFont->drawText((char*)"- S TO BRAKE", 0.10);
                    glTranslatef(0.0, -0.12, 0.0);

                    helpFont->drawText((char*)"- P TO CHANGE POV", 0.10);
                    glTranslatef(0.0, -0.12, 0.0);

                    helpFont->drawText((char*)"- J TO ROTATE CAMERA LEFT", 0.10);
                    glTranslatef(0.0, -0.12, 0.0);

                    helpFont->drawText((char*)"- L TO ROTATE CAMERA RIGHT", 0.10);
                glPopMatrix();
            }
            glEnable(GL_DEPTH_TEST);
        }
       else {
            myCam->camReset();
            glPushMatrix();
                glScalef(4.33,4.33,1.0);
                pauseMenuBackground.drawParallax(width,height);
            glPopMatrix();
            glPushMatrix();
                pauseMenuElements[pauseResume].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,pauseMenuElements[pauseResume].pos,0,0,1.8,1.0));
                pauseMenuElements[pauseExit].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,pauseMenuElements[pauseExit].pos,0,0,1.8,1.0));
            glPopMatrix();
            if (pauseMenuElements[pauseResume].clicked) {
                pauseMenuElements[pauseResume].clicked = false;
                paused = false;
                myCam->eye = plyr->pos;
                myCam->des = plyr->pos;
                myCam->eye.z -= 3;
                myCam->eye.y += 1;
                inGameTimer->resume();
            }
            else if (pauseMenuElements[pauseExit].clicked) {
                    pauseMenuElements[pauseExit].clicked = false;
                    //gameState = mainMenu;
                    paused = false;

                    resetScoreAndCollectibles();
                    playerHealth.reset();
                    plyr->crashed = false;
                    pendingGameOver = false;
                    justCrashed = false;
                    timeLimit = 60000;
                    inGameTimer->start();
                    gameState = mainMenu;

                    plyr->pos = plyr->menuPos;
                    plyr->rot = plyr->menuRot;
                    plyr->scale = 1.0;
                    menuMsc->playMusic("sounds/04 GARAGE TALK.mp3");
            }

        }
        break;
      case gameOver:
        // 1. Draw the world frozen (same camera as inGame but no updateInGame)
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45 * (plyr->speed + 1),(float)width/(float)height,0.1,1000.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        myCam->des = plyr->pos;
        myCam->des.y += 0.5;
        myCam->rotAngle.x = 180;
        myCam->rotAngle.y = 0;
        myCam->distance = 4.0 / (1 +  2 * plyr->speed);
        myCam->rotateXY();

        myCam->setUpCamera();

        // Draw player & obstacles exactly like inGame, but without moving them
        glPushMatrix();
            plyr->drawPlayer();
        glPopMatrix();

        glEnable(GL_FOG);

        levels->drawLevel();

        for (int i = 0; i < 10; i++) {
            glPushMatrix();
                obstcls[i].drawObstacle(obstacleMdl);
            glPopMatrix();
        }

        glPushMatrix();
            daySky->drawSkyBox();
        glPopMatrix();

        // 2. Draw dark overlay in screen space
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, 0, height);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_BLEND);
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        glBegin(GL_QUADS);
            glVertex2f(0,      0);
            glVertex2f(width,  0);
            glVertex2f(width,  height);
            glVertex2f(0,      height);
        glEnd();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        //glDisable(GL_BLEND);

        // 3. Go back to the same projection style that the menu uses
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45,(float)width/(float)height,0.1,1000.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_FOG);
        // Text (uses your PNG font)
        glPushMatrix();
            glTranslatef(-2.0,1.0,0);
            textUpperWhite->drawText((char*)"YOU CRASHED", 0.4f);
            glTranslatef(0.4f, -0.4f, 0.0f);
            textUpperWhite->drawText((char*)"YOU CANT PARK THERE", 0.2f);
        glPopMatrix();

        // 4. Buttons – SAME pattern as main menu (no ortho here)
        glPushMatrix();
            gameOverButtons[goPlayAgain].drawButton(
                width/10,
                height/10,
                myCol->isPlanoCol(mousePos, gameOverButtons[goPlayAgain].pos, 0,0,1.8,1.0)
            );
            gameOverButtons[goMainMenu].drawButton(
                width/10,
                height/10,
                myCol->isPlanoCol(mousePos, gameOverButtons[goMainMenu].pos, 0,0,1.8,1.0)
            );
        glPopMatrix();

        // 5. Handle clicks for game over buttons
        if (gameOverButtons[goPlayAgain].clicked) {
            plyrScore->resetScore();
            plyr->crashed = false;
            gameOverButtons[goPlayAgain].clicked = false;

            levels->setUpLevel(level,numLanes,plyr);
            resetScoreAndCollectibles();
            // Reset player state (same as starting a new game from main menu)
            plyr->rot.x = 0;
            plyr->rot.y = 0;
            plyr->rot.z = 0;
            //plyr->scale = 0.1f;
            plyr->pos.x = 0.0f;
            //plyr->pos.z = 0.0f;
            //plyr->pos.y = 0.11f;
            plyr->speed = 0.0f;
            plyr->accelerating = false;

            resetObstacles();

            // Reset camera like main menu "New Game"
            myCam->eye = plyr->pos;
            myCam->des = plyr->pos;
            myCam->eye.z -= 3.0f;
            myCam->eye.y += 1.0f;

            // Reset health
            playerHealth.reset();
            timeLimit = 60000;
            inGameTimer->start();

            // Stay in gameplay, keep game music
            gameState = inGame;
        }
        else if (gameOverButtons[goMainMenu].clicked) {
            plyrScore->resetScore();
            plyr->crashed = false;
            gameOverButtons[goMainMenu].clicked = false;
            resetScoreAndCollectibles();

            // Reset player + obstacles + health
            plyr->rot = plyr->menuRot;
            plyr->scale = 1.0f;
            plyr->pos = plyr->menuPos;
            plyr->speed = 0.0f;
            plyr->accelerating = false;

            for (int i = 0; i < 10; i++) {
                obstcls[i].rot.x = obstcls[i].rot.y = 0;
                obstcls[i].rot.z = 270;
                obstcls[i].scale = 0.1f;
                obstcls[i].pos.y = 0.11f;
                obstcls[i].pos.x = 1.05f - (rand6(rng) * 0.30f);
                obstcls[i].pos.z = 15 + i * 6;
            }

            playerHealth.reset();

            // Switch back to menu music
            menuMsc->playMusic("sounds/04 GARAGE TALK.mp3");
            gameState = mainMenu;
        }

        glEnable(GL_DEPTH_TEST);
        justCrashed = false;    // we’ve shown the screen now
        break;



   case mainMenu:
       glPushMatrix();
            glScalef(4.33,4.33,1.0);
            glColor3f(1.0,1.0,1.0);
            menuBackground.drawParallax(width,height);
        glPopMatrix();
       if(helpMenu) {
            glPushMatrix();
                glScalef(4.33,4.33,1.0);
                helpMenuBackground.drawParallax(width,height);
            glPopMatrix();
            helpMenuReturn.drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,helpMenuReturn.pos,0,0,1.8,1.0));
            if (helpMenuReturn.clicked) { helpMenuReturn.clicked = false; helpMenu = false; }
        }
        else if (levelSelect) {
            glPushMatrix();
                for (int i = 0; i < 4; i++) levelSelectButtons[i].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,levelSelectButtons[i].pos,0,0,1.8 * levelSelectButtons[i].scale,1.0 * levelSelectButtons[i].scale));
            glPopMatrix();
            for (int i = 0; i < 3; i++) {
                if (levelSelectButtons[i].clicked) {
                    levelSelectButtons[i].clicked = false;
                    level = i + 1;

                    resetScoreAndCollectibles();
                    playerHealth.reset();

                    levels->setUpLevel(level,numLanes,plyr);
                    levelSelect = false;
                    resetObstacles();
                    //gameState = inGame;
                    resetObstacles();       // make sure cars are in front again
                    playerHealth.reset();   // reset health on new game
                    plyr->rot.x = 0;
                    plyr->rot.y = 0;
                    plyr->rot.z = 0;
                    plyr->scale = 0.15;
                    plyr->pos.x = plyr->pos.z = 0;
                    plyr->pos.y = 0.05;
                    plyr->speed = 0;

                    camOrbitAngle = 0.0;  //reset orbit when levels are selected
                    myCam->eye = plyr->pos;
                    myCam->des = plyr->pos;
                    myCam->eye.z -= 3;
                    myCam->eye.y += 1;

                    timeLimit = 60000;
                    inGameTimer->start();
                    gameState = inGame;
                    menuMsc->playMusic("sounds/14 Quiet Curves.mp3");
                }
            }
            if (levelSelectButtons[3].clicked) {
                levelSelectButtons[3].clicked = false;
                levelSelect = false;
            }
        }
        else {
        plyr->movement = plyr->menu;
        myCam->eye.x = myCam->eye.y = myCam->eye.z = 0;
        myCam->des.x = myCam->des.y = 0;
        myCam->des.z = -10;
        myCam->eye.x = myCam->eye.y = myCam->eye.z = 0;
        glPushMatrix();
            glPushMatrix();
                plyr->rot.x = 30;
                plyr->drawPlayer();
            glPopMatrix();
            if (animationTimer->getTicks() >= 10) { plyr->rot.y += 0.2; animationTimer->reset(); }
        glPopMatrix();
        glPushMatrix();
            glColor3f(1.0,1.0,1.0);
            mainMenuElements[newGame].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[newGame].pos,0,0,1.8,1.0));
            mainMenuElements[help].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[help].pos,0,0,2,1.0));
            mainMenuElements[exit].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[exit].pos,0,0,1.8,1.0));
        glPopMatrix();
        if (mainMenuElements[newGame].clicked) {
            mainMenuElements[newGame].clicked = false;
            /*resetObstacles();       // make sure cars are in front again
            playerHealth.reset();   // reset health on new game
            plyr->rot.x = 0;
            plyr->rot.y = 0;
            plyr->rot.z = 0;
            plyr->scale = 0.15;
            plyr->pos.x = plyr->pos.z = 0;
            plyr->pos.y = 0.05;
            myCam->eye = plyr->pos;
            myCam->des = plyr->pos;
            myCam->eye.z -= 3;
            myCam->eye.y += 1;
            levels->setUpLevel(level,numLanes,plyr);
            inGameTimer->start();
            gameState = inGame;
            menuMsc->playMusic("sounds/14 Quiet Curves.mp3");*/
            levelSelect = true;
        }

        if (mainMenuElements[help].clicked) { mainMenuElements[help].clicked = false; helpMenu = true; }
        if (mainMenuElements[exit].clicked) { mainMenuElements[exit].clicked = false; killWin = true; }
        }

        break;

    case landing:
        myCam->eye.x = myCam->eye.y = myCam->eye.z = 0;
        myCam->des.x = myCam->des.y = 0;
        myCam->des.z = -10;
        myCam->eye.x = myCam->eye.y = myCam->eye.z = 0;
        glPushMatrix();
            glScalef(4.33,4.33,1.0);
            landingPage->drawParallax(width,height);
        glPopMatrix();
        break;

   }
}

void _Scene::mouseMapping(int x, int y)
{
    GLint viewPort[4];
    GLdouble ModelViewM[16];
    GLdouble projectionM[16];
    GLfloat winX,winY,winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewM);
    glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
    glGetIntegerv(GL_VIEWPORT,viewPort);

    winX =(GLfloat)x;
    winY = (GLfloat)y;


    glReadPixels(x,(int)winY,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
    gluUnProject(winX,winY,0.0,projectionM,ModelViewM,viewPort,&msX,&msY,&msZ);                 // get mouse pos without z (for buttons)
    mousePos.y = -msY * 100;
    mousePos.x = msX * 100;
    glReadPixels(x,(int)winY,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
    gluUnProject(winX,winY,winZ,projectionM,ModelViewM,viewPort,&msX,&msY,&msZ);
}

void _Scene::updateInGame()
{
    // Approximate 60 FPS for now
    const float dt = 0.016f;

    // -----------------------------------------
    // 1. Handle delayed Game Over (third crash)
    // -----------------------------------------
    if (pendingGameOver)
    {
        for (int i = 0; i < 4; i++) fogColor[i] -= (dt / (5 * levels->fogColor[i]));
        glFogf(GL_FOG_DENSITY,levels->fogDensity + (5 - ((crashDelayTimer -= dt)))/50);
        glFogfv(GL_FOG_COLOR,fogColor);

        // still allow flashing red effect during delay
        playerHealth.update(dt);

        if (crashDelayTimer <= 0.0f) {
            pendingGameOver = false;
            gameState = gameOver;
        }

        // Do NOT continue normal gameplay updates
        return;
    }

    // -----------------------------------------
    // 2. Normal gameplay updates
    // -----------------------------------------
    playerHealth.update(dt);

    checkPlayerObstacleCollisions();
    updateCollectibles(dt);
    checkCollectibleCollisions();

    // Update floating “+1 / +4” timer
    if (pickupTextTimer > 0.0f) {
        pickupTextTimer -= dt;
        if (pickupTextTimer < 0.0f)
            pickupTextTimer = 0.0f;
    }
}

void _Scene::checkPlayerObstacleCollisions()
{
    // Only process collisions while actually in gameplay
    if (gameState != inGame) return;

    // Player position in 2D (X, Z plane)
    vec3 p3 = plyr->pos;
    vec2 p2;
    p2.x = p3.x;
    p2.y = p3.z;

    // Player bounding box (tune these to match car model size)
    // tweak to change how size of hit box
    float pW = 0.08;
    float pH = 0.35;

    for (int i = 0; i < 10; ++i) { // you spawn 10 in initGL
        vec3 o3 = obstcls[i].pos;
        vec2 o2;
        o2.x = o3.x;
        o2.y = o3.z;

        float oW = 0.08;
        float oH = 0.35;

        if (myCol->isPlanoCol(p2, o2, pW, pH, oW, oH)) {

            // was the player already in the "hit" state?
            bool wasFlashing = playerHealth.isFlashing();

            playerHealth.registerHit();

            if (playerHealth.isDead()) {
                // play special third-crash sound
                fogColor[0] = levels->fogColor[0];
                fogColor[1] = levels->fogColor[1];
                fogColor[2] = levels->fogColor[2];
                fogColor[3] = levels->fogColor[3];
                menuMsc->playSounds("sounds/thirdCrash.mp3");
                plyr->crashed = true;

                // start countdown to game over
                pendingGameOver = true;
                crashDelayTimer = 5.0f;

                // freeze gameplay movement
                plyr->speed = 0.0;
                plyr->accelerating = false;
                paused = false;

                return;
            }

            // Otherwise, play normal crash sound only for a *new* hit
            if (!wasFlashing) {
                menuMsc->playSounds("sounds/crashSound.mp3");
            }
        }
    }
}


int _Scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            myInput->wParam = wParam;
//            myInput->keyPressed(plyr->mdl);
            //myInput->keyPressed(road);
            //myInput->keyPressed(daySky);
            //myInput->keyPressed(mySprite);
            //myInput->keyPressed(myCam);
            if (gameState == landing) gameState = mainMenu;
            if (gameState == inGame && wParam == 27 && escKeyRelease) {
                escKeyRelease = false; paused = !paused;
                if (paused) inGameTimer->pause();
                else inGameTimer->resume();
            }
            if (gameState == inGame) {
                if (wParam == 87) { // W
                    if (!plyr->accelerating) plyr->accelTmr = chrono::system_clock::now();
                    plyr->accelerating = true;
                }
                if(wParam == 83){ // S
                    plyr->braking = true;
                    plyr->accelerating = false; // stop accelerating if braking
                }
                if (wParam == 65 && (Apressed == false || Dpressed == false)) { Apressed = true; plyr->movement = plyr->left; }
                if (wParam == 68 && (Apressed == false || Dpressed == false)) { Dpressed = true; plyr->movement = plyr->right; }
                if(wParam == 'P'){
                camMode = (camMode + 1) % 3; // cycle through camera mode 1,2,3
                }
                if(wParam == 'H'){
                    showInGameHelp = true;
                }
                //rotate around car
                if(wParam == 'J' && camMode == 0){
                    rotateLeft = true;
                }
                if(wParam == 'L' && camMode == 0){
                    rotateRight = true;
                }
            }
 //           myInput->keyPressed(mdl3D,mdl3DW);
        break;

        case WM_KEYUP:
            myInput->wParam = wParam;
            plyr->movement = plyr->none;
            if (wParam == 87) plyr->accelerating = false;
            if(wParam == 83) plyr->braking = false;
            if(wParam == 'J') rotateLeft = false;
            if(wParam == 'L') rotateRight = false;
            if(wParam == 'H') showInGameHelp = false;
            if (wParam == 65) {
                Apressed = false;
                if (Dpressed == false) plyr->movement = plyr->none;
                else plyr->movement = plyr->right;
            }
            if (wParam == 68) {
                Dpressed = false;
                if (Apressed == false) plyr->movement = plyr->none;
                else plyr->movement = plyr->left;
            }
            //myInput->keyUp(mySprite);
            //mdl3D->actionTrigger=mdl3D->STAND;
            //mdl3DW->actionTrigger=mdl3DW->STAND;
            escKeyRelease = true;
        break;

        case WM_LBUTTONDOWN:
            myInput->wParam = wParam;
            if (gameState == mainMenu) {
                if (helpMenu) {
                    if (myCol->isPlanoCol(mousePos,helpMenuReturn.pos,0,0,1.8,1.0)) helpMenuReturn.clicked = true;
                }
                else if (levelSelect) {
                    for (int i = 0; i < 4; i++) {
                        if (myCol->isPlanoCol(mousePos,levelSelectButtons[i].pos,0,0,1.8 * levelSelectButtons[i].scale,1.0 * levelSelectButtons[i].scale)) { levelSelectButtons[i].clicked = true; break; }
                    }
                }
                else if (myCol->isPlanoCol(mousePos,mainMenuElements[newGame].pos,0,0,1.8,1.0)) mainMenuElements[newGame].clicked = true;
                else if (myCol->isPlanoCol(mousePos,mainMenuElements[help].pos,0,0,1.8,1.0)) mainMenuElements[help].clicked = true;
                else if (myCol->isPlanoCol(mousePos,mainMenuElements[exit].pos,0,0,1.8,1.0)) mainMenuElements[exit].clicked = true;
            }

            else if (gameState == inGame && paused) {
                if (myCol->isPlanoCol(mousePos,pauseMenuElements[pauseResume].pos,0,0,1.8,1.0)) pauseMenuElements[pauseResume].clicked = true;
                else if (myCol->isPlanoCol(mousePos,pauseMenuElements[pauseExit].pos,0,0,1.8,1.0)) pauseMenuElements[pauseExit].clicked = true;
            }
            else if (gameState == gameOver) {
                if (myCol->isPlanoCol(mousePos, gameOverButtons[goPlayAgain].pos, 0,0,1.8,1.0)) {
                    gameOverButtons[goPlayAgain].clicked = true;
                }
                else if (myCol->isPlanoCol(mousePos, gameOverButtons[goMainMenu].pos, 0,0,1.8,1.0)) {
                    gameOverButtons[goMainMenu].clicked = true;
                }
            }
            if (gameState == landing) gameState = mainMenu;
             mouseMapping(LOWORD(lParam), HIWORD(lParam));
//            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;

        case WM_RBUTTONDOWN:
            myInput->wParam = wParam;
//            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;

         case WM_MBUTTONDOWN:
             myInput->wParam = wParam;
//             myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            myInput->wParam = wParam;
            myInput->mouseEventUp();
            break;

        case WM_MOUSEMOVE:
              myInput->wParam = wParam;
              mouseMapping(LOWORD(lParam), HIWORD(lParam));
//              myInput->mouseMove(myModel,LOWORD(lParam),HIWORD(lParam));
            break;
        case WM_MOUSEWHEEL:
              myInput->wParam = wParam;
//              myInput->mouseWheel(myModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
            break;

        default:
            break;

    }
}
