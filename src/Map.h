//
// Created by paolo on 08/01/21.
//

#ifndef MARIO_MAKER_MAP_H
#define MARIO_MAKER_MAP_H
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

class Map {
public:
    Map(char* filename);

    virtual ~Map();

    void loadMap();

private:
    char* name;
    int height;
    int width;
};


#endif //MARIO_MAKER_MAP_H
