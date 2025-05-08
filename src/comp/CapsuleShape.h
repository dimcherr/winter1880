#pragma once
#include "tun/builder.h"
#include "tun/math.h"

namespace comp {

struct CapsuleShape {
    float halfHeight {};
    float radius {};
};

}

template <>
struct CompBuilder<comp::CapsuleShape> : public BaseCompBuilder<comp::CapsuleShape> {
    CompBuilder& halfHeight(float halfHeight) {
        comp->halfHeight = halfHeight;
        return *this;
    }

    CompBuilder& radius(float radius) {
        comp->radius = radius;
        return *this;
    }
};

template <>
struct std::formatter<comp::CapsuleShape> : std::formatter<std::string> {
    auto format(comp::CapsuleShape shape, format_context& ctx) const {
        return formatter<string>::format(std::format("[CapsuleShape halfHeight={} radius={}]", shape.halfHeight, shape.radius), ctx);
    }
};