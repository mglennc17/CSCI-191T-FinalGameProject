#include "_scene.h"

_Scene::_Scene()
{
    //ctor
    rng = mt19937(dev());
    rand100 = uniform_int_distribution<mt19937::result_type>(1,100);
    rand6 = uniform_int_distribution<mt19937::result_type>(1,6);
    rand2 = uniform_int_distribution<mt19937::result_type>(1,2);
    myTime->startTime = clock();
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


    myTexture->loadTexture("images/tex.jpg");
    myPrlx->parallaxInit("images/prlx.jpg");

    mySkyBox->skyBoxInit();
    mySkyBox->tex[0] = mySkyBox->textures->loadTexture("images/front.jpg");
    mySkyBox->tex[1] = mySkyBox->textures->loadTexture("images/back.jpg");
    mySkyBox->tex[2] = mySkyBox->textures->loadTexture("images/top.jpg");
    mySkyBox->tex[3] = mySkyBox->textures->loadTexture("images/bottom.jpg");
    mySkyBox->tex[4] = mySkyBox->textures->loadTexture("images/right.jpg");
    mySkyBox->tex[5] = mySkyBox->textures->loadTexture("images/left.jpg");
    mySkyBox->tex[6] = mySkyBox->textures->loadTexture("images/Stairs.jpg");

    mySprite->spriteInit("images/eg.png",6,4);
    mdl3D->initModel("models/car/Ford_Focus.md2");

    //mdl3DW->initModel("models/Tekk/weapon.md2");

    myCam->camInit();

}

void _Scene::drawScene()
{

    frameCount++;

    if (rand100(rng) == 1) {
        cars.push(_car(1,rand6(rng),rand2(rng)));
    }

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear bits in each itteration
    glLoadIdentity();             // calling identity matrix

    myCam->setUpCamera();

    glPushMatrix();
        glScalef(4.33,4.33,1);
        mySkyBox->drawSkyBox();
    glPopMatrix();

    glPushMatrix();

    glPopMatrix();

    carRot += 0.2;

   glPushMatrix();

        glTranslatef(0,0,-3);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glRotatef(carRot,0,0,1);
        //glTranslatef(0,-2.0,0);
        //glTranslatef(mdl3D->pos.x,mdl3D->pos.y,mdl3D->pos.z);

        mdl3D->Actions();
        //mdl3DW->Actions();
        //mdl3DW->Draw();
        mdl3D->Draw();
    glPopMatrix();
}

int _Scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            myInput->wParam = wParam;
            myInput->keyPressed(myModel);
            myInput->keyPressed(myPrlx);
            myInput->keyPressed(mySkyBox);
            myInput->keyPressed(mySprite);
            myInput->keyPressed(myCam);
            myInput->keyPressed(mdl3D,mdl3DW);
        break;

        case WM_KEYUP:
            myInput->wParam = wParam;
            myInput->keyUp(mySprite);
            mdl3D->actionTrigger=mdl3D->STAND;
            mdl3DW->actionTrigger=mdl3DW->STAND;
        break;

        case WM_LBUTTONDOWN:
            myInput->wParam = wParam;
            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;

        case WM_RBUTTONDOWN:
            myInput->wParam = wParam;
            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;

         case WM_MBUTTONDOWN:
             myInput->wParam = wParam;
             myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            myInput->wParam = wParam;
            myInput->mouseEventUp();
            break;

        case WM_MOUSEMOVE:
              myInput->wParam = wParam;
              myInput->mouseMove(myModel,LOWORD(lParam),HIWORD(lParam));
            break;
        case WM_MOUSEWHEEL:
              myInput->wParam = wParam;
              myInput->mouseWheel(myModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
            break;

        default:
            break;

    }
}
