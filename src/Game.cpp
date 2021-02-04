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
    mapRenderer = new MapRenderer(pMap_, SMB1_TILESET_PATH);
    textureRenderer = new TextureRenderer(SBM1_PLAYER_TILESET_PATH);
    enemiesRenderer = new EnemiesRenderer(SMB1_ENEMIES_TILESET_PATH);
    objectRenderer = new ObjectRenderer(SMB1_OBJECT_TILESET_PATH);
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
            world_->disableSystem(animationSystem_);
            lag -= MS_PER_UPDATE;
        }

        world_->enableSystem(animationSystem_);

        // Drawing
        BeginDrawing();

        ClearBackground({0, 0, 0});

        BeginMode2D(camera.get().camera);

        render(lag / MS_PER_UPDATE);

        // just for showing the center of the camera
        if (DEBUG) {
            DrawLine(camera->camera.target.x, - screenHeight_ * 10, camera->camera.target.x, screenHeight_ * 10, GREEN);
            DrawLine(- screenWidth_ * 5, camera->camera.target.y, screenWidth_ * 5, camera->camera.target.y, GREEN);
        }

        EndMode2D();

        EndDrawing();
    }

}

Game::~Game() {
    delete world_;
    delete mapRenderer;
    delete textureRenderer;
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

    initIdsMap();

    registerSystems();
}

void Game::initPlayers() {
    Vector2 spawnPositionP1 = pMap_->getSpawnPositionP1();
    Vector2 spawnPositionP2 = pMap_->getSpawnPositionP2();

    // First Player
    ECS::Entity* mario = world_->create();
    player1Id_ = mario->getEntityId();
    mario->assign<PlayerComponent>();
    mario->assign<AABBComponent>(
            Rectangle {
                    spawnPositionP1.x * 32,
                    spawnPositionP1.y * 32,
                    32,
                    32});
    mario->assign<TextureComponent>(MARIO_STAND);
    mario->assign<LeadCameraComponent>();
    mario->assign<CommandComponent>(std::map<Command, int> {
                                            {JUMP, KEY_UP},
                                            {MOVE_LEFT, KEY_LEFT},
                                            {MOVE_RIGHT, KEY_RIGHT},
                                            {DUCK, KEY_DOWN},
                                            {SHOOT, KEY_Z}
                                    });
    mario->assign<GravityComponent>();
    mario->assign<SolidComponent>();
    mario->assign<KineticComponent>(0.0f, 0.0f);
    mario->assign<MarioComponent>();

    // Second Player
    if (secondPlayer) {
        ECS::Entity* luigi = world_->create();
        luigi->assign<PlayerComponent>();
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
                {DUCK, KEY_S},
                {SHOOT, KEY_F}
        });
        luigi->assign<GravityComponent>();
        luigi->assign<SolidComponent>();
        luigi->assign<KineticComponent>(0.0f, 0.0f);
        luigi->assign<LuigiComponent>();
    }
}

void Game::registerSystems() {
    world_->registerSystem(new CameraSystem(
            screenWidth_,
            screenHeight_,
            pMap_->getPixelWidth(),
            pMap_->getPixelHeight()));
    world_->registerSystem(new PlayerSystem());
    world_->registerSystem(new EnemySystem());
    animationSystem_ = world_->registerSystem(new AnimationSystem());
    world_->registerSystem(new PhysicSystem());
    world_->registerSystem(new TileSystem());
    world_->registerSystem(new TimerSystem());
    world_->registerSystem(new IdsMapSystem());
    world_->disableSystem(world_->registerSystem(new FlagSystem()));
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
        else if (IsKeyDown(KEY_Z)) it->setCurrentCommand(KEY_Z);
        else if (IsKeyDown(KEY_F)) it->setCurrentCommand(KEY_F);
        else it->setNullCommand();
    }
}

void Game::initIdsMap() {
    int mapWidth, mapHeight;
    mapWidth = pMap_->getWidth();
    mapHeight = pMap_->getHeight();
    auto staticEntitiesMap = world_->create();
    auto idsMapComponent = staticEntitiesMap->assign<IdsMapComponent>(mapWidth, mapHeight);
    staticEntitiesMap->assign<StaticEntitiesMapComponent>();

    // init static ids map
    for (ECS::Entity* object : world_->each<TileComponent, AABBComponent, SolidComponent>()) {
        auto aabb = object->get<AABBComponent>();

        if (round(aabb->collisionBox_.width) <= GAME_TILE_SIZE && round(aabb->collisionBox_.height) <= GAME_TILE_SIZE) {
            unsigned int x = (int)round(aabb->left() / 32);
            unsigned int y = (int)round(aabb->top() / 32);
            idsMapComponent->set(object->getEntityId(), x, y);
        } else {
            for (int j = (int)(round(aabb->top() / 32)); j < (int)(aabb->bottom() / 32); j++) {
                for (int i = (int)(round(aabb->left() / 32)); i < (int)(aabb->right() / 32); i++) {
                    idsMapComponent->set(object->getEntityId(), i, j);
                }
            }
        }
    }

    auto kineticEntitiesMap = world_->create();
    auto KineticIdsMapComponent = kineticEntitiesMap->assign<IdsMapComponent>(mapWidth, mapHeight);
    kineticEntitiesMap->assign<KineticEntitiesMapComponent>();

    // init kinetic ids map
    for (ECS::Entity* entity : world_->each<AABBComponent, KineticComponent, SolidComponent>()) {
        auto aabb = entity->get<AABBComponent>();

        if (round(aabb->collisionBox_.width) <= GAME_TILE_SIZE && round(aabb->collisionBox_.height) <= GAME_TILE_SIZE) {
            unsigned int x = (int)round(aabb->left() / 32);
            unsigned int y = (int)round(aabb->top() / 32);
            KineticIdsMapComponent->set(entity->getEntityId(), x, y);
        } else {
            for (int j = (int)(round(aabb->top() / 32)); j < (int)(aabb->bottom() / 32); j++) {
                for (int i = (int)(round(aabb->left() / 32)); i < (int)(aabb->right() / 32); i++) {
                    KineticIdsMapComponent->set(entity->getEntityId(), i, j);
                }
            }
        }
    }
}

void Game::render(float d) {
    mapRenderer->renderBackground(world_);
    textureRenderer->renderTextureEntities(world_, d);
    textureRenderer->renderTileCollisionRect(world_);
    enemiesRenderer->renderEnemies(world_, d);
    objectRenderer->render(world_);
    mapRenderer->render(world_, d);
}
