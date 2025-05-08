#pragma once
#include "tun/builder.h"

namespace comp {

struct Countdown {
    float countdown {300.f};
    bool running {false};
};

}

template <>
struct CompBuilder<comp::Countdown> : public BaseCompBuilder<comp::Countdown> {
    CompBuilder& countdown(int countdown) {
        comp->countdown = countdown;
        return *this;
    }
};
