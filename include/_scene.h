#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>
#include<_light.h>
#include<_model.h>
#include<_inputs.h>
#include<_textureloader.h>
#include<_parallax.h>
#include<_skybox.h>
#include<_sprite.h>
#include<_timer.h>
#include<_3dmodelloader.h>
#include <_camera.h>
#include <_car.h>
#include <_obstacle.h>
#include <_player.h>
#include <_button.h>
#include <_collisioncheck.h>
#include <_sounds.h>
#include <_objLoader.h>
#include <_playerHealth.h>
#include <_textDisplay.h>
#include <_score.h>
#include <_collectables.h>

class _Scene
{
    public:
        _Scene();           //constructor
        virtual ~_Scene();  //Destructor

        void mouseMapping(int,int);
        vec2 mousePos;
        double msX,msY,msZ;

        _sounds *menuMsc = new _sounds();

        _light *myLight = new _light();   //light settings
        _collisionCheck *myCol = new _collisionCheck();
        _inputs *myInput = new _inputs(); // input handle
        _textureLoader *myTexture = new _textureLoader();// for loading images
        _sprite *mySprite = new _sprite();
        _timer *animationTimer = new _timer();
        _camera *myCam = new _camera();
        _player *plyr = new _player();
        _3DModelLoader *obstacleMdl = new _3DModelLoader();
        _objLoader *obj = new _objLoader();
        _textDisplay *textUpper = new _textDisplay();
        _textDisplay *textLower = new _textDisplay();
        _textDisplay *textNum = new _textDisplay();
        _score *plyrScore = new _score();
        _obstacle obstcls[20];
        GLuint tex;

        _objLoader *sky = new _objLoader();

        _skyBox *carShop = new _skyBox();
        _skyBox *daySky = new _skyBox();
        _skyBox *duskSky = new _skyBox();
        _skyBox *nightSky = new _skyBox();

        _parallax *road = new _parallax();
        _parallax *ground = new _parallax();

        _parallax *landingPage = new _parallax();

        _parallax menuBackground;
        _parallax menuLogo;
        _button mainMenuElements[3];
        enum {help,newGame,exit};

        _parallax pauseMenuBackground;
        _button pauseMenuElements[2];
        enum {pauseResume,pauseExit};

        _parallax helpMenuBackground;
        _button helpMenuReturn;

        _textDisplay *goText = new _textDisplay();
        enum gameOverButtonsIndex { goPlayAgain = 0, goMainMenu = 1 };
        _button gameOverButtons[2];
        bool justCrashed = false;

        _3DModelLoader *mdl3D = new _3DModelLoader();
        _3DModelLoader *mdl3DW = new _3DModelLoader();

        void reSizeScene(int width, int height);  // resize window
        void initGL();                            // initialize GL graphics
        void drawScene();                         // render scene
        int winMsg(HWND,UINT,WPARAM,LPARAM);      // to get keyboard interrupts and pass it to inputs

        int width, height;  // keep record of the screen size
        int frameCount;

        int gameState;
        enum {landing,mainMenu,pauseMenu,inGame,gameOver};

        int curLevel;

        bool helpMenu = false;
        bool paused = false;
        bool killWin = false;
        bool escKeyRelease = true;
        bool loaded = false;

        random_device dev;
        mt19937 rng;
        uniform_int_distribution<mt19937::result_type> rand2;
        uniform_int_distribution<mt19937::result_type> rand6;
        uniform_int_distribution<mt19937::result_type> rand100;

        // 1:7 ratio for coin and dollar spawns
        uniform_int_distribution<mt19937::result_type> rand8;

        vector<vec3> vertices;
        vector<vec2> uvs;
        vector<vec3> normals;

        float carRot = 0;

        void updateInGame();
        void checkPlayerObstacleCollisions();
        float crashDelayTimer = 0.0;
        bool pendingGameOver = false;

        // collectibles
        // 50 coins and 25 dollars
        static const int MAX_COINS = 50;
        static const int MAX_DOLLARS  = 25;

        _collectables coins[MAX_COINS];
        _collectables dollars[MAX_DOLLARS];

        // keeping individual scores and total score
        int coinScore;
        int dollarScore;
        int totalScore;

        float collectableSpawnTimer;

        // +1 .. +4 floating as collectibles are hit
        float pickupTextTimer;
        int lastPickupValue;

        vec3 lastPickupPos;

        void objCollection();

        void updateCollectibles(float dt);
        void spawnCollectibles();
        void checkCollectibleCollisions();
        bool isSafeCollectibleSpawn(float lanx, float zStart);

        bool Apressed = false;
        bool Dpressed = false;

        vec2 prevMouse;



    protected:

    private:
         _playerHealth playerHealth;           // new
        void resetObstacles();
};

#endif // _SCENE_H
