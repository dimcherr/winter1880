#include "prefab/PhysicsSceneLib.h"
#include "comp/SlidingManholeComp.h"
#include "comp/TransformComp.h"

void prefab::SlidingManhole(Entity entity, Entity modelAsset, int index) {
    prefab::StaticBody(entity, modelAsset);
    auto& transform = hub::Reg().get<TransformComp>(entity);
    auto& slidingManhole = hub::AddComp<SlidingManholeComp>(entity);
    slidingManhole.originTranslation = transform.translation;
    if (index == 0) {
        slidingManhole.openDistance = 10.f;
    } else if (index == 1) {
        slidingManhole.openDistance = -10.f;
    }
}
