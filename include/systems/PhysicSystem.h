//
// Created by paolo on 15/01/21.
//

#ifndef MARIO_MAKER_PHYSICSYSTEM_H
#define MARIO_MAKER_PHYSICSYSTEM_H
#include <raylib.h>
#include "ECS.h"
#include "Components.h"
#include "Constants.h"


using namespace ECS;

class PhysicSystem : public EntitySystem {
public:

    PhysicSystem();

    void configure(World* world) override;

    void unconfigure(World* world) override;

    void tick(World* world, float delta) override;

    virtual ~PhysicSystem();

private:

    void checkYCollision(Entity* ent, Entity* object);

    void checkXCollision(Entity* ent1, Entity* object);

    bool checkRecCollision(Rectangle rec1, Rectangle rec2);

};


#endif //MARIO_MAKER_PHYSICSYSTEM_H
