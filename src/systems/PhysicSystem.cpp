//
// Created by paolo on 15/01/21.
//

#include <cmath>
#include <unordered_set>
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

    //Kinetic-Kinetic Collsions
    for (auto ent : world->each<AABBComponent, KineticComponent, SolidComponent>())
    {
        for (auto ent2 : world->each<AABBComponent, KineticComponent, SolidComponent>())
        {
            if (ent == ent2) continue;

            checkYCollision(ent, ent2);

            checkXCollision(ent, ent2);
        }
    }

    // Kinetic-Tiles Collisions
    auto objMapEntity = world->findFirst<ObjectMapComponent>();
    if (objMapEntity) {
        auto map = objMapEntity->get<ObjectMapComponent>();

        for (auto ent : world->each<KineticComponent, AABBComponent, SolidComponent>())
        {
            auto aabb = ent->get<AABBComponent>();
            int x = (int) round(aabb->left() / 32);
            int y = (int) round(aabb->top() / 32);
            std::unordered_set<int> neighbors = {
                    map->get(x + 1, y),
                    map->get(x - 1, y),
                    map->get(x, y + 1),
                    map->get(x, y - 1),
                    map->get(x + 1, y + 1),
                    map->get(x + 1, y - 1),
                    map->get(x - 1, y + 1),
                    map->get(x - 1, y - 1)
            };

            for (auto id : neighbors) {
                if (id == ent->getEntityId()) continue;
                if (id == -1) continue;

                auto object = world->getById(id);
                if (!object) continue;
                if (!object->has<AABBComponent, SolidComponent, TileComponent>()) continue;

                checkYCollision(ent, object);

                checkXCollision(ent, object);
            }
        }
    } else {
        throw std::invalid_argument("There isn't any objMapEntity!");
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

void PhysicSystem::checkYCollision(Entity *ent1, Entity *ent2) {
    auto aabb = ent1->get<AABBComponent>();
    auto kinetic = ent1->get<KineticComponent>();
    Rectangle objCollisionBox = ent2->get<AABBComponent>()->collisionBox_;
    float objectBottom = objCollisionBox.y + objCollisionBox.height;
    float objectYCenter = objCollisionBox.y + objCollisionBox.height / 2;
    Rectangle kineticEntityCollBox = Rectangle{
            aabb->collisionBox_.x + kinetic->speedX_,
            aabb->collisionBox_.y + kinetic->speedY_,
            aabb->collisionBox_.width,
            aabb->collisionBox_.height + 1
    };

    Rectangle collisionRect = GetCollisionRec(kineticEntityCollBox, objCollisionBox);

    // Check y collision
    if (collisionRect.height != 0 && collisionRect.height != aabb->collisionBox_.height) {
        if (kineticEntityCollBox.y + kineticEntityCollBox.height >= objCollisionBox.y
            && kineticEntityCollBox.y + kineticEntityCollBox.height < objectYCenter) {
            // Bottom collision
            if (aabb->bottom() + kinetic->speedY_ >= objCollisionBox.y) {
                aabb->setBottom(objCollisionBox.y);
                ent2->assign<TopCollisionComponent>();
                kinetic->accY_ = std::min(0.0f, kinetic->accY_);
                kinetic->speedY_ = std::min(0.0f, kinetic->speedY_);
                ent1->assign<BottomCollisionComponent>();
            }
        } else if (kineticEntityCollBox.y <= objectBottom
                   && kineticEntityCollBox.y > objectYCenter) {
            // Top collision
            if (aabb->top() + kinetic->speedY_ <= objCollisionBox.y + objCollisionBox.height) {
                aabb->setTop(objCollisionBox.y + objCollisionBox.height);
                ent2->assign<BottomCollisionComponent>();
                kinetic->accY_ = std::max(0.0f, kinetic->accY_);
                kinetic->speedY_ = std::max(0.0f, kinetic->speedY_);
                ent1->assign<TopCollisionComponent>();
            }
        }
    }
}

void PhysicSystem::checkXCollision(Entity *ent1, Entity *ent2) {
    auto aabb = ent1->get<AABBComponent>();
    auto kinetic = ent1->get<KineticComponent>();
    Rectangle objCollisionBox = ent2->get<AABBComponent>()->collisionBox_;
    float objectRight = objCollisionBox.x + objCollisionBox.width;
    float objectXCenter = objCollisionBox.x + objCollisionBox.width / 2;

    Rectangle kineticEntityCollBox = Rectangle{
            aabb->left() + kinetic->speedX_,
            aabb->top() + kinetic->speedY_ + 2,
            aabb->collisionBox_.width,
            aabb->collisionBox_.height - 4
    };

    Rectangle collisionRect = GetCollisionRec(kineticEntityCollBox, objCollisionBox);

    if (collisionRect.width != 0 && collisionRect.width != aabb->collisionBox_.width) {
        if (kineticEntityCollBox.x <= objectRight && kineticEntityCollBox.x > objectXCenter) {
            // Left Collision
            if (kineticEntityCollBox.x < objectRight) {
                aabb->collisionBox_.x += std::min(.5f, objectRight - aabb->left());
            } else {
                aabb->setLeft(objectRight);
            }

            if (!ent2->has<KineticComponent, PlayerComponent>()) {
                kinetic->accX_ = std::max(0.0f, kinetic->accX_);
                kinetic->speedX_ = std::max(0.0f, kinetic->speedX_);
            } else {
                auto kineticEnt2 = ent2->get<KineticComponent>();
                float acc =  kinetic->accX_ / 2;
                float speed = kinetic->speedX_ * 0.85f;
                kinetic->accX_ = acc;
                kineticEnt2->accX_ = acc;
                kinetic->speedX_ = speed;
                kineticEnt2->speedX_ = speed;
            }
            ent2->assign<RightCollisionComponent>();
            ent1->assign<LeftCollisionComponent>();
        } else if (kineticEntityCollBox.x + kineticEntityCollBox.width >= objCollisionBox.x
                   && kineticEntityCollBox.x + kineticEntityCollBox.width < objectXCenter) {
            // Right collision
            if (kineticEntityCollBox.x + kineticEntityCollBox.width > objCollisionBox.x) {
                aabb->collisionBox_.x -= std::min(.5f, aabb->right() - objCollisionBox.x);
            } else {
                aabb->setRight(objCollisionBox.x);
            }

            if (!ent2->has<KineticComponent, PlayerComponent>()) {
                kinetic->accX_ = std::min(0.0f, kinetic->accX_);
                kinetic->speedX_ = std::min(0.0f, kinetic->speedX_);
            } else {
                auto kineticEnt2 = ent2->get<KineticComponent>();
                float acc =  kinetic->accX_ / 2;
                float speed = kinetic->speedX_ * 0.85f;
                kinetic->accX_ = acc;
                kineticEnt2->accX_ = acc;
                kinetic->speedX_ = speed;
                kineticEnt2->speedX_ = speed;
            }

            ent2->assign<LeftCollisionComponent>();
            ent1->assign<RightCollisionComponent>();
        }
    }
}
