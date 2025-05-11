#include "prefab/PhysicsSceneLib.h"
#include "comp/GearComp.h"
#include "Tags.h"

void prefab::Furnace(Entity entity, Entity modelAsset) {
    prefab::StaticBody(entity, modelAsset);
    hub::AddTag<tag::Furnace>(entity);
}
