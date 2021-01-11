//
// Created by paolo on 13/01/21.
//
#include <gtest/gtest.h>
#include "TextureRenderer.h"

TEST(textureManagerTest, testRenderTexture) {
    InitWindow(250, 250, "Test texture Loader");
    SetTargetFPS(60);
    int c = 0;

    TextureRenderer* textureLoader = new TextureRenderer("../../assets/imgs/characters.gif");

    while(++c < 100) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        textureLoader->renderTexture(MARIO_STAND, (Rectangle) {250.f / 2, 250.f / 2, 16, 16});

        DrawText("this IS a texture loaded from an image!", 300, 370, 10, GRAY);

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

        DrawText("this IS a texture loaded from an image!", 300, 370, 10, GRAY);

        EndDrawing();
    }

    delete textureLoader;

    CloseWindow();

    EXPECT_TRUE(true);
}