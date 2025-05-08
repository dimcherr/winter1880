#include "prefab/PhysicsSceneLib.h"
#include "comp/Door.h"

void prefab::Door(Entity entity, Entity modelAsset) {
    prefab::StaticBody(entity, modelAsset);
    hub::Modify(entity)
        .Add<comp::Door>().angle(0.f, -90.f).Next();
}
