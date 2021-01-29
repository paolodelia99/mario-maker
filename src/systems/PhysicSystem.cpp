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

    checkIfOutsideWorld(world);

    applyGravity(world);

    checkKineticKineticCollisions(world);

    checkKineticTileCollisions(world);

    moveWalkComponents(world);

    applyForces(world);
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
            aabb->collisionBox_.width - TILE_ROUNDNESS / 2,
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
                checkKillEnemy(ent1, ent2);
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
                checkIfBreakComponent(ent1, ent2);
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

            if (!ent2->has<PlayerComponent>()) {
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

            if (!ent2->has<PlayerComponent>()) {
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
        checkPlayerEnemyCollision(ent1, ent2);
    }
}

void PhysicSystem::moveWalkComponents(World *world) {
    world->each<WalkComponent, AABBComponent, KineticComponent>([&](
                    Entity* entity,
                    ComponentHandle<WalkComponent> walkComponent,
                    ComponentHandle<AABBComponent> aabb,
                    ComponentHandle<KineticComponent> kinetic) {
        if (entity->has<LeftCollisionComponent>()) {
            walkComponent->speed = std::abs(walkComponent->speed);
            if (entity->has<TextureComponent, EnemyComponent>()) {
                entity->get<TextureComponent>()->flipH = true;
            }
        } else if (entity->has<RightCollisionComponent>()) {
            walkComponent->speed = -std::abs(walkComponent->speed);
            if (entity->has<TextureComponent, EnemyComponent>()) {
                entity->get<TextureComponent>()->flipH = false;
            }
        }

        kinetic->speedX_ = walkComponent->speed;
    });
}

void PhysicSystem::checkKineticTileCollisions(World *world) {
    auto objMapEntity = world->findFirst<ObjectMapComponent>();
    if (objMapEntity) {
        ComponentHandle<ObjectMapComponent> map = objMapEntity->get<ObjectMapComponent>();

        for (auto ent : world->each<KineticComponent, AABBComponent, SolidComponent>())
        {
            ComponentHandle<AABBComponent> aabb = ent->get<AABBComponent>();
            std::unordered_set<int> neighbors = getNeighborIds(map, aabb);

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
}

void PhysicSystem::applyForces(World *world) {
    for (auto ent : world->each<AABBComponent, KineticComponent>())
    {
        // if (ent->has<FrozenComponent>()) continue;
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

void PhysicSystem::checkKineticKineticCollisions(World* world) {
    for (auto ent : world->each<AABBComponent, KineticComponent, SolidComponent>())
    {
        for (auto ent2 : world->each<AABBComponent, KineticComponent, SolidComponent>())
        {
            if (ent == ent2) continue;

            checkYCollision(ent, ent2);

            checkXCollision(ent, ent2);
        }
    }
}

void PhysicSystem::applyGravity(World* world) {
    for (auto ent : world->each<GravityComponent, KineticComponent>())
    {
        ent->get<KineticComponent>()->accY_ += GRAVITY;
    }
}

std::unordered_set<int> PhysicSystem::getNeighborIds(ComponentHandle<ObjectMapComponent> map, ComponentHandle<AABBComponent> aabb) {
    int height = (int) std::round(aabb->collisionBox_.height / 32);
    int width = (int) std::round(aabb->collisionBox_.width / 32);
    std::unordered_set<int> neighbors;
    int x = (int) std::round(aabb->left() / 32);
    int y = (int) std::round(aabb->top() / 32);

    //todo change this hardcoded stuff
    if (height == 1 && width == 1) {
        neighbors = {
                map->get(x + 1, y),
                map->get(x - 1, y),
                map->get(x, y + 1),
                map->get(x, y - 1),
                map->get(x + 1, y + 1),
                map->get(x + 1, y - 1),
                map->get(x - 1, y + 1),
                map->get(x - 1, y - 1)
        };
    } else if (height == 2 && width == 1) {
        neighbors = {
                map->get(x - 1, y - 1),
                map->get(x, y - 1),
                map->get(x + 1, y - 1),
                map->get(x - 1, y),
                map->get(x + 1, y),
                map->get(x - 1, y + 1),
                map->get(x + 1, y + 1),
                map->get(x - 1, y + 2),
                map->get(x, y + 2),
                map->get(x + 1, y +2)
        };
    } else if (height == 2 && width == 2) {
        neighbors = {
                map->get(x - 1, y - 1),
                map->get(x, y - 1),
                map->get(x + 1, y - 1),
                map->get(x + 2, y - 1),
                map->get(x - 1, y),
                map->get(x + 2, y),
                map->get(x - 1, y + 1),
                map->get(x + 2, y + 1),
                map->get(x - 1, y + 2),
                map->get(x, y + 2),
                map->get(x + 1, y + 2),
                map->get(x + 2, y + 2),
        };
    }

    return neighbors;
}

void PhysicSystem::checkIfOutsideWorld(World *world) {
    auto objMap = world->findFirst<ObjectMapComponent>()->get<ObjectMapComponent>();

    world->each<AABBComponent>([&](
            Entity* entity,
            ComponentHandle<AABBComponent> aabb) {
       if ((aabb->right() < 0 || aabb->left() > static_cast<float> (objMap->width_ * GAME_TILE_SIZE))
            || (aabb->top() > objMap->height_ * GAME_TILE_SIZE)) {
           world->destroy(entity);
       }
    });
}

void PhysicSystem::checkIfBreakComponent(Entity *ent1, Entity *ent2) {
    World* world = ent1->getWorld();
    ComponentHandle<ObjectMapComponent> objMap = world->findFirst<ObjectMapComponent>()->get<ObjectMapComponent>();

    if (ent1->has<PlayerComponent>() && ent2->has<BreakableComponent, AABBComponent>()) {
        if (ent1->has<SuperComponent>() || ent1->has<SuperFlameComponent>() || ent1->has<MegaComponent>()) {
            auto aabbBreakable = ent2->get<AABBComponent>();
            int x = (int) std::round(aabbBreakable->left() / GAME_TILE_SIZE);
            int y = (int) std::round(aabbBreakable->top() / GAME_TILE_SIZE);
            world->destroy(ent2);
            objMap->set(-1, x, y);
            world->emit<BreakEvent>({aabbBreakable->left(), aabbBreakable->top()});
        }
    }
}

void PhysicSystem::checkKillEnemy(Entity *ent1, Entity *ent2) {
    if (ent2->has<EnemyComponent>() && ent1->has<PlayerComponent>()) {
        World* world = ent1->getWorld();
        world->emit<KillEnemyEvent>(KillEnemyEvent(ent2));
        // Make the player bounce
        auto playerKinetic = ent1->get<KineticComponent>();
        playerKinetic->accY_ = -0.8f;
        playerKinetic->speedY_ = -MARIO_BOUNCE;
    }
}

void PhysicSystem::checkPlayerEnemyCollision(Entity *ent1, Entity *ent2) {
    World* world = ent1->getWorld();

    if (ent1->has<PlayerComponent>() && ent2->has<EnemyComponent>()) {
        EnemyCollisionEvent event{ent1, ent2};
        world->emit<EnemyCollisionEvent>(event);
    } else if (ent1->has<EnemyComponent>() && ent2->has<PlayerComponent>()) {
        EnemyCollisionEvent event{ent2, ent1};
        world->emit<EnemyCollisionEvent>(event);
    }
}
