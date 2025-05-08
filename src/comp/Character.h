#pragma once
#include "tun/builder.h"
#include "tun/physics.h"

namespace comp {

struct Character {
    float mass {70.f};
    float maxStrength {100.f};
    float maxSlopeAngle {glm::radians(45.f)};
    JPH::CharacterVirtual* character {};
    Vec movementVector {tun::vecZero};
    float speed {300.f};
    float walkSpeed {300.f};
    float runSpeed {600.f};
    float jumpStrength {5.f};
    bool jumping {false};
    Entity pickable {entt::null};
};

}

template <>
struct CompBuilder<comp::Character> : public BaseCompBuilder<comp::Character> {
    CompBuilder& mass(float mass) {
        comp->mass = mass;
        return *this;
    }

    CompBuilder& maxStrength(float maxStrength) {
        comp->maxStrength = maxStrength;
        return *this;
    }

    CompBuilder& maxSlopeAngle(float maxSlopeAngleDegrees) {
        comp->maxSlopeAngle = glm::radians(maxSlopeAngleDegrees);
        return *this;
    }

    CompBuilder& speed(float walkSpeed, float runSpeed) {
        comp->speed = walkSpeed;
        comp->walkSpeed = walkSpeed;
        comp->runSpeed = runSpeed;
        return *this;
    }

    CompBuilder& jumpStrength(float jumpStrength) {
        comp->jumpStrength = jumpStrength;
        return *this;
    }
};

template <>
struct std::formatter<comp::Character> : std::formatter<std::string> {
    auto format(comp::Character character, format_context& ctx) const {
        return formatter<string>::format(std::format("[Character]"), ctx);
    }
};