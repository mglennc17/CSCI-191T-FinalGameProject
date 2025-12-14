#include "_saveManager.h"
#include <fstream>
#include <sstream>

_saveManager::_saveManager()
{
    money = 0;
    car_default = true;
    car_sport = false;
    car_truck = false;
    activeCar = "default";
}

_saveManager::~_saveManager()
{
    // nothing needed here
}



void _saveManager::load()
{
    std::ifstream file("save.dat");

    if (!file.is_open()) {
        save(); // create default save
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key, value;

        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
            if (key == "money") money = std::stoi(value);
            else if (key == "car_default") car_default = (value == "1");
            else if (key == "car_sport")   car_sport   = (value == "1");
            else if (key == "car_truck")   car_truck   = (value == "1");
            else if (key == "active_car")  activeCar = value;
        }
    }

    file.close();
}

void _saveManager::save()
{
    std::ofstream file("save.dat", std::ios::trunc);

    file << "money=" << money << "\n";
    file << "car_default=" << car_default << "\n";
    file << "car_sport="   << car_sport   << "\n";
    file << "car_truck="   << car_truck   << "\n";
    file << "active_car="  << activeCar << "\n";

    file.close();
}
