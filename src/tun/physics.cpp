#include "tun/physics.h"

void phys::Init() {
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();
}

void phys::Test() {
    auto& state = phys::State::Get();
	JPH::BodyInterface& bodyInterface = state.physicsSystem.GetBodyInterface();

	// SHAPE
	JPH::BoxShapeSettings floorShapeSettings(JPH::Vec3(100.f, 1.f, 100.f));
	floorShapeSettings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.
	// Create the shape
	JPH::ShapeSettings::ShapeResult floorShapeResult = floorShapeSettings.Create();
	JPH::ShapeRefC floorShape = floorShapeResult.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

	// BODY
	// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
	JPH::BodyCreationSettings floorSettings(floorShape, JPH::RVec3(0.f, -1.f, 0.f), JPH::Quat::sIdentity(), JPH::EMotionType::Static, phys::Layers::nonMoving);
	// Create the actual rigid body
	JPH::Body* floor = bodyInterface.CreateBody(floorSettings); // Note that if we run out of bodies this can return nullptr


	// Add it to the world
	bodyInterface.AddBody(floor->GetID(), JPH::EActivation::DontActivate);


	// SHORTHAND BODY
	// Now create a dynamic body to bounce on the floor
	// Note that this uses the shorthand version of creating and adding a body to the world
	JPH::BodyCreationSettings sphereSettings(new JPH::SphereShape(0.5f), JPH::RVec3(0.f, 2.f, 0.f), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, phys::Layers::moving);
	JPH::BodyID sphereId = bodyInterface.CreateAndAddBody(sphereSettings, JPH::EActivation::Activate);




	// Now you can interact with the dynamic body, in this case we're going to give it a velocity.
	// (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
	bodyInterface.SetLinearVelocity(sphereId, JPH::Vec3(0.f, -5.f, 0.f));

	// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
	const float deltaTime = 1.0f / 60.0f;

	// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
	// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
	// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
	state.physicsSystem.OptimizeBroadPhase();

	// Now we're ready to simulate the body, keep simulating until it goes to sleep
	JPH::uint step = 0;
	while (bodyInterface.IsActive(sphereId))
	{
		// Next step
		++step;

		// Output current position and velocity of the sphere
		JPH::RVec3 position = bodyInterface.GetCenterOfMassPosition(sphereId);
		JPH::Vec3 velocity = bodyInterface.GetLinearVelocity(sphereId);

		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int collisionSteps = 1;

		// Step the world
		state.physicsSystem.Update(deltaTime, collisionSteps, &state.tempAllocator, &state.jobSystem);
	}

	// Remove the sphere from the physics system. Note that the sphere itself keeps all of its state and can be re-added at any time.
	bodyInterface.RemoveBody(sphereId);

	// Destroy the sphere. After this the sphere ID is no longer valid.
	bodyInterface.DestroyBody(sphereId);

	// Remove and destroy the floor
	bodyInterface.RemoveBody(floor->GetID());
	bodyInterface.DestroyBody(floor->GetID());
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
