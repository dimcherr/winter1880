#pragma once
#include "tun/builder.h"
#include "tun/tunstring.h"
#include "tun/math.h"

namespace comp {

struct HintLabel {
    Entity brickWallEntity {entt::null};
};

}

template <>
struct CompBuilder<comp::HintLabel> : public BaseCompBuilder<comp::HintLabel> {

};
