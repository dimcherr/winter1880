#include "prefab/GameplayPrefab.h"
#include "comp/PointLightComp.h"
#include "comp/TransformComp.h"
#include "comp/FlashlightComp.h"

Entity prefab::Flashlight() {
    auto& state = State::Get();
    Entity entity = hub::Create().GetEntity();

    auto& transform = hub::AddComp<TransformComp>(entity);

    auto& pointLight = hub::AddComp<PointLightComp>(entity);
    pointLight.color = tun::white;
    pointLight.intensity = 1.f;
    pointLight.range = 1.f;

    auto& flashlight = hub::AddComp<FlashlightComp>(entity);
    flashlight.SetTurnedOn(true);

    return entity;
}
