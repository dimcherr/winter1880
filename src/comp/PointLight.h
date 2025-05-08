#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"
#include "tun/list.h"

namespace comp {

struct PointLight {
    String name {};
    Color color {tun::white};
    float intensity {1.f};
    float range {1.f};
    bool initialized {false};
    bool flickering {false};
    float startIntensity {1.f};
};

}

template <>
struct CompBuilder<comp::PointLight> : public BaseCompBuilder<comp::PointLight> {
    CompBuilder& name(StringView name) {
        comp->name = name;
        return *this;
    }

    CompBuilder& color(Color color) {
        comp->color = color;
        return *this;
    }

    CompBuilder& intensity(float intensity) {
        comp->intensity = intensity;
        return *this;
    }

    CompBuilder& range(float range) {
        comp->range = range;
        return *this;
    }
};

template <>
struct std::formatter<comp::PointLight> : std::formatter<std::string> {
    auto format(comp::PointLight light, format_context& ctx) const {
        return formatter<string>::format(std::format("[PointLight name={} color={} intensity={:.2f} range={:.2f}]",
             light.name, light.color, light.intensity, light.range), ctx);
    }
};