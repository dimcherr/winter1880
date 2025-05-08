#pragma once
#include "tun/builder.h"
#include "tun/tunstring.h"
#include "tun/math.h"
#include "tun/gl.h"

namespace comp {

struct Font {
    ImFont* font {};
};

}

template <>
struct CompBuilder<comp::Font> : public BaseCompBuilder<comp::Font> {
    CompBuilder& path(StringView path, float fontSize) {
        comp->font = gl::LoadFont(path, fontSize);
        return *this;
    }
};

template <>
struct std::formatter<comp::Font> : std::formatter<std::string> {
    auto format(comp::Font font, format_context& ctx) const {
        return formatter<string>::format(std::format("[Font]"), ctx);
    }
};