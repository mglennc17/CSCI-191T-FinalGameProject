#ifndef _COLLECTABLES_H
#define _COLLECTABLES_H

#include <GL/gl.h>
#include <_textureloader.h>




class _collectables
{
    public:

        float x;
        float y;
        float z;

        float size;
        bool active;
        GLuint texID;

        _textureLoader textures;

        float rotZ;
        float spinSpeed;


        _collectables();
        virtual ~_collectables();


        void init (float cx, float cy, float cz, float cSize, char* cfileName);

        void draw();
        void deactivate();

        bool isActive() const {
            return active;
            }

        float xVal() const { return x; }

        float yVal() const { return y; }

        float zVal() const { return z; }

        float grabSize() const { return size; }

    protected:

    private:
};

#endif // _COLLECTABLES_H
