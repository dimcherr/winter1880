#include "tun/physics.h"

void phys::Init() {
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();
}

phys::State::State() {
    physicsSystem.Init(
        maxBodies,
        numBodyMutexes,
        maxBodyPairs,
        maxContactConstraints,
        broadPhaseLayerInterface,
        objectVsBroadphaseLayerFilter,
        objectVsObjectLayerFilter
    );

	physicsSystem.SetBodyActivationListener(&bodyActivationListener);
	physicsSystem.SetContactListener(&contactListener);
}
