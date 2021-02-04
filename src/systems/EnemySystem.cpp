//
// Created by paolo on 29/01/21.
//

#include <EnemySystem.h>

EnemySystem::EnemySystem() {

}

EnemySystem::~EnemySystem() {

}

void EnemySystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    for (auto destroy : world->each<DestroyDelayedComponent>()) {
        if (destroy->get<DestroyDelayedComponent>()->shouldDestroy()) {
            world->destroy(destroy);
        }
    }

    manageEnemyEntities(world);

    for (auto ent : world->each<EnemyComponent>()) {
        ent->remove<BottomCollisionComponent>();
        ent->remove<TopCollisionComponent>();
        ent->remove<LeftCollisionComponent>();
        ent->remove<RightCollisionComponent>();
    }
}

void EnemySystem::configure(World *world) {
    EntitySystem::configure(world);

    world->subscribe<KillEnemyEvent>(this);
}

void EnemySystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);

    world->unsubscribeAll(this);
}

void EnemySystem::receive(World *world, const KillEnemyEvent& killEnemyEvent) {
    auto enemy = killEnemyEvent.enemy;

    if (killEnemyEvent.killedByFireball
        && enemy->get<EnemyComponent>()->type_ != Enemy::TARTOSSO) {
        killEnemyWithFireball(enemy);
    } else {
        killEnemyWithJump(enemy);
    }
}

void EnemySystem::killEnemyWithFireball(Entity *enemy) {
    auto enemyComponent = enemy->get<EnemyComponent>();
    auto aabb = enemy->get<AABBComponent>();
    Enemy::Type type = enemy->get<EnemyComponent>()->type_;
    float xVelocity = enemy->has<LeftCollisionComponent>() ? 2.0f : -2.0f;
    ComponentHandle<TextureComponent> textureComponent;

    switch (type) {
        case Enemy::GOOMBA:
            enemy->removeAll();
            textureComponent = enemy->assign<TextureComponent>(TextureId::GOOMBA_1);
            break;
        case Enemy::KOOPA_TROOPA:
        case Enemy::RED_KOOPA_TROOPA:
            enemy->removeAll();
             textureComponent = enemy->assign<TextureComponent>(
                     type == Enemy::KOOPA_TROOPA ?
                     TextureId::G_TURLE_SHELL_STAND_1 : TextureId::R_TURLE_SHELL_STAND_1);
             textureComponent->h = GAME_TILE_SIZE;
             textureComponent->w = GAME_TILE_SIZE;
             aabb->collisionBox_.width = GAME_TILE_SIZE;
             aabb->collisionBox_.height = GAME_TILE_SIZE;
            break;
        case Enemy::PIRANHA_PLANT:
            enemy->removeAll();
            textureComponent = enemy->assign<TextureComponent>(TextureId::PIRANHA_PLANT_1);
        default:
            break;
    }

    enemy->assign<EnemyComponent>(enemyComponent->type_);
    enemy->assign<TileComponent>();
    textureComponent->flipV = true;
    enemy->assign<AABBComponent>(aabb->collisionBox_);
    enemy->assign<GravityComponent>();
    enemy->assign<KineticComponent>(xVelocity, -1.5f, 0.0f, -0.50f);
}

void EnemySystem::killEnemyWithJump(Entity *enemy) {
    auto enemyComponent = enemy->get<EnemyComponent>();
    auto aabb = enemy->get<AABBComponent>();
    Rectangle collisionRec = aabb->collisionBox_;
    auto kinetic = enemy->get<KineticComponent>();
    Enemy::Type type = enemy->get<EnemyComponent>()->type_;

    switch (type) {
        case Enemy::GOOMBA:
            enemy->removeAll();
            enemy->assign<EnemyComponent>(enemyComponent->type_);
            enemy->assign<TileComponent>();
            enemy->assign<TextureComponent>(TextureId::GOOMBA_DEAD);
            enemy->assign<AABBComponent>(aabb->collisionBox_);
            enemy->assign<DestroyDelayedComponent>(100);
            break;
        case Enemy::KOOPA_TROOPA:
        case Enemy::RED_KOOPA_TROOPA:
            enemy->removeAll();
            enemy->assign<EnemyComponent>(type == Enemy::KOOPA_TROOPA ?
                                          Enemy::GREEN_TURTLE_SHELL : Enemy::RED_TURTLE_SHELL);
            enemy->assign<TileComponent>();
            enemy->assign<TurtleShellComponent>(false);
            enemy->assign<SolidComponent>();
            enemy->assign<TextureComponent>(TextureId::G_TURLE_SHELL_STAND_1);
            enemy->assign<KineticComponent>(kinetic->speedX_, kinetic->speedY_);
            enemy->assign<AABBComponent>(Rectangle{
                    collisionRec.x,
                    collisionRec.y + GAME_TILE_SIZE / 2,
                    GAME_TILE_SIZE,
                    GAME_TILE_SIZE
            });
            break;
        case Enemy::GREEN_TURTLE_SHELL:
        case Enemy::RED_TURTLE_SHELL:
            if (enemy->get<TurtleShellComponent>()) {
                bool isMoving = enemy->get<TurtleShellComponent>()->isMoving_;
                if (isMoving) {
                    enemy->remove<AnimationComponent>();
                    enemy->remove<WalkComponent>();
                    enemy->assign<TextureComponent>(TextureId::G_TURLE_SHELL_STAND_1);
                } else {
                    enemy->assign<WalkComponent>(-1.5f);
                    enemy->assign<AnimationComponent>(std::vector<TextureId>{
                            TextureId::G_TURLE_SHELL_MOVE_1,
                            TextureId::G_TURLE_SHELL_MOVE_2,
                            TextureId::G_TURLE_SHELL_MOVE_3,
                            TextureId::G_TURLE_SHELL_MOVE_4,
                    }, 4);
                }
                enemy->get<TurtleShellComponent>()->isMoving_ = !isMoving;
            }
            break;
        case Enemy::TARTOSSO:
            if (enemy->get<TartossoComponent>()->tartossoState == Enemy::TartossoState::LIVE) {
                enemy->get<TartossoComponent>()->tartossoState = Enemy::TartossoState::DEAD;
                float prevVelocity = enemy->get<WalkComponent>()->speed;
                enemy->get<TartossoComponent>()->prevVelocity = prevVelocity;
                enemy->remove<WalkComponent>();
                enemy->assign<AnimationComponent>(std::vector<TextureId>{
                        TextureId::TARTOSSO_D_1,
                        TextureId::TARTOSSO_D_2,
                        TextureId::TARTOSSO_D_3,
                        TextureId::TARTOSSO_DEAD,
                }, 8, false, false, false);
            break;
        }
    }
}

void EnemySystem::manageEnemyEntities(World* world) {
    manageTartossos(world);

    managePiranhaPlants(world);
}

void EnemySystem::managePiranhaPlants(World* world) {
    world->each<EnemyComponent, GrowComponent>([&](
            Entity* entity,
            ComponentHandle<EnemyComponent> enemy,
            ComponentHandle<GrowComponent> growComponent) {
        if (enemy->type_ == Enemy::PIRANHA_PLANT) {
            if (!growComponent->finished()) {
                entity->get<AABBComponent>()->collisionBox_.y +=
                        growComponent->isGoingUp() ?
                        -MUSHROOM_GROW_SPEED : MUSHROOM_GROW_SPEED;
            } else {
                growComponent->wait();
            }
        }
    });
}

void EnemySystem::manageTartossos(World *world) {
    for (auto tartosso : world->each<TartossoComponent>()) {
        Enemy::TartossoState tartossoState = tartosso->get<TartossoComponent>()->tartossoState;
        switch (tartossoState) {
            case Enemy::DEAD:
                if (!tartosso->get<TartossoComponent>()->dead()) {
                    tartosso->assign<AnimationComponent>(std::vector<TextureId>{
                        TextureId::TARTOSSO_DEAD,
                        TextureId::TARTOSSO_D_3,
                        TextureId::TARTOSSO_D_2,
                        TextureId::TARTOSSO_D_1,
                        TextureId::TARTOSSO_1
                    }, 8);
                }
                break;
            case Enemy::TRANSFORMING:
                if (!tartosso->get<TartossoComponent>()->isTransforming()) {
                    tartosso->assign<WalkComponent>(tartosso->get<TartossoComponent>()->prevVelocity);
                    tartosso->get<TextureComponent>()->flipH = tartosso->get<TartossoComponent>()->prevVelocity > 0;
                    tartosso->assign<AnimationComponent>(std::vector<TextureId>{
                            TextureId::TARTOSSO_1,
                            TextureId::TARTOSSO_2
                    }, 8);
                }
                break;
            case Enemy::LIVE:
                break;
        }
    }
}
