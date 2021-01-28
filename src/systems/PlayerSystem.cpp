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


    for (auto player : world->each<PlayerComponent, CommandComponent, KineticComponent>()) {
        Command currentCommand = player->get<CommandComponent>()->currentCommand_;
        ComponentHandle<PlayerComponent> playerComponent = player->get<PlayerComponent>();

        if (player->has<FrozenComponent>()) {
            handleFrozenTransform(player);
        }

        movePlayer(player, playerComponent, currentCommand);

        setRightAnimation(player);

        collectCollectible(world, player);

        int lookingLeft = 0;
        if (playerComponent->left || playerComponent->right) lookingLeft = playerComponent->left;
        player->get<TextureComponent>()->flipH = lookingLeft;
        player->remove<BottomCollisionComponent>();
        player->remove<LeftCollisionComponent>();
        player->remove<RightCollisionComponent>();
        player->remove<TopCollisionComponent>();
    }
}

void PlayerSystem::setAnimation(Entity *playerEntity, PlayerState state) {
    bool isMario = playerEntity->has<MarioComponent>();
    ComponentHandle<PlayerComponent> playerComponent = playerEntity->get<PlayerComponent>();

    if (playerComponent) {
        if (playerComponent->current_state_ == state) return;
        playerEntity->remove<AnimationComponent>();

        switch (state) {
            case RUNNING:
                if (isMario) {
                    if (playerEntity->has<SuperComponent>()) {
                        playerEntity->assign<AnimationComponent>(
                                std::vector<TextureId>{
                                        TextureId::SUPER_MARIO_RUN_1,
                                        TextureId::SUPER_MARIO_RUN_2,
                                        TextureId::SUPER_MARIO_RUN_3},
                                RUNNING_ANIMATION_SPEED);
                    } else if (playerEntity->has<MegaComponent>()) {
                        playerEntity->assign<AnimationComponent>(
                                std::vector<TextureId>{
                                        TextureId::MARIO_MEGA_RUN_1,
                                        TextureId::MARIO_MEGA_RUN_2,
                                        TextureId::MARIO_MEGA_RUN_3},
                                RUNNING_ANIMATION_SPEED);
                    } else if (playerEntity->has<SuperFlameComponent>()) {
                        playerEntity->assign<AnimationComponent>(
                                std::vector<TextureId>{
                                        TextureId::MARIO_FLAME_RUN_1,
                                        TextureId::MARIO_FLAME_RUN_2,
                                        TextureId::MARIO_FLAME_RUN_3},
                                RUNNING_ANIMATION_SPEED);
                    } else {
                        playerEntity->assign<AnimationComponent>(
                                std::vector<TextureId>{
                                        TextureId::MARIO_RUN_1,
                                        TextureId::MARIO_RUN_2,
                                        TextureId::MARIO_RUN_3},
                                RUNNING_ANIMATION_SPEED
                        );
                    }
                } else {
                    if (playerEntity->has<SuperComponent>()) {
                        playerEntity->assign<AnimationComponent>(
                                std::vector<TextureId>{
                                        TextureId::SUPER_LUIGI_RUN_1,
                                        TextureId::SUPER_LUIGI_RUN_2,
                                        TextureId::SUPER_LUIGI_RUN_3},
                                RUNNING_ANIMATION_SPEED
                        );
                    } else if (playerEntity->has<MegaComponent>()) {
                        playerEntity->assign<AnimationComponent>(
                                std::vector<TextureId>{
                                        TextureId::LUIGI_MEGA_RUN_1,
                                        TextureId::LUIGI_MEGA_RUN_2,
                                        TextureId::LUIGI_MEGA_RUN_3},
                                RUNNING_ANIMATION_SPEED
                        );
                    } else if (playerEntity->has<SuperFlameComponent>()) {
                        playerEntity->assign<AnimationComponent>(
                                std::vector<TextureId>{
                                        TextureId::LUIGI_FLAME_RUN_1,
                                        TextureId::LUIGI_FLAME_RUN_2,
                                        TextureId::LUIGI_FLAME_RUN_3},
                                RUNNING_ANIMATION_SPEED);
                    } else {
                        playerEntity->assign<AnimationComponent>(
                                std::vector<TextureId>{
                                        TextureId::LUIGI_RUN_1,
                                        TextureId::LUIGI_RUN_2,
                                        TextureId::LUIGI_RUN_3},
                                RUNNING_ANIMATION_SPEED
                        );
                    }
                }
                break;
            case STANDING:
                if (playerEntity->has<SuperComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::SUPER_MARIO_STAND : TextureId::SUPER_LUIGI_STAND);
                } else if (playerEntity->has<MegaComponent>()) {
                  playerEntity->assign<TextureComponent>(
                          isMario ?
                          TextureId::MARIO_MEGA_STAND : TextureId::LUIGI_MEGA_STAND);
                } else if (playerEntity->has<SuperFlameComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_FLAME_STAND : TextureId::LUIGI_FLAME_STAND);
                } else {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_STAND : TextureId::LUIGI_STAND
                    );
                }
                break;
        case DRIFTING:
                if (playerEntity->has<SuperComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::SUPER_MARIO_DRIFT : TextureId::SUPER_LUIGI_DRIFT);
                } else if (playerEntity->has<MegaComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_MEGA_DRIFT : TextureId::LUIGI_MEGA_DRIFT);
                } else if (playerEntity->has<SuperFlameComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_FLAME_DRIFT : TextureId::LUIGI_FLAME_DRIFT);
                } else {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_DRIFT : TextureId::LUIGI_DRIFT
                    );
                }
                break;
            case JUMPING:
                if (playerEntity->has<SuperComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::SUPER_MARIO_JUMP : TextureId::SUPER_LUIGI_JUMP);
                } else if (playerEntity->has<MegaComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_MEGA_JUMP : TextureId::LUIGI_MEGA_JUMP);
                } else if (playerEntity->has<SuperFlameComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_FLAME_JUMP : TextureId::LUIGI_FLAME_JUMP);
                } else {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_JUMP : TextureId::LUIGI_JUMP);
                }
                break;
            case DUCKING:
                if (playerEntity->has<SuperComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::SUPER_MARIO_DUCK : TextureId::SUPER_LUIGI_DUCK);
                } else if (playerEntity->has<MegaComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_MEGA_DUCK : TextureId::LUIGI_MEGA_DUCK);
                } else if (playerEntity->has<SuperFlameComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_FLAME_DUCK : TextureId::LUIGI_FLAME_DUCK);
                } else {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_DUCK : TextureId::LUIGI_DUCK);
                }
                break;
            case INVINCIBLE:
                break;
            default:
                break;
        }

        if (playerEntity->has<SuperComponent>() || playerEntity->has<SuperFlameComponent>()) {
            playerEntity->get<TextureComponent>()->h = GAME_TILE_SIZE * 2;
            playerEntity->get<TextureComponent>()->w = GAME_TILE_SIZE;
        } else if (playerEntity->has<MegaComponent>()) {
            playerEntity->get<TextureComponent>()->h = GAME_TILE_SIZE * 2;
            playerEntity->get<TextureComponent>()->w = GAME_TILE_SIZE * 2;
        } else {
            playerEntity->get<TextureComponent>()->h = GAME_TILE_SIZE;
            playerEntity->get<TextureComponent>()->w = GAME_TILE_SIZE;
        }

        playerComponent->current_state_ = state;
    }
}

bool CheckCollisionRecs_(Rectangle rec1, Rectangle rec2)
{
    bool collision = false;

    if ((rec1.x <= (rec2.x + rec2.width) && (rec1.x + rec1.width) >= rec2.x) &&
        (rec1.y <= (rec2.y + rec2.height) && (rec1.y + rec1.height) >= rec2.y)) collision = true;

    return collision;
}

void PlayerSystem::collectCollectible(World *world, Entity *player) {
    auto playerAABB = player->get<AABBComponent>()->collisionBox_;
    auto playerComponent = player->get<PlayerComponent>();
    world->each<CollectibleComponent, AABBComponent>([&](
            Entity* entity,
            ComponentHandle<CollectibleComponent> collectible,
            ComponentHandle<AABBComponent> collectibleAABB
            ) {
       if (CheckCollisionRecs_(playerAABB, collectibleAABB->collisionBox_)) {
           auto collectibleType = collectible->type;
           playerComponent->current_state_ = STANDING;
           eatMushroom(player, collectibleType);
           world->destroy(entity);
       }
    });
}

void PlayerSystem::eatMushroom(Entity *entity, Collectible::CollectibleType type) {

    bool isMario = entity->has<MarioComponent>();
    TextureId currentTexture = entity->get<TextureComponent>()->textureId_;
    TextureId transformTexture = getRightTransitionAnimation(entity, type, isMario);

    switch (type) {
        case Collectible::SUPER_MARIO_MUSHROOM:
            if (!(entity->has<SuperComponent>() || entity->has<SuperFlameComponent>()
                    || entity->has<MegaComponent>())) {
                entity->assign<SuperComponent>();
                entity->assign<AnimationComponent>(std::vector<TextureId>{
                        currentTexture,
                        transformTexture,
                        currentTexture,
                        currentTexture,
                        transformTexture,
                        transformTexture,
                }, 4, false, false, false);
                entity->assign<FrozenComponent>();
                entity->assign<TimerComponent>([entity]() {
                    auto aabb = entity->get<AABBComponent>();
                    entity->remove<FrozenComponent>();
                    entity->remove<AnimationComponent>();
                    aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                    }, 40);
            }
            break;
        case Collectible::MEGA_MUSHROOM:
            if (!entity->has<MegaComponent>()) {
                entity->assign<MegaComponent>();
                entity->assign<AnimationComponent>(std::vector<TextureId>{
                        currentTexture,
                        transformTexture,
                        currentTexture,
                        currentTexture,
                        transformTexture,
                        transformTexture,
                        currentTexture,
                        currentTexture,
                        transformTexture,
                        transformTexture,
                }, 8, false, false, false);
                entity->assign<FrozenComponent>();
                entity->assign<TimerComponent>([entity]() {
                    auto aabb = entity->get<AABBComponent>();
                    entity->remove<FrozenComponent>();
                    entity->remove<AnimationComponent>();
                    if (entity->has<SuperComponent>()) entity->remove<SuperComponent>();
                    if (entity->has<SuperFlameComponent>()) entity->remove<SuperFlameComponent>();
                    aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                    aabb->collisionBox_.width = GAME_TILE_SIZE * 2;
                }, 80);
            }
            break;
        case Collectible::FLAME_MUSHROOM:
            if (!(entity->has<SuperFlameComponent>() || entity->has<MegaComponent>())) {
                entity->assign<SuperFlameComponent>();
                entity->assign<AnimationComponent>(std::vector<TextureId>{
                        currentTexture,
                        transformTexture,
                        currentTexture,
                        currentTexture,
                        transformTexture,
                        transformTexture,
                }, 4, false, false, false);
                entity->assign<FrozenComponent>();
                entity->assign<TimerComponent>([entity]() {
                    auto aabb = entity->get<AABBComponent>();
                    entity->remove<FrozenComponent>();
                    entity->remove<AnimationComponent>();
                    if (entity->has<SuperComponent>()) entity->remove<SuperComponent>();
                    aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                }, 40);
            }
            break;
        default:
            break;
    }
}

void PlayerSystem::handleFrozenTransform(Entity *entity) {
    auto texture = entity->get<TextureComponent>();
    auto aabb = entity->get<AABBComponent>();

    if (texture) {
        if (entity->has<SuperComponent>() && !entity->has<SuperFlameComponent>() && !entity->has<MegaComponent>()) {
            if (isSuperTexture(texture->textureId_)) {
                if (aabb->collisionBox_.height == GAME_TILE_SIZE) aabb->collisionBox_.y -= GAME_TILE_SIZE;
                aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                texture->h = GAME_TILE_SIZE * 2;
            } else {
                if (aabb->collisionBox_.height == GAME_TILE_SIZE * 2) aabb->collisionBox_.y += GAME_TILE_SIZE;
                texture->h = GAME_TILE_SIZE;
                aabb->collisionBox_.height = GAME_TILE_SIZE;
            }
        } else if (entity->has<SuperFlameComponent>() && !entity->has<MegaComponent>()) {
            if (isSuperTexture(texture->textureId_) || isFlameTexture(texture->textureId_)) {
                if (aabb->collisionBox_.height == GAME_TILE_SIZE) aabb->collisionBox_.y -= GAME_TILE_SIZE;
                aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                texture->h = GAME_TILE_SIZE * 2;
            } else {
                if (aabb->collisionBox_.height == GAME_TILE_SIZE * 2) aabb->collisionBox_.y += GAME_TILE_SIZE;
                texture->h = GAME_TILE_SIZE;
                aabb->collisionBox_.height = GAME_TILE_SIZE;
            }
        } else if (entity->has<MegaComponent>()) {
            std::cout << aabb->bottom() << std::endl;
            if (isMegaTexture(texture->textureId_)) {
                if (aabb->collisionBox_.height == GAME_TILE_SIZE) aabb->collisionBox_.y -= GAME_TILE_SIZE;
                aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                aabb->collisionBox_.width = GAME_TILE_SIZE * 2;
                texture->h = GAME_TILE_SIZE * 2;
                texture->w = GAME_TILE_SIZE * 2;
            } else if (isFlameTexture(texture->textureId_) || isSuperTexture(texture->textureId_)) {
                aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                aabb->collisionBox_.width = GAME_TILE_SIZE;
                texture->h = GAME_TILE_SIZE * 2;
                texture->w = GAME_TILE_SIZE;
            } else {
                if (aabb->collisionBox_.height == GAME_TILE_SIZE * 2) aabb->collisionBox_.y += GAME_TILE_SIZE;
                aabb->collisionBox_.height = GAME_TILE_SIZE;
                aabb->collisionBox_.width = GAME_TILE_SIZE;
                texture->h = GAME_TILE_SIZE;
                texture->w = GAME_TILE_SIZE;
            }
        }
    }
}

void PlayerSystem::setRightAnimation(Entity* player) {
    auto playerComponent = player->get<PlayerComponent>();
    auto kinetic = player->get<KineticComponent>();

    if (!player->has<FrozenComponent>()) {
        if (player->has<BottomCollisionComponent>()) {
            if ((bool) std::abs(kinetic->speedY_) || (bool) std::abs(kinetic->accX_)) {
                if ((kinetic->speedX_ > 0 && kinetic->accX_ < 0) ||
                    (kinetic->speedX_ < 0 && kinetic->accX_ > 0)) {
                    setAnimation(player, PlayerState::DRIFTING);
                } else {
                    setAnimation(player, PlayerState::RUNNING);
                }
            } else {
                setAnimation(player, PlayerState::STANDING);
            }
        } else {
            setAnimation(player, PlayerState::JUMPING);
        }

        if (playerComponent->duck){
            setAnimation(player, PlayerState::DUCKING);
            kinetic->accX_ = 0.0f;
        }

    } else {
        if (player->has<BottomCollisionComponent>()) {
            setAnimation(player, PlayerState::STANDING);
        } else {
            setAnimation(player, PlayerState::JUMPING);
        }
    }
}

TextureId
PlayerSystem::getRightTransitionAnimation(Entity *entity, Collectible::CollectibleType mushroom, bool isMario) {
    auto playerState = entity->get<PlayerComponent>()->current_state_;

    if (isMario) {
        switch (playerState) {
            case RUNNING:
            case STANDING:
            case DRIFTING:
            case DUCKING:
                switch (mushroom) {
                    case Collectible::MEGA_MUSHROOM:
                        return TextureId::MARIO_MEGA_STAND;
                    case Collectible::FLAME_MUSHROOM:
                        return TextureId::MARIO_FLAME_STAND;
                    case Collectible::SUPER_MARIO_MUSHROOM:
                    default:
                        return TextureId::SUPER_MARIO_STAND;
                }
            case JUMPING:
                switch (mushroom) {
                    case Collectible::MEGA_MUSHROOM:
                        return TextureId::MARIO_MEGA_JUMP;
                    case Collectible::FLAME_MUSHROOM:
                        return TextureId::MARIO_FLAME_JUMP;
                    case Collectible::SUPER_MARIO_MUSHROOM:
                    default:
                        return TextureId::SUPER_MARIO_JUMP;
                }
            default:
                return TextureId::EMPTY;
        }
    } else {
        switch (playerState) {
            case RUNNING:
            case STANDING:
            case DRIFTING:
            case DUCKING:
                switch (mushroom) {
                    case Collectible::MEGA_MUSHROOM:
                        return TextureId::LUIGI_MEGA_STAND;
                    case Collectible::FLAME_MUSHROOM:
                        return TextureId::LUIGI_FLAME_STAND;
                    case Collectible::SUPER_MARIO_MUSHROOM:
                    default:
                        return TextureId::SUPER_LUIGI_STAND;
                }
            case JUMPING:
                switch (mushroom) {
                    case Collectible::MEGA_MUSHROOM:
                        return TextureId::LUIGI_MEGA_JUMP;
                    case Collectible::FLAME_MUSHROOM:
                        return TextureId::LUIGI_FLAME_JUMP;
                    case Collectible::SUPER_MARIO_MUSHROOM:
                    default:
                        return TextureId::SUPER_LUIGI_JUMP;
                }
            default:
                return TextureId::EMPTY;
        }
    }
}

bool PlayerSystem::isSuperTexture(TextureId textureId) {
    switch (textureId) {
        case SUPER_MARIO_STAND: case SUPER_LUIGI_STAND: case SUPER_LUIGI_DUCK:
        case SUPER_LUIGI_DRIFT: case SUPER_MARIO_JUMP: case SUPER_MARIO_DRIFT:
        case SUPER_MARIO_DUCK: case SUPER_MARIO_RUN_1: case SUPER_LUIGI_JUMP:
        case SUPER_LUIGI_RUN_1: case SUPER_LUIGI_RUN_2: case SUPER_MARIO_RUN_2:
        case SUPER_LUIGI_RUN_3: case SUPER_MARIO_RUN_3:
            return true;
        default:
            return false;
    }
}

bool PlayerSystem::isFlameTexture(TextureId textureId) {
    switch (textureId) {
        case MARIO_FLAME_STAND: case MARIO_FLAME_DRIFT: case MARIO_FLAME_DUCK:
        case MARIO_FLAME_JUMP: case MARIO_FLAME_RUN_1: case MARIO_FLAME_RUN_2:
        case MARIO_FLAME_RUN_3: case LUIGI_FLAME_DRIFT: case LUIGI_FLAME_DUCK:
        case LUIGI_FLAME_JUMP: case LUIGI_FLAME_RUN_1: case LUIGI_FLAME_RUN_2:
        case LUIGI_FLAME_RUN_3: case LUIGI_FLAME_STAND:
            return true;
        default:
            return false;
    }
}

bool PlayerSystem::isMegaTexture(TextureId textureId) {
    switch (textureId) {
        case MARIO_MEGA_STAND: case MARIO_MEGA_DRIFT: case MARIO_MEGA_DUCK:
        case MARIO_MEGA_JUMP: case MARIO_MEGA_RUN_1: case MARIO_MEGA_RUN_2:
        case MARIO_MEGA_RUN_3: case LUIGI_MEGA_DRIFT: case LUIGI_MEGA_DUCK:
        case LUIGI_MEGA_JUMP: case LUIGI_MEGA_RUN_1: case LUIGI_MEGA_RUN_2:
        case LUIGI_MEGA_RUN_3: case LUIGI_MEGA_STAND:
            return true;
        default:
            return false;
    }
}

void PlayerSystem::movePlayer(Entity *player, ComponentHandle<PlayerComponent> playerComponent, Command command) {
    auto kinetic = player->get<KineticComponent>();

    switch (command) {
        case NONE_COMMAND:
            kinetic->accX_ = 0;
            kinetic->accY_ = 0;
            playerComponent->duck = false;
            break;
        case JUMP:
            if (player->has<BottomCollisionComponent>()) {
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
}
