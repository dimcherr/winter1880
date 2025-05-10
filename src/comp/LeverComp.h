#pragma once
#include "tun/math.h"

struct LeverComp {
    float rotationSpeed {2.f};
    float delta {0.f};
    Quat baseRotation {tun::quatIdentity};
    float state {0.f};
    float offAngle {0.f};
    float onAngle {-60.f};
};
