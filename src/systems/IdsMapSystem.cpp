//
// Created by paolo on 04/02/21.
//

#include "systems/IdsMapSystem.h"

IdsMapSystem::IdsMapSystem() {}

IdsMapSystem::~IdsMapSystem() {

}

void IdsMapSystem::configure(World *world) {
    EntitySystem::configure(world);
}

void IdsMapSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);
}

void IdsMapSystem::tick(World *world, ECS::DefaultTickData data) {
    EntitySystem::tick(world, data);

    auto kineticMapEnt = world->findFirst<SpacialHashMapComponent, KineticEntitiesMapComponent>();
    if (kineticMapEnt) {
        ComponentHandle<SpacialHashMapComponent> kineticMap = kineticMapEnt->get<SpacialHashMapComponent>();

        // fixme: there might be a more sofisticated way to update the map without clear it all the time
        //idea: store prev position and compare it with actual position
        kineticMap->clear();

        for (auto entity : world->each<AABBComponent, KineticComponent, SolidComponent>()) {
            if (entity->has<FrozenComponent>()) continue;
            auto aabb = entity->get<AABBComponent>();

            if (round(aabb->collisionBox_.width) <= GAME_TILE_SIZE && round(aabb->collisionBox_.height) <= GAME_TILE_SIZE) {
                unsigned int x = (int)round(aabb->left() / 32);
                unsigned int y = (int)round(aabb->top() / 32);
                kineticMap->set(x, y, entity->getEntityId());
            } else {
                for (int j = (int)(round(aabb->top() / 32)); j < (int)(aabb->bottom() / 32); j++) {
                    for (int i = (int)(round(aabb->left() / 32)); i < (int)(aabb->right() / 32); i++) {
                        kineticMap->set(i, j, entity->getEntityId());
                    }
                }
            }
        }
    } else {
        throw std::invalid_argument("There isn't any kineticMapEntity!");
    }
}
