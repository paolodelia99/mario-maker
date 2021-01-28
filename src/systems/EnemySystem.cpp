//
// Created by paolo on 29/01/21.
//

#include <EnemySystem.h>

#include "EnemySystem.h"

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
        case Enemy::GREEN_TURTLE:
        case Enemy::RED_TURTLE:
            enemy->removeAll();
            enemy->assign<EnemyComponent>(type == Enemy::GREEN_TURTLE ?
            Enemy::GREEN_TURTLE_SHELL : Enemy::RED_TURTLE_SHELL);
            enemy->assign<TileComponent>();
            enemy->assign<TurtleShellComponent>(false);
            enemy->assign<SolidComponent>();
            enemy->assign<TextureComponent>(TextureId::G_TURLE_SHELL_STAND_1);
            enemy->assign<KineticComponent>(kinetic->speedX_, kinetic->speedY_);
            enemy->assign<AABBComponent>(Rectangle{
                collisionRec.x,
                collisionRec.y + 16,
                GAME_TILE_SIZE,
                GAME_TILE_SIZE
            });
            break;
        case Enemy::GREEN_TURTLE_SHELL:
        case Enemy::RED_TURTLE_SHELL:
            ComponentHandle<TurtleShellComponent> turtleShellComponent = enemy->get<TurtleShellComponent>();
            if (turtleShellComponent) {
                if (turtleShellComponent->isMoving_) {
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
                turtleShellComponent->isMoving_ = !turtleShellComponent->isMoving_;
            }
            break;
    }
}
