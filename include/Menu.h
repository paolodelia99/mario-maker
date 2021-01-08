//
// Created by paolo on 06/01/21.
//

#ifndef MARIO_MAKER_MENU_H
#define MARIO_MAKER_MENU_H
#include <raylib.h>
#include "Game.h"

class Menu {
public:
    Menu(int width, int height);

    virtual ~Menu();

    void menuLoop() const;

    void startGame();

private:
    int width;
    int height;

    void startGame() const;
};


#endif //MARIO_MAKER_MENU_H
