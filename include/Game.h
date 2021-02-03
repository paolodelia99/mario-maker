//
// Created by paolo on 07/01/21.
//

#ifndef MARIO_MAKER_GAME_H
#define MARIO_MAKER_GAME_H
#include <raylib.h>
#include <iostream>
#include "ECS.h"
#include "Constants.h"
#include "renderers/MapRenderer.h"
#include "renderers/TextureRenderer.h"
#include "renderers/EnemiesRenderer.h"
#include "renderers/ObjectRenderer.h"
#include "CameraSystem.h"
#include "PlayerSystem.h"
#include "PhysicSystem.h"
#include "AnimationSystem.h"
#include "TileSystem.h"
#include "TimerSystem.h"
#include "EnemySystem.h"

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

    void initObjectMap();

    void render(float d);

private:
    bool run;
    bool pause;
    bool secondPlayer;
    ECS::World* world_;
    Camera2D * pCamera_;
    Map* pMap_;
    size_t cameraId_;
    size_t player1Id_;
    const int screenWidth_;
    const int screenHeight_;
    ECS::EntitySystem* animationSystem_;
    MapRenderer* mapRenderer;
    TextureRenderer* textureRenderer;
    EnemiesRenderer *enemiesRenderer;
    ObjectRenderer* objectRenderer;
};


#endif //MARIO_MAKER_GAME_H
