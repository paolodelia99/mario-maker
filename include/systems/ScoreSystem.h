//
// Created by paolo on 12/02/21.
//

#ifndef MARIO_MAKER_SCORESYSTEM_H
#define MARIO_MAKER_SCORESYSTEM_H
#include "ECS.h"
#include "Events.h"

using namespace ECS;

class ScoreSystem :
        public EntitySystem,
        public EventSubscriber<CollisionWithCoin> {
public:

    ScoreSystem();

    ~ScoreSystem();

    void configure(World *world) override;

    void unconfigure(World *world) override;

    void tick(World *world, ECS::DefaultTickData data) override;

    void receive(World *world, const CollisionWithCoin &event) override;

};


#endif //MARIO_MAKER_SCORESYSTEM_H
