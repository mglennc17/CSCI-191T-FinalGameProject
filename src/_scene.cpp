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

    glMatrixMode(GL_PROJECTION);  // To setup ptrojection
    glLoadIdentity();             // calling identity matrix
    gluPerspective(45, aspectRatio,0.1,1000.0); // setting perspective projection

    this->width = GetSystemMetrics(SM_CXSCREEN);
    this->height= GetSystemMetrics(SM_CYSCREEN);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             // calling identity matrix
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

    plyr->mdl->initModel("models/car/Ford_Focus.md2","models/car/Tex.png");//,"models/car/Tex.png");
    plyr->mdl->actionTrigger = plyr->mdl->STAND;
    plyr->mdl->pos.x = 0;
    plyr->mdl->pos.y = 1;
    plyr->mdl->pos.z = -7;
    plyr->scale = 1.0;

    //mdl3DW->initModel("models/Tekk/weapon.md2");

    myCam->camInit();

}

void _Scene::drawScene()
{

   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear bits in each itteration
   glLoadIdentity();             // calling identity matrix

   myCam->setUpCamera();
   myCam->des = plyr->mdl->pos;

   switch (gameState) {
   case inGame:
       break;

   case mainMenu:
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
            plyr->rot.z += 0.2;
        glPopMatrix();
        glPushMatrix();
            mainMenuElements[newGame].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[newGame].pos,0,0,1.8,1.0));
            mainMenuElements[help].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[help].pos,0,0,2,1.0));
            mainMenuElements[exit].drawButton(width/10,height/10,myCol->isPlanoCol(mousePos,mainMenuElements[exit].pos,0,0,1.8,1.0));
        glPopMatrix();
        if (mainMenuElements[newGame].clicked) { mainMenuElements[newGame].clicked = false; gameState = inGame; }
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

int _Scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            myInput->wParam = wParam;
//            myInput->keyPressed(plyr->mdl);
            myInput->keyPressed(road);
            myInput->keyPressed(daySky);
            myInput->keyPressed(mySprite);
            myInput->keyPressed(myCam);
 //           myInput->keyPressed(mdl3D,mdl3DW);
        break;

        case WM_KEYUP:
            myInput->wParam = wParam;
            myInput->keyUp(mySprite);
            mdl3D->actionTrigger=mdl3D->STAND;
            mdl3DW->actionTrigger=mdl3DW->STAND;
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
