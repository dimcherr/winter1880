#include "prefab/UIPrefabLib.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "comp/BlackScreenComp.h"
#include "Tags.h"

Entity prefab::BlackScreen() {
    auto& state = State::Get();
    Entity entity = hub::Create()
        .Add<comp::BoundsWidget>().color(tun::black, 1.f).minSize({3000.f, 3000.f}).pos({0.f, 0.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).visible(true).Next()
        .GetEntity();

    auto& blackScreen = hub::AddComp<BlackScreenComp>(entity);
    blackScreen.alpha = 1.f;
    blackScreen.delta = 0.f;
    blackScreen.speed = 1.f;

    return entity;
}
