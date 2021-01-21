//
// Created by paolo on 21/01/21.
//

#include "TileSystem.h"

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

        if (!bounceComponent->finished()) {
            ent->get<AABBComponent>()->collisionBox_.y += (float) bounceComponent->getHeight();
        } else {
            bounceComponent->reset();
            bounceComponent->hit = false;
        }
    }

    for (auto entity : world->each<TileComponent>()) {
        entity->remove<BottomCollisionComponent>();
        entity->remove<TopCollisionComponent>();
        entity->remove<LeftCollisionComponent>();
        entity->remove<RightCollisionComponent>();
    }
}

void TileSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);
}

void TileSystem::configure(World *world) {
    EntitySystem::configure(world);
}

