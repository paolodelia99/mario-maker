//
// Created by paolo on 09/01/21.
//
#pragma once

#include <cstdio>
#include <raylib.h>
#include "renderers/TextureRenderer.h"
#include <map>
#include <utility>
#include <iostream>

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

    void setBottom(float value) {
        collisionBox_.y = value - collisionBox_.height;
    }

    void setTop(float value) {
        collisionBox_.y = value;
    }

    void setLeft(float value) {
        collisionBox_.x = value;
    }

    void setRight(float value) {
        collisionBox_.x = value - collisionBox_.width;
    }

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

enum PlayerState {
    STANDING,
    JUMPING,
    RUNNING,
    DRIFTING,
    DUCKING,
    INVINCIBLE
};

struct PlayerComponent {

    PlayerComponent(Vector2 home)
    : home_(home)
    {
        current_state_ = STANDING;
    };

    Vector2 home_;
    PlayerState current_state_;
    bool duck = false;
    bool sprint = false;
    bool jump = false;
    int left = 0;
    int right = 0;
};

struct MarioComponent {};

struct LuigiComponent {};

struct GravityComponent {};

struct SolidComponent {};

struct FrozenComponent {};

struct BottomCollisionComponent {};

struct TopCollisionComponent {};

struct RightCollisionComponent {};

struct LeftCollisionComponent {};

struct BounceComponent {

    int getHeight() {
        frames--;
        return height[frames];
    }

    [[nodiscard]] bool finished() const {
        return frames == 0;
    }

    void reset() {
        frames = 13;
    }

public:
    bool hit = false;

private:
    int frames = 13;
    int height[13] = {-2, 1, 3, 2, 1, 1, 1, 0, 0, -1, -1, -4, -1};;
};

struct AnimationComponent {
    explicit AnimationComponent(
                std::vector<TextureId> textures,
                int duration,
                bool flipH = false,
                bool flipV = false,
                bool loop = true
    ) : textures{std::move(textures)},
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
    std::vector<TextureId> textures;
};

struct TileTexture_
{
    Texture2D texture;
    int8_t id;
};
typedef TileTexture_* TileTexture;

struct TileComponent {};

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
    DUCK,
    SPRINT,
    SPECIAL
};

struct CommandComponent {

    CommandComponent(std::map<Command, int> commandsTable)
    : commandsKeyTable_(std::move(commandsTable))
    {
        createKeyCommandTable();
    }

    void setNullCommand() { currentCommand_ = NONE_COMMAND; }

    void setCurrentCommand(int key) {
        auto it = keyCommandTable_.find(key);
        if (it != keyCommandTable_.end()) {
            currentCommand_ = it->second;
            return;
        }
        currentCommand_ = NONE_COMMAND;
    }

    void createKeyCommandTable() {
        for (std::map<Command, int>::iterator i = commandsKeyTable_.begin(); i != commandsKeyTable_.end(); ++i)
            keyCommandTable_[i->second] = i->first;
    }

    void unsetCurrentCommand(int key) {
        auto it = keyCommandTable_.find(key);
        if (it != keyCommandTable_.end()) {
            if (currentCommand_ == it->second) {
                currentCommand_ = NONE_COMMAND;
            }
        }
    }

    void setNewKey(Command command, int newKey) {
        auto it = commandsKeyTable_.find(command);
        if (it != commandsKeyTable_.end()) {
            it->second = newKey;
        }

        createKeyCommandTable();
    }

    std::map<Command, int> commandsKeyTable_;
    std::map<int, Command> keyCommandTable_;
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

struct ObjectMapComponent {

    explicit ObjectMapComponent(int width, int height)
    :width_(width), height_(height)
    {
        idsMatrix_ = std::vector<int>(width * height);
        std::fill(idsMatrix_.begin(), idsMatrix_.end(), -1);;
    }

    int get(int x, int y) {
        if (x < 0 || y < 0) return -1;
        if (x >= width_ || y >= height_) return -1;
        return idsMatrix_.at(x + y * width_);
    }

    void set(size_t entId, int x, int y) {
        idsMatrix_.at(x + y * width_) =  entId;
    }

    ~ObjectMapComponent() {}

private:
    std::vector<int> idsMatrix_;
    unsigned int width_;
    unsigned int height_;
};
