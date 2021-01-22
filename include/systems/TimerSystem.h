//
// Created by paolo on 22/01/21.
//

#ifndef MARIO_MAKER_TIMERSYSTEM_H
#define MARIO_MAKER_TIMERSYSTEM_H
#include "ECS.h"
#include "Components.h"

using namespace ECS;

class TimerSystem : public EntitySystem {
public:

    TimerSystem();

    ~TimerSystem() override;

    void tick(World* world, float delta);

    void configure(World* world) override;

    void unconfigure(World* world) override;

};


#endif //MARIO_MAKER_TIMERSYSTEM_H
