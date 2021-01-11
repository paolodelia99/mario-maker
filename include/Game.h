//
// Created by paolo on 07/01/21.
//

#ifndef MARIO_MAKER_GAME_H
#define MARIO_MAKER_GAME_H
#include <raylib.h>
#include "ECS.h"
#include "MapRenderer.h"
#include "TextureRenderer.h"

class Game {
public:
    Game(char* mapName, Vector2 windowSize);

    void mainLoop();

    virtual ~Game();

private:

    void initWorld();

    void initPlayers();

private:
    bool run;
    bool pause;
    ECS::World* world_;
    Map* map_;
    MapRenderer* mapRenderer_;
    TextureRenderer* textureRenderer_;
    Camera2D* camera2D_;
    Vector2 windowsSize_;
};


#endif //MARIO_MAKER_GAME_H
