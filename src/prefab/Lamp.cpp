#include "prefab/PhysicsSceneLib.h"
#include "comp/Lamp.h"

void prefab::Lamp(Entity entity, Entity modelAsset) {
    prefab::StaticBody(entity, modelAsset);
    hub::Modify(entity)
        .Add<comp::Lamp>().Next();
}
