//
// Created by paolo on 13/01/21.
//

#include "renderers/TextureRenderer.h"

#include <utility>
#include <Components.h>
#include <Constants.h>

TextureRenderer::TextureRenderer(const char* filepath)
{
    setFilePath(filepath);
    // Mario Textures
    texturePositions_.insert({MARIO_STAND, new Rectangle{1, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_RUN_1, new Rectangle{35, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_RUN_2, new Rectangle{52, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_RUN_3, new Rectangle{69, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_DRIFT, new Rectangle{86, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_JUMP, new Rectangle{103, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_SIT_1, new Rectangle{222, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_SIT_2, new Rectangle{239, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_DUCK, new Rectangle{18, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_DEAD, new Rectangle{256, 9, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({SUPER_MARIO_STAND, new Rectangle{1, 43, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_MARIO_DUCK, new Rectangle{18, 43, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_MARIO_RUN_1, new Rectangle{35, 43, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_MARIO_RUN_2, new Rectangle{52, 43, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_MARIO_RUN_3, new Rectangle{69, 43, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_MARIO_DRIFT, new Rectangle{86, 43, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_MARIO_JUMP, new Rectangle{103, 43, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({MARIO_MEGA_STAND, new Rectangle{1, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_RUN_1, new Rectangle{35, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_RUN_2, new Rectangle{52, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_RUN_3, new Rectangle{69, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_DRIFT, new Rectangle{86, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_JUMP, new Rectangle{103, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_SIT_1, new Rectangle{222, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_SIT_2, new Rectangle{239, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_DUCK, new Rectangle{18, 26, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MARIO_MEGA_DEAD, new Rectangle{256, 26, TILE_SIZE, TILE_SIZE}});

    // Luigi Textures
    texturePositions_.insert({LUIGI_STAND, new Rectangle{1, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_RUN_1, new Rectangle{35, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_RUN_2, new Rectangle{52, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_RUN_3, new Rectangle{69, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_DRIFT, new Rectangle{86, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_JUMP, new Rectangle{103, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_SIT_1, new Rectangle{222, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_SIT_2, new Rectangle{239, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_DUCK, new Rectangle{18, 157, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_DEAD, new Rectangle{256, 157, TILE_SIZE, TILE_SIZE}});

    texturePositions_.insert({SUPER_MUSHROOM, new Rectangle{1, 597, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({ONE_UP_MUSHROOM, new Rectangle{18, 597, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MEGA_MUSHROOM, new Rectangle{1, 614, 32, 32}});

    // fill texture map
    loadTextures();
}

TextureRenderer::~TextureRenderer() {
    for (auto & texture : textures_)
    {
        UnloadTexture(texture.second);
    }
}

void TextureRenderer::renderTexture(TextureId textureId, Rectangle dstRect, bool flipH, bool flip) {
    auto it = textures_.find(textureId);
    auto it1 = texturePositions_.find(textureId);
    if (it != textures_.end())
    {
        if (dstRect.width == 0) dstRect.width = it1->second->width;
        if (dstRect.height == 0) dstRect.height = it1->second->height;
        Texture2D texture2D = it->second;
        if (DEBUG) DrawRectangleLinesEx(dstRect, 2, RED);
        DrawTextureRec(texture2D,
                       Rectangle{0, 0, (flipH ? -1 : 1) * dstRect.width, dstRect.height } ,
                       Vector2{dstRect.x, dstRect.y},
                       WHITE);
    }
}

std::unordered_map<TextureId, Texture2D>::iterator TextureRenderer::returnTexturesIt() {
    return textures_.begin();
}

void TextureRenderer::renderTextureEntities(ECS::World *world) {
    for (auto entity : world->each<TextureComponent, AABBComponent>())
    {
        renderEntity(entity);
    }
}

void TextureRenderer::renderEntity(ECS::Entity *pEntity) {
    auto aabb = pEntity->get<AABBComponent>();
    auto texture = pEntity->get<TextureComponent>();

    Rectangle rect{
        aabb->left() + texture->offSetX,
        aabb->top() + texture->offSetY,
        texture->w > 0 ? texture->w : aabb->collisionBox_.width,
        texture->h > 0 ? texture->h : aabb->collisionBox_.height
    };

    renderTexture(texture->textureId_, rect, texture->flipH, texture->flipV);
}

void TextureRenderer::renderCollisionRect(ECS::World* world) {
    for (auto ent : world->each<AABBComponent, TileComponent, SolidComponent>()) {
        auto aabb = ent->get<AABBComponent>();

        if (DEBUG) DrawRectangleLinesEx(aabb->collisionBox_, 2, RED);
    }
}
