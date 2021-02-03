//
// Created by paolo on 03/02/21.
//

#include "systems/FlagSystem.h"

FlagSystem::FlagSystem() {}

void FlagSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);

    world->unsubscribeAll(this);
}

void FlagSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    if (winner_ && winningFlag_) {
        switch (state_) {
            case NONE:
                break;
            case TAKING_FLAG_DOWN: {
                auto winnerAABB = winner_->get<AABBComponent>();
                auto flagAABB = winningFlag_->get<AABBComponent>();

                if (winnerAABB->bottom() >= flagPoleBottom_) state_ = WALKING_TO_CASTE;
                else {
                    winnerAABB->collisionBox_.y  += 1.0f;
                    flagAABB->collisionBox_.y += 1.0f;
                }
            }
                break;
            case WALKING_TO_CASTE:
                break;
        }
    }
}

void FlagSystem::configure(World *world) {
    EntitySystem::configure(world);

    world->subscribe<CollisionWithFinalPole>(this);
}

FlagSystem::~FlagSystem() {

}

void FlagSystem::receive(World *world, const CollisionWithFinalPole &collisionWithFinalPole) {
    world->enableSystem(this);

    winner_ = collisionWithFinalPole.player;
    winner_->remove<CommandComponent>();
    winner_->get<PlayerComponent>()->current_state_ = PlayerState::SIT;
    winner_->remove<GravityComponent>();
    // froze all the entities
    world->each<WalkComponent, KineticComponent>(
            [=](Entity* entity,
                ComponentHandle<WalkComponent> walk,
                ComponentHandle<KineticComponent> kineticEnt) {
                if (entity != winner_) {
                    entity->remove<WalkComponent>();
                    entity->remove<KineticComponent>();
                    entity->remove<AnimationComponent>();
                    entity->remove<TimerComponent>();
                }
            });
    winningFlag_ = world->findFirst<WinnerFlagComponent>();
    flagPoleBottom_ = collisionWithFinalPole.flagPoleBottom;
}
