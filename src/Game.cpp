//
// Created by paolo on 07/01/21.
//

#include "../include/Game.h"
#include "../include/Map.h"

Game::Game(char *mapName)
:mapName(mapName)
{
    run = true;
    pause = false;
}

void Game::mainLoop() {

    SetTargetFPS(60);
    Map map("../assets/maps/test_world.tmx");
    map.loadMap();

    while (run && !WindowShouldClose()) {
        BeginDrawing();

        ClearBackground({0, 0, 0});

        EndDrawing();
    }

}

Game::~Game() {

}
