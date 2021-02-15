//
// Created by paolo on 14/02/21.
//

#include <iostream>
#include "SoundSystem.h"

SoundSystem::SoundSystem() {
    audioManager_ = new AudioManager();
}

SoundSystem::~SoundSystem() {
    delete audioManager_;
}

void SoundSystem::configure(World *world) {
    EntitySystem::configure(world);

    world->subscribe<SoundEvent>(this);
    world->subscribe<SetMusicEvent>(this);
}

void SoundSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);

    world->unsubscribeAll(this);
}

void SoundSystem::tick(World *world, ECS::DefaultTickData data) {
    EntitySystem::tick(world, data);
}

bool doesSoundStopMusic(SoundId soundId) {
    switch (soundId) {
        case SoundId::MARIO_DIE:
        case SoundId::DOWN_THE_FLAG_POLE:
        case SoundId::STAGE_CLEAR:
            return true;
        default:
            return false;
    }
}

void SoundSystem::receive(World *world, const SoundEvent &event) {
    audioManager_->playSound(event.soundId);
    if (doesSoundStopMusic(event.soundId)) audioManager_->stopCurrentMusic();
}

void SoundSystem::receive(World *world, const SetMusicEvent &event) {
    std::cout << "play music" << std::endl;
    audioManager_->playMusic(event.musicId);
}

Music SoundSystem::getCurrentMusic() {
    return audioManager_->getCurrentMusic();
}
