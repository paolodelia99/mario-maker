//
// Created by paolo on 06/01/21.
//
#pragma once

constexpr const char* SBM1_PLAYER_TILESET_PATH = "../assets/imgs/players-tileset.png";
constexpr const char* SMB1_TILESET_PATH = "../assets/tilesets/tiles-2.png";

constexpr bool DEBUG = false;
constexpr bool SECOND_PLAYER = true;

constexpr int GAME_TILE_SIZE = 32;
constexpr int TILE_SIZE = 16;
constexpr int TILE_ROUNDNESS = 4;

constexpr float MS_PER_UPDATE = 1.f / 120;

constexpr float GRAVITY = .10f;
constexpr float FRICTION = .94;
constexpr float MAX_SPEED_X = 1.75;
constexpr float MAX_SPEED_Y = 2;

constexpr float MARIO_ACCELERATION_X = .08f;
constexpr float MARIO_JUMP_ACCELERATION = 6.5f;

constexpr float MUSHROOM_GROW_SPEED = .25f;
constexpr float MUSHROOM_MOVE_SPEED = 0.8f;
