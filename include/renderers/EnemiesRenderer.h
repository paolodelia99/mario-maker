//
// Created by paolo on 28/01/21.
//

#ifndef MARIO_MAKER_ENEMIESRENDERER_H
#define MARIO_MAKER_ENEMIESRENDERER_H

#include "renderers/Renderer.h"
#include "Constants.h"
#include "ECS.h"
#include "Components.h"

class EnemiesRenderer : public Renderer {
public:

    explicit EnemiesRenderer(const char* filepath);

    ~EnemiesRenderer();

    void renderEnemies(ECS::World* world);

    void renderTexture(TextureId textureId, Rectangle dstRect, bool flipH, bool flipV);
};


#endif //MARIO_MAKER_ENEMIESRENDERER_H
