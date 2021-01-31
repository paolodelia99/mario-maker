//
// Created by paolo on 13/01/21.
//

#ifndef MARIO_MAKER_TEXTURERENDERER_H
#define MARIO_MAKER_TEXTURERENDERER_H

#include <unordered_map>
#include <raylib.h>
#include "ECS.h"
#include "Renderer.h"
#include "TextureId.h"

class TextureRenderer : public Renderer {
public:

    explicit TextureRenderer(const char* filepath);

    ~TextureRenderer();

    void renderTexture(TextureId textureId, Rectangle dstRect, bool flipH = false, bool flipV = false);

    void renderTextureEntities(ECS::World* world, float delta);

    void renderTileCollisionRect(ECS::World* world);

    std::unordered_map<TextureId, Texture2D>::iterator returnTexturesIt();

private:

    void renderEntity(ECS::Entity *entity, float d);

    void renderRotationEntity(TextureId textureId, Rectangle destRect, int rotation);
};


#endif //MARIO_MAKER_TEXTURERENDERER_H
