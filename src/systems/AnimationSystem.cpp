//
// Created by paolo on 16/01/21.
//
#include "systems/AnimationSystem.h"

AnimationSystem::~AnimationSystem() {

}

AnimationSystem::AnimationSystem() {

}

void AnimationSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    for (auto ent : world->each<AnimationComponent, TextureComponent>()) {
        auto animation = ent->get<AnimationComponent>();
        auto texture = ent->get<TextureComponent>();
        animation->counter++;

        if (animation->counter > animation->duration) {
            if (animation->flipH) texture->flipH = !texture->flipH;
            if (animation->flipV) texture->flipV = !texture->flipV;
            animation->counter = 0;
            animation->currentTexture = (animation->currentTexture + 1) % animation->textures.size();
            texture->textureId_ = animation->textures[animation->currentTexture];
            if (!animation->loop && animation->currentTexture == animation->textures.size() - 1) {
                ent->remove<AnimationComponent>();
            }
        }
    }
}

void AnimationSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);
}

void AnimationSystem::configure(World *world) {
    EntitySystem::configure(world);
}
