#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/tunrandom.h"
#include "tun/physics.h"
#include "comp/TransformComp.h"
#include "comp/BodyComp.h"
#include "comp/BoxShape.h"
#include "comp/Model.h"
#include "comp/CapsuleShape.h"
#include "comp/Camera.h"
#include "comp/Character.h"
#include "comp/Sound.h"
#include "Tags.h"

void work::UpdatePhysics() {
    using comp::BoxShape;
    using comp::CapsuleShape;
    using comp::Character;
    using comp::Camera;

    auto& physState = phys::State::Get();
    const int collisionSteps = 1;
    physState.physicsSystem.Update(hub::GetDeltaTime(), collisionSteps, &physState.tempAllocator, &physState.jobSystem);


    hub::Reg().view<Character, TransformComp, CapsuleShape, Camera, comp::Sound, tag::Current>().each([](Character& character, TransformComp& transform, CapsuleShape& shape, Camera& camera, comp::Sound& sound) {
        if (!character.character) {
            JPH::CharacterVirtualSettings settings {};
            settings.mShape = new JPH::CapsuleShape(shape.halfHeight, shape.radius);
            settings.mMass = character.mass;
            settings.mMaxSlopeAngle = character.maxSlopeAngle;
            settings.mMaxStrength = character.maxStrength;
            settings.mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), -shape.halfHeight);
            character.character = new JPH::CharacterVirtual(&settings, Convert(transform.translation), Convert(transform.rotation), &phys::State::Get().physicsSystem);
        } else {
            if (!State::Get().firstPerson) return;

            if (State::Get().keys[(int)Key::shift]) {
                sound.minPeriod = 0.45f * 0.7f;
                sound.maxPeriod = 0.45f * 0.7f;
                character.speed = character.runSpeed;
            } else {
                sound.minPeriod = 0.45f;
                sound.maxPeriod = 0.45f;
                character.speed = character.walkSpeed;
            }

            if (character.character->GetLinearVelocity().LengthSq() > 10.f && character.character->GetGroundState() == JPH::CharacterVirtual::EGroundState::OnGround) {
                if (sound.elapsedTime > sound.period) {
                    static int stepSide = 0;
                    float pan = stepSide * 0.3f - 0.15f + 0.5f;
                    stepSide = (stepSide + 1) % 2;
                    sound.Play(pan);
                }
            }

            // update movement
            character.movementVector = tun::vecZero;
            if (hub::IsKeyPressed(Key::w)) {
                character.movementVector += Vec(0.f, 0.f, 1.f);
            }
            if (hub::IsKeyPressed(Key::s)) {
                character.movementVector += Vec(0.f, 0.f, -1.f);
            }
            if (hub::IsKeyPressed(Key::a)) {
                character.movementVector += Vec(-1.f, 0.f, 0.f);
            }
            if (hub::IsKeyPressed(Key::d)) {
                character.movementVector += Vec(1.f, 0.f, 0.f);
            }
            character.jumping = hub::IsKeyPressed(Key::space);
            if (glm::length(character.movementVector) > 0.001f) {
                Quat rot = Quat({0.f, camera.yaw, 0.f});
                character.movementVector = rot * glm::normalize(character.movementVector);
            }

            JPH::CharacterVirtual::ExtendedUpdateSettings updateSettings {};
            auto& state = phys::State::Get();
            auto& physicsSystem = phys::State::Get().physicsSystem;
            JPH::Vec3 movementVel = Convert(character.movementVector * character.speed);
            float deltaTime = hub::GetDeltaTime();
            if (character.character->GetGroundState() == JPH::CharacterVirtual::EGroundState::OnGround) {
                JPH::Vec3 jumpVel = character.jumping ? Convert(character.jumpStrength * Vec(0.f, 1.f, 0.f)) : JPH::Vec3::sZero();
                character.character->SetLinearVelocity((physicsSystem.GetGravity() + movementVel + jumpVel) * deltaTime + character.character->GetGroundVelocity());
            } else {
                JPH::Vec3 verticalVel = JPH::Vec3(0.f, character.character->GetLinearVelocity().GetY(), 0.f);
                character.character->SetLinearVelocity((physicsSystem.GetGravity() + movementVel) * deltaTime + verticalVel);
            }
            character.character->ExtendedUpdate(
                hub::GetDeltaTime(),
                physicsSystem.GetGravity(),
                updateSettings,
                physicsSystem.GetDefaultBroadPhaseLayerFilter(phys::Layers::moving),
                physicsSystem.GetDefaultLayerFilter(phys::Layers::moving),
                state.characterBodyFilter,
                state.characterShapeFilter,
                state.tempAllocator
            );

            transform.translation = Convert(character.character->GetPosition());
            transform.Update();

            // update pickable position
            if (hub::Reg().valid(character.pickable)) {
                auto [pickableTransform, pickableBody] = hub::Reg().get<TransformComp, BodyComp>(character.pickable);
                Vec forwardVector = glm::normalize(transform.rotation * tun::forward) * 1.5f;
                Vec desiredLocation = transform.translation + camera.offset * 0.5f + forwardVector;
                Vec currentBodyLocation = Convert(phys::State::Get().physicsSystem.GetBodyInterface().GetPosition(pickableBody.id));
                phys::State::Get().physicsSystem.GetBodyInterface().SetLinearVelocity(pickableBody.id, Convert(desiredLocation - currentBodyLocation) * 10.f);
                phys::State::Get().physicsSystem.GetBodyInterface().SetAngularVelocity(pickableBody.id, phys::State::Get().physicsSystem.GetBodyInterface().GetAngularVelocity(pickableBody.id) * 0.9f);
                float distance = glm::length(desiredLocation - currentBodyLocation);
                if (distance > 5.f) {
                    character.pickable = entt::null;
                }
            }
        }
    });

    hub::Reg().view<BoxShape, BodyComp, TransformComp>().each([](BoxShape& shape, BodyComp& body, TransformComp& transform) {
        auto& bodyInterface = phys::State::Get().physicsSystem.GetBodyInterface();
        if (body.id.IsInvalid()) {
            Vec shapeSize = shape.size * transform.scale * 0.5f;
            JPH::BodyCreationSettings settings(
                new JPH::RotatedTranslatedShape(Convert(shape.offset), JPH::Quat::sIdentity(), new JPH::BoxShape(Convert(shapeSize))),
                Convert(transform.translation),
                Convert(transform.rotation),
                body.motionType,
                body.layer 
            );
            body.id = bodyInterface.CreateAndAddBody(settings, JPH::EActivation::Activate);
            bodyInterface.SetLinearVelocity(body.id, Convert(body.velocity));
        }
    });

    hub::Reg().view<CapsuleShape, BodyComp, TransformComp>().each([](CapsuleShape& shape, BodyComp& body, TransformComp& transform) {
        auto& bodyInterface = phys::State::Get().physicsSystem.GetBodyInterface();
        if (body.id.IsInvalid()) {
            JPH::BodyCreationSettings settings(
                new JPH::CapsuleShape(shape.halfHeight, shape.radius),
                Convert(transform.translation),
                Convert(transform.rotation),
                body.motionType,
                body.layer 
            );
            body.id = bodyInterface.CreateAndAddBody(settings, JPH::EActivation::Activate);
            bodyInterface.SetLinearVelocity(body.id, Convert(body.velocity));
        }
    });

    hub::Reg().view<BodyComp, TransformComp, comp::Model>().each([](BodyComp& body, TransformComp& transform, comp::Model& model) {
        auto& bodyInterface = phys::State::Get().physicsSystem.GetBodyInterface();
        if (!model.active) {
            bodyInterface.DeactivateBody(body.id);
            if (bodyInterface.IsAdded(body.id)) {
                bodyInterface.RemoveBody(body.id);
            }
        } else {
            bodyInterface.ActivateBody(body.id);
            if (!bodyInterface.IsAdded(body.id)) {
                bodyInterface.AddBody(body.id, JPH::EActivation::Activate);
            }
        }

        if (bodyInterface.IsActive(body.id)) {
            JPH::RVec3 pos {};
            JPH::Quat rot {};
            bodyInterface.GetPositionAndRotation(body.id, pos, rot);
            transform.translation = Convert(pos);
            transform.rotation = Convert(rot);
            transform.Update();
        }
    });
}
