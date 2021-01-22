//
// Created by paolo on 13/01/21.
//
#include <gtest/gtest.h>
#include "renderers/TextureRenderer.h"

TEST(textureManagerTest, testRenderTexture) {
    InitWindow(250, 250, "Test texture Loader");
    SetTargetFPS(60);
    int c = 0;

    TextureRenderer* textureLoader = new TextureRenderer("../../assets/imgs/players-tileset.png");

    while(++c < 100) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        textureLoader->renderTexture(SUPER_MARIO_JUMP, (Rectangle) {250.f / 2, 250.f / 2, 32, 64}, false);

        DrawText("this IS a texture loaded_ from an image!", 300, 370, 10, GRAY);

        EndDrawing();
    }

    delete textureLoader;

    CloseWindow();

    EXPECT_TRUE(true);
}

TEST(textureManagerTest, testLoadTextures) {
    InitWindow(250, 250, "Test texture Loader");
    SetTargetFPS(60);
    int c = 0;

    TextureRenderer* textureLoader = new TextureRenderer("../../assets/imgs/characters.gif");

    auto it = textureLoader->returnTexturesIt();
    EXPECT_EQ(it->first, MARIO_STAND);


    while(++c < 360) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(it->second, 250/2, 250/2, WHITE);

        DrawText("this IS a texture loaded_ from an image!", 300, 370, 10, GRAY);

        EndDrawing();
    }

    delete textureLoader;

    CloseWindow();

    EXPECT_TRUE(true);
}

TEST(textureManagerTest, testOthersTextures) {
    InitWindow(250, 250, "Test texture Loader");
    SetTargetFPS(60);
    int c = 0;

    const char* filepath = "../../assets/imgs/players-tileset.png";

    Image image = LoadImage(filepath);
    ImageCrop(&image, Rectangle {18, 9, 16, 16});
    ImageResize(&image, 32, 32);

    Texture2D texture2D = LoadTextureFromImage(image);
    UnloadImage(image);

    while(++c < 360) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(texture2D,
                       250 / 2, 250 / 2, WHITE);

        EndDrawing();
    }

    UnloadTexture(texture2D);

    CloseWindow();

    EXPECT_TRUE(true);
}