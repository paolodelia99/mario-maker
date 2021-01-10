//
// Created by paolo on 07/01/21.
//

#include <iostream>
#include "../include/Game.h"

Game::Game(char *mapName, Vector2 windowSize)
:mapName(mapName), windowsSize_(windowSize)
{
    run = true;
    pause = false;
    world = ECS::World::createWorld();

    auto* camera2D = new Camera2D();
    camera2D_ = camera2D;
    mapRenderer_ = new MapRenderer(new Map(("../assets/maps/test_world.tmx")));
}

void Game::mainLoop() {

    Vector2 postion{6 * 32, 11 * 32};

    camera2D_->target = Vector2({postion.x, postion.y});
    camera2D_->offset = Vector2({ windowsSize_.x / 4 + 34, (3 * windowsSize_.y) / 4 + 59});
    camera2D_->rotation = 0.0f;
    camera2D_->zoom = 1.0f;

    SetTargetFPS(60);

    while (run && !WindowShouldClose()) {
        // Handle Inputs
        if (IsKeyDown(KEY_RIGHT)) postion.x += 1;
        else if (IsKeyDown(KEY_LEFT)) postion.x -= 1;
        else if (IsKeyDown(KEY_DOWN)) postion.y += 1;
        else if (IsKeyDown(KEY_UP)) postion.y -= 1;

        camera2D_->target = (Vector2) {postion.x + 20, postion.y + 20};

        //Update

        // Drawing
        BeginDrawing();

        ClearBackground({0, 0, 0});

        BeginMode2D(*camera2D_);

        mapRenderer_->render();

        EndMode2D();

        EndDrawing();
    }

}

Game::~Game() {
    delete world;
    delete mapRenderer_;
}
