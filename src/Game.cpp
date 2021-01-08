//
// Created by paolo on 07/01/21.
//

#include "Game.h"

Game::Game(char *mapName)
:mapName(mapName)
{
    run = true;
    pause = false;
}

void Game::mainLoop() {

    SetTargetFPS(60);

    while (run && !WindowShouldClose()) {
        BeginDrawing();

        ClearBackground({0, 0, 0});

        EndDrawing();
    }

}

Game::~Game() {

}
