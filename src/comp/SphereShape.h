#pragma once
#include "tun/builder.h"

namespace comp {

struct SphereShape {
    float radius {};
};

}

template <>
struct CompBuilder<comp::SphereShape> : public BaseCompBuilder<comp::SphereShape> {
    CompBuilder& radius(float radius) {
        comp->radius = radius;
        return *this;
    }
};

template <>
struct std::formatter<comp::SphereShape> : std::formatter<std::string> {
    auto format(comp::SphereShape shape, format_context& ctx) const {
        return formatter<string>::format(std::format("[SphereShape radius={:.2f}]", shape.radius), ctx);
    }
};