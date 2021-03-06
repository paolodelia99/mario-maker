//
// Created by paolo on 06/01/21.
//
#pragma once

// remove the comment if you want to display the collision boxes
// #define DEBUG

constexpr const char* SBM1_PLAYER_TILESET_PATH = "../assets/tilesets/players-tileset.png";
constexpr const char* SMB1_TILESET_PATH = "../assets/tilesets/tiles-2.png";
constexpr const char* SMB1_ENEMIES_TILESET_PATH = "../assets/tilesets/enemies-tileset.png";
constexpr const char* SMB1_OBJECT_TILESET_PATH = "../assets/tilesets/objects-tileset.png";

constexpr const char* FONT_FILE_NAME = "../assets/fonts/SuperMario256.ttf";

constexpr bool SECOND_PLAYER = false;

constexpr int GAME_TILE_SIZE = 32;
constexpr int TILE_SIZE = 16;
constexpr int TILE_ROUNDNESS = 4;

constexpr int FPS = 60;
constexpr float MS_PER_UPDATE = 0.01f;

constexpr float GRAVITY = .15f;
constexpr float FRICTION = .94;
constexpr float MAX_SPEED_X = 2.3;
constexpr float MAX_SPEED_Y = 2.75;

constexpr float MARIO_ACCELERATION_X = .08f;
constexpr float MARIO_JUMP_ACCELERATION = 7.0f;
constexpr float MARIO_BOUNCE = 10.0f;

constexpr float MUSHROOM_GROW_SPEED = .25f;
constexpr float MUSHROOM_MOVE_SPEED = 0.8f;
