//
// Created by paolo on 14/02/21.
//

#include <iostream>
#include "SoundSystem.h"

SoundSystem::SoundSystem() {
    soundManager_ = new SoundManager();
}

SoundSystem::~SoundSystem() {
    delete soundManager_;
}

void SoundSystem::configure(World *world) {
    EntitySystem::configure(world);

    world->subscribe<SoundEvent>(this);
}

void SoundSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);

    world->unsubscribeAll(this);
}

void SoundSystem::tick(World *world, ECS::DefaultTickData data) {
    EntitySystem::tick(world, data);
}

void SoundSystem::receive(World *world, const SoundEvent &event) {
    std::cout << "sound received" << std::endl;
    soundManager_->playSound(event.soundId);
}
