//
// Created by paolo on 14/02/21.
//
#include "SoundManager.h"

SoundManager::SoundManager() {
    soundMap_.insert({POWER_UP, LoadSound("../assets/sounds/smb_powerup.wav")});
    soundMap_.insert({POWER_APPEARS, LoadSound("../assets/sounds/smb_powerup_appears.wav")});
    soundMap_.insert({MARIO_DIE, LoadSound("../assets/sounds/smb_mariodie.wav")});
    soundMap_.insert({DOWN_THE_FLAG_POLE, LoadSound("../assets/sounds/smb_flagpole.wav")});
    soundMap_.insert({STOMP, LoadSound("../assets/sounds/smb_stomp.wav")});
    soundMap_.insert({UP_1, LoadSound("../assets/sounds/smb_1-up.wav")});
    soundMap_.insert({COIN, LoadSound("../assets/sounds/smb_coin.wav")});
    soundMap_.insert({STAGE_CLEAR, LoadSound("../assets/sounds/smb_stage_clear.wav")});
    soundMap_.insert({JUMP_SMALL, LoadSound("../assets/sounds/smb_jump-small.wav")});
    soundMap_.insert({JUMP_SUPER, LoadSound("../assets/sounds/smb_jump-super.wav")});
}

void SoundManager::playSound(SoundId soundId) {
    auto it = soundMap_.find(soundId);
    if (it != soundMap_.end()) PlaySound(it->second);
}

SoundManager::~SoundManager() {
    for (auto soundIt = soundMap_.begin(); soundIt != soundMap_.end(); soundIt++) {
        UnloadSound(soundIt->second);
    }
}
