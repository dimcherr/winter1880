#pragma once
#include "tun/builder.h"
#include "tun/tunstring.h"
#include "tun/math.h"

namespace comp {

struct BoundsWidget {
    Vec2 pos {};
    Vec2 size {128.f, 128.f};
    Vec2 minSize {128.f, 128.f};
    Vec2 padding {32.f, 32.f};
    tun::Anchors anchors {};
    tun::Anchors parentAnchors {};
    tun::Anchors textAnchors {};
    void(*onClick)(Entity);
    float timeSinceClick {1.f};
    float timeToActualClick {0.2f};
    bool clickPending {false};
    bool visible {false};
    Vec4 color {0.f, 0.f, 0.f, 0.f};
};

}

template <>
struct CompBuilder<comp::BoundsWidget> : public BaseCompBuilder<comp::BoundsWidget> {
    CompBuilder& padding(Vec2 padding) {
        comp->padding = padding;
        return *this;
    }

    CompBuilder& color(Color color, float opacity = 1.f) {
        comp->color = {color.r, color.g, color.b, opacity};
        return *this;
    }

    CompBuilder& visible(bool visible) {
        comp->visible = visible;
        return *this;
    }

    CompBuilder& pos(Vec2 pos) {
        comp->pos = pos;
        return *this;
    }

    CompBuilder& size(Vec2 size) {
        comp->size = size;
        return *this;
    }

    CompBuilder& minSize(Vec2 minSize) {
        comp->minSize = minSize;
        return *this;
    }

    CompBuilder& anchors(tun::Anchors anchors) {
        comp->anchors = anchors;
        return *this;
    }

    CompBuilder& parentAnchors(tun::Anchors parentAnchors) {
        comp->parentAnchors = parentAnchors;
        return *this;
    }

    CompBuilder& textAnchors(tun::Anchors textAnchors) {
        comp->textAnchors = textAnchors;
        return *this;
    }

    CompBuilder& onClick(void(*onClick)(Entity)) {
        comp->onClick = onClick;
        return *this;
    }
};