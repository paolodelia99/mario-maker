//
// Created by paolo on 21/01/21.
//

#ifndef MARIO_MAKER_RENDERER_H
#define MARIO_MAKER_RENDERER_H
#include <raylib.h>
#include "TextureId.h"
#include "Constants.h"
#include <unordered_map>

class Renderer {
public:

    Renderer(const char* filepath)
    : filepath_(filepath)
    {};

    char* getFilePath() {
        return const_cast<char *>(filepath_);
    }

protected:

    void loadTextures() {
        auto it = texturePositions_.begin();

        for (; it != texturePositions_.end(); it++)
        {
            auto rect = it->second;
            float width = rect->width, height = rect->height;
            textures_.insert({it->first, loadTexture(it->first, width, height)});
        }
    }

    Texture2D loadTexture(TextureId textureId, float width, float height) {
        auto it = texturePositions_.find(textureId);

        Rectangle rectangle = *it->second;
        std::pair<int, int> newDims = getTextureDimensions(textureId, (int) width, (int) height);

        Image image = LoadImage(filepath_);
        ImageCrop(&image, rectangle);
        ImageResize(&image, newDims.first, newDims.second);

        Texture2D texture2D = LoadTextureFromImage(image);
        UnloadImage(image);

        return texture2D;
    }

    void renderEntityTexture(TextureId textureId, Rectangle dstRect, bool flipH, bool flipV) {
        auto it = textures_.find(textureId);
        auto it1 = texturePositions_.find(textureId);
        if (it != textures_.end())
        {
            if (dstRect.width == 0) dstRect.width = it1->second->width;
            if (dstRect.height == 0) dstRect.height = it1->second->height;
            Texture2D texture2D = it->second;

#ifdef DEBUG
            DrawRectangleLinesEx(dstRect, 2, RED);
#endif
            DrawTextureRec(texture2D,
                           Rectangle{0, 0, (flipH ? -1 : 1) * dstRect.width, (flipV ? -1 : 1) * dstRect.height } ,
                           Vector2{dstRect.x, dstRect.y},
                           WHITE);
        }
    }

    void render2XEntityTexture(TextureId textureId, Rectangle dstRect, bool flipH, bool flipV) {
        auto it = textures_.find(textureId);
        auto it1 = texturePositions_.find(textureId);
        if (it != textures_.end())
        {
            if (dstRect.width == 0) dstRect.width = it1->second->width;
            if (dstRect.height == 0) dstRect.height = it1->second->height;
            Texture2D texture2D = it->second;
            Rectangle sourceRect = {
                    0.0f,
                    0.0f,
                    (flipH ? -1.0f : 1.0f) * dstRect.width / 2,
                    (flipV ? -1.0f : 1.0f) * dstRect.height / 2 };
            Rectangle destRect = { dstRect.x, dstRect.y, dstRect.width, dstRect.height };
            Vector2 origin = { 0.0f, 0.0f };

#ifdef DEBUG
            DrawRectangleLinesEx(destRect, 2, RED);
#endif

            DrawTexturePro(
                    texture2D,
                    sourceRect,
                    destRect,
                    origin,
                    0.0f,
                    WHITE );
        }
    }

    void renderRotationEntity(TextureId textureId, Rectangle dstRect, int rotation) {
        auto it = textures_.find(textureId);
        if (it != textures_.end())
        {
            Texture2D texture2D = it->second;
            Rectangle sourceRect = {
                    0.0f,
                    0.0f,
                    dstRect.width,
                    dstRect.height };
            Rectangle destRect = {
                    dstRect.x,
                    dstRect.y,
                    dstRect.width,
                    dstRect.height };
            Vector2 origin = { 0.0f , 0.0f };

#ifdef DEBUG
            DrawRectangleLinesEx(destRect, 2, RED);
#endif

            // fixme: doesn't rotate in the right way
            DrawTexturePro(texture2D, sourceRect, destRect, origin, (float) 0.0f, WHITE);
        }
    }

private:

    std::pair<int, int> getTextureDimensions(TextureId textureId, int width, int height) {
        std::pair<int, int> dims;

        switch (textureId) {
            case MARIO_MEGA_DEAD:  case MARIO_MEGA_DRIFT:  case MARIO_MEGA_DUCK:
            case MARIO_MEGA_JUMP:  case MARIO_MEGA_RUN_1:  case MARIO_MEGA_RUN_2:
            case MARIO_MEGA_RUN_3: case MARIO_MEGA_STAND:  case MARIO_MEGA_SIT_1:
            case MARIO_MEGA_SIT_2: case LUIGI_MEGA_STAND:  case LUIGI_MEGA_DUCK:
            case LUIGI_MEGA_JUMP:  case LUIGI_MEGA_DRIFT:  case LUIGI_MEGA_RUN_1:
            case LUIGI_MEGA_RUN_2: case LUIGI_MEGA_RUN_3:  case LUIGI_MEGA_DEAD:
            case LUIGI_MEGA_SIT_1: case LUIGI_MEGA_SIT_2:  case MARIO_MEGA_GREET:
                dims.first = width * 4;
                dims.second = height * 4;
                break;
            default:
                dims.first = width * 2;
                dims.second = height * 2;
                break;
        }

        return dims;
    }

protected:
    std::unordered_map<TextureId, Rectangle*> texturePositions_{};
    std::unordered_map<TextureId, Texture2D> textures_{};

private:
    const char* filepath_;
};
#endif //MARIO_MAKER_RENDERER_H
