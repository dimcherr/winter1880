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
void GearSlot(Entity entity, Entity modelAsset, float rotationDelta);
void GearPickable(Entity entity, Entity modelAsset);
void MainGate(Entity entity, Entity modelAsset, float rotation);

}