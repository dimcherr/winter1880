#pragma once
#include "tun/builder.h"

struct HackerKeyPair {
    Key key {};
    String symbol {};
};

namespace comp {

struct HackerKey {
    Key key {Key::e};
    String symbol {"e"};
    float timeSincePressed {0.2f};
    float period {0.2f};
    float animIntensity {20.f};
    void(*onPress)(Key);
};

}

template <>
struct CompBuilder<comp::HackerKey> : public BaseCompBuilder<comp::HackerKey> {
    CompBuilder& symbol(StringView symbol, Key key) {
        comp->symbol = symbol;
        comp->key = key;
        return *this;
    }

    CompBuilder& period(float period) {
        comp->period = period;
        comp->timeSincePressed = period;
        return *this;
    }

    CompBuilder& animIntensity(float intensity) {
        comp->animIntensity = intensity;
        return *this;
    }

    CompBuilder& onPress(void(*onPress)(Key)) {
        comp->onPress = onPress;
        return *this;
    }
};
