//
// Created by paolo on 08/01/21.
//
#include <gtest/gtest.h>
#include "Map.h"

TEST(mapTest, TestInitMap) {
    Map map("../assets/maps/test_world.tmx");
    EXPECT_FALSE(map.isMapLoaded());
}
