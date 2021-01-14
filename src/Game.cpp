//
// Created by paolo on 07/01/21.
//

#include <iostream>
#include <Commands.h>
#include <CameraSystem.h>
#include "../include/Game.h"

Game::Game(char *mapName, const int screenWidth, const int screenHeight)
:screenWidth_(screenWidth), screenHeight_(screenHeight)
{
    run = true;
    pause = false;
    world_ = ECS::World::createWorld();
    pMap_ = new Map(mapName);
    pMap_->loadMap(world_);
    pMapRenderer_ = new MapRenderer(pMap_);
    pTextureRenderer_ = new TextureRenderer("../assets/imgs/characters.gif");
}

void Game::mainLoop() {

    initWorld();

    ECS::ComponentHandle<Camera2D> pCamera = world_->getById(cameraId_)->get<Camera2D>();
    Vector2 cameraPos = pCamera->target;

//    ECS::Entity* entity = world_->getById(player1Id_);
//    Vector2 position = Vector2{
//        entity->get<AABBComponent>()->getCenterX(),
//        entity->get<AABBComponent>()->getCenterY()};

    SetTargetFPS(60);

    while (run && !WindowShouldClose()) {
        // Handle Inputs
        if (IsKeyDown(KEY_RIGHT)) cameraPos.x += 2;
        else if (IsKeyDown(KEY_LEFT)) cameraPos.x -= 2;
        else if (IsKeyDown(KEY_DOWN)) cameraPos.y += 2;
        else if (IsKeyDown(KEY_UP)) cameraPos.y -= 2;

        pCamera->target = (Vector2) {cameraPos.x, cameraPos.y };

        //Update
        world_->tick(0.0f);

        // Drawing
        BeginDrawing();

        ClearBackground({0, 0, 0});

        BeginMode2D(pCamera.get());

        pMapRenderer_->render();
        pTextureRenderer_->renderTextureEntities(world_);

        // just for showing the center of the camera
        DrawLine(pCamera->target.x, - screenHeight_ * 10, pCamera->target.x, screenHeight_ * 10, GREEN);
        DrawLine(- screenWidth_ * 5, pCamera->target.y, screenWidth_ * 5, pCamera->target.y, GREEN);

        EndMode2D();

        EndDrawing();

        std::cout << "x: " << pCamera->target.x << std::endl;
        std::cout << "y: " << pCamera->target.y << std::endl;
    }

}

Game::~Game() {
    delete world_;
    delete pMapRenderer_;
    delete pTextureRenderer_;
}

void Game::initWorld() {
    Vector2 spawnPositionP1 = pMap_->getSpawnPositionP1();

    initPlayers();

    // Init camera
    ECS::Entity* camera = world_->create();
    camera->assign<Camera2D>();
    cameraId_ = camera->getEntityId();
    auto pCamera = camera->get<Camera2D>();
    pCamera->target = Vector2({screenWidth_ / 2.f,  screenHeight_ / 2.f});
    pCamera->offset = Vector2({ screenWidth_ / 2.f, screenHeight_ / 2.f});
    pCamera->rotation = 0.0f;
    pCamera->zoom = 1.0f;

    registerSystems();
}

void Game::initPlayers() {
    Vector2 spawnPositionP1 = pMap_->getSpawnPositionP1();
    Vector2 spawnPositionP2 = pMap_->getSpawnPositionP2();

    ECS::Entity* mario = world_->create();
    player1Id_ = mario->getEntityId();
    mario->assign<PlayerComponent>(Vector2 {spawnPositionP1.x * 32, spawnPositionP1.y * 32});
    mario->assign<AABBComponent>(
            Rectangle {
                    spawnPositionP1.x * 32,
                    spawnPositionP1.y * 32,
                    16,
                    16});
    mario->assign<TextureComponent>(MARIO_STAND);
    mario->assign<LeadCameraPlayer>();
    mario->assign<CommandsComponent>(
        new JumpCommand(),
        new NoneCommand(),
        new MoveLeftCommand(),
        new MoveRightCommand());


    ECS::Entity* luigi = world_->create();
    luigi->assign<PlayerComponent>(Vector2 {spawnPositionP2.x * 32, spawnPositionP2.y * 32});
    luigi->assign<AABBComponent>(
            Rectangle {
                    spawnPositionP2.x * 32,
                    spawnPositionP2.y * 32,
                    16,
                    16});
    luigi->assign<TextureComponent>(LUIGI_STAND);
}

void Game::registerSystems() {
    world_->registerSystem(new CameraSystem(
            screenWidth_,
            screenHeight_,
            pMap_->getPixelWidth(),
            pMap_->getPixelHeight()));
}
