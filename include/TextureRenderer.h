//
// Created by paolo on 13/01/21.
//

#ifndef MARIO_MAKER_TEXTURERENDERER_H
#define MARIO_MAKER_TEXTURERENDERER_H

#include <unordered_map>
#include <raylib.h>
#include "ECS.h"

enum TextureId {
    EMPTY,
    MARIO_STAND,
    MARIO_RUN_1,
    MARIO_RUN_2,
    MARIO_RUN_3,
    MARIO_JUMP,
    MARIO_DRIFT,
    MARIO_DEAD,
    MARIO_SIT_1,
    MARIO_SIT_2,
    MARIO_DUCK,
    MARIO_GROWING,
    SUPER_MARIO_STAND,
    SUPER_MARIO_RUN_1,
    SUPER_MARIO_RUN_2,
    SUPER_MARIO_RUN_3,
    SUPER_MARIO_JUMP,
    SUPER_MARIO_DRIFT,
    SUPER_MARIO_DUCK,
    LUIGI_STAND,
    LUIGI_RUN_1,
    LUIGI_RUN_2,
    LUIGI_RUN_3,
    LUIGI_JUMP,
    LUIGI_DRIFT,
    LUIGI_DEAD,
    LUIGI_SIT_1,
    LUIGI_SIT_2,
    LUIGI_DUCK
};

class TextureRenderer {
public:

    explicit TextureRenderer(std::string filepath);

    ~TextureRenderer();

    void renderTexture(TextureId textureId, Rectangle dstRect, bool flipH = false, bool flip = false);

    Texture2D loadTexture(TextureId textureId);

    void loadTextures();

    void renderTextureEntities(ECS::World* world);

    std::unordered_map<TextureId, Texture2D>::iterator returnTexturesIt();

private:
    std::unordered_map<TextureId, Rectangle*> texturePositions_{};
    std::unordered_map<TextureId, Texture2D> textures_{};
    std::string filepath_;

    void renderEntity(ECS::Entity *pEntity);
};


#endif //MARIO_MAKER_TEXTURERENDERER_H
