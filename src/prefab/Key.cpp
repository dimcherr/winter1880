#include "prefab/PhysicsSceneLib.h"
#include "comp/Model.h"
#include "comp/BrickWallKey.h"
#include "Tags.h"

void prefab::Key(Entity entity, Entity modelAsset, AccessType accessType) {
    prefab::PickableBody(entity, modelAsset);
    hub::Modify(entity)
        .Modify<comp::Model>().active(false).Next()
        .Add<comp::BrickWallKey>().type(accessType).Next()
        .Tag<tag::Hacker>();
}
