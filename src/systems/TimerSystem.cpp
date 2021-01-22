//
// Created by paolo on 22/01/21.
//
#include "TimerSystem.h"

void TimerSystem::tick(World *world, float delta) {
    EntitySystem::tick(world, delta);

    world->each<TimerComponent>([&](Entity* entity, ComponentHandle<TimerComponent> timer) {
        if (timer->time > 0) {
            timer->time--;
            if (timer->time == 0) timer->callback();
        } else {
            entity->remove<TimerComponent>();
        }
    });
}

TimerSystem::~TimerSystem() {

}

TimerSystem::TimerSystem() {

}

void TimerSystem::configure(World *world) {
    EntitySystem::configure(world);
}

void TimerSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);
}
