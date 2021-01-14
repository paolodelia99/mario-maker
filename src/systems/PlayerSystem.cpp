//
// Created by paolo on 14/01/21.
//

#include <Components.h>
#include <iostream>
#include "systems/PlayerSystem.h"

PlayerSystem::PlayerSystem() {

}

void PlayerSystem::configure(World *world) {
    EntitySystem::configure(world);
}

void PlayerSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);
}

void PlayerSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    for (auto ent : world->each<PlayerComponent, CommandComponent>()) {
        Command currentCommand = ent->get<CommandComponent>()->currentCommand_;
        ComponentHandle<AABBComponent> playerAABB = ent->get<AABBComponent>();
        switch (currentCommand) {
            case NONE_COMMAND:
                break;
            case JUMP:
                playerAABB->collisionBox_.y -= 2;
                break;
            case MOVE_RIGHT:
                playerAABB->collisionBox_.x += 2;
                break;
            case MOVE_LEFT:
                playerAABB->collisionBox_.x -= 2;
                break;
            case CROUCH_DOWN:
                playerAABB->collisionBox_.y += 2;
                break;
            case SPRINT:
                break;
            case SPECIAL:
                break;
        }
    }
}
