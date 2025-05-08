#include "prefab/UIPrefabLib.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "comp/Subtitle.h"
#include "Tags.h"

Entity prefab::Subtitle(LangString* text, float time) {
    auto& state = State::Get();
    Entity entity = hub::Create()
        .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
        .Add<comp::Subtitle>().time(time).Next()
        .GetEntity();

    auto& textWidget = hub::AddComp<TextWidgetComp>(entity, {
        .text = String(*text),
        .langString = text,
        .font = state.secondaryFont,
        .color = tun::white,
    });

    return entity;
}
