//
// Created by paolo on 15/01/21.
//

#include <iostream>
#include "PhysicSystem.h"

PhysicSystem::PhysicSystem() {

}

void PhysicSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    for (auto ent : world->each<GravityComponent, KineticComponent>())
    {
        ent->get<KineticComponent>()->accY_ += GRAVITY;
    }

    auto camera = world->findFirst<CameraComponent>()->get<CameraComponent>();

    // Kinetic-Tiles Collisions
    for (auto ent : world->each<KineticComponent, AABBComponent, SolidComponent>())
    {
        auto aabb = ent->get<AABBComponent>();
        auto kinetic = ent->get<KineticComponent>();

        for (auto object : world->each<TileComponent, AABBComponent, SolidComponent>())
        {
            if (!object) continue;

            Rectangle objCollisionBox = object->get<AABBComponent>()->collisionBox_;

            Rectangle newCollisionBox = Rectangle{
                    aabb->collisionBox_.x + kinetic->speedX_,
                    aabb->collisionBox_.y + kinetic->speedY_ + 3,
                    aabb->collisionBox_.width + kinetic->speedX_,
                    aabb->collisionBox_.height + kinetic->speedY_
            };

            Rectangle collisionRect = GetCollisionRec(newCollisionBox, objCollisionBox);

            std::cout << "Bottom: " << aabb->bottom() << std::endl;
            std::cout << "collision height: " << collisionRect.height << std::endl;

            // Check y collision
            if (collisionRect.height != 0) {
                if (collisionRect.y > newCollisionBox.y + newCollisionBox.height / 2 ) {
                    // Bottom collision
                    kinetic->accY_ = std::min(0.0f, kinetic->accY_);
                    kinetic->speedY_ = std::min(0.0f, kinetic->speedY_);
                    ent->assign<BottomCollisionComponent>();
                } else if (collisionRect.y <= newCollisionBox.y + newCollisionBox.height / 2) {
                    // Top collision
                    kinetic->accY_ = std::max(0.0f, kinetic->accY_);
                    kinetic->speedY_ = std::max(0.0f, kinetic->speedY_);
                    ent->assign<TopCollisionComponent>();
                }
            }

            newCollisionBox = Rectangle{
                    aabb->collisionBox_.x + kinetic->speedX_,
                    aabb->collisionBox_.y + 1,
                    aabb->collisionBox_.width + kinetic->speedX_,
                    aabb->collisionBox_.height - 4
            };

            collisionRect = GetCollisionRec(newCollisionBox, objCollisionBox);

            if (collisionRect.width != 0 && collisionRect.width != 32) {
                if (collisionRect.x < newCollisionBox.x + newCollisionBox.width / 2) {
                    // Left Collision
                    kinetic->accX_ = std::max(0.0f, kinetic->accX_);
                    kinetic->speedX_ = std::max(0.0f, kinetic->speedX_);
                    ent->assign<LeftCollisionComponent>();
                } else if (collisionRect.x >= newCollisionBox.x + newCollisionBox.width / 2) {
                    // Right collision
                    kinetic->accX_ = std::min(0.0f, kinetic->accX_);
                    kinetic->speedX_ = std::min(0.0f, kinetic->speedX_);
                    ent->assign<RightCollisionComponent>();
                }
            }
        }
    }

    // Apply Forces
    for (auto ent : world->each<AABBComponent, KineticComponent>())
    {
        if (ent->has<FrozenComponent>()) continue;
        auto aabb = ent->get<AABBComponent>();
        auto kinetic = ent->get<KineticComponent>();

        aabb->collisionBox_.x += kinetic->speedX_;
        aabb->collisionBox_.y += kinetic->speedY_;
        kinetic->speedX_ += kinetic->accX_;
        kinetic->speedY_ += kinetic->accY_;

        if (std::abs(kinetic->speedY_) < MARIO_ACCELERATION_X) kinetic->speedY_ = 0;
        if (std::abs(kinetic->speedX_) < MARIO_ACCELERATION_X) kinetic->speedX_ = 0;
        kinetic->speedY_ *= FRICTION;
        kinetic->speedX_ *= FRICTION;

        if (kinetic->speedY_ > MAX_SPEED_Y) kinetic->speedY_ = MAX_SPEED_Y;
        if (kinetic->speedX_ > MAX_SPEED_X) kinetic->speedX_ = MAX_SPEED_X;

        if (kinetic->speedY_ < -MAX_SPEED_Y) kinetic->speedY_ = -MAX_SPEED_Y;
        if (kinetic->speedX_ < -MAX_SPEED_X) kinetic->speedX_ = -MAX_SPEED_X;
    }
}

void PhysicSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);
}

void PhysicSystem::configure(World *world) {
    EntitySystem::configure(world);
}

PhysicSystem::~PhysicSystem() {

}
