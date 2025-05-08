#include "prefab/UIPrefabLib.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "comp/SliderWidget.h"

Entity prefab::Slider(const LangString& text, float value, SliderType sliderType, int uiSlot) {
    auto& state = State::Get();
    Entity entity = hub::Create()
        .Add<comp::BoundsWidget>().visible(true).color(tun::black, 0.75f).minSize({512.f, 64.f}).pos({0.f, uiSlot * 72.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::SliderWidget>().percent(value).type(sliderType).Next()
        .GetEntity();

    auto& textWidget = hub::AddComp<TextWidgetComp>(entity, {
        .text = text,
        .font = state.secondaryFont,
        .color = tun::white,
    });

    return entity;
}
