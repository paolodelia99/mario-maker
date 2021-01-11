//
// Created by paolo on 10/01/21.
//

#include "MapRenderer.h"

MapRenderer::~MapRenderer() {
    map_.unloadTextures();
}

MapRenderer::MapRenderer(Map *map)
:map_(*map) {}

void MapRenderer::render() {
    drawGraphicsLayer(map_.getBackgroundLayer());
    drawGraphicsLayer(map_.getGraphicsLayer());
}

void MapRenderer::drawGraphicsLayer(unsigned int **mapToRender) {
    for (int i = 0; i < map_.getWidth(); i++)
    {
        for (int j = 0; j < map_.getHeight(); j++)
        {
            unsigned int id = mapToRender[i][j];
            if (id != 0)
            {
                Texture2D texture2D = map_.getTexture(id);
                DrawTexture(texture2D, i * 32, j * 32, WHITE);
            }
        }
    }
}
