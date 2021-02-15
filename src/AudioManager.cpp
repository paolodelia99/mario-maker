//
// Created by paolo on 14/02/21.
//
#include "AudioManager.h"

AudioManager::AudioManager() {
    // Sounds
    soundMap_.insert({POWER_UP, LoadSound("../assets/audio/sounds/smb_powerup.wav")});
    soundMap_.insert({POWER_APPEARS, LoadSound("../assets/audio/sounds/smb_powerup_appears.wav")});
    soundMap_.insert({MARIO_DIE, LoadSound("../assets/audio/sounds/smb_mariodie.wav")});
    soundMap_.insert({DOWN_THE_FLAG_POLE, LoadSound("../assets/audio/sounds/smb_flagpole.wav")});
    soundMap_.insert({STOMP, LoadSound("../assets/audio/sounds/smb_stomp.wav")});
    soundMap_.insert({UP_1, LoadSound("../assets/audio/sounds/smb_1-up.wav")});
    soundMap_.insert({COIN, LoadSound("../assets/audio/sounds/smb_coin.wav")});
    soundMap_.insert({STAGE_CLEAR, LoadSound("../assets/audio/sounds/smb_stage_clear.wav")});
    soundMap_.insert({JUMP_SMALL, LoadSound("../assets/audio/sounds/smb_jump-small.wav")});
    soundMap_.insert({JUMP_SUPER, LoadSound("../assets/audio/sounds/smb_jump-super.wav")});
    soundMap_.insert({FIREBALL, LoadSound("../assets/audio/sounds/smb_fireball.wav")});
    soundMap_.insert({BRICK_SMASH, LoadSound("../assets/audio/sounds/smb_breakblock.wav")});
    soundMap_.insert({PIPE_TRAVERSAL, LoadSound("../assets/audio/sounds/smb_pipe.wav")});

    // Music
    musicMap_.insert({SMB1_MAIN_THEME, LoadMusicStream("../assets/audio/music/01 - Super Mario Bros.mp3")});
}

void AudioManager::playSound(SoundId soundId) {
    auto it = soundMap_.find(soundId);
    if (it != soundMap_.end()) PlaySound(it->second);
}

AudioManager::~AudioManager() {
    for (auto soundIt = soundMap_.begin(); soundIt != soundMap_.end(); soundIt++) {
        UnloadSound(soundIt->second);
    }

    for (auto musicIt = musicMap_.begin(); musicIt != musicMap_.end(); musicIt++) {
        UnloadMusicStream(musicIt->second);
    }
}

void AudioManager::playMusic(MusicId musicId) {
    auto it = musicMap_.find(musicId);
    if (it != musicMap_.end()) {
        PlayMusicStream(it->second);
        SetMusicVolume(it->second, 1.0f);
        currentMusic_ = it->second;
        pause = false;
    }
}

void AudioManager::pauseCurrentMusic() {
    if (!pause) {
        PauseMusicStream(currentMusic_);
        pause = true;
    }
}

void AudioManager::resumeCurrentMusic() {
    if (pause) {
        ResumeMusicStream(currentMusic_);
        pause = false;
    }
}

void AudioManager::stopCurrentMusic() {
    StopMusicStream(currentMusic_);
}

Music AudioManager::getCurrentMusic() {
    return currentMusic_;
}

bool AudioManager::isMusicPaused() {
    return pause;
}
