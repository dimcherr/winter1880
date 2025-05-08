#pragma once
#include "tun/physics.h"
#include "tun/math.h"

struct BodyComp {
    JPH::BodyID id {JPH::BodyID::cInvalidBodyID};
    JPH::EMotionType motionType {JPH::EMotionType::Static};
    JPH::ObjectLayer layer {phys::Layers::nonMoving};
    Vec velocity {};
};
