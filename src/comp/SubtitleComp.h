#pragma once
#include "Hub.h"
#include "tun/math.h"

struct SubtitleComp {
    float totalDuration {3.5f};
    float speakingDuration {2.f};
    float time {0.f};
    float soundElapsedTime {0.f};
    bool running {false};
    void(*onEnd)(Entity);
    Entity next {entt::null};

    float GetSpeakingPercent() const {
        return running ? (tun::Clamp(time / speakingDuration, 0.f, 1.f)) : 0.f;
    }
};
