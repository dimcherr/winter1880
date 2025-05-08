#pragma once
#include "tun/physics.h"
#include "tun/math.h"

struct BodyComp {
    JPH::BodyID id {JPH::BodyID::cInvalidBodyID};
    JPH::EMotionType motionType {JPH::EMotionType::Static};
    JPH::ObjectLayer layer {phys::Layers::nonMoving};
    Vec velocity {};
    #if 0
    static BodyComp Static() {
        return {
            JPH::BodyID(JPH::BodyID::cInvalidBodyID),
            JPH::EMotionType::Static,
            phys::Layers::nonMoving, 
            {}
        };
    }

    static BodyComp Dynamic() {
        return {
            JPH::BodyID(JPH::BodyID::cInvalidBodyID),
            JPH::EMotionType::Dynamic,
            phys::Layers::moving,
            {}
        };
    }

    static BodyComp Pickable() {
        return {
            JPH::BodyID(JPH::BodyID::cInvalidBodyID),
            JPH::EMotionType::Dynamic,
            phys::Layers::pickable,
            {}
        };
    }
    #endif
};
