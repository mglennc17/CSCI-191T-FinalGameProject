#include "_car.h"

_car::_car()
{
    //ctor
}

_car::~_car()
{
    //dtor
}

_car::_car(int lvl, int drctn, int rand)
{

    speed = ( rand / 10 ) * lvl;
    if (lvl > 1) dir = drctn;
    else dir = 1;

}

void _car::drawCar()
{

}
