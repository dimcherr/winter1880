#pragma once
#include "tun/builder.h"
#include "tun/sound.h"
#include "tun/tunrandom.h"

namespace comp {

struct Sound {
    ::Sound sound {};
    List<::Sound> foleys {};
    float elapsedTime {};
    float period {};
    float minPeriod {0.3f};
    float maxPeriod {0.35f};
    float volume {1.f};

    void Play(float pan = 0.5f) {
        if (foleys.size() > 0) {
            int i = tun::GetRandomInt(0, foleys.size() - 1);
            sound::PlaySound(foleys[i], pan);
        } else {
            sound::PlaySound(sound, pan);
        }
        elapsedTime = 0.f;
        period = tun::GetRandomFloat(minPeriod, maxPeriod);
    }
};

}

template <>
struct CompBuilder<comp::Sound> : public BaseCompBuilder<comp::Sound> {
    CompBuilder& path(StringView path) {
        comp->sound = sound::LoadSound(path);
        return *this;
    }

    CompBuilder& foleys(StringView basePath, int count) {
        for (int i = 0; i < count; ++i) {
            comp->foleys.push_back(sound::LoadSound(String(basePath) + std::to_string(i) + ".ogg"));
        }
        return *this;
    }

    CompBuilder& period(float min, float max) {
        comp->minPeriod = min;
        comp->maxPeriod = max;
        comp->period = min;
        return *this;
    }
};

template <>
struct std::formatter<comp::Sound> : std::formatter<std::string> {
    auto format(comp::Sound sound, format_context& ctx) const {
        return formatter<string>::format(std::format("[Sound]"), ctx);
    }
};