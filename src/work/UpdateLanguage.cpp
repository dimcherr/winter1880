#include "Work.h"
#include "tun/builder.h"
#include "comp/TextWidgetComp.h"
#include "comp/SliderWidget.h"

void work::UpdateLanguage() {
    auto& state = State::Get();
    int nextLang = ((int)state.currentLang + 1) % 2;
    state.currentLang = (Lang)nextLang;

    hub::Reg().view<TextWidgetComp>().each([](Entity entity, TextWidgetComp& text) {
        if (hub::Reg().any_of<comp::SliderWidget>(entity)) {
            // do nothing here
        } else {
            if (text.langString) {
                text.text = *text.langString;
            }
        }
    });
}
