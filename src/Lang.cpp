#include "Lang.h"
#include "State.h"

LangString::operator String() const {
    return variants[(int)State::Get().currentLang];
}

String LangString::Get() const {
    return variants[(int)State::Get().currentLang];
}
