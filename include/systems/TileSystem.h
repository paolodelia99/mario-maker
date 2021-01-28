//
// Created by paolo on 21/01/21.
//

#ifndef MARIO_MAKER_TILESYSTEM_H
#define MARIO_MAKER_TILESYSTEM_H
#include "ECS.h"
#include "Components.h"
#include "Constants.h"

using namespace ECS;

struct BreakEvent {
    float x;
    float y;
};

class TileSystem : public EntitySystem, public EventSubscriber<BreakEvent> {
public:

    TileSystem();

    ~TileSystem();

    void tick(World* world, float delta);

    void configure(World* world) override;

    void unconfigure(World* world) override;

private:

    void createCoin(World *world, Entity *ent);

    void spawnSuperMarioMushroom(World *world, Entity *ent);

    void spawnMegaMushroom(World *world, Entity *ent);

    void spawnFlameMushroom(World *world, Entity *ent);

    void spawnOneUpMushroom(World *world, Entity *ent);

    void manageGrowComponents(World *world);

    void removeCollisionComponents(World* world);

    void manageBounceComponents(World* world);

    virtual void receive(World* world, const BreakEvent& breakEvent) override;

    void createDebris(World *world, float xf, float yf);
};


#endif //MARIO_MAKER_TILESYSTEM_H
