#include "prefab/PhysicsSceneLib.h"
#include "comp/GearPickableComp.h"
#include "comp/BodyComp.h"
#include "Tags.h"
#include "tun/physics.h"

void prefab::GearPickable(Entity entity, Entity modelAsset) {
    prefab::PickableBody(entity, modelAsset);
    GearPickableComp& gearPickable = hub::AddComp<GearPickableComp>(entity);
    hub::AddTag<tag::Pickable>(entity);
}
