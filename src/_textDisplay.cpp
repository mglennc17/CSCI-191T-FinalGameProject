#include "_textDisplay.h"

_textDisplay::_textDisplay()
{
    //ctor
    xMin = yMin = 0;
    xMax = yMax = 1;
}

_textDisplay::~_textDisplay()
{
    //dtor
}

void _textDisplay::textInit(char* fileName,int width,int height)
{
    font->loadTexture(fileName);
    w = width;
    h = height;
    vertX[0].x = -0.5; vertX[0].y = -0.5; vertX[0].z=1;
    vertX[1].x =  0.5; vertX[1].y = -0.5; vertX[1].z=1;
    vertX[2].x =  0.5; vertX[2].y =  0.5; vertX[2].z=1;
    vertX[3].x = -0.5; vertX[3].y =  0.5; vertX[3].z=1;
}

void _textDisplay::drawText(char* str, float fontSize)
{
    int idx;
    int x;
    int y;
    glClearColor(1.0,1.0,1.0,1.0);
    font->bindTexture();
    glPushMatrix();
    glTranslatef(-0.5,1.0,0);
    glScalef(1.0 * fontSize,1.0 * fontSize,1.0);
    for (int i = 0; i < strlen(str); i++) {
        if (isalpha(str[i])) idx = str[i] - 'A';
        else idx = 26;      //space
        //if (isdigit(str[i])) idx = str[i] - '0' + 55;

        y = (idx / w);
        x = idx - (y * w);

        xMin = (float)x/(float)w        +   (0.1/(float)w);
        xMax = xMin + 0.7 /(float)w;//     -   (0.4/(float)w);
        yMin = (float)y/(float)h        +   (0.2/(float)h);
        yMax = yMin + 0.7 / (float)h;//    -   (0.4/(float)h);
        glPushMatrix();
        glTranslatef((i + 1) * 0.8,0,-5.0);
        glBegin(GL_QUADS);
            glTexCoord2f(xMin,yMax);
            glVertex2f(vertX[0].x,vertX[0].y);

            glTexCoord2f(xMax,yMax);
            glVertex2f(vertX[1].x,vertX[1].y);

            glTexCoord2f(xMax,yMin);
            glVertex2f(vertX[2].x,vertX[2].y);

            glTexCoord2f(xMin,yMin);
            glVertex2f(vertX[3].x,vertX[3].y);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
}
