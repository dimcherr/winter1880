#include "prefab/PhysicsSceneLib.h"
#include "Tags.h"

void prefab::MainGate(Entity entity, Entity modelAsset, float rotation) {
    prefab::Door(entity, modelAsset, rotation);
    hub::Modify(entity)
        .Tag<tag::MainGate>();
}
