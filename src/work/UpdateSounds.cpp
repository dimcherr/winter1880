#include "Work.h"
#include "tun/builder.h"
#include "comp/Music.h"
#include "comp/Sound.h"
#include "tun/sound.h"
#include "Tags.h"

void work::UpdateSounds() {
    hub::Reg().view<comp::Music>().each([](comp::Music& music) {
        if (music.delta != 0.f) {
            music.currentVolume += music.delta * music.fadeSpeed * hub::GetDeltaTime();
            if (music.currentVolume < 0.f) {
                music.currentVolume = 0.f;
                music.delta = 0.f;
                PauseMusicStream(music.music);
            } else if (music.currentVolume > music.volume) {
                music.currentVolume = music.volume;
                music.delta = 0.f;
            }
        }

        SetMusicVolume(music.music, glm::clamp(music.currentVolume * State::Get().masterMusicVolume, 0.f, 1.f));
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
