#include "_collectables.h"

_collectables::_collectables()
{
    //ctor

    x = 0.0;
    y = 0.0;
    z = -5.0;

    size = 0.5;
    active = false;
    texID = 0;

    rotZ = 0.0;
    spinSpeed = 180.0;  // speed of the spinning animation on collectibles


}

_collectables::~_collectables()
{
    //dtor
}
void _collectables::init(float cx, float cy, float cz, float cSize, char* cfileName)
{
    //setting size and position
    x = cx;
    y = cy;
    z = cz;
    size = cSize;

    // make the the collectables show
    active = true;

    // adding texture
    texID = textures.loadTexture(cfileName);
}

void _collectables::draw()
{
    // only draw if it's meant to be drawn
    if(!active || texID == 0) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    glPushMatrix();
        glTranslatef(x, y, z);

        // making coin and dollar spin in place
        glRotatef(rotZ, 0.0, 0.0, 1.0);

        // facing camera
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, 0.0);
            glTexCoord2f(1.0, 0.0); glVertex3f( size, -size, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f( size,  size, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(-size,  size, 0.0);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void _collectables::deactivate()
{
    active = false;
}
