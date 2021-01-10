//
// Created by paolo on 07/01/21.
//

#ifndef MARIO_MAKER_GAME_H
#define MARIO_MAKER_GAME_H
#include <raylib.h>
#include "ECS.h"
#include "MapRenderer.h"

class Game {
public:
    Game(char* mapName, Vector2 windowSize);

    void mainLoop();

    virtual ~Game();

private:
    char* mapName;
    bool run;
    bool pause;
    ECS::World* world;
    MapRenderer* mapRenderer_;
    Camera2D* camera2D_;
    Vector2 windowsSize_;
};


#endif //MARIO_MAKER_GAME_H
