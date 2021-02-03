//
// Created by paolo on 03/02/21.
//
#include "renderers/ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(const char *filepath) : Renderer(filepath) {
    texturePositions_.insert({FINAL_FLAG_POLE, new Rectangle{770, 510, TILE_SIZE / 2, 152}});
    texturePositions_.insert({FINAL_FLAG, new Rectangle{766, 485, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({BUBBLE, new Rectangle{670, 517, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({PARACHUTE, new Rectangle{580, 467, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({WINGS_DOWN, new Rectangle{563, 467, TILE_SIZE /2, TILE_SIZE}});
    texturePositions_.insert({WINGS_UP, new Rectangle{546, 467, TILE_SIZE /2, TILE_SIZE}});

    loadTextures();
}

ObjectRenderer::~ObjectRenderer() {
    for (auto & texture : textures_)
    {
        UnloadTexture(texture.second);
    }
}

void ObjectRenderer::render(ECS::World *world) {
    for (auto obj : world->each<ObjectComponent, AABBComponent, TextureComponent>()) {
        auto aabb = obj->get<AABBComponent>();
        auto textureComponent = obj->get<TextureComponent>();
        bool flipH = textureComponent->flipH;
        bool flipV = textureComponent->flipV;

        Renderer::renderEntityTexture(textureComponent->textureId_, aabb->collisionBox_, flipH, flipV);
    }
}
