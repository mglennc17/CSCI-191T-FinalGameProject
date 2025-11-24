#include "_scene.h"

_Scene::_Scene()
{
    //ctor
    rng = mt19937(dev());
    rand100 = uniform_int_distribution<mt19937::result_type>(1,100);
    rand6 = uniform_int_distribution<mt19937::result_type>(1,6);
    rand2 = uniform_int_distribution<mt19937::result_type>(1,2);
    animationTimer->startTime = clock();
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
        obstcls[i].pos.x = 1.05 - (rand6(rng) * 0.30);
        //if (i % 3) obstcls[i].pos.x = -0.7;
        //else if (i % 2) obstcls[i].pos.x = 0.7;
        obstcls[i].pos.z = 15.0 + i * 6.0;
    }
}

void _Scene::initGL()
{

    gameState = mainMenu;
    frameCount = 0;

    glShadeModel(GL_FLAT); // to handle GPU shaders
    glClearColor(0.0f,0.0f,0.0f,0.0f); // black background color
    glClearDepth(2.0f);         //depth test for layers
    glEnable(GL_DEPTH_TEST);    //activate depth test
    glDepthFunc(GL_LEQUAL);     // depth function type

    glEnable(GL_TEXTURE_2D);

    myLight->setLight(GL_LIGHT0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    obstacleMdl->initModel("models/car/Ford_Focus.md2","models/car/Tex.png");
    obstacleMdl->actionTrigger = obstacleMdl->STAND;

    daySky->skyBoxInit();
    daySky->tex[0] = daySky->textures->loadTexture("images/skybox/day/back.png");
    daySky->tex[1] = daySky->textures->loadTexture("images/skybox/day/front.png");
    daySky->tex[2] = daySky->textures->loadTexture("images/skybox/day/top.png");
    daySky->tex[3] = daySky->textures->loadTexture("images/skybox/day/bottom.png");
    daySky->tex[4] = daySky->textures->loadTexture("images/skybox/day/right.png");
    daySky->tex[5] = daySky->textures->loadTexture("images/skybox/day/left.png");

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

    road->parallaxInit("images/textures/road.png");
    road->xMax = 25;
    road->yMax = 2;

    ground->parallaxInit("images/textures/dirt.jpg");
    ground->xMax = 50;
    ground->yMax = 50;

    plyr->mdl->initModel("models/car/Ford_Focus.md2","models/car/Tex.png");//,"models/car/Tex.png");
    plyr->mdl->actionTrigger = plyr->mdl->STAND;
    plyr->pos.x = 0;
    plyr->pos.y = 1;
    plyr->pos.z = -7;
    plyr->scale = 1.0;

    resetObstacles();
    myCam->camInit();

    menuMsc->initSounds();
    menuMsc->playMusic("sounds/04 GARAGE TALK.mp3");

    textUpper->textInit("images/fonts/upper.png",9,3);
    textLower->textInit("images/fonts/lower.png",9,3);
    textNum->textInit("images/fonts/numEtc.png",9,3);

    //bool res = obj->loadOBJ("models/cube.obj",vertices,uvs,normals);


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
            gluPerspective(45 * (plyr->speed + 1),(float)width/(float)height,0.1,1000.0);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            myCam->des = plyr->pos;
            myCam->des.y += 0.5;
            myCam->rotAngle.x = 180;
            myCam->rotAngle.y = 0;
            myCam->distance = 4.0 / (1 +  2 * plyr->speed);
            myCam->rotateXY();


            // screen shake when recently hit
            if (playerHealth.isFlashing()) {
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
                plyr->drawPlayer();
            glPopMatrix();
            // reset color so we don't tint other things
            glColor3f(1.0, 1.0, 1.0);
            if (animationTimer->getTicks()>= 10) {
                for (int i = 0; i < 20; i++) {
                    obstcls[i].pos.z -= (0.1 * plyr->speed);
                    if(obstcls[i].pos.z <= -20) { obstcls[i].pos.z = 40; obstcls[i].pos.x = 1.05 - (rand6(rng) * 0.30); }
                }
                ground->prlxScrollAuto("right",0.03 * plyr->speed);
                road->prlxScrollAuto("right", 0.03 * plyr->speed);
                animationTimer->reset();
            }
            for (int i = 0; i < 10; i++){
                glPushMatrix();
                obstcls[i].drawObstacle(obstacleMdl);
                glPopMatrix();
            }
            glPushMatrix();

                daySky->drawSkyBox();
                glPopMatrix();
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
        }
       else {
            myCam->camReset();
            //myCam->eye.x = myCam->eye.y = myCam->eye.z = 0;
            //myCam->des.x = myCam->des.y = 0;
            //myCam->des.z = -10;
            //myCam->eye.x = myCam->eye.y = myCam->eye.z = 0;
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
            }
            else if (pauseMenuElements[pauseExit].clicked) {
                    pauseMenuElements[pauseExit].clicked = false;
                    gameState = mainMenu;
                    paused = false;

                    plyr->pos.x = 0;
                    plyr->pos.y = 1;
                    plyr->pos.z = -7;
                    plyr->scale = 1.0;
                    menuMsc->playMusic("sounds/04 GARAGE TALK.mp3");
            }
        }
        break;

   case mainMenu:
       plyr->movement = plyr->menu;
        myCam->eye.x = myCam->eye.y = myCam->eye.z = 0;
        myCam->des.x = myCam->des.y = 0;
        myCam->des.z = -10;
        myCam->eye.x = myCam->eye.y = myCam->eye.z = 0;
        glPushMatrix();
            glScalef(4.33,4.33,1.0);
            menuBackground.drawParallax(width,height);
        glPopMatrix();
        glPushMatrix();
            plyr->drawPlayer();
            if (animationTimer->getTicks() >= 10) { plyr->rot.z += 0.2; animationTimer->reset(); }
        glPopMatrix();
        glPushMatrix();
            mainMenuElements[newGame].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[newGame].pos,0,0,1.8,1.0));
            mainMenuElements[help].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[help].pos,0,0,2,1.0));
            mainMenuElements[exit].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[exit].pos,0,0,1.8,1.0));
        glPopMatrix();
        if (mainMenuElements[newGame].clicked) {
            mainMenuElements[newGame].clicked = false;
            resetObstacles();       // make sure cars are in front again
            playerHealth.reset();   // reset health on new game
            plyr->rot.x = plyr->rot.y = 0;
            plyr->rot.z = 270;
            plyr->scale = 0.1;
            plyr->pos.x = plyr->pos.z = 0;
            plyr->pos.y = 0.11;
            myCam->eye = plyr->pos;
            myCam->des = plyr->pos;
            myCam->eye.z -= 3;
            myCam->eye.y += 1;
            gameState = inGame;
            menuMsc->playMusic("sounds/14 Quiet Curves.mp3");
        }

        if (mainMenuElements[help].clicked) { mainMenuElements[help].clicked = false; helpMenu = true; }
        if (mainMenuElements[exit].clicked) { mainMenuElements[exit].clicked = false; killWin = true; }

        if(helpMenu) {
            glPushMatrix();
                glScalef(4.33,4.33,1.0);
                helpMenuBackground.drawParallax(width,height);
            glPopMatrix();
            helpMenuReturn.drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,helpMenuReturn.pos,0,0,1.8,1.0));
            if (helpMenuReturn.clicked) { helpMenuReturn.clicked = false; helpMenu = false; }
        }
        glPushMatrix();
            textUpper->drawText("TEST TEXT",0.2);
            glTranslatef(0,-0.2,0);
            textLower->drawText("TEST LOWER",0.2);
            glTranslatef(0,-0.2,0);
            textNum->drawText("ABCDE",0.2);
        glPopMatrix();
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
    gluUnProject(winX,winY,winZ,ModelViewM,projectionM,viewPort,&msX,&msY,&msZ);
//    gluUnProject(winX,winY,-10.0,ModelViewM,projectionM,viewPort,&mousePos.x,&mousePos.y,&msZ);
    mousePos.y = -msY;
    mousePos.x = msX;
}

void _Scene::updateInGame()
{
    // Approximate 60 FPS for now; you can replace with real dt later if needed
    const float dt = 0.016;
    playerHealth.update(dt);
    checkPlayerObstacleCollisions();
}

void _Scene::checkPlayerObstacleCollisions()
{
    // Player position in 2D (X, Z plane)
    vec3 p3 = plyr->pos;
    vec2 p2;
    p2.x = p3.x;
    p2.y = p3.z;


    // Player bounding box (tune these to match your car model size)
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


        // was the player already in the “hit” state?
        bool wasFlashing = playerHealth.isFlashing();


        playerHealth.registerHit();


        // play crash sound only on a *new* hit
        if (!wasFlashing) {
            menuMsc->playSounds("sounds/crashSound.mp3");
        }


        // If we hit the max collisions, handle "game over"
        if (playerHealth.isDead()) {
            gameState = mainMenu;
            paused = false;
            plyr->pos.x = 0;
            plyr->pos.y = 1;
            plyr->pos.z = -7;
            plyr->scale = 1.0;
            playerHealth.reset();
            resetObstacles();
            menuMsc->playMusic("sounds/04 GARAGE TALK.mp3");
            return;
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
            }
            if (gameState == inGame) {
                if (wParam == 87) {
                    if (!plyr->accelerating) plyr->accelTmr = chrono::system_clock::now();
                    plyr->accelerating = true;
                }
                if (wParam == 65) plyr->movement = plyr->right;
                else if (wParam == 68) plyr->movement = plyr->left;
            }
 //           myInput->keyPressed(mdl3D,mdl3DW);
        break;

        case WM_KEYUP:
            myInput->wParam = wParam;
            plyr->movement = plyr->none;
            if (wParam == 87) plyr->accelerating = false;
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
                else if (myCol->isPlanoCol(mousePos,mainMenuElements[newGame].pos,0,0,1.8,1.0)) mainMenuElements[newGame].clicked = true;
                else if (myCol->isPlanoCol(mousePos,mainMenuElements[help].pos,0,0,1.8,1.0)) mainMenuElements[help].clicked = true;
                else if (myCol->isPlanoCol(mousePos,mainMenuElements[exit].pos,0,0,1.8,1.0)) mainMenuElements[exit].clicked = true;
            }

            else if (gameState == inGame && paused) {
                if (myCol->isPlanoCol(mousePos,pauseMenuElements[pauseResume].pos,0,0,1.8,1.0)) pauseMenuElements[pauseResume].clicked = true;
                else if (myCol->isPlanoCol(mousePos,pauseMenuElements[pauseExit].pos,0,0,1.8,1.0)) pauseMenuElements[pauseExit].clicked = true;
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
