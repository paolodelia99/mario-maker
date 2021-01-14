//
// Created by paolo on 09/01/21.
//
#pragma once

#include <cstdio>
#include <raylib.h>
#include "TextureRenderer.h"
#include <map>
#include <utility>

ECS_TYPE_IMPLEMENTATION;

struct IntPosition
{
    IntPosition(int x, int y): x(x), y(y) {}
    IntPosition(): x(0), y(0) {}

    int8_t x;
    int8_t y;
};

struct AABBComponent {

    AABBComponent(const Rectangle box) : collisionBox_(box) {}

    AABBComponent operator+(const Vector2 &offset)
    {
        return AABBComponent(Rectangle{
                collisionBox_.x + offset.x,
                collisionBox_.y + offset.y,
                collisionBox_.width,
                collisionBox_.height
        });
    }

    [[ nodiscard ]] float right() const { return collisionBox_.x + collisionBox_.width; }

    float left() const { return collisionBox_.x; }

    float top() const { return collisionBox_.y; }

    float bottom() const { return collisionBox_.y + collisionBox_.height; }

    float getCenterX() const { return collisionBox_.x + collisionBox_.width / 2.0f; }

    float getCenterY() const { return collisionBox_.y + collisionBox_.height / 2.0f; }

    Rectangle collisionBox_;
};

ECS_DEFINE_TYPE(AABBComponent);

struct KineticComponent {

    KineticComponent(
            float speedX,
            float speedY,
            float accX = 0.0,
            float accY = 0.0)
    : speedX_(speedX), speedY_(speedY), accX_(accX), accY_(accY) {}

    float speedX_ = 0;
    float speedY_ = 0;
    float accX_ = 0;
    float accY_ = 0;
};

ECS_DEFINE_TYPE(KineticComponent);

struct BrickComponent {};

struct PipeComponent {};

struct CoinBoxComponent {};

struct GroundComponent {};

struct PlayerComponent {

    PlayerComponent(Vector2 home): home_(home) {};

    Vector2 home_;
};

struct GravityComponent {};

struct BottomCollisionComponent {};

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

ECS_DEFINE_TYPE(TextureComponent);

struct LeadCameraPlayer {};

enum Command {
    NONE_COMMAND,
    JUMP,
    MOVE_LEFT,
    MOVE_RIGHT,
    CROUCH_DOWN,
    SPRINT,
    SPECIAL
};

struct CommandComponent {

    CommandComponent(std::map<Command, int> commandsTable)
    : commandsTable_(std::move(commandsTable)) {}

    void setCurrentCommmand(int key) {
        auto it = commandsTable_.begin();
        for (; it != commandsTable_.end(); it++)
        {
            if (it->second == key) {
                currentCommand_ = it->first;
                return;
            }
        }
        currentCommand_ = NONE_COMMAND;
    }

    void setNewKey(Command command, int newKey) {
        auto it = commandsTable_.find(command);
        if (it != commandsTable_.end()) {
            it->second = newKey;
        }
    }

    std::map<Command, int> commandsTable_;
    Command currentCommand_;
};

ECS_DEFINE_TYPE(CommandComponent);

struct CameraComponent {

    CameraComponent(Vector2 target, Vector2 offset, float rotation, float zoom)
    {
        Camera2D camera2D;
        camera2D.target = target;
        camera2D.offset = offset;
        camera2D.rotation = rotation;
        camera2D.zoom = zoom;
        camera = camera2D;
    }

    Camera2D camera;
};
