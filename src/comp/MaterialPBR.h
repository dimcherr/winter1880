#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"
#include "tun/list.h"

namespace comp {

struct MaterialPBR {
    String name {};
    Color tint {tun::white};
    Entity baseColorTexture {};
    Entity normalTexture {};
    Entity ormTexture {};
    Entity emissiveTexture {entt::null};
    float metallicFactor {0.5f};
    float roughnessFactor {0.5f};
    float normalScale {1.f};
    float emissiveFactor {1.f};
};

}

template <>
struct CompBuilder<comp::MaterialPBR> : public BaseCompBuilder<comp::MaterialPBR> {
    CompBuilder& name(StringView name) {
        comp->name = name;
        return *this;
    }

    CompBuilder& tint(Color tint) {
        comp->tint = tint;
        return *this;
    }

    CompBuilder& baseColorTexture(Entity baseColorTexture) {
        comp->baseColorTexture = baseColorTexture;
        return *this;
    }

    CompBuilder& normalTexture(Entity normalTexture) {
        comp->normalTexture = normalTexture;
        return *this;
    }

    CompBuilder& ormTexture(Entity ormTexture) {
        comp->ormTexture = ormTexture;
        return *this;
    }

    CompBuilder& emissiveTexture(Entity emissiveTexture) {
        comp->emissiveTexture = emissiveTexture;
        return *this;
    }

    CompBuilder& metallicFactor(float metallicFactor) {
        comp->metallicFactor = metallicFactor;
        return *this;
    }

    CompBuilder& emissiveFactor(float emissiveFactor) {
        comp->emissiveFactor = emissiveFactor;
        return *this;
    }

    CompBuilder& roughnessFactor(float roughnessFactor) {
        comp->roughnessFactor = roughnessFactor;
        return *this;
    }

    CompBuilder& normalScale(float normalScale) {
        comp->normalScale = normalScale;
        return *this;
    }
};

template <>
struct std::formatter<comp::MaterialPBR> : std::formatter<std::string> {
    auto format(comp::MaterialPBR material, format_context& ctx) const {
        return formatter<string>::format(
            std::format("[MaterialPBR BC={} N={} ORM={}]",
                material.baseColorTexture, material.normalTexture, material.ormTexture), ctx);
    }
};