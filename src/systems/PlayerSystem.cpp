//
// Created by paolo on 14/01/21.
//

#include <Components.h>
#include <constants.h>
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

    for (auto ent : world->each<PlayerComponent, CommandComponent, KineticComponent>()) {
        auto kinetic = ent->get<KineticComponent>();
        Command currentCommand = ent->get<CommandComponent>()->currentCommand_;
        switch (currentCommand) {
            case NONE_COMMAND:
                kinetic->accX_ = 0;
                kinetic->accY_ = 0;
                break;
            case JUMP:
                if (ent->has<BottomCollisionComponent>()) {
                    kinetic->accY_ = -MARIO_JUMP_ACCELERATION;
                }
                break;
            case MOVE_RIGHT:
                kinetic->accX_ = (float) (MARIO_ACCELERATION_X) * 1.7f;
                break;
            case MOVE_LEFT:
                kinetic->accX_ = (float) -1 * (MARIO_ACCELERATION_X) * 1.7f;
                break;
            case CROUCH_DOWN:
                kinetic->accX_ = 0;
                break;
            case SPRINT:
                break;
            case SPECIAL:
                break;
        }

        ent->remove<BottomCollisionComponent>();
        ent->remove<LeftCollisionComponent>();
        ent->remove<RightCollisionComponent>();
        ent->remove<TopCollisionComponent>();
    }
}
