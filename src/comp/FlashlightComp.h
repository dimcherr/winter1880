#pragma once
#include "tun/math.h"
#include "tun/builder.h"

struct FlashlightComp {
    float enabled {0.f};
    float target {0.f};

    void SetTurnedOn(bool turnedOn) {
        target = turnedOn ? 1.f : 0.f;
    }

    void Update() {
        enabled = tun::Lerp(enabled, target, 10.f * hub::GetDeltaTime());
    }
};
