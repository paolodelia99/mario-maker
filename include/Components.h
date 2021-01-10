//
// Created by paolo on 09/01/21.
//
#pragma once

#include <stdio.h>
#include <raylib.h>

struct Position
{
    Position(float x, float y):x(x), y(x) {}
    Position():x(0.f), y(0.f) {}

    float x;
    float y;
};

struct MapPosition
{
    MapPosition(int x, int y):x(x), y(y) {}
    MapPosition():x(0), y(0) {}

    int8_t x;
    int8_t y;
};

struct MapTile
{
    int8_t tileId;
    Position position;
    unsigned int size;
};

struct TileTexture_
{
    Texture2D texture;
    int8_t id;
};
typedef TileTexture_* TileTexture;