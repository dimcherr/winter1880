#pragma once
#include "tun/builder.h"
#include "tun/tunstring.h"
#include "tun/math.h"

namespace comp {

struct TextWidget {
    String text {};
    //;LangString text {};
    Entity font {};
    Color color {};
};

}

template <>
struct CompBuilder<comp::TextWidget> : public BaseCompBuilder<comp::TextWidget> {
    CompBuilder& text(StringView text) {
        comp->text = text;
        return *this;
    }

    CompBuilder& font(Entity font) {
        comp->font = font;
        return *this;
    }

    CompBuilder& color(Color color) {
        comp->color = color;
        return *this;
    }
};

template <>
struct std::formatter<comp::TextWidget> : std::formatter<std::string> {
    auto format(comp::TextWidget widget, format_context& ctx) const {
        return formatter<string>::format(std::format("[TextWidget]"), ctx);
    }
};