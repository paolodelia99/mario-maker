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

                if (winnerAABB->bottom() < flagPoleBottom_) winnerAABB->collisionBox_.y += .5f;
                if (flagAABB->bottom() >= flagPoleBottom_) state_ = WALKING_TO_CASTE;
                else flagAABB->collisionBox_.y += 1.0f;
            }
                break;
            case WALKING_TO_CASTE:
                finalPole_->remove<SolidComponent>();
                winner_->get<PlayerComponent>()->sit = false;
                winner_->assign<GravityComponent>();
                winner_->get<AABBComponent>()->collisionBox_.y += 3.0f;
                winner_->get<AABBComponent>()->collisionBox_.x += 2.0f;
                winner_->get<KineticComponent>()->accX_ = MARIO_ACCELERATION_X;

                winner_->assign<TimerComponent>([&]() {
                    winner_->get<KineticComponent>()->accX_ = 0.0f;
                    winner_->get<PlayerComponent>()->greet = true;
                }, 150);
                world->disableSystem(this);
                break;
        }
    }
}

void FlagSystem::configure(World *world) {
    EntitySystem::configure(world);

    world->subscribe<CollisionWithFinalPoleEvent>(this);
}

FlagSystem::~FlagSystem() {

}

void FlagSystem::receive(World *world, const CollisionWithFinalPoleEvent &collisionWithFinalPole) {
    world->enableSystem(this);

    collisionWithFinalPole.player->get<PlayerComponent>()->sit = true;
    winner_ = collisionWithFinalPole.player;
    winner_->get<CommandComponent>()->enable = false;
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
    finalPole_ = collisionWithFinalPole.pole;
    flagPoleBottom_ = collisionWithFinalPole.pole->get<AABBComponent>()->bottom();
    state_ = TAKING_FLAG_DOWN;
}
