//
// Created by paolo on 29/01/21.
//

#ifndef MARIO_MAKER_ENEMYSYSTEM_H
#define MARIO_MAKER_ENEMYSYSTEM_H
#include "ECS.h"
#include "Events.h"
#include "Components.h"

using namespace ECS;

class EnemySystem : public EntitySystem, public EventSubscriber<KillEnemyEvent> {
public:
    EnemySystem();

    ~EnemySystem() override;

    void tick(World* world, float delta) override;

    void configure(World* world) override;

    void unconfigure(World* world) override;

    void receive(World* world, const KillEnemyEvent& killEnemyEvent) override;

private:

    void killEnemyWithFireball(Entity *entity);

    void killEnemyWithJump(Entity *entity);

    void managePiranhaPlants(World* world);
};


#endif //MARIO_MAKER_ENEMYSYSTEM_H
