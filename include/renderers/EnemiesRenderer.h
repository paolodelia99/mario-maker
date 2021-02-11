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

    void renderUnderTileEnemies(ECS::World* world, float delta);

    void renderOverTileEnemies(ECS::World* world, float delta);

private:
    void renderEnemy(ECS::Entity* enemy, float delta);
};


#endif //MARIO_MAKER_ENEMIESRENDERER_H
