#pragma once
#include "tun/math.h"
#include "tun/list.h"

struct TransformComp {
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

    void SetParent(Entity entity, Entity parent) {
        this->parent = parent;
        if (auto* parentTransform = hub::Reg().try_get<TransformComp>(parent)) {
            parentTransform->children.push_back(entity);
        }
    }

    void SetRotationDegrees(float pitch, float yaw, float roll) {
        rotation = Quat(Vec(glm::radians(pitch), glm::radians(yaw), glm::radians(roll)));
    }
};
