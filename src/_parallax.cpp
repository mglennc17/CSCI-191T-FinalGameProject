#include "_parallax.h"



_parallax::_parallax()
{
    //ctor
    xMin = yMin = 0.0;
    xMax = yMax = 1.0;

    mytime->reset();
}

_parallax::~_parallax()
{
    //dtor
}
void _parallax::drawParallax(float w, float h)
{

    glDisable(GL_LIGHTING);
    btex->bindTexture();

   glBegin(GL_POLYGON);

    glTexCoord2f(xMin,yMax);
    glVertex3f(-w/h, -1,-10);

    glTexCoord2f(xMax,yMax);
    glVertex3f(w/h, -1,-10);

    glTexCoord2f(xMax,yMin);
    glVertex3f(w/h,1,-10);

    glTexCoord2f(xMin,yMin);
    glVertex3f(-w/h, 1,-10);

   glEnd();
   glEnable(GL_LIGHTING);

}

void _parallax::parallaxInit(char* fileName)
{
    btex->loadTexture(fileName);
}

void _parallax::prlxScrollAuto(string dir , float speed)
{
    //if(mytime->getTicks()>40)
    //{

    if(dir == "left"){xMin +=speed;xMax += speed;}
    else if(dir == "right"){xMin -=speed;xMax -= speed;}
    else if(dir == "up"){yMin -=speed;yMax-=speed;}
    else if(dir == "down"){yMin +=speed;yMax+=speed;}

    //mytime->reset();
    //}
}
