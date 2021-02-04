//
// Created by paolo on 14/01/21.
//

#include <Components.h>
#include <iostream>
#include "systems/CameraSystem.h"

void CameraSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    followLeadPlayer(world);

    defreezeCloseEnemies(world);
}

CameraSystem::CameraSystem(const int screenW, const int screenH, const int mapW, const int mapH)
:screenWidth_(screenW), screenHeight_(screenH), mapWidth_(mapW), mapHeight_(mapH)
{

}

void CameraSystem::configure(World *world) {
    EntitySystem::configure(world);

    CameraComponent* camera2D = &world->findFirst<CameraComponent>()->get<CameraComponent>().get();

    if (!camera2D) throw std::invalid_argument("Camera Component not found in Camera System");

    pCamera_ = camera2D;
}

void CameraSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);
}

CameraSystem::~CameraSystem() {

}

void CameraSystem::followLeadPlayer(World *world) {
    Entity* leadPlayer = world->findFirst<PlayerComponent, AABBComponent, LeadCameraComponent>();

    if (leadPlayer) {
        AABBComponent playerAABB = leadPlayer->get<AABBComponent>().get();

        if (playerAABB.right() >= pCamera_->camera.target.x) {
            pCamera_->camera.target.x = playerAABB.right();
        }

        if (pCamera_->camera.target.x - playerAABB.getCenterX() > screenWidth_ / 4) {
            pCamera_->camera.target.x -= 2;
        }

        if (playerAABB.top() > pCamera_->camera.target.y) {
            pCamera_->camera.target.y = playerAABB.top();
        }

        // Check if the camera doesn't exist the map
        if (pCamera_->camera.target.x < screenWidth_ / 2) {
            pCamera_->camera.target.x = screenWidth_ / 2;
        } else if (pCamera_->camera.target.x > mapWidth_ - screenWidth_ / 2) {
            pCamera_->camera.target.x = mapWidth_ - screenWidth_ / 2;
        }

        if (pCamera_->camera.target.y < screenHeight_ / 2) {
            pCamera_->camera.target.y = screenHeight_ / 2;
        } else if (pCamera_->camera.target.y > mapHeight_ - screenHeight_ / 2) {
            pCamera_->camera.target.y = mapHeight_ - screenHeight_ / 2;
        }
    }
}

void CameraSystem::defreezeCloseEnemies(World *world) {
    Entity* leadPlayer = world->findFirst<PlayerComponent, AABBComponent, LeadCameraComponent>();
    if (leadPlayer) {
        auto aabb = leadPlayer->get<AABBComponent>();
        Vector2 playerPos = { aabb->left(), aabb->top() };

        for (auto enemy : world->each<EnemyComponent, FrozenComponent, AABBComponent>()) {
            auto enemyAABB = enemy->get<AABBComponent>();
            Vector2 enemyPos = { enemyAABB->left(), enemyAABB->top() };

            if (std::abs(playerPos.x - enemyPos.x + GAME_TILE_SIZE) <= screenWidth_ / 2
                && std::abs(playerPos.y - enemyPos.y) <= screenHeight_ / 2) {
                enemy->remove<FrozenComponent>();
            }
        }
    }
}
