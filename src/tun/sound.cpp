#include "tun/sound.h"
#include "tun/tunrandom.h"

bool sound::Init() {
    InitAudioDevice();
    return true;
}

void sound::PlaySound(const Sound& sound, float pan) {
    ::SetSoundPitch(sound, tun::GetRandomFloat(0.95f, 1.05f));
    ::SetSoundPan(sound, pan);
    ::PlaySound(sound);
}

void sound::StopSound(const Sound& sound) {
    ::StopSound(sound);
}

void sound::PlayMusic(const Music& music, float t) {
    ::PlayMusicStream(music);
    ::SeekMusicStream(music, t);
}

void sound::ResumeMusic(const Music& music) {
    ::ResumeMusicStream(music);
}

void sound::PauseMusic(const Music& music) {
    ::PauseMusicStream(music);
}

void sound::StopMusic(const Music& music) {
    ::StopMusicStream(music);
}

void sound::UpdateMusic(const Music& music) {
    if (::IsMusicStreamPlaying(music)) {
        ::UpdateMusicStream(music);
    }
}

bool sound::IsMusicPlaying(const Music& music) {
    return ::IsMusicStreamPlaying(music);
}

Sound sound::LoadSound(StringView path) {
    return ::LoadSound(path.data());
}

void sound::UnloadSound(const Sound& sound) {
    ::UnloadSound(sound);
}

Music sound::LoadMusic(StringView path) {
    return ::LoadMusicStream(path.data());
}

void sound::UnloadMusic(const Music& music) {
    ::UnloadMusicStream(music);
}
