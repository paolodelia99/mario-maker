//
// Created by paolo on 06/01/21.
//

#ifndef MARIO_MAKER_MENU_H
#define MARIO_MAKER_MENU_H
#include <raylib.h>
#include <iostream>
#include <dirent.h>
#include "Game.h"

class Menu {
public:
    Menu(const char* mapFilepath, int width, int height);

    virtual ~Menu();

    void menuLoop() const;

private:

    void startGame() const;

private:
    int width;
    int height;
    const char* mapFilepath_;
};


#endif //MARIO_MAKER_MENU_H
