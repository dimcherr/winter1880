#pragma once
#include "tun/builder.h"

namespace comp {

struct Healthbar {
    int health = 5;
};

}

template <>
struct CompBuilder<comp::Healthbar> : public BaseCompBuilder<comp::Healthbar> {
    CompBuilder& health(int health) {
        comp->health = health;
        return *this;
    }
};
