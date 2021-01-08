//
// Created by paolo on 08/01/21.
//

#include "../include/Map.h"

Map::Map(std::string filename)
:name(filename)
{
    loaded = false;
    width = 0;
    height = 0;
}

Map::~Map() {
    delete graphicsLayer;
    delete backgroundLayer;
}

void Map::loadMap() {
    tmx::Map map;
    if(map.load(name))
    {
        // Get Map sizes
        loadMapBasicInfo(map.getTileCount());

        // Load properties
        loadProperties(map.getProperties());

        // Load layers
        std::set<unsigned int> usedTiles = loadLayers(map.getLayers());

        // Load tileset
        const std::vector<tmx::Tileset>& tileset = map.getTilesets();
        const auto& tiles = tileset.at(0).getTiles();


    } else {
        throw "Cannot load map!";
    }

}

void Map::loadMapBasicInfo(const tmx::Vector2u &orientation) {
    width = orientation.x;
    height = orientation.y;

    graphicsLayer = new unsigned int*[width];
    backgroundLayer = new unsigned int*[width];
    for (int i = 0; i < width; i++)
    {
        graphicsLayer[i] = new unsigned int[height];
        backgroundLayer[i] = new unsigned int[height];
    }
}

void Map::loadProperties(const std::vector<tmx::Property> properties) {
    Vector2 marioSpawn;
    for (const tmx::Property& property : properties)
    {
        int value = std::stoi(property.getStringValue());
        if (property.getName() == "spawn_mario_x") {
            marioSpawn.x = value;

        } else if (property.getName() == "spawn_mario_y") {
            marioSpawn.y = value;
        }
    }

    spawnPositionP1 = marioSpawn;
    spawnPostionP2 = Vector2{marioSpawn.x - 2, marioSpawn.y};
}

std::set<unsigned int> Map::loadLayers(const std::vector<tmx::Layer::Ptr>& layers) {
    std::set<unsigned int> usedTilesSet;

    for (const auto& layer : layers)
    {
        if(layer->getType() == tmx::Layer::Type::Object)
        {
            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
            const auto& objects = objectLayer.getObjects();
            for(const auto& object : objects)
            {
                //do stuff with object properties
            }
        }
        else if(layer->getType() == tmx::Layer::Type::Tile)
        {
            unsigned int** mapToLoad;
            if (layer->getName() == "background")
            {
                mapToLoad = backgroundLayer;
            } else if (layer->getName() == "graphics")
            {
                mapToLoad = graphicsLayer;
            }

            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto & tiles = tileLayer.getTiles();
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    unsigned int value = tiles.at(i + j).ID;
                    mapToLoad[i][j] = value;
                    usedTilesSet.insert(value);
                }
            }
        }
    }

    return usedTilesSet;
}
