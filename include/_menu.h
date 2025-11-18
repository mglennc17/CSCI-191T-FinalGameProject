#ifndef _MENU_H
#define _MENU_H

#include <_common.h>
#include <_button.h>

class _menu
{
    public:
        _menu();
        virtual ~_menu();

        _button buttons[5];
        int numBtns;

    protected:

    private:
};

#endif // _MENU_H
