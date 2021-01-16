//
// Created by paolo on 16/01/21.
//

#ifndef MARIO_MAKER_ANIMATIONSYSTEM_H
#define MARIO_MAKER_ANIMATIONSYSTEM_H
#include "ECS.h"
#include "Components.h"

using namespace ECS;

class AnimationSystem : public EntitySystem {
public:

    AnimationSystem();

    void tick(World* world, float delta) override;

    void configure(World* world) override;

    virtual ~AnimationSystem();

    void unconfigure(World* world) override;

};


#endif //MARIO_MAKER_ANIMATIONSYSTEM_H
