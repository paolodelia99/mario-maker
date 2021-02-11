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

    // Coin
    texturePositions_.insert({COIN_1, new Rectangle{154, 1, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_2, new Rectangle{171, 1, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_3, new Rectangle{188, 1, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_4, new Rectangle{205, 1, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_5, new Rectangle{222, 1, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_6, new Rectangle{239, 1, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_7, new Rectangle{256, 1, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_8, new Rectangle{273, 1, TILE_SIZE, TILE_SIZE}});

    // Coin 10
    texturePositions_.insert({COIN_10_1, new Rectangle{1, 170, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_10_2, new Rectangle{34, 170, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_10_3, new Rectangle{67, 170, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_10_4, new Rectangle{100, 170, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_10_5, new Rectangle{133, 170, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_10_6, new Rectangle{166, 170, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_10_7, new Rectangle{199, 170, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_10_8, new Rectangle{232, 170, TILE_SIZE * 2, TILE_SIZE * 2}});

    // Coin 30
    texturePositions_.insert({COIN_30_1, new Rectangle{1, 203, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_30_2, new Rectangle{34, 203, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_30_3, new Rectangle{67, 203, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_30_4, new Rectangle{100, 203, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_30_5, new Rectangle{133, 203, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_30_6, new Rectangle{166, 203, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_30_7, new Rectangle{199, 203, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_30_8, new Rectangle{232, 203, TILE_SIZE * 2, TILE_SIZE * 2}});

    // Coin 50
    texturePositions_.insert({COIN_50_1, new Rectangle{1, 236, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_50_2, new Rectangle{34, 236, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_50_3, new Rectangle{67, 236, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_50_4, new Rectangle{100, 236, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_50_5, new Rectangle{133, 236, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_50_6, new Rectangle{166, 236, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_50_7, new Rectangle{199, 236, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({COIN_50_8, new Rectangle{232, 236, TILE_SIZE * 2, TILE_SIZE * 2}});

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
