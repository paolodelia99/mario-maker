//
// Created by paolo on 07/01/21.
//

#ifndef MARIO_MAKER_GAME_H
#define MARIO_MAKER_GAME_H
#include <raylib.h>

class Game {
public:
    Game(char* mapName);

    void mainLoop();

    virtual ~Game();

private:
    char* mapName;
    bool run;
    bool pause;
};


#endif //MARIO_MAKER_GAME_H
