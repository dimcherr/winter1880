#pragma once
#include "tun/tunstring.h"

enum class Lang : int {
    eng = 0,
    rus = 1,
};

struct LangString {
    String variants[2] {};
    operator StringView() const;
};

class LangStrings {
    LangString play {"I am ready", "Я готов"};
};
