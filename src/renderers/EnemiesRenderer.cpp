//
// Created by paolo on 28/01/21.
//

#include "renderers/EnemiesRenderer.h"

EnemiesRenderer::EnemiesRenderer(const char *filepath)
:Renderer(filepath)
{
    // Goomba
    texturePositions_.insert({GOOMBA_1, new Rectangle{1, 2, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({GOOMBA_2, new Rectangle{18, 2, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({GOOMBA_FALLING, new Rectangle{56, 1, TILE_SIZE, 19}});
    texturePositions_.insert({GOOMBA_DEAD, new Rectangle{98, 2, TILE_SIZE, TILE_SIZE}});

    // Green Turtle
    texturePositions_.insert({GREEN_TURTLE_1, new Rectangle{1, 42, TILE_SIZE, 24}});
    texturePositions_.insert({GREEN_TURTLE_2, new Rectangle{18, 42, TILE_SIZE, 24}});
    texturePositions_.insert({G_TURLE_SHELL_STAND_1, new Rectangle{39, 50, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({G_TURLE_SHELL_STAND_2, new Rectangle{56, 50, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({G_TURLE_SHELL_MOVE_1, new Rectangle{77, 50, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({G_TURLE_SHELL_MOVE_2, new Rectangle{94, 50, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({G_TURLE_SHELL_MOVE_3, new Rectangle{111, 50, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({G_TURLE_SHELL_MOVE_4, new Rectangle{128, 50, TILE_SIZE, TILE_SIZE}});

    loadTextures();
}

EnemiesRenderer::~EnemiesRenderer() {
    for (auto & texture : textures_)
    {
        UnloadTexture(texture.second);
    }
}

void EnemiesRenderer::renderEnemies(ECS::World *world) {
    for (auto ent : world->each<AABBComponent, EnemyComponent, TextureComponent>()) {
        auto aabb = ent->get<AABBComponent>();
        auto textureComponent = ent->get<TextureComponent>();
        bool flipH = textureComponent->flipH;
        bool flipV = textureComponent->flipV;

        Renderer::renderEntityTexture(textureComponent->textureId_, aabb->collisionBox_, flipH, flipV);
    }
}
