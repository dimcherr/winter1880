#pragma once
#include "tun/math.h"

struct PointLightComp {
    Color color {tun::white};
    float intensity {1.f};
    float range {1.f};
};
