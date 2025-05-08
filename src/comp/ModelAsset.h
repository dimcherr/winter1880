#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"
#include "tun/list.h"

namespace comp {

struct ModelAsset {
    String name {};
};

}

template <>
struct CompBuilder<comp::ModelAsset> : public BaseCompBuilder<comp::ModelAsset> {
    CompBuilder& name(StringView name) {
        comp->name = name;
        return *this;
    }
};

template <>
struct std::formatter<comp::ModelAsset> : std::formatter<std::string> {
    auto format(comp::ModelAsset model, format_context& ctx) const {
        return formatter<string>::format(std::format("[ModelAsset name={}]", model.name), ctx);
    }
};