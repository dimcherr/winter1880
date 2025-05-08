#pragma once
#include "tun/builder.h"

namespace comp {

struct Door {
    float closeAngle {0.f};
    float openAngle {90.f};
    float doorState {0.f}; // 0 close 1 open
    float stateDelta {0.f}; // 1 opening -1 closing
    float openingSpeed {2.5f};
};

}

template <>
struct CompBuilder<comp::Door> : public BaseCompBuilder<comp::Door> {
    CompBuilder& angle(float closeAngle, float openAngle) {
        comp->closeAngle = closeAngle;
        comp->openAngle = openAngle;
        return *this;
    }
};
