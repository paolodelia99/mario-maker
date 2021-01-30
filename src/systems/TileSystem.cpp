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

    manageGrowComponents(world);

    manageBounceComponents(world);

    removeCollisionComponents(world);
}

void TileSystem::createCoin(World* world, Entity* ent) {
    auto coin = world->create();
    auto entAABBComponent = ent->get<AABBComponent>();
    coin->assign<TextureComponent>(TextureId::COIN_1);
    coin->assign<AABBComponent>(
            Rectangle{
                    entAABBComponent->left(),
                    entAABBComponent->top(),
                    GAME_TILE_SIZE,
                    GAME_TILE_SIZE,
            });
    coin->assign<GravityComponent>();

    coin->assign<TileComponent>();
    coin->assign<KineticComponent>(0.0f, -80.f);
    coin->assign<TimerComponent>([=]() {
        //todo: implement score components
        world->destroy(coin);
        }, 25);

    //todo: sounds component is missing
}

void TileSystem::spawnSuperMarioMushroom(World* world, Entity* ent) {
    auto mushroom = world->create();
    auto entAABB = ent->get<AABBComponent>();
    mushroom->assign<TextureComponent>(TextureId::SUPER_MUSHROOM);

    mushroom->assign<GrowComponent>();
    mushroom->assign<CollectibleComponent>(Collectible::CollectibleType::SUPER_MARIO_MUSHROOM);
    mushroom->assign<AABBComponent>(Rectangle{
        entAABB->left() + 4,
        entAABB->top() - entAABB->collisionBox_.width / 2,
        GAME_TILE_SIZE,
        GAME_TILE_SIZE
    });
}

void TileSystem::spawnMegaMushroom(World *world, Entity *ent) {
    auto mushroom = world->create();
    auto entAABB = ent->get<AABBComponent>();
    int mushroomSize = GAME_TILE_SIZE * 2;
    mushroom->assign<TextureComponent>(TextureId::MEGA_MUSHROOM);

    mushroom->assign<CollectibleComponent>(Collectible::CollectibleType::MEGA_MUSHROOM);
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

    mushroom->assign<GrowComponent>();
    mushroom->assign<CollectibleComponent>(Collectible::CollectibleType::FLAME_MUSHROOM);
    mushroom->assign<AABBComponent>(Rectangle{
            entAABB->left(),
            entAABB->top() - entAABB->collisionBox_.width / 2,
            static_cast<float>(mushroomSize),
            static_cast<float>(mushroomSize)
    });
    mushroom->assign<GravityComponent>();
    mushroom->assign<SolidComponent>();
    mushroom->assign<TileComponent>();
}

void TileSystem::spawnOneUpMushroom(World* world, Entity* ent) {
    auto mushroom = world->create();
    auto entAABB = ent->get<AABBComponent>();
    mushroom->assign<TextureComponent>(TextureId::ONE_UP_MUSHROOM);

    mushroom->assign<GrowComponent>();
    mushroom->assign<CollectibleComponent>(Collectible::CollectibleType::ONE_UP_MUSHROOM);
    mushroom->assign<AABBComponent>(Rectangle{
            entAABB->left() + 4,
            entAABB->top() - entAABB->collisionBox_.width / 2,
            GAME_TILE_SIZE,
            GAME_TILE_SIZE
    });
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
    for (auto ent : world->each<GrowComponent, AABBComponent>()) {
        auto grow = ent->get<GrowComponent>();
        if (!grow->finished()) {
            ent->get<AABBComponent>()->collisionBox_.y -= MUSHROOM_GROW_SPEED;
        } else {
            ent->remove<GrowComponent>();

            if (ent->get<CollectibleComponent>()->type != Collectible::FLAME_MUSHROOM) {
                ent->assign<WalkComponent>(MUSHROOM_MOVE_SPEED);
                ent->assign<KineticComponent>();
            }
            ent->assign<GravityComponent>();
            ent->assign<SolidComponent>();
            ent->assign<TileComponent>();
        }
    }
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
