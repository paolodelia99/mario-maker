//
// Created by paolo on 31/01/21.
//
#pragma once

#include "ECS.h"

using namespace ECS;

struct EnemyCollisionEvent {
    EnemyCollisionEvent(Entity* player, Entity* enemy)
            : player(player), enemy(enemy) {}

    Entity* player;
    Entity* enemy;
};

struct BreakEvent {
    float x;
    float y;
};

struct KillEnemyEvent {

    explicit KillEnemyEvent(Entity* enemy): enemy(enemy) {}

    KillEnemyEvent(Entity *enemy, bool killedByFireball)
    : enemy(enemy), killedByFireball(killedByFireball) {}

    Entity* enemy;
    bool killedByFireball = false;
};

struct CollisionWithFinalPole {

    CollisionWithFinalPole(Entity *player, float flagPoleBottom)
    : player(player), flagPoleBottom(flagPoleBottom) {}

    Entity* player;
    float flagPoleBottom;
};