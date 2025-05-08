#include "prefab/UIPrefabLib.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "Tags.h"

Entity prefab::Button(LangString* text, void(*onClick)(Entity), int uiSlot) {
    auto& state = State::Get();

    Entity entity = hub::Create()
        .Add<comp::BoundsWidget>().color(tun::black, 1.f).minSize({512.f, 64.f}).pos({0.f, uiSlot * 72.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).visible(true).onClick(onClick).Next()
        .GetEntity();

    auto& textWidget = hub::AddComp<TextWidgetComp>(entity, {
        .text = String(*text),
        .langString = text,
        .font = state.regularFont,
        .color = tun::orange,
    });

    return entity;
}
