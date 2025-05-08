#pragma once
#include "Jolt/Jolt.h"
#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/Factory.h"
#include "Jolt/Core/TempAllocator.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Physics/PhysicsSettings.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"
#include "Jolt/Physics/Character/CharacterVirtual.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Body/BodyActivationListener.h"
#include "Jolt/Physics/Collision/RayCast.h"
#include "Jolt/Physics/Collision/CastResult.h"
#include <iostream>
#include <cstdarg>
#include <thread>
#include "tun/log.h"

namespace phys {

// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers {
	static constexpr JPH::ObjectLayer nonMoving = 0;
	static constexpr JPH::ObjectLayer moving = 1;
	static constexpr JPH::ObjectLayer pickable = 2;
	static constexpr JPH::ObjectLayer count = 3;
};

/// Class that determines if two object layers can collide
class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override {
		switch (inObject1) {
		case Layers::nonMoving:
			return inObject2 == Layers::moving || inObject2 == Layers::pickable; // Non moving only collides with moving
		case Layers::moving:
			return inObject2 != Layers::pickable; // Moving collides with everything
		case Layers::pickable:
			return inObject2 != Layers::moving; // Moving collides with everything
		default:
			return false;
		}
	}
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers {
	static constexpr JPH::BroadPhaseLayer nonMoving(0);
	static constexpr JPH::BroadPhaseLayer moving(1);
	static constexpr JPH::uint count(2);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
public:
    BPLayerInterfaceImpl() {
		// Create a mapping table from object to broad phase layer
		objectToBroadPhase[Layers::nonMoving] = BroadPhaseLayers::nonMoving;
		objectToBroadPhase[Layers::moving] = BroadPhaseLayers::moving;
	}

	virtual JPH::uint GetNumBroadPhaseLayers() const override {
		return BroadPhaseLayers::count;
	}

	virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
		JPH_ASSERT(inLayer < Layers::count);
		return objectToBroadPhase[inLayer];
	}

private:
	JPH::BroadPhaseLayer objectToBroadPhase[Layers::count];
};

class NonPickableObjectFilter : public JPH::ObjectLayerFilter {
	bool ShouldCollide(JPH::ObjectLayer layer) const override {
		return layer != phys::Layers::pickable;
	}
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override {
		switch (inLayer1) {
		case Layers::nonMoving:
			return inLayer2 == BroadPhaseLayers::moving;
		case Layers::moving:
			return true;
		case Layers::pickable:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

// An example contact listener
class MyContactListener : public JPH::ContactListener {
public:
	// See: ContactListener
	virtual JPH::ValidateResult OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) override {
        //tun::log("Contact validate callback");
		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	virtual void OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override {
        //tun::log("A contact was added");
	}

	virtual void OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override {
        //tun::log("A contact was persisted");
	}

	virtual void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override {
        //tun::log("A contact was removed");
	}
};

// An example activation listener
class MyBodyActivationListener : public JPH::BodyActivationListener {
public:
	virtual void OnBodyActivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override {
        //tun::log("A body got activated");
	}

	virtual void OnBodyDeactivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override {
        //tun::log("A body went to sleep");
	}
};

class MyCastRayCollector : public JPH::CastRayCollector {
	void AddHit(const ResultType &inResult) override {
	} 
};

class State {
public:
	JPH::TempAllocatorImpl tempAllocator {10 * 1024 * 1024};
	JPH::JobSystemThreadPool jobSystem = JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
	const JPH::uint maxBodies = 65536;
	const JPH::uint numBodyMutexes = 0;
	const JPH::uint maxBodyPairs = 65536;
	const JPH::uint maxContactConstraints = 10240;
	BPLayerInterfaceImpl broadPhaseLayerInterface {};
	ObjectVsBroadPhaseLayerFilterImpl objectVsBroadphaseLayerFilter {};
	ObjectLayerPairFilterImpl objectVsObjectLayerFilter {};
	JPH::PhysicsSystem physicsSystem {};
	MyBodyActivationListener bodyActivationListener {};
	MyContactListener contactListener;
	JPH::BroadPhaseLayerFilter characterBroadPhaseLayerFilter {};
	JPH::ObjectLayerFilter characterObjectLayerFilter {};
	NonPickableObjectFilter nonPickableObjectFilter {};
	JPH::BodyFilter characterBodyFilter {};
	JPH::ShapeFilter characterShapeFilter {};

    static State& Get() {
        static State state;
        return state;
    }

    State(const State&) = delete;
    State& operator=(const State&) = delete;

private:
    State();
};

void Init();
void Test();

}