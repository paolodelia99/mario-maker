//
// Created by paolo on 08/01/21.
//
#include <gtest/gtest.h>
#include <tmxlite/Layer.hpp>
#include "Map.h"

TEST(mapTest, TestInitMap)
{
    Map map("../../assets/maps/test_world.tmx");
    EXPECT_FALSE(map.isMapLoaded());
}

TEST(mapTest, TestLoadMap)
{
    Map map("../../assets/maps/test_world.tmx");
    map.loadMap();
    EXPECT_EQ(map.getWidth(), 100);
    EXPECT_EQ(map.getHeight(), 13);
    EXPECT_EQ(map.getSpawnPositionP1().x, 6.f);
    EXPECT_EQ(map.getSpawnPositionP1().y, 11.f);
    EXPECT_EQ(map.getSpawnPositionP2().x, 4.f);
    EXPECT_EQ(map.getSpawnPositionP2().y, 11.f);
    EXPECT_EQ(map.getMapTiles().size(), 27);
}

TEST(mapTest, TestMapTilesLoading)
{
    // Needed beacause if it won't load texture
    InitWindow(100, 100, "Mario Maker");
    SetTargetFPS(60);

    Map map("../../assets/maps/test_world.tmx");
    map.loadMap();
    EXPECT_TRUE(map.isMapLoaded());
    std::vector<TileTexture> mapTiles = map.getMapTiles();
    EXPECT_EQ(mapTiles.size(), 27);
    for (TileTexture maptile : mapTiles)
    {
        EXPECT_EQ(maptile->texture.width, 32);
        EXPECT_EQ(maptile->texture.height, 32);
    }
}