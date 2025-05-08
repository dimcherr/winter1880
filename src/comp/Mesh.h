#pragma once
#include "tun/log.h"
#include "tun/builder.h"

namespace comp {

struct Mesh {
    Entity asset {};
    Entity model {};
};

}

template <>
struct CompBuilder<comp::Mesh> : public BaseCompBuilder<comp::Mesh> {
    CompBuilder& asset(Entity asset) {
        comp->asset = asset;
        return *this;
    }

    CompBuilder& model(Entity model) {
        comp->model = model;
        return *this;
    }
};

template <>
struct std::formatter<comp::Mesh> : std::formatter<std::string> {
    auto format(comp::Mesh mesh, format_context& ctx) const {
        return formatter<string>::format(std::format("[Mesh asset={} model={}]", mesh.asset, mesh.model), ctx);
    }
};