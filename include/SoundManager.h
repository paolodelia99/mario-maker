//
// Created by paolo on 14/02/21.
//

#ifndef MARIO_MAKER_SOUNDMANAGER_H
#define MARIO_MAKER_SOUNDMANAGER_H
#include <raylib.h>
#include <unordered_map>
#include "SoundId.h"

class SoundManager {
public:

    SoundManager();

    ~SoundManager();

    void playSound(SoundId soundId);

private:
    std::unordered_map<SoundId, Sound> soundMap_;

};


#endif //MARIO_MAKER_SOUNDMANAGER_H
