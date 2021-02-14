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

    world->subscribe<EnemyCollisionEvent>(this);
    world->subscribe<PLayerCollectableCollisionEvent>(this);
}

void PlayerSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);

    world->unsubscribeAll(this);
}

void PlayerSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);


    for (auto player : world->each<PlayerComponent, CommandComponent, KineticComponent>()) {
        Command currentCommand = player->get<CommandComponent>()->currentCommand_;
        ComponentHandle<PlayerComponent> playerComponent = player->get<PlayerComponent>();

        if (player->has<FrozenComponent>()) {
            handleFrozenTransform(player);
        }

        movePlayer(player);

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
            case SHOOTING:
                playerEntity->assign<TextureComponent>(
                        isMario ?
                        TextureId::MARIO_FLAME_SHOOT  : TextureId::LUIGI_FLAME_SHOOT);
                break;
            case SIT: {
                float xOffset = 6.0f;
                if (playerEntity->has<SuperComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::SUPER_MARIO_SIT_2 : TextureId::SUPER_LUIGI_SIT_2);
                } else if (playerEntity->has<MegaComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_MEGA_SIT_2 : TextureId::LUIGI_MEGA_SIT_2);
                    xOffset = 18.0f;
                } else if (playerEntity->has<SuperFlameComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_FLAME_SIT_2 : TextureId::LUIGI_FLAME_SIT_2);
                } else {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_SIT_2 : TextureId::LUIGI_SIT_2);
                }
                playerEntity->get<TextureComponent>()->offSetX = xOffset;
            }
                break;
            case GREET:
                if (playerEntity->has<SuperComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::SUPER_MARIO_STAND : TextureId::SUPER_LUIGI_STAND);
                } else if (playerEntity->has<MegaComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_MEGA_GREET : TextureId::LUIGI_MEGA_STAND);
                } else if (playerEntity->has<SuperFlameComponent>()) {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_FLAME_STAND : TextureId::LUIGI_FLAME_STAND);
                } else {
                    playerEntity->assign<TextureComponent>(
                            isMario ?
                            TextureId::MARIO_GREET : TextureId::LUIGI_STAND);
                }
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

void PlayerSystem::eatMushroom(Entity *entity, Collectible::Type type) {
    World* world = entity->getWorld();
    bool isMario = entity->has<MarioComponent>();
    auto aabb = entity->get<AABBComponent>();
    TextureId currentTexture = entity->get<TextureComponent>()->textureId_;
    TextureId transformTexture = getRightTransitionAnimation(entity, type, isMario);
    Vector2 scorePosition = {aabb->left() + GAME_TILE_SIZE / 2, aabb->top() - GAME_TILE_SIZE / 2};

    switch (type) {
        case Collectible::SUPER_MARIO_MUSHROOM:
            if (!(entity->has<SuperComponent>() || entity->has<SuperFlameComponent>()
                    || entity->has<MegaComponent>())) {
                float oldHeight = entity->get<AABBComponent>()->top();
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
                world->emit<AddScoreEvent>(AddScoreEvent(1000, scorePosition));
                entity->assign<TimerComponent>([entity, oldHeight]() {
                    auto aabb = entity->get<AABBComponent>();
                    entity->remove<FrozenComponent>();
                    entity->remove<AnimationComponent>();
                    aabb->setTop(oldHeight - GAME_TILE_SIZE);
                    aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                    }, 60);
            }
            world->emit<SoundEvent>(SoundId::POWER_UP);
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
                }, 10, false, false, false);
                world->emit<AddScoreEvent>(AddScoreEvent(1000, scorePosition));
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
            world->emit<SoundEvent>(SoundId::POWER_UP);
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
                world->emit<AddScoreEvent>(AddScoreEvent(1000, scorePosition));
                entity->assign<TimerComponent>([entity]() {
                    auto aabb = entity->get<AABBComponent>();
                    entity->remove<FrozenComponent>();
                    entity->remove<AnimationComponent>();
                    if (entity->has<SuperComponent>()) entity->remove<SuperComponent>();
                    aabb->collisionBox_.height = GAME_TILE_SIZE * 2;
                }, 60);
            }
            world->emit<SoundEvent>(SoundId::POWER_UP);
            break;
        case Collectible::Type::ONE_UP_MUSHROOM:
            world->emit<AddScoreEvent>(AddScoreEvent(0, scorePosition, true));
            world->emit<SoundEvent>(SoundEvent{SoundId::UP_1});
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

TextureId
PlayerSystem::getRightTransitionAnimation(Entity *entity, Collectible::Type mushroom, bool isMario) {
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
        case LUIGI_FLAME_RUN_3: case LUIGI_FLAME_STAND: case MARIO_FLAME_SHOOT:
        case LUIGI_FLAME_SHOOT:
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

void PlayerSystem::movePlayer(Entity *player) {
    World* world = player->getWorld();
    auto kinetic = player->get<KineticComponent>();
    ComponentHandle<CommandComponent> commandComponent = player->get<CommandComponent>();
    Command command = commandComponent->currentCommand_;
    ComponentHandle<PlayerComponent> playerComponent = player->get<PlayerComponent>();
    PlayerState playerState = PlayerState::STANDING;

    if (commandComponent->enable) {
        switch (command) {
            case NONE_COMMAND:
                kinetic->accX_ = 0;
                kinetic->accY_ = 0;
                if (player->has<BottomCollisionComponent>()) {
                    playerState = PlayerState::STANDING;
                } else {
                    playerState = PlayerState::JUMPING;
                }
                break;
            case JUMP:
                if (player->has<BottomCollisionComponent>()) {
                    kinetic->accY_ = -MARIO_JUMP_ACCELERATION;
                    if (player->has<SuperComponent>()
                        || player->has<SuperFlameComponent>()
                        || player->has<MegaComponent>()) world->emit<SoundEvent>(SoundEvent{SoundId::JUMP_SUPER});
                    else world->emit<SoundEvent>(SoundEvent{SoundId::JUMP_SMALL});
                }
                playerState = PlayerState::JUMPING;
                break;
            case MOVE_RIGHT:
                kinetic->accX_ = (float) (MARIO_ACCELERATION_X) * 1.7f;
                if (playerComponent->sprint) kinetic->accX_ *= 1.5;
                playerComponent->right = true;
                playerComponent->left = false;
                if (player->has<BottomCollisionComponent>()) {
                    if ((kinetic->speedX_ > 0 && kinetic->accX_ < 0) ||
                        (kinetic->speedX_ < 0 && kinetic->accX_ > 0)) {
                        playerState = PlayerState::DRIFTING;
                    } else {
                        playerState = PlayerState::RUNNING;
                    }
                } else {
                    playerState = PlayerState::JUMPING;
                }
                break;
            case MOVE_LEFT:
                kinetic->accX_ = (float) -1 * (MARIO_ACCELERATION_X) * 1.7f;
                if (playerComponent->sprint) kinetic->accX_ *= 1.5;
                playerComponent->right = false;
                playerComponent->left = true;
                if (player->has<BottomCollisionComponent>()) {
                    if ((kinetic->speedX_ > 0 && kinetic->accX_ < 0) ||
                        (kinetic->speedX_ < 0 && kinetic->accX_ > 0)) {
                        playerState = PlayerState::DRIFTING;
                    } else {
                        playerState = PlayerState::RUNNING;
                    }
                } else {
                    playerState = PlayerState::JUMPING;
                }
                break;
            case DUCK:
                kinetic->accX_ = 0.0f;
                playerState = PlayerState::DUCKING;
                break;
            case SHOOT:
                if (player->has<SuperFlameComponent>() && playerComponent->canShoot) {
                    auto world = player->getWorld();
                    createFireBullet(world, player);
                    playerComponent->canShoot = false;
                    playerState = PlayerState::SHOOTING;
                    player->assign<TimerComponent>([player]() {
                        player->get<PlayerComponent>()->canShoot = true;
                    }, 40);
                }
                break;
            case SPRINT:
                break;
            case SPECIAL:
                break;
        }
    } else {
        if (player->has<BottomCollisionComponent>()) {
            if (kinetic->accX_ > 0.f) {
                playerComponent->right = true;
                playerComponent->left = false;
                playerState = PlayerState::RUNNING;
            } else if (kinetic->accX_ < 0.f) {
                playerComponent->right = false;
                playerComponent->left = true;
                playerState = PlayerState::RUNNING;
            } else {
                playerState = PlayerState::STANDING;
            }
        } else {
            playerState = PlayerState::JUMPING;
        }

        if (playerComponent->sit) {
            playerState = PlayerState::SIT;
        }

        if (playerComponent->greet) {
            playerState = PlayerState::GREET;
        }
    }

    setAnimation(player, playerState);
}

void PlayerSystem::receive(World *world, const EnemyCollisionEvent &enemyCollisionEvent) {
    Entity* player = enemyCollisionEvent.player;
    Entity* enemy = enemyCollisionEvent.enemy;

    if (player->has<SuperComponent>() || player->has<SuperFlameComponent>()
            || player->has<MegaComponent>()) {
        shrink(player);
    } else {
        auto kinetic = player->get<KineticComponent>();

        player->remove<AnimationComponent>();
        player->remove<CommandComponent>();
        player->get<TextureComponent>()->textureId_ =
                player->has<MarioComponent>() ? TextureId::MARIO_DEAD : TextureId::LUIGI_DEAD;
        player->assign<DeadComponent>();
        player->remove<SolidComponent>();
        player->remove<GravityComponent>();

        kinetic->speedY_ = 0.0f;
        kinetic->speedX_ = 0.0f;
        kinetic->accX_ = 0.0f;
        kinetic->accY_ = 0.0f;

        auto otherPlayer = world->findFirst<PlayerComponent, CommandComponent>();

        if (!otherPlayer) {
            world->each<WalkComponent, KineticComponent>(
                    [=](Entity* entity,
                        ComponentHandle<WalkComponent> walk,
                        ComponentHandle<KineticComponent> kineticEnt) {
                        if (entity != player) {
                            entity->assign<FrozenComponent>();
                            entity->remove<WalkComponent>();
                            entity->remove<KineticComponent>();
                            entity->remove<AnimationComponent>();
                            entity->remove<TimerComponent>();
                        }
                    });
            world->emit<SoundEvent>(SoundEvent{SoundId::MARIO_DIE});
        } else {
            if (player->has<LeadCameraComponent>()) {
                player->remove<LeadCameraComponent>();
                otherPlayer->assign<LeadCameraComponent>();
            }

            // freeze enemy for a little bit
            enemy->assign<FrozenComponent>();
            enemy->assign<TimerComponent>([=]{
                enemy->remove<FrozenComponent>();
            }, 100);
        }

        player->assign<TimerComponent>([=] {
            player->assign<GravityComponent>();
            kinetic->speedY_ = 0.0f;
            kinetic->speedX_ = 0.0f;
            kinetic->accX_ = 0.0f;
            kinetic->accY_ = -5.0f;
        }, 100);
    }
}

void PlayerSystem::shrink(Entity *player) {
    bool isMario = player->has<MarioComponent>();
    TextureId currentTexture = player->get<TextureComponent>()->textureId_;
    TextureId transformTexture;

    switch (currentTexture) {
        case MARIO_MEGA_JUMP:
        case LUIGI_MEGA_JUMP:
        case MARIO_FLAME_JUMP:
        case LUIGI_FLAME_JUMP:
        case SUPER_MARIO_JUMP:
        case SUPER_LUIGI_JUMP:
            transformTexture =
                    isMario ? MARIO_JUMP : LUIGI_JUMP;
            break;
        default:
            transformTexture =
                    isMario ? MARIO_STAND : LUIGI_STAND;
            break;
    }

    player->assign<AnimationComponent>(std::vector<TextureId>{
        currentTexture,
        transformTexture,
        transformTexture,
        currentTexture,
        currentTexture,
        transformTexture,
        transformTexture
    }, 8);

    if (player->has<SuperComponent>()) {
        player->remove<SuperComponent>();
    } else if (player->has<SuperFlameComponent>()) {
        player->remove<SuperFlameComponent>();
    } else if (player->has<MegaComponent>()) {
        player->remove<MegaComponent>();
    }

    player->assign<FrozenComponent>();
    player->assign<TimerComponent>([player]() {
        auto aabb = player->get<AABBComponent>();
        player->remove<FrozenComponent>();
        player->remove<AnimationComponent>();
        aabb->setTop(aabb->top() - GAME_TILE_SIZE);
        aabb->collisionBox_.height = GAME_TILE_SIZE;
        aabb->collisionBox_.width = GAME_TILE_SIZE;
    }, 40);
}

void PlayerSystem::createFireBullet(World *world, Entity *entity) {
    Entity* fireBullet = world->create();
    Entity* player = entity;
    auto playerComponent = player->get<PlayerComponent>();
    auto aabb = player->get<AABBComponent>();
    Rectangle posRect;
    float velocity = 1.8f;

    fireBullet->assign<TextureComponent>(TextureId::FIRE_BULLET);
    fireBullet->assign<FireBulletComponent>(aabb->getCenterY());
    if (playerComponent->left) {
        posRect = Rectangle{
                aabb->left(),
                aabb->getCenterY() - 8,
                GAME_TILE_SIZE / 2,
                GAME_TILE_SIZE / 2};
        velocity = -velocity;
    } else {
        posRect = Rectangle{
                aabb->right(),
                aabb->getCenterY() - 8,
                GAME_TILE_SIZE / 2,
                GAME_TILE_SIZE / 2};
    }
    fireBullet->assign<RotationComponent>();
    fireBullet->assign<AABBComponent>(posRect);
    fireBullet->assign<WalkComponent>(velocity);
    fireBullet->assign<KineticComponent>();
    fireBullet->assign<SolidComponent>();
    fireBullet->assign<GravityComponent>();
    fireBullet->assign<BouncingComponent>();
}

void PlayerSystem::receive(World *world, const PLayerCollectableCollisionEvent &pLayerCollectableCollisionEvent) {
    Entity* collectibleEntity = pLayerCollectableCollisionEvent.collectable;
    Entity* player = pLayerCollectableCollisionEvent.player;

    player->get<PlayerComponent>()->current_state_ = STANDING;
    eatMushroom(player, collectibleEntity->get<CollectibleComponent>()->type);
    world->destroy(collectibleEntity);
}
