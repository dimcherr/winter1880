#include "prefab/PhysicsSceneLib.h"
#include "comp/Model.h"
#include "comp/BoxShape.h"
#include "comp/BodyComp.h"

void prefab::StaticBody(Entity entity, Entity modelAsset) {
    comp::BoxShape& assetBoxShape = hub::GetComp<comp::BoxShape>(modelAsset);

    comp::BoxShape& boxShape = hub::AddComp<comp::BoxShape>(entity);
    boxShape = assetBoxShape;

    BodyComp body = hub::AddComp<BodyComp>(entity);
    body.motionType = JPH::EMotionType::Static;
    body.layer = phys::Layers::nonMoving;
}
