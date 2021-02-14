//
// Created by paolo on 21/01/21.
//

#include "TileSystem.h"
#include "Constants.h"

TileSystem::~TileSystem() {}

TileSystem::TileSystem() {}

void TileSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    for (auto ent : world->each<TileComponent, BounceComponent, BottomCollisionComponent>()) {
        // Play sound
        ent->get<BounceComponent>()->hit = true;
    }

    manageCannons(world);

    manageGrowComponents(world);

    manageBounceComponents(world);

    removeCollisionComponents(world);
}

void TileSystem::createCoin(World* world, Entity* ent) {
    auto coin = world->create();
    auto entAABBComponent = ent->get<AABBComponent>();
    coin->assign<AABBComponent>(
            Rectangle{
                    entAABBComponent->left(),
                    entAABBComponent->top(),
                    GAME_TILE_SIZE,
                    GAME_TILE_SIZE,
            });
    coin->assign<GravityComponent>();

    coin->assign<ObjectComponent>(Object::Type::COIN);
    coin->assign<TextureComponent>(TextureId::COIN_1);
    coin->assign<AnimationComponent>(std::vector<TextureId>{
            TextureId::COIN_1,
            TextureId::COIN_2,
            TextureId::COIN_3,
            TextureId::COIN_4,
            TextureId::COIN_5,
            TextureId::COIN_6,
            TextureId::COIN_7,
            TextureId::COIN_8,
    }, 4);
    coin->assign<KineticComponent>(0.0f, -80.f);
    coin->assign<TimerComponent>([=]() {
        world->destroy(coin);
        }, 30);
}

void TileSystem::spawnSuperMarioMushroom(World* world, Entity* ent) {
    auto mushroom = world->create();
    auto entAABB = ent->get<AABBComponent>();
    mushroom->assign<TextureComponent>(TextureId::SUPER_MUSHROOM);
    mushroom->assign<TileComponent>();
    mushroom->assign<VerticalGrowComponent>();
    mushroom->assign<CollectibleComponent>(Collectible::Type::SUPER_MARIO_MUSHROOM);
    mushroom->assign<AABBComponent>(Rectangle{
        entAABB->left() + 4,
        entAABB->top() - entAABB->collisionBox_.width / 2,
        GAME_TILE_SIZE,
        GAME_TILE_SIZE
    });
    world->emit<SoundEvent>(SoundEvent{SoundId::POWER_APPEARS});
}

void TileSystem::spawnMegaMushroom(World *world, Entity *ent) {
    auto mushroom = world->create();
    auto entAABB = ent->get<AABBComponent>();
    int mushroomSize = GAME_TILE_SIZE * 2;
    mushroom->assign<TextureComponent>(TextureId::MEGA_MUSHROOM);

    mushroom->assign<CollectibleComponent>(Collectible::Type::MEGA_MUSHROOM);
    mushroom->assign<AABBComponent>(Rectangle{
            entAABB->left(),
            entAABB->top() - mushroomSize,
            static_cast<float>(mushroomSize),
            static_cast<float>(mushroomSize)
    });
    mushroom->assign<WalkComponent>(MUSHROOM_MOVE_SPEED);
    mushroom->assign<GravityComponent>();
    mushroom->assign<SolidComponent>();
    mushroom->assign<TileComponent>();
    mushroom->assign<KineticComponent>(0.0f, 0.0f, +0.5f, -1.0f);
    world->emit<SoundEvent>(SoundEvent{SoundId::POWER_APPEARS});
}

void TileSystem::spawnFlameMushroom(World* world, Entity* ent) {
    auto mushroom = world->create();
    auto entAABB = ent->get<AABBComponent>();
    int mushroomSize = GAME_TILE_SIZE;
    mushroom->assign<TextureComponent>(TextureId::FLAME_FLOWER_1);
    mushroom->assign<AnimationComponent>(std::vector<TextureId>{
        TextureId::FLAME_FLOWER_1,
        TextureId::FLAME_FLOWER_2,
        TextureId::FLAME_FLOWER_3,
        TextureId::FLAME_FLOWER_4,
    }, 4);

    mushroom->assign<VerticalGrowComponent>();
    mushroom->assign<CollectibleComponent>(Collectible::Type::FLAME_MUSHROOM);
    mushroom->assign<AABBComponent>(Rectangle{
            entAABB->left(),
            entAABB->top() - entAABB->collisionBox_.width / 2,
            static_cast<float>(mushroomSize),
            static_cast<float>(mushroomSize)
    });
    mushroom->assign<GravityComponent>();
    mushroom->assign<SolidComponent>();
    mushroom->assign<TileComponent>();
    world->emit<SoundEvent>(SoundEvent{SoundId::POWER_APPEARS});
}

void TileSystem::spawnOneUpMushroom(World* world, Entity* ent) {
    auto mushroom = world->create();
    auto entAABB = ent->get<AABBComponent>();

    mushroom->assign<TextureComponent>(TextureId::ONE_UP_MUSHROOM);
    mushroom->assign<VerticalGrowComponent>();
    mushroom->assign<TileComponent>();
    mushroom->assign<CollectibleComponent>(Collectible::Type::ONE_UP_MUSHROOM);
    mushroom->assign<AABBComponent>(Rectangle{
            entAABB->left() + 4,
            entAABB->top() - entAABB->collisionBox_.width / 2,
            GAME_TILE_SIZE,
            GAME_TILE_SIZE
    });
    world->emit<SoundEvent>(SoundEvent{SoundId::POWER_APPEARS});
}

void TileSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);

    world->unsubscribeAll(this);
}

void TileSystem::configure(World *world) {
    EntitySystem::configure(world);

    world->subscribe<BreakEvent>(this);
}

void TileSystem::manageGrowComponents(World *world) {
    for (auto ent : world->each<VerticalGrowComponent, AABBComponent, TileComponent>()) {
        auto grow = ent->get<VerticalGrowComponent>();
        if (!grow->finished()) {
            ent->get<AABBComponent>()->collisionBox_.y -= MUSHROOM_GROW_SPEED;
        } else {
            ent->remove<VerticalGrowComponent>();

            if (ent->get<CollectibleComponent>()->type != Collectible::FLAME_MUSHROOM) {
                ent->assign<WalkComponent>(MUSHROOM_MOVE_SPEED);
            }
            ent->assign<KineticComponent>();
            ent->assign<GravityComponent>();
            ent->assign<SolidComponent>();
            ent->assign<TileComponent>();
        }
    }

    world->each<HorizontalGrowComponent, AABBComponent>([=](
            Entity* entity,
            ComponentHandle<HorizontalGrowComponent> growComponent,
            ComponentHandle<AABBComponent> aabb) {
        if (growComponent->finished()) {
            entity->assign<SolidComponent>();
            auto enemyComponent = entity->get<EnemyComponent>();
            auto collectibleComponent = entity->get<CollectibleComponent>();

            if (enemyComponent) {
                switch (enemyComponent->type_) {
                    case Enemy::GOOMBA:
                        if (growComponent->isGoingLeft()) {
                            entity->assign<WalkComponent>();
                        } else {
                            entity->assign<WalkComponent>(0.6f);
                        }
                        entity->assign<AnimationComponent>(std::vector<TextureId>{
                                TextureId::GOOMBA_1,
                                TextureId::GOOMBA_2
                        }, 8);
                        entity->assign<GravityComponent>();
                        entity->assign<KineticComponent>();
                        entity->remove<FrozenComponent>();
                        break;
                    case Enemy::GOOMBRAT:
                        break;
                    case Enemy::GREEN_TURTLE_SHELL:
                        break;
                    case Enemy::RED_TURTLE_SHELL:
                        break;
                    case Enemy::BULLET_BILL:
                        entity->remove<UnderTileComponent>();
                        entity->assign<OverTileComponent>();
                        if (growComponent->isGoingLeft()) {
                            entity->assign<WalkComponent>(-1.8f);
                        } else {
                            entity->assign<WalkComponent>(1.8f);
                        }
                        entity->assign<KineticComponent>();
                        break;
                    default:
                        break;
                }
            } else if (collectibleComponent) {
                entity->assign<GravityComponent>();
                entity->assign<KineticComponent>();
                if (growComponent->isGoingLeft()) {
                    entity->assign<WalkComponent>();
                } else {
                    entity->assign<WalkComponent>(0.6f);
                }
            }
            entity->remove<HorizontalGrowComponent>();
        } else {
            if (growComponent->isGoingLeft()) {
                aabb->collisionBox_.x -= MUSHROOM_GROW_SPEED;
            } else {
                aabb->collisionBox_.x += MUSHROOM_GROW_SPEED;
            }
        }
    });
}

void TileSystem::removeCollisionComponents(World* world) {
    for (auto entity : world->each<TileComponent>()) {
        entity->remove<BottomCollisionComponent>();
        entity->remove<TopCollisionComponent>();
        entity->remove<LeftCollisionComponent>();
        entity->remove<RightCollisionComponent>();
    }
}

void TileSystem::manageBounceComponents(World *world) {
    for (auto ent : world->each<BounceComponent>()) {
        auto bounceComponent = ent->get<BounceComponent>();
        if (!bounceComponent->hit) continue;

        if (ent->has<QuestionBlockComponent>()) {
            ent->remove<AnimationComponent>();
            ent->assign<TextureComponent>(TextureId::QUESTION_BLOCK_OFF);
        }

        if (!bounceComponent->finished()) {
            ent->get<AABBComponent>()->collisionBox_.y += (float) bounceComponent->getHeight();
        } else {
            bounceComponent->reset();
            bounceComponent->hit = false;
            if (ent->has<QuestionBlockComponent>()) {
                auto questionComponent = ent->get<QuestionBlockComponent>();
                if (questionComponent->coin) createCoin(world, ent);
                if (questionComponent->superMarioMushroom) spawnSuperMarioMushroom(world, ent);
                if (questionComponent->megaMushroom) spawnMegaMushroom(world, ent);
                if (questionComponent->flameMushroom) spawnFlameMushroom(world, ent);
                if (questionComponent->oneUpMushroom) spawnOneUpMushroom(world, ent);
                ent->remove<QuestionBlockComponent>();
                ent->remove<BounceComponent>();
            }
        }
    }
}

void TileSystem::receive(World *world, const BreakEvent &breakEvent) {
    createDebris(world, breakEvent.x, breakEvent.y);
}

void TileSystem::createDebris(World *world, float xf, float yf) {
    auto ent1 = world->create();
    ent1->assign<TextureComponent>(TextureId::BRICK_DEBRIS_1);
    ent1->assign<AABBComponent>(Rectangle{xf, yf, GAME_TILE_SIZE / 2, GAME_TILE_SIZE / 2});
    ent1->assign<TileComponent>();
    ent1->assign<GravityComponent>();
    ent1->assign<KineticComponent>(-0.0f, -0.0f, -2.5f, -2.5f);

    auto ent2 = world->create();
    ent2->assign<TextureComponent>(TextureId::BRICK_DEBRIS_2);
    ent2->assign<AABBComponent>(Rectangle{xf + 8, yf, GAME_TILE_SIZE / 2, GAME_TILE_SIZE / 2});
    ent2->assign<GravityComponent>();
    ent2->assign<TileComponent>();
    ent2->assign<KineticComponent>(-0.0f, -0.0f, 2.5f, -2.5f);

    auto ent3 = world->create();
    ent3->assign<TextureComponent>(TextureId::BRICK_DEBRIS_3);
    ent3->assign<AABBComponent>(Rectangle{xf, yf - 8, GAME_TILE_SIZE / 2, GAME_TILE_SIZE / 2});
    ent3->assign<GravityComponent>();
    ent3->assign<KineticComponent>(-0.0f, -0.0f, -2.0f, -1.5f);
    ent3->assign<TileComponent>();

    auto ent4 = world->create();
    ent4->assign<TextureComponent>(TextureId::BRICK_DEBRIS_4);
    ent4->assign<AABBComponent>(Rectangle{xf + 8, yf - 8, GAME_TILE_SIZE / 2, GAME_TILE_SIZE / 2});
    ent4->assign<TileComponent>();
    ent4->assign<GravityComponent>();
    ent4->assign<KineticComponent>(-0.0f, -0.0f, +2.0f, -1.5f);
}

void TileSystem::manageCannons(World *world) {
    world->each<CannonComponent, AABBComponent>([&](
            Entity* entity,
            ComponentHandle<CannonComponent> cannonComponent,
            ComponentHandle<AABBComponent> aabb) {
        if (entity->has<FrozenComponent>()) return;

        if (cannonComponent->canShoot()) {
            Entity* player = world->findFirst<LeadCameraComponent>();
            auto playerAABB = player->get<AABBComponent>();
            bool shootLeft = true;

            if (playerAABB->getCenterX() >= aabb->getCenterX()) shootLeft = false;

            spawnEntityFromCannon(world, cannonComponent->getType(), aabb->collisionBox_, shootLeft);
        }
    });
}

void TileSystem::spawnEntityFromCannon(World *world, Enemy::BulletType type, Rectangle rectangle, bool shootLeft) {
    Entity* entity = world->create();
    float xOffset = shootLeft ? -4.0f : 4.0f;
    entity->assign<AABBComponent>(Rectangle{
        rectangle.x + xOffset,
        rectangle.y - 2,
        GAME_TILE_SIZE,
        GAME_TILE_SIZE});
    entity->assign<HorizontalGrowComponent>(shootLeft, 120);
    entity->assign<UnderTileComponent>();

    switch (type) {
        case Enemy::BulletType::B_BULLET_BILL:
            if (shootLeft) {
                entity->assign<TextureComponent>(TextureId::BULLET_BILL);
            } else {
                entity->assign<TextureComponent>(TextureId::BULLET_BILL, !shootLeft);
            }
            entity->assign<EnemyComponent>(Enemy::Type::BULLET_BILL);
            break;
        case Enemy::BulletType::R_BULLET_BILL:
            break;
        case Enemy::BulletType::BLUE_BULLET_BILL:
            break;
        case Enemy::BulletType::SUPER_MUSHROOM:
            entity->assign<CollectibleComponent>(Collectible::Type::SUPER_MARIO_MUSHROOM);
            entity->assign<TextureComponent>(TextureId::SUPER_MUSHROOM);
            entity->assign<TileComponent>();
            break;
        case Enemy::BulletType::ONE_UP_MUSHROOM:
            entity->assign<CollectibleComponent>(Collectible::Type::ONE_UP_MUSHROOM);
            entity->assign<TextureComponent>(TextureId::ONE_UP_MUSHROOM);
            entity->assign<TileComponent>();
            break;
        case Enemy::BulletType::B_GOOMBA:
            if (shootLeft) {
                entity->assign<TextureComponent>(TextureId::GOOMBA_1);
            } else {
                entity->assign<TextureComponent>(TextureId::GOOMBA_1, !shootLeft);
            }
            entity->assign<EnemyComponent>(Enemy::Type::GOOMBA);
            break;
        default:
            break;
    }
}
