#pragma once
#include "tun/tunstring.h"
#include "raudio/raudio.h"

namespace sound {

bool Init();
void PlaySound(const Sound& sound, float pan = 0.5f);
void StopSound(const Sound& sound);
void PlayMusic(const Music& music, float t = 0.f);
void ResumeMusic(const Music& music);
void PauseMusic(const Music& music);
void StopMusic(const Music& music);
void UpdateMusic(const Music& music);
bool IsMusicPlaying(const Music& music);
Sound LoadSound(StringView path);
void UnloadSound(const Sound& sound);
Music LoadMusic(StringView path);
void UnloadMusic(const Music& music);

}