#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"
#include "tun/list.h"

namespace comp {

struct Lamp {
    Color color {tun::white};
};

}

template <>
struct CompBuilder<comp::Lamp> : public BaseCompBuilder<comp::Lamp> {
    CompBuilder& color(Color color) {
        comp->color = color;
        return *this;
    }
};
