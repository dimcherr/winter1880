#include "prefab/UIPrefabLib.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "comp/SubtitleComp.h"
#include "Tags.h"

Entity prefab::Subtitle(LangString* text, float duration) {
    auto& state = State::Get();
    Entity entity = hub::Create()
        .Add<comp::BoundsWidget>().color(tun::black, 0.5f).minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
        .GetEntity();

    auto& subtitle = hub::AddComp<SubtitleComp>(entity);
    subtitle.totalDuration = duration;
    subtitle.speakingDuration = subtitle.totalDuration * 0.2f;

    auto& textWidget = hub::AddComp<TextWidgetComp>(entity, {
        .text = String(*text),
        .langString = text,
        .font = state.secondaryFont,
        .color = tun::white,
    });

    return entity;
}
