#pragma once
#include "tun/builder.h"
#include "tun/math.h"

namespace comp {

struct BoxShape {
    Vec size {tun::vecOne};
    Vec offset {tun::vecZero};
};

}

template <>
struct CompBuilder<comp::BoxShape> : public BaseCompBuilder<comp::BoxShape> {
    CompBuilder& size(Vec size) {
        comp->size = size;
        return *this;
    }

    CompBuilder& offset(Vec offset) {
        comp->offset = offset;
        return *this;
    }
};

template <>
struct std::formatter<comp::BoxShape> : std::formatter<std::string> {
    auto format(comp::BoxShape shape, format_context& ctx) const {
        return formatter<string>::format(std::format("[BoxShape size={}]", shape.size), ctx);
    }
};