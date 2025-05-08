#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"

namespace comp {

struct MaterialColor {
    Color color {tun::white};
};

}

template <>
struct CompBuilder<comp::MaterialColor> : public BaseCompBuilder<comp::MaterialColor> {
    CompBuilder& color(Color color) {
        comp->color = color;
        return *this;
    }
};

template <>
struct std::formatter<comp::MaterialColor> : std::formatter<std::string> {
    auto format(comp::MaterialColor material, format_context& ctx) const {
        return formatter<string>::format(std::format("[ColorMaterial color={}]", material.color), ctx);
    }
};