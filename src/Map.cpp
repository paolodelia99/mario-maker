//
// Created by paolo on 08/01/21.
//

#include <iostream>
#include "../include/Map.h"

Map::Map(std::string filename)
:name(filename)
{
    loaded = false;
    width = 0;
    height = 0;
}

Map::~Map() {
    for (int i = 0; i < width; i++)
    {
        delete[] graphicsLayer[i];
        delete[] backgroundLayer[i];
    }
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
        loadMapTiles(const_cast<std::vector<tmx::Tileset> &>(map.getTilesets()), usedTiles);
    } else {
        throw "Cannot load map from " + name;
    }

    this->loaded = true;
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
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    unsigned int value = tiles.at(i + j * width).ID;
                    mapToLoad[i][j] = value;
                    usedTilesSet.insert(value);
                }
            }
        }
    }

    return usedTilesSet;
}

Texture2D Map::getTexture(const std::string& path, tmx::Vector2u tilePosition, tmx::Vector2u tileSize) {
    Image image = LoadImage(path.c_str());
    ImageCrop(&image, (Rectangle){
        static_cast<float>(tilePosition.x),
        static_cast<float>(tilePosition.y),
        static_cast<float>(tileSize.x),
        static_cast<float>(tileSize.y)});
    ImageResize(&image, 32, 32);

    Texture2D texture2D = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture2D;
}

void Map::loadMapTiles(std::vector<tmx::Tileset> &tileset, const std::set<unsigned int>& usedTiles) {
    const auto& tiles = tileset.at(0).getTiles();
    for(const auto& tile : tiles)
    {
        if (usedTiles.count(tile.ID))
        {
            Texture2D texture2D = getTexture(tile.imagePath, tile.imagePosition, tile.imageSize);
            TileTexture pTileTexture = static_cast<TileTexture>(malloc(sizeof(TileTexture)));
            pTileTexture->texture = texture2D;
            pTileTexture->id = tile.ID;
            tileTextures.push_back(pTileTexture);
        }
    }
}

int Map::getHeight() const {
    return height;
}

int Map::getWidth() const {
    return width;
}

const Vector2 &Map::getSpawnPositionP1() const {
    return spawnPositionP1;
}

const Vector2 &Map::getSpawnPositionP2() const {
    return spawnPostionP2;
}

const std::vector<TileTexture> &Map::getMapTiles() const {
    return tileTextures;
}
