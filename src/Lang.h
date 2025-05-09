#pragma once
#include "tun/tunstring.h"

enum class Lang : int {
    eng = 0,
    rus = 1,
};

struct LangString {
    String variants[2] {};
    StringView Get() const;
    operator StringView() const;
};

namespace LangStrings {
    static LangString play {"I am ready", "Я готов"};
    static LangString replay {"Again", "Ещё раз"};
    static LangString currentLang {"English", "Русский"};
    static LangString tooltip {"Tooltip", "Подсказка"};
    static LangString mouseSense {"Mouse sensitivity", "Чувствительность мыши"};
    static LangString soundVolume {"Sound Volume", "Громкость звуков"};
    static LangString musicVolume {"Music Volume", "Громкость музыки"};
    static LangString testSubtitle0 {"I'm freezing", "Так холодно"};
    static LangString testSubtitle1 {"I feel like I'm gonna die here", "Мне кажется, я умираю"};
};
