#include "prefab/PhysicsSceneLib.h"
#include "comp/GearSlotComp.h"

void prefab::GearSlot(Entity entity, Entity modelAsset, float rotationDelta) {
    prefab::StaticBody(entity, modelAsset);
    GearSlotComp& gearSlot = hub::AddComp<GearSlotComp>(entity);
    gearSlot.rotationSpeed = rotationDelta;
}
