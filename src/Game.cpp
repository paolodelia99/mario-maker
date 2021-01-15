//
// Created by paolo on 07/01/21.
//

#include "../include/Game.h"

Game::Game(const char *mapName, const int screenWidth, const int screenHeight, bool secondPlayer)
:screenWidth_(screenWidth), screenHeight_(screenHeight), secondPlayer(secondPlayer)
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

    ECS::ComponentHandle<CameraComponent> camera = world_->getById(cameraId_)->get<CameraComponent>();

    SetTargetFPS(60);
    double previous = GetTime();
    double lag = 0.0;

    while (run && !WindowShouldClose()) {
        double current = GetTime();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        // Handle Inputs
        handleInput();

        //Update
        while (lag >= MS_PER_UPDATE) {
            world_->tick(0.0f);
            lag -= MS_PER_UPDATE;
        }

        // Drawing
        BeginDrawing();

        ClearBackground({0, 0, 0});

        BeginMode2D(camera.get().camera);

        pMapRenderer_->render();
        pTextureRenderer_->renderTextureEntities(world_);

        // just for showing the center of the camera
        DrawLine(camera->camera.target.x, - screenHeight_ * 10, camera->camera.target.x, screenHeight_ * 10, GREEN);
        DrawLine(- screenWidth_ * 5, camera->camera.target.y, screenWidth_ * 5, camera->camera.target.y, GREEN);

        EndMode2D();

        EndDrawing();
    }

}

Game::~Game() {
    delete world_;
    delete pMapRenderer_;
    delete pTextureRenderer_;
}

void Game::initWorld() {

    initPlayers();

    // Init camera
    ECS::Entity* camera = world_->create();
    camera->assign<CameraComponent>(
            Vector2{screenWidth_ / 2.f,  screenHeight_ / 2.f},
            Vector2{screenWidth_ / 2.f,  screenHeight_ / 2.f},
            0.0f,
            1.0f
            );
    cameraId_ = camera->getEntityId();

    registerSystems();
}

void Game::initPlayers() {
    Vector2 spawnPositionP1 = pMap_->getSpawnPositionP1();
    Vector2 spawnPositionP2 = pMap_->getSpawnPositionP2();

    // First Player
    ECS::Entity* mario = world_->create();
    player1Id_ = mario->getEntityId();
    mario->assign<PlayerComponent>(Vector2 {spawnPositionP1.x * 32, spawnPositionP1.y * 32});
    mario->assign<AABBComponent>(
            Rectangle {
                    spawnPositionP1.x * 32,
                    spawnPositionP1.y * 32,
                    32,
                    32});
    mario->assign<TextureComponent>(MARIO_STAND);
    mario->assign<LeadCameraPlayer>();
    mario->assign<CommandComponent>(std::map<Command, int> {
                                            {JUMP, KEY_UP},
                                            {MOVE_LEFT, KEY_LEFT},
                                            {MOVE_RIGHT, KEY_RIGHT},
                                            {CROUCH_DOWN, KEY_DOWN}
                                    });
    mario->assign<GravityComponent>();
    mario->assign<SolidComponent>();
    mario->assign<KineticComponent>(0.0f, 0.0f);

    // Second Player
    if (secondPlayer) {
        ECS::Entity* luigi = world_->create();
        luigi->assign<PlayerComponent>(Vector2 {spawnPositionP2.x * 32, spawnPositionP2.y * 32});
        luigi->assign<AABBComponent>(
                Rectangle {
                        spawnPositionP2.x * 32,
                        spawnPositionP2.y * 32,
                        32,
                        32});
        luigi->assign<TextureComponent>(LUIGI_STAND);
        luigi->assign<CommandComponent>(std::map<Command, int> {
                {JUMP, KEY_W},
                {MOVE_LEFT, KEY_A},
                {MOVE_RIGHT, KEY_D},
                {CROUCH_DOWN, KEY_S}
        });
        luigi->assign<GravityComponent>();
        luigi->assign<SolidComponent>();
    }
}

void Game::registerSystems() {
    world_->registerSystem(new CameraSystem(
            screenWidth_,
            screenHeight_,
            pMap_->getPixelWidth(),
            pMap_->getPixelHeight()));
    world_->registerSystem(new PlayerSystem());
    world_->registerSystem(new PhysicSystem());
}

void Game::handleInput() {
    for (auto ent : world_->each<CommandComponent>())
    {
        auto it = ent->get<CommandComponent>();
        if (IsKeyDown(KEY_RIGHT)) it->setCurrentCommand(KEY_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) it->setCurrentCommand(KEY_LEFT);
        else if (IsKeyDown(KEY_DOWN)) it->setCurrentCommand(KEY_DOWN);
        else if (IsKeyDown(KEY_UP)) it->setCurrentCommand(KEY_UP);
        else if (IsKeyDown(KEY_W)) it->setCurrentCommand(KEY_W);
        else if (IsKeyDown(KEY_D)) it->setCurrentCommand(KEY_D);
        else if (IsKeyDown(KEY_A)) it->setCurrentCommand(KEY_A);
        else if (IsKeyDown(KEY_S)) it->setCurrentCommand(KEY_S);
        else it->setNullCommand();
    }
}
