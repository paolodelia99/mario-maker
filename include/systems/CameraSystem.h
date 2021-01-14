//
// Created by paolo on 14/01/21.
//

#ifndef MARIO_MAKER_CAMERASYSTEM_H
#define MARIO_MAKER_CAMERASYSTEM_H

#include <raylib.h>
#include <math.h>
#include "ECS.h"

using namespace ECS;

class CameraSystem : public EntitySystem {
public:
    CameraSystem(int screenW, int screenH, int mapW, int mapH);

    void configure(World* world);

    void unconfigure(World* world);

    virtual ~CameraSystem();

    void tick(World* world, float delta);

private:
    CameraComponent* pCamera_;
    const int screenWidth_;
    const int screenHeight_;
    const int mapWidth_;
    const int mapHeight_;
};

#endif //MARIO_MAKER_CAMERASYSTEM_H
