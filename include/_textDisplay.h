#ifndef _TEXTDISPLAY_H
#define _TEXTDISPLAY_H
#include <_common.h>
#include <_textureloader.h>

class _textDisplay
{
    public:
        _textDisplay();
        virtual ~_textDisplay();

        _textureLoader *font = new _textureLoader();

        void textInit(char *,int,int);
        void drawText(char *,float);
        void drawTime(int,int,int,float);

        float xMin,xMax,yMin,yMax;
        vec3 vertX[4];
        int w,h;

        char m[3];
        char s[3];
        char ms[3];
    protected:

    private:
};

#endif // _TEXTDISPLAY_H
