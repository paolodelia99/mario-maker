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
#include "Events.h"

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

    void checkKineticStaticCollisions(World* world);

    void applyForces(World* world);

    void applyGravity(World* world);

    std::unordered_set<int> getNeighborIds(ComponentHandle<IdsMapComponent> map, ComponentHandle<AABBComponent> handle);

    std::vector<std::unordered_set<int>> getNeighborIds(ComponentHandle<SpacialHashMapComponent> map, ComponentHandle<AABBComponent> aabb);

    void checkIfBreakComponent(Entity *ent1, Entity *ent2);

    void checkKillEnemy(Entity *ent1, Entity *ent2);

    void checkCollisionWithEnemy(Entity *ent1, Entity *ent2);

    void handlingBouncingComponents(World *world);

    bool validXCollision(Entity *ent1, Entity *ent2);

    bool validYCollision(Entity *ent1, Entity *ent2);

    void checkCollisionWithObject(Entity *ent1, Entity *ent2);
};


#endif //MARIO_MAKER_PHYSICSYSTEM_H
