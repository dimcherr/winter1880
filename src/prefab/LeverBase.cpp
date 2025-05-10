#include "prefab/PhysicsSceneLib.h"
#include "Hub.h"
#include "Tags.h"
#include "comp/LeverBaseComp.h"

void prefab::LeverBase(Entity entity, Entity modelAsset, int leverType) {
    prefab::StaticBody(entity, modelAsset);
    hub::AddComp<LeverBaseComp>(entity);
    if (leverType == 0) {
        hub::AddTag<tag::LeverMainLeft>(entity);
    } else if (leverType == 1) {
        hub::AddTag<tag::LeverMainRight>(entity);
    }
}
