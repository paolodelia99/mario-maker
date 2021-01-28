//
// Created by paolo on 10/01/21.
//

#include <Constants.h>
#include "renderers/MapRenderer.h"

MapRenderer::~MapRenderer() {
    map_.unloadTextures();
}

MapRenderer::MapRenderer(Map *map, const char* filepath)
:map_(*map)
{
    setFilePath(filepath);

    texturePositions_.insert({QUESTION_BLOCK_1, new Rectangle{368, 0, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({QUESTION_BLOCK_2, new Rectangle{384, 0, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({QUESTION_BLOCK_3, new Rectangle{400, 0, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({QUESTION_BLOCK_OFF, new Rectangle{416, 0, TILE_SIZE, TILE_SIZE}});

    texturePositions_.insert({BRICK, new Rectangle{32, 0, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({BRICK_DEBRIS_1, new Rectangle{32, 0,
                                                                (int) (TILE_SIZE / 2),
                                                                (int) (TILE_SIZE / 2)}});
    texturePositions_.insert({BRICK_DEBRIS_2, new Rectangle{40, 0,
                                                            (int) (TILE_SIZE / 2),
                                                            (int) (TILE_SIZE / 2)}});
    texturePositions_.insert({BRICK_DEBRIS_3, new Rectangle{32, 8,
                                                            (int) (TILE_SIZE / 2),
                                                            (int) (TILE_SIZE / 2)}});
    texturePositions_.insert({BRICK_DEBRIS_4, new Rectangle{40, 8,
                                                            (int) (TILE_SIZE / 2),
                                                            (int) (TILE_SIZE / 2)}});

    texturePositions_.insert({COIN_1, new Rectangle{368, 16, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_2, new Rectangle{384, 16, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({COIN_3, new Rectangle{400, 16, TILE_SIZE, TILE_SIZE}});

    loadTextures();
}

void MapRenderer::render(ECS::World* world) {
    drawGraphicsLayer(map_.getBackgroundLayer(), world, false);
    drawGraphicsLayer(map_.getGraphicsLayer(), world, true);
    renderOtherEntities(world);
}

void MapRenderer::drawGraphicsLayer(unsigned int **mapToRender, ECS::World* world, bool graphics) {

    auto objectMapComponent = world->findFirst<ObjectMapComponent>();

    for (int i = 0; i < map_.getWidth(); i++)
    {
        for (int j = 0; j < map_.getHeight(); j++)
        {
            unsigned int id = mapToRender[i][j];
            if (id != 0)
            {
                Texture2D texture2D = map_.getTexture(id);
                if (!graphics)
                {
                    DrawTexture(texture2D, i * 32, j * 32, WHITE);
                } else {
                    auto map = objectMapComponent->get<ObjectMapComponent>();
                    int id = map->get(i, j);
                    auto ent = world->getById(id);

                    if (!ent) continue;
                    if (!ent->has<AABBComponent, SolidComponent, TileComponent>()) continue;

                    if (ent->has<TextureComponent>()) {
                        auto aabb = ent->get<AABBComponent>();
                        auto texture = ent->get<TextureComponent>();

                        renderTexture(texture->textureId_, (int) aabb->left(), (int) aabb->top());
                    } else {
                        DrawTexture(texture2D, i * 32, j * 32, WHITE);
                    }
                }
            }
        }
    }
}

void MapRenderer::renderTexture(TextureId textureId, int x, int y) {
    auto it = textures_.find(textureId);
    if (it != textures_.end())
    {
        Texture2D texture2D = it->second;
        DrawTexture(texture2D, x, y, WHITE);
    }
}

void MapRenderer::renderOtherEntities(ECS::World *world) {
    for (auto ent : world->each<TextureComponent, TileComponent, AABBComponent>()) {
        auto aabb = ent->get<AABBComponent>();
        auto textureComponent = ent->get<TextureComponent>();
        renderTexture(textureComponent->textureId_, (int) aabb->left(), (int) aabb->top());
    }
}

