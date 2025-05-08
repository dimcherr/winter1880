#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"
#include "tun/list.h"

namespace comp {

struct Model {
    Entity modelAsset {};
    Color tint {tun::white};
    Color highlight {tun::black};
    bool active {true};
    bool visible {true};
};

}

template <>
struct CompBuilder<comp::Model> : public BaseCompBuilder<comp::Model> {
    CompBuilder& modelAsset(Entity asset) {
        comp->modelAsset = asset;
        return *this;
    }

    CompBuilder& active(bool active) {
        comp->active = active;
        return *this;
    }

    CompBuilder& visible(bool visible) {
        comp->visible = visible;
        return *this;
    }
};

template <>
struct std::formatter<comp::Model> : std::formatter<std::string> {
    auto format(comp::Model model, format_context& ctx) const {
        return formatter<string>::format(std::format("[Model asset={}]", model.modelAsset), ctx);
    }
};