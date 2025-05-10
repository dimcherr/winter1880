#include "prefab/PhysicsSceneLib.h"
#include "Tags.h"

void prefab::Instruction(Entity entity, Entity modelAsset, Lang lang) {
    if (lang == Lang::eng) {
        hub::AddTag<tag::InstructionEng>(entity);
    } else if (lang == Lang::rus) {
        hub::AddTag<tag::InstructionRus>(entity);
    }
}
