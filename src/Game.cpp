//
// Created by paolo on 07/01/21.
//

#include <iostream>
#include "../include/Game.h"

Game::Game(char *mapName, Vector2 windowSize)
:windowsSize_(windowSize)
{
    run = true;
    pause = false;
    world_ = ECS::World::createWorld();
    camera2D_ = new Camera2D();
    map_ = new Map(mapName);
    map_->loadMap(world_);
    mapRenderer_ = new MapRenderer(map_);
    textureRenderer_ = new TextureRenderer("../assets/imgs/characters.gif");
}

void Game::mainLoop() {

    initWorld();

    Vector2 position{6 * 32, 11 * 32};

    camera2D_->target = Vector2({position.x, position.y});
    camera2D_->offset = Vector2({ windowsSize_.x / 4 + 14, (3 * windowsSize_.y) / 4 + 39});
    camera2D_->rotation = 0.0f;
    camera2D_->zoom = 1.0f;

    SetTargetFPS(60);

    while (run && !WindowShouldClose()) {
        // Handle Inputs
        if (IsKeyDown(KEY_RIGHT)) position.x += 2;
        else if (IsKeyDown(KEY_LEFT)) position.x -= 2;
        else if (IsKeyDown(KEY_DOWN)) position.y += 2;
        else if (IsKeyDown(KEY_UP)) position.y -= 2;

        camera2D_->target = (Vector2) {position.x, position.y};

        //Update

        // Drawing
        BeginDrawing();

        ClearBackground({0, 0, 0});

        BeginMode2D(*camera2D_);

        mapRenderer_->render();
        textureRenderer_->renderTextureEntities(world_);

        EndMode2D();

        EndDrawing();
    }

}

Game::~Game() {
    delete world_;
    delete mapRenderer_;
    delete textureRenderer_;
}

void Game::initWorld() {
    initPlayers();
}

void Game::initPlayers() {
    Vector2 spawnPositionP1 = map_->getSpawnPositionP1();
    Vector2 spawnPositionP2 = map_->getSpawnPositionP2();

    ECS::Entity* mario = world_->create();
    mario->assign<PlayerComponent>(Vector2 {spawnPositionP1.x * 32, spawnPositionP1.y * 32});
    mario->assign<AABBComponent>(
            Vector2{
                spawnPositionP1.x * 32,
                spawnPositionP1.y * 32},
                Vector2{16, 16});
    mario->assign<TextureComponent>(MARIO_STAND);

    ECS::Entity* luigi = world_->create();
    luigi->assign<PlayerComponent>(Vector2 {spawnPositionP2.x * 32, spawnPositionP2.y * 32});
    luigi->assign<AABBComponent>(
            Vector2{
                    spawnPositionP2.x * 32,
                    spawnPositionP2.y * 32},
            Vector2{16, 16});
    luigi->assign<TextureComponent>(LUIGI_STAND);
}
