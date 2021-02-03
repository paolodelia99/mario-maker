//
// Created by paolo on 03/02/21.
//

#ifndef MARIO_MAKER_OBJECTRENDERER_H
#define MARIO_MAKER_OBJECTRENDERER_H
#include "renderers/Renderer.h"
#include "renderers/TextureId.h"
#include "Components.h"
#include "ECS.h"

class ObjectRenderer : public Renderer {
public:

    explicit ObjectRenderer(const char* filepath);

    ~ObjectRenderer();

    void render(ECS::World* world);

};


#endif //MARIO_MAKER_OBJECTRENDERER_H
