#include "prefab/UIPrefabLib.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "Tags.h"

Entity prefab::Tooltip() {
    auto& state = State::Get();
    Entity entity = hub::Create()
        .Add<comp::BoundsWidget>().color(tun::black, 0.25f).minSize({512.f, 64.f}).pos({0.f, 256.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Tag<tag::Tooltip>()
        .GetEntity();

    auto& textWidget = hub::AddComp<TextWidgetComp>(entity, {
        .text = LangStrings::tooltip,
        .font = state.regularFont,
        .color = tun::white,
    });

    return entity;
}
