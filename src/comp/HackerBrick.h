#pragma once
#include "tun/builder.h"

namespace comp {

struct HackerBrick {
    int x {0};
    int y {0};
};

}

template <>
struct CompBuilder<comp::HackerBrick> : public BaseCompBuilder<comp::HackerBrick> {
    CompBuilder& pos(int x, int y) {
        comp->x = x;
        comp->y = y;
        return *this;
    }
};
