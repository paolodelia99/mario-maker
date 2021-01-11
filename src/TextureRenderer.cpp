//
// Created by paolo on 13/01/21.
//

#include "TextureRenderer.h"

#include <utility>
#include <Components.h>

TextureRenderer::TextureRenderer(std::string filepath)
: filepath_(std::move(filepath))
{
    // Mario Textures
    texturePositions_.insert({MARIO_STAND, new Rectangle{275, 44, 16, 16}});
    texturePositions_.insert({MARIO_RUN_1, new Rectangle{290, 44, 16, 16}});
    texturePositions_.insert({MARIO_RUN_2, new Rectangle{304, 44, 16, 16}});
    texturePositions_.insert({MARIO_RUN_3, new Rectangle{320, 44, 16, 16}});
    texturePositions_.insert({MARIO_DRIFT, new Rectangle{337, 44, 16, 16}});
    texturePositions_.insert({MARIO_JUMP, new Rectangle{355, 44, 16, 16}});
    texturePositions_.insert({MARIO_SIT_1, new Rectangle{372, 44, 16, 16}});
    texturePositions_.insert({MARIO_SIT_2, new Rectangle{387, 44, 16, 16}});
    texturePositions_.insert({MARIO_DEAD, new Rectangle{485, 45, 16, 16}});

    // Luigi Textures
    texturePositions_.insert({LUIGI_STAND, new Rectangle{275, 106, 16, 16}});
    texturePositions_.insert({LUIGI_RUN_1, new Rectangle{290, 106, 16, 16}});

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
        DrawTexture(texture2D, dstRect.x, dstRect.y, WHITE);
    }
}

Texture2D TextureRenderer::loadTexture(TextureId textureId) {

    auto it = texturePositions_.find(textureId);

    Rectangle rectangle = *it->second;

    Image image = LoadImage(filepath_.c_str());
    ImageCrop(&image, rectangle);
    ImageResize(&image, 32, 32);

    Texture2D texture2D = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture2D;
}

void TextureRenderer::loadTextures() {
    auto it = texturePositions_.begin();

    for (; it != texturePositions_.end(); it++)
    {
        textures_.insert({it->first, loadTexture(it->first)});
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
        texture->w > 0 ? texture->w : aabb->size_.x,
        texture->h > 0 ? texture->h : aabb->size_.y
    };

    renderTexture(texture->textureId_, rect, texture->flipH, texture->flipV);
}
