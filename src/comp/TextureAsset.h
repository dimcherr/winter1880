#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"
#include "tun/list.h"

namespace comp {

struct TextureAsset {
    String name {};
    gl::Image image {};
};

}

template <>
struct CompBuilder<comp::TextureAsset> : public BaseCompBuilder<comp::TextureAsset> {
    CompBuilder& name(StringView name) {
        comp->name = name;
        return *this;
    }

    CompBuilder& image(gl::Image image) {
        comp->image = image;
        return *this;
    }
};

template <>
struct std::formatter<comp::TextureAsset> : std::formatter<std::string> {
    auto format(comp::TextureAsset texture, format_context& ctx) const {
        return formatter<string>::format(std::format("[TextureAsset name={}]", texture.name), ctx);
    }
};