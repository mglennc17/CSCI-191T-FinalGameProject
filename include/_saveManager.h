#ifndef _SAVEMANAGER_H
#define _SAVEMANAGER_H

#include <string>

class _saveManager
{
public:
    _saveManager();
    ~_saveManager();

    int money;

    bool car_default;
    bool car_sport;
    bool car_truck;

    std::string activeCar;

    void load();
    void save();
};

#endif

