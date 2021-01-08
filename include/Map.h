//
// Created by paolo on 08/01/21.
//

#ifndef MARIO_MAKER_MAP_H
#define MARIO_MAKER_MAP_H
#include <string>
#include <set>
#include <raylib.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

class Map {
public:
    Map(std::string filename);

    virtual ~Map();

    void loadMap();

    [[nodiscard]] bool isMapLoaded() {
        return loaded;
    }

private:
    void loadMapBasicInfo(const tmx::Vector2u& orientation);

    void loadProperties(const std::vector<tmx::Property> properties);

    std::set<unsigned int> loadLayers(const std::vector<tmx::Layer::Ptr>& layers);

private:
    std::string name;
    int height;
    int width;
    bool loaded;
    unsigned int** graphicsLayer;
    unsigned int** backgroundLayer;
    Vector2 spawnPositionP1;
    Vector2 spawnPostionP2;
};


#endif //MARIO_MAKER_MAP_H
