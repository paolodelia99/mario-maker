//
// Created by paolo on 07/01/21.
//

#ifndef MARIO_MAKER_GAME_H
#define MARIO_MAKER_GAME_H
#include <raylib.h>
#include <iostream>
#include "ECS.h"
#include "Constants.h"
#include "GameMap.h"
#include "SoundManager.h"
#include "renderers/MapRenderer.h"
#include "renderers/TextureRenderer.h"
#include "renderers/EnemiesRenderer.h"
#include "renderers/ObjectRenderer.h"
#include "renderers/TextRenderer.h"
#include "systems/CameraSystem.h"
#include "systems/PlayerSystem.h"
#include "systems/PhysicSystem.h"
#include "systems/AnimationSystem.h"
#include "systems/TileSystem.h"
#include "systems/TimerSystem.h"
#include "systems/EnemySystem.h"
#include "systems/FlagSystem.h"
#include "systems/IdsMapSystem.h"
#include "systems/ScoreSystem.h"
#include "systems/SoundSystem.h"

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

    void initIdsMap();

    void render(float d);

    void initTextEntities();

private:
    bool run;
    bool pause;
    bool secondPlayer;
    ECS::World* world_;
    Camera2D * pCamera_;
    GameMap* pMap_;
    size_t cameraId_;
    size_t player1Id_;
    const int screenWidth_;
    const int screenHeight_;
    ECS::EntitySystem* animationSystem_;
    MapRenderer* mapRenderer;
    TextureRenderer* textureRenderer;
    EnemiesRenderer *enemiesRenderer;
    ObjectRenderer* objectRenderer;
    TextRenderer* textRenderer_;
};


#endif //MARIO_MAKER_GAME_H
