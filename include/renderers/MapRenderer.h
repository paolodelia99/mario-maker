//
// Created by paolo on 10/01/21.
//

#ifndef MARIO_MAKER_MAPRENDERER_H
#define MARIO_MAKER_MAPRENDERER_H
#include <raylib.h>
#include "GameMap.h"
#include "ECS.h"
#include "Renderer.h"

class MapRenderer : public Renderer {
public:

    MapRenderer(GameMap *map, const char* filepath);

    void render(ECS::World* world);

    virtual ~MapRenderer();

private:

    void drawGraphicsLayer(unsigned int** mapToRender, ECS::World* world, bool graphics);

    void renderTexture(TextureId textureId, int x, int y);

private:
    GameMap map_;

    void renderOtherEntities(ECS::World *pWorld);
};


#endif //MARIO_MAKER_MAPRENDERER_H
