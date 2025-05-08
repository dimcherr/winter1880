#include "prefab/PhysicsSceneLib.h"
#include "Tags.h"

void prefab::Kimchi(Entity entity, Entity modelAsset) {
    prefab::StaticBody(entity, modelAsset);
    hub::Modify(entity)
        .Tag<tag::Kimchi>();
}
