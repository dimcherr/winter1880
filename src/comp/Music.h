#pragma once
#include "tun/builder.h"
#include "tun/sound.h"

namespace comp {

struct Music {
    ::Music music {};
    float volume {0.5f};
};

}

template <>
struct CompBuilder<comp::Music> : public BaseCompBuilder<comp::Music> {
    CompBuilder& path(StringView path) {
        comp->music = sound::LoadMusic(path);
        sound::PlayMusic(comp->music, 20.f);
        sound::PauseMusic(comp->music);
        return *this;
    }

    CompBuilder& volume(float volume) {
        comp->volume = volume;
        return *this;
    }
};

template <>
struct std::formatter<comp::Music> : std::formatter<std::string> {
    auto format(comp::Music music, format_context& ctx) const {
        return formatter<string>::format(std::format("[Music]"), ctx);
    }
};