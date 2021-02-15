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
    pMap_ = new GameMap(mapName);
    pMap_->loadMap(world_);
    mapRenderer = new MapRenderer(pMap_, SMB1_TILESET_PATH);
    textureRenderer = new TextureRenderer(SBM1_PLAYER_TILESET_PATH);
    enemiesRenderer = new EnemiesRenderer(SMB1_ENEMIES_TILESET_PATH);
    objectRenderer = new ObjectRenderer(SMB1_OBJECT_TILESET_PATH);
    textRenderer_ = new TextRenderer();
}

void Game::mainLoop() {

    initWorld();

    ECS::ComponentHandle<CameraComponent> camera = world_->getById(cameraId_)->get<CameraComponent>();

    SetTargetFPS(FPS);
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

        updateMusicStream();

        // Drawing
        BeginDrawing();

        ClearBackground({0, 0, 0});

        BeginMode2D(camera.get().camera);

        render(static_cast<float>(lag / MS_PER_UPDATE));

        // just for showing the center of the camera
#ifdef DEBUG
        DrawLine(camera->camera.target.x, - screenHeight_ * 10, camera->camera.target.x, screenHeight_ * 10, GREEN);
        DrawLine(- screenWidth_ * 5, camera->camera.target.y, screenWidth_ * 5, camera->camera.target.y, GREEN);
#endif

        EndMode2D();

        // Draw scores
        textRenderer_->render(world_);

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

    initTextEntities();

    startMusic();
}

void Game::initPlayers() {
    Vector2 spawnPositionP1 = pMap_->getSpawnPositionP1();
    Vector2 spawnPositionP2 = pMap_->getSpawnPositionP2();

    // First Player
    ECS::Entity* mario = world_->create();
    mario->getEntityId();
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
                                            {SPRINT, KEY_LEFT_SHIFT},
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
    world_->registerSystem(new IdsMapSystem());
    world_->registerSystem(new PhysicSystem());
    world_->registerSystem(new TileSystem());
    world_->registerSystem(new TimerSystem());
    world_->disableSystem(world_->registerSystem(new FlagSystem()));
    soundSystem_ = dynamic_cast<SoundSystem *>(world_->registerSystem(new SoundSystem()));
}

void Game::handleInput() {
    for (auto ent : world_->each<CommandComponent>())
    {
        auto commandComponent = ent->get<CommandComponent>();
        auto playerComponent = ent->get<PlayerComponent>();

        if (IsKeyDown(KEY_RIGHT)) commandComponent->setCurrentCommand(KEY_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) commandComponent->setCurrentCommand(KEY_LEFT);
        else if (IsKeyDown(KEY_DOWN)) commandComponent->setCurrentCommand(KEY_DOWN);
        else if (IsKeyDown(KEY_UP)) commandComponent->setCurrentCommand(KEY_UP);
        else if (IsKeyDown(KEY_W)) commandComponent->setCurrentCommand(KEY_W);
        else if (IsKeyDown(KEY_D)) commandComponent->setCurrentCommand(KEY_D);
        else if (IsKeyDown(KEY_A)) commandComponent->setCurrentCommand(KEY_A);
        else if (IsKeyDown(KEY_S)) commandComponent->setCurrentCommand(KEY_S);
        else if (IsKeyDown(KEY_Z)) commandComponent->setCurrentCommand(KEY_Z);
        else if (IsKeyDown(KEY_F)) commandComponent->setCurrentCommand(KEY_F);
        else commandComponent->setNullCommand();

        // make player sprint
        if (playerComponent) {
            if (ent->has<MarioComponent>()) {
                if (IsKeyDown(KEY_LEFT_SHIFT)) playerComponent->sprint = true;
                if (IsKeyUp(KEY_LEFT_SHIFT)) playerComponent->sprint = false;
            } else if (ent->has<LuigiComponent>()) {

            }
        }
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
    kineticEntitiesMap->assign<SpacialHashMapComponent>(mapWidth, mapHeight);
    kineticEntitiesMap->assign<KineticEntitiesMapComponent>();
}

void Game::render(float d) {
    mapRenderer->renderBackground(world_);
    textureRenderer->renderTextureEntities(world_, d);
    textureRenderer->renderTileCollisionRect(world_);
    enemiesRenderer->renderUnderTileEnemies(world_, d);
    objectRenderer->render(world_);
    mapRenderer->render(world_, d);
    enemiesRenderer->renderOverTileEnemies(world_, d);
    textRenderer_->renderScoreTextComponents(world_);
}

void Game::initTextEntities() {
    auto coinCounterText = world_->create();
    coinCounterText->assign<TextComponent>(
            Text::Type::COIN_COUNTER,
            Vector2{20.0f, 20.0f},
            0);

    auto scoreCounterText = world_->create();
    scoreCounterText->assign<TextComponent>(
            Text::Type::SCORE_COUNTER,
            Vector2{screenWidth_ - 175.f, 20.0f},
            0);

    auto timerText = world_->create();
    timerText->assign<TextComponent>(
            Text::Type::TIMER,
            Vector2{screenWidth_ - 45.f, 20.0f},
            360);

    world_->registerSystem(new ScoreSystem());
}

void Game::startMusic() {
    world_->emit<SetMusicEvent>(SetMusicEvent(MusicId::SMB1_MAIN_THEME));
}

void Game::updateMusicStream() {
    UpdateMusicStream(soundSystem_->getCurrentMusic());
}
