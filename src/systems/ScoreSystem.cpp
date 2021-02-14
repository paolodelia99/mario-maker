//
// Created by paolo on 12/02/21.
//
#include <Components.h>
#include "ScoreSystem.h"

ScoreSystem::ScoreSystem() {

}

ScoreSystem::~ScoreSystem() {

}

void ScoreSystem::receive(World *world, const CollisionWithCoinEvent &event) {

    switch (event.coin->get<ObjectComponent>()->type) {
        case Object::COIN_10:
            textCoinCounter_->get<TextComponent>()->incrementValueBy(10);
            textScoreCounter_->get<TextComponent>()->incrementValueBy(100);
            break;
        case Object::COIN_30:
            textCoinCounter_->get<TextComponent>()->incrementValueBy(30);
            textScoreCounter_->get<TextComponent>()->incrementValueBy(100);
            break;
        case Object::COIN_50:
            textCoinCounter_->get<TextComponent>()->incrementValueBy(50);
            textScoreCounter_->get<TextComponent>()->incrementValueBy(100);
            break;
        case Object::COIN:
            textCoinCounter_->get<TextComponent>()->incrementValueBy(1);
            textScoreCounter_->get<TextComponent>()->incrementValueBy(100);
            break;
        default:
            break;
    }

    world->destroy(event.coin);
}

void ScoreSystem::configure(World *world) {
    EntitySystem::configure(world);

    world->subscribe<CollisionWithCoinEvent>(this);
    world->subscribe<AddScoreEvent>(this);

    world->each<TextComponent>([&](
            ECS::Entity* entity,
            ECS::ComponentHandle<TextComponent> textComponent) {
        if (textComponent->type == Text::Type::COIN_COUNTER) {
            textCoinCounter_ = entity;
        } else if (textComponent->type == Text::Type::SCORE_COUNTER) {
            textScoreCounter_ = entity;
        }
    });
}

void ScoreSystem::unconfigure(World *world) {
    EntitySystem::unconfigure(world);

    world->unsubscribeAll(this);
}

void ScoreSystem::tick(World *world, ECS::DefaultTickData data) {
    EntitySystem::tick(world, data);

    destroyScoreTextComponents(world);
}

void ScoreSystem::receive(World *world, const AddScoreEvent &event) {
    textScoreCounter_->get<TextComponent>()->incrementValueBy(event.scoreToAdd);

    if (!event.oneUpEvent) world->create()->assign<GameTextComponent>(event.position, std::to_string(event.scoreToAdd));
    else world->create()->assign<GameTextComponent>(event.position, "1 UP");
}

void ScoreSystem::destroyScoreTextComponents(World *world) {
    world->each<GameTextComponent>([&](
            Entity* entity,
            ComponentHandle<GameTextComponent> scoreTextComponent) {
       if (scoreTextComponent->destroy()) world->destroy(entity);
    });
}
