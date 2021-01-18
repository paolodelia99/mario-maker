//
// Created by paolo on 14/01/21.
//

#include <Components.h>
#include <Constants.h>
#include "systems/PlayerSystem.h"

constexpr int RUNNING_ANIMATION_SPEED = 4;

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
        ComponentHandle<PlayerComponent> playerComponent = ent->get<PlayerComponent>();

        switch (currentCommand) {
            case NONE_COMMAND:
                kinetic->accX_ = 0;
                kinetic->accY_ = 0;
                playerComponent->duck = false;
                break;
            case JUMP:
                if (ent->has<BottomCollisionComponent>()) {
                    kinetic->accY_ = -MARIO_JUMP_ACCELERATION;
                }
                break;
            case MOVE_RIGHT:
                kinetic->accX_ = (float) (MARIO_ACCELERATION_X) * 1.7f;
                playerComponent->right = true;
                playerComponent->left = false;
                break;
            case MOVE_LEFT:
                kinetic->accX_ = (float) -1 * (MARIO_ACCELERATION_X) * 1.7f;
                playerComponent->right = false;
                playerComponent->left = true;
                break;
            case DUCK:
                kinetic->accX_ = 0;
                playerComponent->duck = true;
                break;
            case SPRINT:
                break;
            case SPECIAL:
                break;
        }

        if (ent->has<BottomCollisionComponent>()) {
            if ((bool) std::abs(kinetic->speedY_) || (bool) std::abs(kinetic->accX_)) {
                if ((kinetic->speedX_ > 0 && kinetic->accX_ < 0) ||
                    (kinetic->speedX_ < 0 && kinetic->accX_ > 0)) {
                    setAnimation(ent, PlayerState::DRIFTING);
                } else {
                    setAnimation(ent, PlayerState::RUNNING);
                }
            } else {
                setAnimation(ent, PlayerState::STANDING);
            }
        } else {
            setAnimation(ent, PlayerState::JUMPING);
        }

        if (playerComponent->duck){
            setAnimation(ent, PlayerState::DUCKING);
        }

        int lookingLeft = 0;
        if (playerComponent->left || playerComponent->right) lookingLeft = playerComponent->left;
        ent->get<TextureComponent>()->flipH = lookingLeft;
        ent->remove<BottomCollisionComponent>();
        ent->remove<LeftCollisionComponent>();
        ent->remove<RightCollisionComponent>();
        ent->remove<TopCollisionComponent>();
    }
}

void PlayerSystem::setAnimation(Entity *playerEntity, PlayerState state) {
    ComponentHandle<PlayerComponent> playerComponent = playerEntity->get<PlayerComponent>();
    if (playerComponent) {
        if (playerComponent->current_state_ == state) return;
        playerEntity->remove<AnimationComponent>();

        switch (state) {
            case RUNNING:
                if (playerEntity->has<MarioComponent>()) {
                    playerEntity->assign<AnimationComponent>(
                            std::vector<TextureId>{
                                    TextureId::MARIO_RUN_1,
                                    TextureId::MARIO_RUN_2,
                                    TextureId::MARIO_RUN_3},
                            RUNNING_ANIMATION_SPEED
                    );
                } else {
                    playerEntity->assign<AnimationComponent>(
                            std::vector<TextureId>{
                                    TextureId::LUIGI_RUN_1,
                                    TextureId::LUIGI_RUN_2,
                                    TextureId::LUIGI_RUN_3},
                            RUNNING_ANIMATION_SPEED
                    );
                }
                break;
            case STANDING:
                playerEntity->assign<TextureComponent>(
                        playerEntity->has<MarioComponent>() ?
                        TextureId::MARIO_STAND : TextureId::LUIGI_STAND
                        );
                break;
            case DRIFTING:
                playerEntity->assign<TextureComponent>(
                        playerEntity->has<MarioComponent>() ?
                        TextureId::MARIO_DRIFT : TextureId::LUIGI_DRIFT
                        );
                break;
            case JUMPING:
                playerEntity->assign<TextureComponent>(
                        playerEntity->has<MarioComponent>() ?
                        TextureId::MARIO_JUMP : TextureId::LUIGI_JUMP
                        );
                break;
            case DUCKING:
                playerEntity->assign<TextureComponent>(
                        playerEntity->has<MarioComponent>() ?
                        TextureId::MARIO_DUCK : TextureId::LUIGI_DUCK
                        );
                break;
            case INVINCIBLE:
                break;
            default:
                break;
        }

        playerEntity->get<TextureComponent>()->h = GAME_TILE_SIZE;
        playerEntity->get<TextureComponent>()->offSetY = -1;
        playerEntity->get<TextureComponent>()->w = GAME_TILE_SIZE;
        playerEntity->get<TextureComponent>()->offSetX = -2;
        playerComponent->current_state_ = state;
    }
}
