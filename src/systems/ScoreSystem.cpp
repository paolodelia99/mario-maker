//
// Created by paolo on 12/02/21.
//

#include <iostream>
#include "ScoreSystem.h"

ScoreSystem::ScoreSystem() {

}

ScoreSystem::~ScoreSystem() {

}

void ScoreSystem::receive(World *world, const CollisionWithCoin &event) {
    world->destroy(event.coin);
}

void ScoreSystem::configure(World *world) {
    EntitySystem::configure(world);

    world->subscribe<CollisionWithCoin>(this);
}

void ScoreSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);

    world->unsubscribeAll(this);
}

void ScoreSystem::tick(World *world, ECS::DefaultTickData data) {
    EntitySystem::tick(world, data);
}
