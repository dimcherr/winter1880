#pragma once
#include "tun/builder.h"
#include "tun/tunstring.h"
#include "tun/math.h"

enum class SliderType {
    sound,
    music,
    mouse,
};

namespace comp {

struct SliderWidget {
    float percent {0.5f};
    bool handled {false};
    SliderType type {SliderType::mouse};
};

}

template <>
struct CompBuilder<comp::SliderWidget> : public BaseCompBuilder<comp::SliderWidget> {
    CompBuilder& percent(float percent) {
        comp->percent = percent;
        return *this;
    }

    CompBuilder& type(SliderType type) {
        comp->type = type;
        return *this;
    }
};
