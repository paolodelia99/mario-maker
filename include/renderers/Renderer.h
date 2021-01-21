//
// Created by paolo on 21/01/21.
//

#ifndef MARIO_MAKER_RENDERER_H
#define MARIO_MAKER_RENDERER_H
#include <raylib.h>
#include "TextureId.h"

class Renderer {
public:

    void setFilePath(const char* filepath) {
        this->filepath_ = filepath;
    }

    char* getFilePath() {
        return const_cast<char *>(filepath_);
    }

protected:

    void loadTextures() {
        auto it = texturePositions_.begin();

        for (; it != texturePositions_.end(); it++)
        {
            textures_.insert({it->first, loadTexture(it->first)});
        }
    }

    Texture2D loadTexture(TextureId textureId) {
        auto it = texturePositions_.find(textureId);

        Rectangle rectangle = *it->second;

        Image image = LoadImage(filepath_);
        ImageCrop(&image, rectangle);
        ImageResize(&image, 32, 32);

        Texture2D texture2D = LoadTextureFromImage(image);
        UnloadImage(image);

        return texture2D;
    }

protected:
    std::unordered_map<TextureId, Rectangle*> texturePositions_{};
    std::unordered_map<TextureId, Texture2D> textures_{};

private:
    const char* filepath_;
};
#endif //MARIO_MAKER_RENDERER_H
