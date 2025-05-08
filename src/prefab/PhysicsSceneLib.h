#pragma once
#include "tun/entity.h"
#include "tun/math.h"
#include "tun/builder.h"
#include "comp/BrickWall.h"

namespace prefab {

void StaticBody(Entity entity, Entity modelAsset);
void PickableBody(Entity entity, Entity modelAsset);
void Door(Entity entity, Entity modelAsset);
void Lamp(Entity entity, Entity modelAsset);
void Collision(Entity entity, Entity modelAsset);
void Kimchi(Entity entity, Entity modelAsset);
void Key(Entity entity, Entity modelAsset, AccessType accessType);
void Appliance(Entity entity, Entity modelAsset, int floor, StringView applianceName);

}