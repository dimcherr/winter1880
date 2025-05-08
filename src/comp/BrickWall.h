#pragma once
#include "tun/builder.h"

enum class AccessType : int {
    red = 0,
    blue = 1,
    yellow = 2,
    count = 3,
};

struct Access {
    AccessType type {AccessType::blue};
    bool required {false};
};

namespace comp {

struct BrickWall {
    String name {};
    int progress {100};
    float damage {0.f};
    float damagePerSecond {1.f};
    bool destroyed {false};
    int requiredCount {0};
    bool underAttack {false};
    float highlightTime {0.f};
    Access access[3] {
        {AccessType::red, false},
        {AccessType::blue, false},
        {AccessType::yellow, false}
    };
};

}

template <>
struct CompBuilder<comp::BrickWall> : public BaseCompBuilder<comp::BrickWall> {
    CompBuilder& name(StringView name) {
        comp->name = name;
        return *this;
    }

    CompBuilder& progress(int prog) {
        comp->progress = prog;
        return *this;
    }

    CompBuilder& requireAccess(AccessType type) {
        comp->access[(int)type].required = true;
        ++comp->requiredCount;
        comp->highlightTime = 0.f;
        return *this;
    }

    CompBuilder& grantAccess(AccessType type) {
        comp->access[(int)type].required = false;
        --comp->requiredCount;
        if (comp->requiredCount < 0) {
            comp->requiredCount = 0;
        }
        comp->highlightTime = 0.f;
        return *this;
    }
};
