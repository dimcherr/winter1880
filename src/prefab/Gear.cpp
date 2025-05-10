#include "prefab/PhysicsSceneLib.h"
#include "comp/GearComp.h"

void prefab::Gear(Entity entity, Entity modelAsset, float rotationDelta) {
    GearComp& gear = hub::AddComp<GearComp>(entity);
    gear.rotationSpeed = rotationDelta;
}
