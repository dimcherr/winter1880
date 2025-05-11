#include "prefab/PhysicsSceneLib.h"
#include "Tags.h"

void prefab::FurnaceDoor(Entity entity, Entity modelAsset, float rotation) {
    prefab::Door(entity, modelAsset, rotation);
    hub::Modify(entity)
        .Tag<tag::FurnaceDoor>();
}
