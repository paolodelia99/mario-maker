//
// Created by paolo on 07/01/21.
//

#ifndef MARIO_MAKER_GAME_H
#define MARIO_MAKER_GAME_H
#include <raylib.h>
#include <iostream>
#include "ECS.h"
#include "Constants.h"
#include "MapRenderer.h"
#include "TextureRenderer.h"
#include "CameraSystem.h"
#include "PlayerSystem.h"
#include "PhysicSystem.h"
#include "AnimationSystem.h"

class Game {
public:
    Game(const char* mapName, const int screenWidth, const int screenHeight, bool secondPlayer);

    void mainLoop();

    virtual ~Game();

private:

    void initWorld();

    void initPlayers();

    void registerSystems();

    void handleInput();

private:
    bool run;
    bool pause;
    bool secondPlayer;
    ECS::World* world_;
    Camera2D * pCamera_;
    Map* pMap_;
    MapRenderer* pMapRenderer_;
    TextureRenderer* pTextureRenderer_;
    size_t cameraId_;
    size_t player1Id_;
    const int screenWidth_;
    const int screenHeight_;
    ECS::EntitySystem* animationSystem_;
};


#endif //MARIO_MAKER_GAME_H
