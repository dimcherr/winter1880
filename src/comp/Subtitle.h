#pragma once
#include "tun/builder.h"
#include "tun/sound.h"
#include "tun/tunrandom.h"

namespace comp {

struct Subtitle {
    float startTime {1.f};
};

}

template <>
struct CompBuilder<comp::Subtitle> : public BaseCompBuilder<comp::Subtitle> {
    CompBuilder& time(float startTime) {
        comp->startTime = startTime;
        return *this;
    }
};
