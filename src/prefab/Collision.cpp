#include "prefab/PhysicsSceneLib.h"
#include "comp/Model.h"

void prefab::Collision(Entity entity, Entity modelAsset) {
    prefab::StaticBody(entity, modelAsset);
    hub::Modify(entity)
        .Modify<comp::Model>().visible(false).Next();
}
