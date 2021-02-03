//
// Created by paolo on 03/02/21.
//

#ifndef MARIO_MAKER_FLAGSYSTEM_H
#define MARIO_MAKER_FLAGSYSTEM_H
#include "ECS.h"
#include "Events.h"
#include "Components.h"

using namespace ECS;

enum FlagState {
    NONE,
    TAKING_FLAG_DOWN,
    WALKING_TO_CASTE
};

class FlagSystem : public EntitySystem,
                   public EventSubscriber<CollisionWithFinalPole> {
public:
    FlagSystem();

    ~FlagSystem();

    void tick(World* world, float delta) override;

    void configure(World* world) override;

    void unconfigure(World* world) override;

    void receive(World* world, const CollisionWithFinalPole& collisionWithFinalPole) override;

private:
    Entity* winner_;
    FlagState state_ = NONE;
    Entity* winningFlag_;
    float flagPoleBottom_;
};

#endif //MARIO_MAKER_FLAGSYSTEM_H
