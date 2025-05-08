#include "prefab/PhysicsSceneLib.h"
#include "comp/Model.h"
#include "comp/BoxShape.h"
#include "comp/BodyComp.h"
#include "Tags.h"

void prefab::PickableBody(Entity entity, Entity modelAsset) {
    comp::BoxShape& boxShape = hub::Reg().get<comp::BoxShape>(modelAsset);
    hub::Modify(entity)
        .Add<comp::BoxShape>().size(boxShape.size).offset(boxShape.offset).Next()
        .AddComp<BodyComp>({
            .id = JPH::BodyID(JPH::BodyID::cInvalidBodyID),
            .motionType = JPH::EMotionType::Dynamic,
            .layer = phys::Layers::pickable, 
            .velocity = {},
        })
        .Tag<tag::Pickable>();
}
