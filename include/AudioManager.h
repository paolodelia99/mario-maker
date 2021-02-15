//
// Created by paolo on 14/02/21.
//

#ifndef MARIO_MAKER_AUDIOMANAGER_H
#define MARIO_MAKER_AUDIOMANAGER_H
#include <raylib.h>
#include <unordered_map>
#include "AudioIds.h"

class AudioManager {
public:

    AudioManager();

    ~AudioManager();

    void playSound(SoundId soundId);

    void playMusic(MusicId musicId);

    void pauseCurrentMusic();

    void resumeCurrentMusic();

    void stopCurrentMusic();

    Music getCurrentMusic();

    bool isMusicPaused();

private:
    std::unordered_map<SoundId, Sound> soundMap_;
    std::unordered_map<MusicId, Music> musicMap_;
    Music currentMusic_;
    bool pause = true;
};


#endif //MARIO_MAKER_AUDIOMANAGER_H
