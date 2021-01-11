//
// Created by paolo on 09/01/21.
//
#pragma once

#include <cstdio>
#include <raylib.h>
#include "TextureRenderer.h"

struct IntPosition
{
    IntPosition(int x, int y): x(x), y(y) {}
    IntPosition(): x(0), y(0) {}

    int8_t x;
    int8_t y;
};

struct AABBComponent {

    AABBComponent(const Vector2 &position, const Vector2 &size) : position_(position), size_(size) {}

    AABBComponent operator+(const Vector2 &offset) {
        return AABBComponent(Vector2{
            position_.x + offset.x,
            position_.y + offset.y}, size_);
    }

    float right() const { return position_.x + size_.x; }

    float left() const { return position_.x; }

    float top() const { return position_.y; }

    float bottom() const { return position_.y + size_.y; }

    Vector2 position_;
    Vector2 size_;
};

struct BrickComponent {};

struct PipeComponent {};

struct CoinBoxComponent {};

struct GroundComponent {};

struct PlayerComponent {

    PlayerComponent(Vector2 home): home_(home) {};

    Vector2 home_;
};

struct GravityComponent {};

struct AnimationComponent {
    explicit AnimationComponent(
                std::vector<Texture2D>&& textures,
                int duration,
                bool flipH = false,
                bool flipV = false,
                bool loop = true
    ) : textures{textures},
        duration{duration},
        counter{duration},
        flipH{flipH},
        flipV{flipV},
        loop{loop} {}

    int duration = 0;
    int counter = 0;
    bool flipH = false;
    bool flipV = false;
    bool loop = true;
    int8_t currentTexture = 0;
    std::vector<Texture2D> textures;
};

struct TileTexture_
{
    Texture2D texture;
    int8_t id;
};
typedef TileTexture_* TileTexture;

struct TextureComponent {
    explicit TextureComponent(TextureId textureId) : textureId_(textureId) {};

    ~TextureComponent()  = default;

    TextureId textureId_;

    bool flipH = false;
    bool flipV = false;

    float offSetX = 0;
    float offSetY = 0;
    int w = 0;
    int h = 0;
};