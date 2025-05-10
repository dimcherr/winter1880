#pragma once
#include "tun/builder.h"

struct SlidingManholeComp {
    float closeDistance {0.f};
    float openDistance {3.f};
    float state {0.f}; // 0 close 1 open
    float delta {0.f}; // 1 opening -1 closing
    float speed {0.2f};
    Vec originTranslation {tun::vecZero};
};
