#include "prefab/PhysicsSceneLib.h"
#include "comp/GearSlotComp.h"
#include "Tags.h"

void prefab::GearSlot(Entity entity, Entity modelAsset, float rotationDelta, int index) {
    prefab::StaticBody(entity, modelAsset);
    GearSlotComp& gearSlot = hub::AddComp<GearSlotComp>(entity);
    gearSlot.rotationSpeed = rotationDelta;
    if (index == 0) {
        hub::AddTag<tag::LeverMainLeft>(entity);
    } else {
        hub::AddTag<tag::LeverMainRight>(entity);
    }
}
