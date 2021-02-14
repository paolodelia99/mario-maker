//
// Created by paolo on 12/02/21.
//

#ifndef MARIO_MAKER_TEXTRENDERER_H
#define MARIO_MAKER_TEXTRENDERER_H
#include <raylib.h>
#include "ECS.h"
#include "Components.h"

class TextRenderer {
public:

    TextRenderer();

    ~TextRenderer();

    void render(ECS::World* world);

    void renderScoreTextComponents(ECS::World *world);

private:
    Font textFont;
};


#endif //MARIO_MAKER_TEXTRENDERER_H
