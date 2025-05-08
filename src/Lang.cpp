#include "Lang.h"
#include "State.h"

LangString::operator StringView() const {
    return variants[(int)State::Get().currentLang];
}
