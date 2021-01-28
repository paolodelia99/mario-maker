//
// Created by paolo on 29/01/21.
//

#ifndef MARIO_MAKER_ENEMYSYSTEM_H
#define MARIO_MAKER_ENEMYSYSTEM_H
#include "ECS.h"
#include "Components.h"

using namespace ECS;

struct KillEnemyEvent {

    explicit KillEnemyEvent(Entity* enemy): enemy(enemy) {}

    Entity* enemy;
};


class EnemySystem : public EntitySystem, public EventSubscriber<KillEnemyEvent> {
public:
    EnemySystem();

    ~EnemySystem() override;

    void tick(World* world, float delta) override;

    void configure(World* world) override;

    void unconfigure(World* world) override;

    void receive(World* world, const KillEnemyEvent& killEnemyEvent) override;
};


#endif //MARIO_MAKER_ENEMYSYSTEM_H
