//
// Created by paolo on 21/01/21.
//

#ifndef MARIO_MAKER_TILESYSTEM_H
#define MARIO_MAKER_TILESYSTEM_H
#include "ECS.h"
#include "Components.h"

using namespace ECS;

class TileSystem : public EntitySystem {
public:

    TileSystem();

    ~TileSystem();

    void tick(World* world, float delta);

    void configure(World* world) override;

    void unconfigure(World* world) override;

};


#endif //MARIO_MAKER_TILESYSTEM_H
