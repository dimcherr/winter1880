#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/list.h"

namespace comp {

struct Transform {
    Matrix transform {1.f};
    Vec translation {};
    Vec scale {tun::vecOne};
    Quat rotation {tun::quatIdentity};
    Quat originRotation {tun::quatIdentity};
    Entity parent {entt::null};
    List<Entity> children {};

    void Update() {
        Matrix t = glm::translate({1.f}, translation);
        Matrix r = glm::mat4_cast(rotation);
        Matrix s = glm::scale({1.f}, scale);
        transform = t * r * s;
    }
};

}

template <>
struct CompBuilder<comp::Transform> : public BaseCompBuilder<comp::Transform> {
    CompBuilder& pos(Vec position) {
        comp->translation = position;
        return *this;
    }

    CompBuilder& rot(float pitch, float yaw, float roll) {
        comp->rotation = Quat(Vec(glm::radians(pitch), glm::radians(yaw), glm::radians(roll)));
        return *this;
    }

    CompBuilder& rot(Quat quat) {
        comp->rotation = quat;
        comp->originRotation = quat;
        return *this;
    }

    CompBuilder& scale(float scale) {
        comp->scale = Vec(scale);
        return *this;
    }

    CompBuilder& scale(Vec scale) {
        comp->scale = scale;
        return *this;
    }

    CompBuilder& update() {
        comp->Update();
        return *this;
    }

    CompBuilder& parent(Entity parent) {
        comp->parent = parent;
        if (auto* parentTransform = hub::Reg().try_get<comp::Transform>(parent)) {
            parentTransform->children.push_back(entity);
        }
        return *this;
    }
};

template <>
struct std::formatter<comp::Transform> : std::formatter<std::string> {
    auto format(comp::Transform transform, format_context& ctx) const {
        return formatter<string>::format(std::format("[Transform T={}, R={}, S={} parent={}]", transform.translation, transform.rotation, transform.scale, transform.parent), ctx);
    }
};