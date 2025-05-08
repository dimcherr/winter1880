#include "Work.h"
#include "tun/builder.h"
#include "comp/Music.h"
#include "comp/Sound.h"
#include "tun/sound.h"
#include "Tags.h"

void work::UpdateSounds() {
    hub::Reg().view<comp::Music>().each([](comp::Music& music) {
        SetMusicVolume(music.music, glm::clamp(music.volume * State::Get().masterMusicVolume, 0.f, 1.f));
        sound::UpdateMusic(music.music);
    });

    hub::Reg().view<comp::Sound>().each([](comp::Sound& sound) {
        float vol = glm::clamp(sound.volume * State::Get().masterSoundVolume, 0.f, 1.f);
        SetSoundVolume(sound.sound, vol);
        for (auto& s : sound.foleys) {
            vol = glm::clamp(sound.volume * State::Get().masterSoundVolume, 0.f, 1.f);
            SetSoundVolume(s, vol);
        }
        sound.elapsedTime += hub::GetDeltaTime();
    });
}

void work::SetMusicPlaying(bool playing) {
    auto& state = State::Get();
    state.musicPlaying = playing;
    hub::Reg().view<comp::Music, tag::GameMusic>().each([&playing](comp::Music& music) {
        if (playing) {
            sound::ResumeMusic(music.music);
        } else {
            sound::PauseMusic(music.music);
        }
    });

    hub::Reg().view<comp::Music, tag::MenuMusic>().each([&playing](comp::Music& music) {
        if (!playing) {
            sound::ResumeMusic(music.music);
        } else {
            sound::PauseMusic(music.music);
        }
    });
}
