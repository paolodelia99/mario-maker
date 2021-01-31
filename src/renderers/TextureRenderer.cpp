//
// Created by paolo on 13/01/21.
//

#include "renderers/TextureRenderer.h"

#include <Components.h>
#include <Constants.h>

TextureRenderer::TextureRenderer(const char* filepath)
:Renderer(filepath)
{
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
    texturePositions_.insert({MARIO_FLAME_STAND, new Rectangle{1, 76, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({MARIO_FLAME_DUCK, new Rectangle{18, 76, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({MARIO_FLAME_RUN_1, new Rectangle{35, 76, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({MARIO_FLAME_RUN_2, new Rectangle{52, 76, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({MARIO_FLAME_RUN_3, new Rectangle{69, 76, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({MARIO_FLAME_DRIFT, new Rectangle{86, 76, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({MARIO_FLAME_JUMP, new Rectangle{103, 76, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({MARIO_FLAME_SHOOT, new Rectangle{256, 76, TILE_SIZE, TILE_SIZE * 2}});
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
    texturePositions_.insert({SUPER_LUIGI_STAND, new Rectangle{1, 191, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_LUIGI_DUCK, new Rectangle{18, 191, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_LUIGI_RUN_1, new Rectangle{35, 191, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_LUIGI_RUN_2, new Rectangle{52, 191, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_LUIGI_RUN_3, new Rectangle{69, 191, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_LUIGI_DRIFT, new Rectangle{86, 191, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({SUPER_LUIGI_JUMP, new Rectangle{103, 191, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_FLAME_STAND, new Rectangle{1, 224, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_FLAME_DUCK, new Rectangle{18, 224, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_FLAME_RUN_1, new Rectangle{35, 224, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_FLAME_RUN_2, new Rectangle{52, 224, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_FLAME_RUN_3, new Rectangle{69, 224, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_FLAME_DRIFT, new Rectangle{86, 224, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_FLAME_JUMP, new Rectangle{103, 224, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_FLAME_SHOOT, new Rectangle{256, 224, TILE_SIZE, TILE_SIZE * 2}});
    texturePositions_.insert({LUIGI_MEGA_STAND, new Rectangle{1, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_RUN_1, new Rectangle{35, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_RUN_2, new Rectangle{52, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_RUN_3, new Rectangle{69, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_DRIFT, new Rectangle{86, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_JUMP, new Rectangle{103, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_SIT_1, new Rectangle{222, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_SIT_2, new Rectangle{239, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_DUCK, new Rectangle{18, 174, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({LUIGI_MEGA_DEAD, new Rectangle{256, 174, TILE_SIZE, TILE_SIZE}});

    // Power ups
    texturePositions_.insert({SUPER_MUSHROOM, new Rectangle{1, 597, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({ONE_UP_MUSHROOM, new Rectangle{18, 597, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({MEGA_MUSHROOM, new Rectangle{1, 614, TILE_SIZE * 2, TILE_SIZE * 2}});
    texturePositions_.insert({FLAME_FLOWER_1, new Rectangle{69, 597, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({FLAME_FLOWER_2, new Rectangle{86, 597, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({FLAME_FLOWER_3, new Rectangle{103, 597, TILE_SIZE, TILE_SIZE}});
    texturePositions_.insert({FLAME_FLOWER_4, new Rectangle{120, 597, TILE_SIZE, TILE_SIZE}});

    texturePositions_.insert({FIRE_BULLET, new Rectangle{358, 76, TILE_SIZE / 2, TILE_SIZE / 2}});

    // fill texture map
    loadTextures();
}

TextureRenderer::~TextureRenderer() {
    for (auto & texture : textures_)
    {
        UnloadTexture(texture.second);
    }
}

void TextureRenderer::renderTexture(TextureId textureId, Rectangle dstRect, bool flipH, bool flipV) {
    Renderer::renderEntityTexture(textureId, dstRect, flipH, flipV);
}

std::unordered_map<TextureId, Texture2D>::iterator TextureRenderer::returnTexturesIt() {
    return textures_.begin();
}

void TextureRenderer::renderTextureEntities(ECS::World *world, float delta) {
    for (auto entity : world->each<TextureComponent, AABBComponent>())
    {
        renderEntity(entity, delta);
    }
}

void TextureRenderer::renderEntity(ECS::Entity *entity, float d) {
    auto aabb = entity->get<AABBComponent>();
    auto texture = entity->get<TextureComponent>();

    if (entity->has<KineticComponent>()) {
        auto kinetic = entity->get<KineticComponent>();

        Rectangle rect{
                aabb->left() + texture->offSetX - kinetic->speedX_ * d,
                aabb->top() + texture->offSetY - kinetic->speedY_ * d,
                texture->w > 0 ? texture->w : aabb->collisionBox_.width,
                texture->h > 0 ? texture->h : aabb->collisionBox_.height
        };

//        if (entity->has<RotationComponent>()) {
//            // fixme: is not working
//            renderRotationEntity(texture->textureId_, rect, entity->get<RotationComponent>()->rotation_);
//        }

        Renderer::renderEntityTexture(texture->textureId_, rect, texture->flipH, texture->flipV);
    } else {
        Rectangle rect{
                aabb->left() + texture->offSetX,
                aabb->top() + texture->offSetY,
                texture->w > 0 ? texture->w : aabb->collisionBox_.width,
                texture->h > 0 ? texture->h : aabb->collisionBox_.height
        };

        Renderer::renderEntityTexture(texture->textureId_, rect, texture->flipH, texture->flipV);
    }
}

void TextureRenderer::renderTileCollisionRect(ECS::World* world) {
    for (auto ent : world->each<AABBComponent, TileComponent, SolidComponent>()) {
        auto aabb = ent->get<AABBComponent>();

        if (DEBUG) DrawRectangleLinesEx(aabb->collisionBox_, 2, RED);
    }
}

void TextureRenderer::renderRotationEntity(TextureId textureId, Rectangle destRect, int rotation) {
    auto it = textures_.find(textureId);
    if (it != textures_.end())
    {
        Texture2D texture2D = it->second;
        Rectangle sourceRect = {0.0f, 0.0f, TILE_SIZE / 2, TILE_SIZE / 2 };
        Vector2 origin = {TILE_SIZE / 2, TILE_SIZE / 2 };
        if (DEBUG) DrawRectangleLinesEx(destRect, 2, RED);
        DrawTexturePro(texture2D, sourceRect, destRect, origin, (float) rotation, WHITE);
    }
}
