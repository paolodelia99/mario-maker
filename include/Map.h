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
#include "Components.h"
#include "ECS.h"

class Map {
public:
    Map(std::string filename);

    virtual ~Map();

    int getHeight() const;

    int getWidth() const;

    int getPixelHeight() const;

    int getPixelWidth() const;

    void loadMap(ECS::World* world);

    [[nodiscard]] bool isMapLoaded() {
        return loaded_;
    }

    const Vector2 &getSpawnPositionP1() const;

    const Vector2 &getSpawnPositionP2() const;

    void unloadTextures();

    unsigned int **getGraphicsLayer() const;

    unsigned int **getBackgroundLayer() const;

    const std::map<unsigned int, TileTexture> &getTextureTable() const;

    Texture2D getTexture(unsigned int id);

private:
    void loadMapBasicInfo(const tmx::Vector2u& orientation);

    void loadProperties(std::vector<tmx::Property> properties);

    void loadMapTiles(std::vector<tmx::Tileset>& tileset, const std::set<unsigned int>& usedTiles);

    std::set<unsigned int> loadLayers(const std::vector<tmx::Layer::Ptr>& layers, ECS::World* world);

    Texture2D getTexture(const std::string& path, tmx::Vector2u tilePosition, tmx::Vector2u tileSize);


private:
    std::string name;
    int height_;
    int width_;
    bool loaded_;
    unsigned int** graphicsLayer_;
    unsigned int** backgroundLayer_;
    Vector2 spawnPositionP1_;
    Vector2 spawnPostionP2_;
    std::map<unsigned int, TileTexture> mapTextureTable_;
};


#endif //MARIO_MAKER_MAP_H
