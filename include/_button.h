#ifndef _BUTTON_H
#define _BUTTON_H

#include <_parallax.h>
#include <_common.h>

class _button
{
    public:
        _button();
        virtual ~_button();

        void initButton(char* fileName,float,float);
        _parallax *btnImage = new _parallax();

        void drawButton(float,float,bool);
        bool clicked;

        vec2 pos;

    protected:

    private:
};

#endif // _BUTTON_H
