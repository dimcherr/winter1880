#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"

namespace comp {

struct MaterialGrid {
    Color color {tun::black};
    float segmentSize {50.f};
    int segmentCount {10};
};

}

template <>
struct CompBuilder<comp::MaterialGrid> : public BaseCompBuilder<comp::MaterialGrid> {
    CompBuilder& color(Color color) {
        comp->color = color;
        return *this;
    }

    CompBuilder& segmentSize(float segmentSize) {
        comp->segmentSize = segmentSize;
        return *this;
    }

    CompBuilder& segmentCount(int segmentCount) {
        comp->segmentCount = segmentCount;
        return *this;
    }
};

template <>
struct std::formatter<comp::MaterialGrid> : std::formatter<std::string> {
    auto format(comp::MaterialGrid material, format_context& ctx) const {
        return formatter<string>::format(std::format("[GridMaterial size={}, count={}, color={}]", material.segmentSize, material.segmentCount, material.color), ctx);
    }
};