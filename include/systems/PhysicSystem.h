//
// Created by paolo on 15/01/21.
//

#ifndef MARIO_MAKER_PHYSICSYSTEM_H
#define MARIO_MAKER_PHYSICSYSTEM_H
#include <raylib.h>
#include <unordered_set>
#include "ECS.h"
#include "Components.h"
#include "Constants.h"
#include "TileSystem.h"

using namespace ECS;

class PhysicSystem : public EntitySystem {
public:

    PhysicSystem();

    void configure(World* world) override;

    void unconfigure(World* world) override;

    void tick(World* world, float delta) override;

    virtual ~PhysicSystem();

private:

    void checkIfOutsideWorld(World* world);

    void checkYCollision(Entity* ent1, Entity* ent2);

    void checkXCollision(Entity* ent1, Entity* ent2);

    void moveWalkComponents(World* world);

    void checkKineticKineticCollisions(World* world);

    void checkKineticTileCollisions(World* world);

    void applyForces(World* world);

    void applyGravity(World* world);

    std::unordered_set<int> getNeighborIds(ComponentHandle<ObjectMapComponent> map, ComponentHandle<AABBComponent> handle);

    void checkIfBreakComponent(Entity *ent1, Entity *ent2);
};


#endif //MARIO_MAKER_PHYSICSYSTEM_H
