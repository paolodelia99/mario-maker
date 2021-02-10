//
// Created by paolo on 29/01/21.
//

#ifndef MARIO_MAKER_ENEMYSYSTEM_H
#define MARIO_MAKER_ENEMYSYSTEM_H
#include "ECS.h"
#include "Events.h"
#include "Components.h"

using namespace ECS;

class EnemySystem :
        public EntitySystem,
        public EventSubscriber<KillEnemyEvent>,
        public EventSubscriber<EnemyCollectableCollisionEvent> {
public:
    EnemySystem();

    ~EnemySystem() override;

    void tick(World* world, float delta) override;

    void configure(World* world) override;

    void unconfigure(World* world) override;

    void receive(World* world, const KillEnemyEvent& killEnemyEvent) override;

    void receive(World* world, const EnemyCollectableCollisionEvent& event) override;

private:

    void manageEnemyEntities(World* world);

    void killEnemyWithFireball(Entity *entity);

    void killEnemyWithJump(Entity *entity);

    void managePiranhaPlants(World* world);

    void manageTartossos(World *world);

    void manageParachutes(World *world);

    void createChildGoombas(World *world, Rectangle sourceRect, bool b);

    void eatMushroom(Entity *entity, Collectible::CollectibleType type);

    void manageThwomps(World *world);
};


#endif //MARIO_MAKER_ENEMYSYSTEM_H
