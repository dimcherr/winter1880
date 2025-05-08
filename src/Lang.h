#pragma once
#include "tun/tunstring.h"

enum class Lang : int {
    eng = 0,
    rus = 1,
};

struct LangString {
    String variants[2] {};
    String Get() const;
    operator String() const;
};

namespace LangStrings {
    static LangString play {"I am ready", "Я готов"};
    static LangString replay {"Again", "Ещё раз"};
    static LangString tooltip {"Tooltip", "Подсказка"};
    static LangString mouseSense {"Mouse sensitivity", "Чувствительность мыши"};
    static LangString soundVolume {"Sound Volume", "Громкость звуков"};
    static LangString musicVolume {"Music Volume", "Громкость музыки"};
    static LangString testSubtitle0 {"Subtitle one", "Субтитр один"};
    static LangString testSubtitle1 {"Subtitle two", "Субтитр два"};
};
