//
// Created by paolo on 14/02/21.
//

#ifndef MARIO_MAKER_SOUNDSYSTEM_H
#define MARIO_MAKER_SOUNDSYSTEM_H
#include "ECS.h"
#include "AudioManager.h"
#include "AudioIds.h"
#include "Events.h"

using namespace ECS;

class SoundSystem :
        public EntitySystem,
        public EventSubscriber<SoundEvent>,
        public EventSubscriber<SetMusicEvent> {
public:
    SoundSystem();

    ~SoundSystem();

    void configure(World *world) override;

    void unconfigure(World *world) override;

    void tick(World *world, ECS::DefaultTickData data) override;

    void receive(World *world, const SoundEvent &event) override;

    void receive(World *world, const SetMusicEvent &event) override;

    Music getCurrentMusic();

private:
    AudioManager* audioManager_;
};


#endif //MARIO_MAKER_SOUNDSYSTEM_H
