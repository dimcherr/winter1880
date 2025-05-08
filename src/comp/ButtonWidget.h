#pragma once
#include "tun/builder.h"
#include "tun/tunstring.h"
#include "tun/math.h"

namespace comp {

struct ButtonWidget {
    String text {};
    Entity font {};
    Color bgColor {};
    Color textColor {};
};

}

template <>
struct CompBuilder<comp::ButtonWidget> : public BaseCompBuilder<comp::ButtonWidget> {
    CompBuilder& text(StringView text) {
        comp->text = text;
        return *this;
    }

    CompBuilder& font(Entity font) {
        comp->font = font;
        return *this;
    }

    CompBuilder& bgColor(Color bgColor) {
        comp->bgColor = bgColor;
        return *this;
    }

    CompBuilder& textColor(Color textColor) {
        comp->textColor = textColor;
        return *this;
    }
};

template <>
struct std::formatter<comp::ButtonWidget> : std::formatter<std::string> {
    auto format(comp::ButtonWidget widget, format_context& ctx) const {
        return formatter<string>::format(std::format("[ButtonWidget]"), ctx);
    }
};