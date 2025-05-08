#include "builder.h"

EntityBuilder::EntityBuilder() : entity(hub::AddEntity()) {}

EntityBuilder::EntityBuilder(Entity entity) : entity(entity) {}

Entity EntityBuilder::GetEntity() {
    return entity;
}