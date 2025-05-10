#include "prefab/PhysicsSceneLib.h"
#include "Hub.h"
#include "Tags.h"
#include "comp/LeverComp.h"

void prefab::LeverStick(Entity entity, Entity modelAsset, int leverType) {
    auto& leverStick = hub::AddComp<LeverComp>(entity);
    if (leverType == 0) {
        hub::AddTag<tag::LeverMainLeft>(entity);
        leverStick.onAngle = 60.f;
    } else if (leverType == 1) {
        hub::AddTag<tag::LeverMainRight>(entity);
        leverStick.onAngle = -60.f;
    }
}
