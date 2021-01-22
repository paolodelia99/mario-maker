//
// Created by paolo on 14/01/21.
//

#ifndef MARIO_MAKER_PLAYERSYSTEM_H
#define MARIO_MAKER_PLAYERSYSTEM_H
#include "ECS.h"
#include <raylib.h>

using namespace ECS;

class PlayerSystem : public EntitySystem {
public:
    PlayerSystem();

    void configure(World* world) override;

    void unconfigure(World* world) override;

    void tick(World* world, float delta) override;

    void setAnimation(Entity* playerEntity, PlayerState state);

private:

    void collectCollectible(World* world, Entity* player);

    void eatMushroom(Entity *pEntity, Collectible::CollectibleType type);

    void handleFrozenTransform(Entity* entity);
};


#endif //MARIO_MAKER_PLAYERSYSTEM_H
