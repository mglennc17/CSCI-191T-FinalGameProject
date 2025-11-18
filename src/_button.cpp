#include "_button.h"

_button::_button()
{
    clicked = false;
}

_button::~_button()
{
    //dtor
}

void _button::initButton(char* fileName,float posX,float posY)
{
    btnImage->parallaxInit(fileName);
    pos.x = posX;
    pos.y = posY;
}

void _button::drawButton(float w, float h, bool hover)
{
    glPushMatrix();
        glTranslatef(pos.x,pos.y,0);
        if (hover) glScalef(1.1,1.1,1.0);
        btnImage->drawParallax(w,h);
    glPopMatrix();
}


