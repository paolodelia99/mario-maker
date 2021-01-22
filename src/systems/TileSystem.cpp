//
// Created by paolo on 21/01/21.
//

#include "TileSystem.h"
#include "Constants.h"

TileSystem::~TileSystem() {

}

TileSystem::TileSystem() {

}

void TileSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    for (auto ent : world->each<TileComponent, BounceComponent, BottomCollisionComponent>())
    {
        // Play sound
        ent->get<BounceComponent>()->hit = true;
    }

    for (auto ent : world->each<BounceComponent>())
    {
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

    for (auto entity : world->each<TileComponent>()) {
        entity->remove<BottomCollisionComponent>();
        entity->remove<TopCollisionComponent>();
        entity->remove<LeftCollisionComponent>();
        entity->remove<RightCollisionComponent>();
    }
}

void TileSystem::createCoin(World* world, Entity* ent) {
    std::cout << "Coin created" << std::endl;
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

}

void TileSystem::spawnMegaMushroom(World *world, Entity *ent) {

}

void TileSystem::spawnFlameMushroom(World* world, Entity* ent) {

}

void TileSystem::spawnOneUpMushroom(World* world, Entity* ent) {

}

void TileSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);
}

void TileSystem::configure(World *world) {
    EntitySystem::configure(world);
}

