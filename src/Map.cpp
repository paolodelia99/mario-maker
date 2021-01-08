//
// Created by paolo on 08/01/21.
//

#include "Map.h"

Map::Map(char *filename)
:name(filename)
{

}

Map::~Map() {

}

void Map::loadMap() {
    tmx::Map map;
    if(map.load(name))
    {

    }

}
