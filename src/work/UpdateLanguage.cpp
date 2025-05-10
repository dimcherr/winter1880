#include "Work.h"
#include "tun/builder.h"
#include "comp/TextWidgetComp.h"
#include "comp/SliderWidget.h"
#include "comp/Model.h"
#include "Tags.h"

void work::UpdateLanguage() {
    auto& state = State::Get();
    int nextLang = ((int)state.currentLang + 1) % 2;
    state.currentLang = (Lang)nextLang;

    hub::Reg().view<tag::InstructionEng, comp::Model>().each([](comp::Model& model) {
        model.visible = State::Get().currentLang == Lang::eng;
    });

    hub::Reg().view<tag::InstructionRus, comp::Model>().each([](comp::Model& model) {
        model.visible = State::Get().currentLang == Lang::rus;
    });

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
