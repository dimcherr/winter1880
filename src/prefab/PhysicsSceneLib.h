#pragma once
#include "tun/entity.h"
#include "tun/math.h"
#include "tun/builder.h"

namespace prefab {

void StaticBody(Entity entity, Entity modelAsset);
void PickableBody(Entity entity, Entity modelAsset);
void Door(Entity entity, Entity modelAsset, float rotation);
void Collision(Entity entity, Entity modelAsset);
void SlidingManhole(Entity entity, Entity modelAsset);
void Gear(Entity entity, Entity modelAsset, float rotationDelta);
void GearSlot(Entity entity, Entity modelAsset, float rotationDelta, int index);
void GearPickable(Entity entity, Entity modelAsset);
void MainGate(Entity entity, Entity modelAsset, float rotation);
void Instruction(Entity entity, Entity modelAsset, Lang lang);
void LeverBase(Entity entity, Entity modelAsset, int leverType);
void LeverStick(Entity entity, Entity modelAsset, int leverType);

}