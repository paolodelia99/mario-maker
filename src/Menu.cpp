//
// Created by paolo on 06/01/21.
//

#include <iostream>
#include "../include/Menu.h"

Menu::Menu(int width, int height)
:width(width), height(height)
{
    InitWindow(width, height, "Mario Maker");
    SetTargetFPS(60);
}

Menu::~Menu() {

}

void Menu::menuLoop() const {
    const char* fontFileName = "../assets/fonts/SuperMario256.ttf";
    const char* menuItems[] = {"Play Game", "Options", "Build Map"};
    Vector2 menuItemsPositions[] = {
            (Vector2){200.f, 200.0f},
            (Vector2){200.f, 260.0f},
            (Vector2){200.f, 320.0f}
    };
    Font titleFont = LoadFontEx(fontFileName, 40, 0, 250);
    Font textFont = LoadFontEx(fontFileName, 32, 0, 250);
    Image coin = LoadImage("../assets/imgs/Gold Coin.png");
    ImageResize(&coin, 35, 35);
    Texture2D coinTexture = LoadTextureFromImage(coin);
    int c = 0;

    UnloadImage(coin);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyReleased(KEY_UP)) c = c - 1 >= 0 ? (c - 1) % 3 : 2;
        if (IsKeyReleased(KEY_DOWN)) c = (c + 1) % 3;
        if (IsKeyReleased(KEY_ENTER)) {
            switch (c) {
                case 0:
                    startGame();
                    break;
                case 1:
                    break;
                case 2:
                    break;
            }
        };

        BeginDrawing();

        ClearBackground({0, 0, 0});

        DrawTextEx(titleFont, "Mario Maker", (Vector2){200.f, 100.0f}, titleFont.baseSize, 2, RED);
        for (int i = 0; i < 3; i++) {
            DrawTextEx(textFont, menuItems[i], menuItemsPositions[i], titleFont.baseSize, 2, WHITE);
        }

        DrawTexture(coinTexture, 150.0f, menuItemsPositions[c].y, WHITE);

        EndDrawing();
    }

    UnloadFont(titleFont);
    UnloadFont(textFont);

    CloseWindow();
}

void Menu::startGame() const {
    const char* mapPath = "../assets/maps/test_world.tmx";
    Game game(mapPath, width, height, true);

    game.mainLoop();
}
