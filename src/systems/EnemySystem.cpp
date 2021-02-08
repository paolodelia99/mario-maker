//
// Created by paolo on 29/01/21.
//
#include "systems/EnemySystem.h"

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
    world->subscribe<EnemyCollectableCollisionEvent>(this);
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
    bool wasBig = enemyComponent->isBig;

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
        case Enemy::GREEN_TURTLE_SHELL:
        case Enemy::RED_TURTLE_SHELL:
            enemy->removeAll();
            textureComponent = enemy->assign<TextureComponent>(
                    type == Enemy::GREEN_TURTLE_SHELL ?
                    TextureId::G_TURLE_SHELL_STAND_1 : TextureId::R_TURLE_SHELL_STAND_1);
            break;
        case Enemy::PIRANHA_PLANT:
            enemy->removeAll();
            textureComponent = enemy->assign<TextureComponent>(TextureId::PIRANHA_PLANT_1);
        default:
            break;
    }

    enemy->assign<EnemyComponent>(enemyComponent->type_);
    enemy->get<EnemyComponent>()->isBig = wasBig;
    enemy->assign<TileComponent>();
    textureComponent->flipV = true;
    enemy->assign<AABBComponent>(aabb->collisionBox_);
    enemy->get<TextureComponent>()->setDimensions(aabb->collisionBox_.width, aabb->collisionBox_.height);
    enemy->assign<GravityComponent>();
    enemy->assign<KineticComponent>(xVelocity, -1.5f, 0.0f, -0.50f);
}

void EnemySystem::killEnemyWithJump(Entity *enemy) {
    auto enemyComponent = enemy->get<EnemyComponent>();
    auto aabb = enemy->get<AABBComponent>();
    Rectangle collisionRec = aabb->collisionBox_;
    auto kinetic = enemy->get<KineticComponent>();
    Enemy::Type type = enemy->get<EnemyComponent>()->type_;
    bool wasBig = enemyComponent->isBig;
    World* world = enemy->getWorld();

    switch (type) {
        case Enemy::GOOMBA:
            enemy->removeAll();
            enemy->assign<EnemyComponent>(enemyComponent->type_);
            enemy->get<EnemyComponent>()->isBig = wasBig;
            enemy->assign<TileComponent>();
            enemy->assign<TextureComponent>(TextureId::GOOMBA_DEAD);
            enemy->get<TextureComponent>()->setDimensions(collisionRec.width, collisionRec.height);
            enemy->assign<AABBComponent>(collisionRec);
            enemy->assign<DestroyDelayedComponent>(100);
            if (wasBig) createChildGoombas(world, collisionRec);
            break;
        case Enemy::KOOPA_TROOPA:
        case Enemy::RED_KOOPA_TROOPA:
            enemy->removeAll();
            enemy->assign<EnemyComponent>(type == Enemy::KOOPA_TROOPA ?
                                          Enemy::GREEN_TURTLE_SHELL : Enemy::RED_TURTLE_SHELL);
            enemy->assign<TileComponent>();
            enemy->assign<TurtleShellComponent>(false);
            enemy->assign<SolidComponent>();
            enemy->assign<GravityComponent>();
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

    manageThwomps(world);

    managePiranhaPlants(world);

    manageParachutes(world);
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

void EnemySystem::manageParachutes(World *world) {
    for (auto parachute : world->each<ParachuteComponent>()) {
        auto entity = parachute->get<ParachuteComponent>()->associatedEntity;
        if (entity->has<BottomCollisionComponent>()) {
            world->destroy(parachute);
            entity->get<EnemyComponent>()->hasParachute = false;
            entity->assign<WalkComponent>();
        } else {
            parachute->get<AABBComponent>()->setBottom(entity->get<AABBComponent>()->top());
        }
    }
}

Entity* createGoomba(World* world, Rectangle collisionBox) {
    Entity* goomba = world->create();
    goomba->assign<SolidComponent>();
    goomba->assign<GravityComponent>();
    goomba->assign<EnemyComponent>(Enemy::Type::GOOMBA);
    goomba->assign<AABBComponent>(collisionBox);
    goomba->assign<KineticComponent>();
    goomba->assign<TextureComponent>(TextureId::GOOMBA_1);
    goomba->assign<AnimationComponent>(std::vector<TextureId>{
            TextureId::GOOMBA_1,
            TextureId::GOOMBA_2
    }, 8);

    return goomba;
}

void EnemySystem::createChildGoombas(World *world, Rectangle sourceRect) {
    float centerX = sourceRect.x + sourceRect.width / 2;
    float centerY = sourceRect.y + sourceRect.height / 2;
    Rectangle collisionBox1 = {
            sourceRect.x,
            centerY,
            GAME_TILE_SIZE,
            GAME_TILE_SIZE
    };
    Rectangle collisionBox2 = {
            centerX,
            centerY,
            GAME_TILE_SIZE,
            GAME_TILE_SIZE
    };
    float accX = 0.5f, accY = -1.5f;

    Entity* goomba1 = createGoomba(world, collisionBox1);
    Entity* goomba2 = createGoomba(world, collisionBox2);
    auto kinetic1 = goomba1->get<KineticComponent>();
    auto kinetic2 = goomba2->get<KineticComponent>();

    kinetic1->accX_ = -accX;
    kinetic1->accY_ = accY;
    kinetic2->accX_ = accX;
    kinetic2->accY_ = accY;

    goomba1->assign<TimerComponent>([goomba1]() {
        goomba1->assign<WalkComponent>();
    }, 100);
    goomba2->assign<TimerComponent>([goomba2]() {
        goomba2->assign<WalkComponent>();
        goomba2->get<WalkComponent>()->invertSpeed();
    }, 100);
}

void EnemySystem::receive(World *world, const EnemyCollectableCollisionEvent &event) {
    Entity* collectibleEntity = event.collectable;
    Entity* enemy = event.enemy;

    eatMushroom(enemy, collectibleEntity->get<CollectibleComponent>()->type);
    world->destroy(collectibleEntity);
}

void EnemySystem::eatMushroom(Entity *entity, Collectible::CollectibleType type) {

    TextureId currentTexture = entity->get<TextureComponent>()->textureId_;

    if (entity->get<EnemyComponent>()->type_ == Enemy::THWOMP) return;

    switch (type) {
        case Collectible::CollectibleType::SUPER_MARIO_MUSHROOM:
        case Collectible::MEGA_MUSHROOM: {
            if (!entity->get<EnemyComponent>()->isBig) {
                float oldHeight = entity->get<AABBComponent>()->top();
                auto animComponent = entity->get<AnimationComponent>();
                std::vector<TextureId> oldAnimation = animComponent->textures;
                int oldDuration = animComponent->duration;
                entity->assign<AnimationComponent>(std::vector<TextureId>{
                    currentTexture,
                    TextureId::EMPTY,
                    currentTexture,
                    TextureId::EMPTY,
                    currentTexture,
                    currentTexture,
                    TextureId::EMPTY,
                    TextureId::EMPTY,
                    currentTexture,
                }, 4, false, false, false);
                entity->assign<FrozenComponent>();
                entity->assign<TimerComponent>([=]() {
                    auto aabb = entity->get<AABBComponent>();
                    entity->remove<FrozenComponent>();
                    entity->remove<AnimationComponent>();
                    aabb->setTop(oldHeight - GAME_TILE_SIZE);
                    aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                    aabb->collisionBox_.width = GAME_TILE_SIZE * 2;
                    entity->get<EnemyComponent>()->isBig = true;
                    entity->assign<AnimationComponent>(oldAnimation, oldDuration);
                    entity->get<TextureComponent>()->setDimensions(aabb->getWidth(), aabb->getHeight());
                }, 40);
            }
        }
            break;
        case Collectible::NONE:
        case Collectible::FLAME_MUSHROOM:
        case Collectible::ONE_UP_MUSHROOM:
            break;
    }

}

void EnemySystem::manageThwomps(World *world) {
    for (auto thwomp : world->each<ThwompComponent, AABBComponent, EnemyComponent>()) {
        if (thwomp->has<FrozenComponent>()) continue;
        auto thwompComponent = thwomp->get<ThwompComponent>();
        auto state = thwompComponent->state;
        auto aabb = thwomp->get<AABBComponent>();
        float minDist = FLT_MAX;

        switch (state) {
            case Enemy::ThwompState::RESTING:
                for (auto player : world->each<PlayerComponent, AABBComponent>()) {
                    auto playerAABB = player->get<AABBComponent>();
                    float dist = std::sqrt(
                            std::pow(aabb->left() - playerAABB->left(), 2) +
                            std::pow(aabb->top() - playerAABB->top(), 2));
                    if (dist < minDist) minDist = dist;
                }
                if (minDist <= 300) {
                    thwompComponent->state = Enemy::ThwompState::GOING_DOWN;
                    thwomp->get<TextureComponent>()->textureId_ = TextureId::THWOMP_ANGRY_V;
                    thwomp->assign<TimerComponent>([=]() {
                        auto kinetic = thwomp->get<KineticComponent>();
                        if (kinetic) kinetic->accY_ = +4.0f;
                    }, 50);
                } else {
                    thwomp->get<TextureComponent>()->textureId_ = TextureId::THWOMP_2_V;
                }
                break;
            case Enemy::ThwompState::GOING_DOWN:
                if (thwomp->has<BottomCollisionComponent>()) {
                    thwompComponent->state = Enemy::ThwompState::WAITING;
                    thwomp->assign<TimerComponent>([=]() {
                        auto kinetic = thwomp->get<KineticComponent>();
                        thwompComponent->state = Enemy::ThwompState::GOING_UP;
                        if (kinetic) kinetic->accY_ = -1.5f;
                    }, 80);
                }
                break;
            case Enemy::ThwompState::WAITING:
                break;
            case Enemy::ThwompState::GOING_UP:
                if (aabb->top() <= thwompComponent->initialHeight) {
                    auto kinetic = thwomp->get<KineticComponent>();
                    if (kinetic) {
                        kinetic->accY_ = 0.0f;
                        kinetic->speedY_ = 0.0f;
                    }
                    thwompComponent->state = Enemy::ThwompState::RESTING;
                }
                break;
        }
    }
}
