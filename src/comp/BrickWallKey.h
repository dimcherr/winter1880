#pragma once
#include "tun/builder.h"
#include "comp/BrickWall.h"

namespace comp {

struct BrickWallKey {
    AccessType type {AccessType::blue};
};

}

template <>
struct CompBuilder<comp::BrickWallKey> : public BaseCompBuilder<comp::BrickWallKey> {
    CompBuilder& type(AccessType type) {
        comp->type = type;
        return *this;
    }
};
