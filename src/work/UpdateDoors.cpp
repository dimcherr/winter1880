#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Camera.h"
#include "comp/TransformComp.h"
#include "comp/BoxShape.h"
#include "comp/BodyComp.h"
#include "comp/Door.h"
#include "Tags.h"

void work::UpdateDoors() {

    hub::Reg().view<comp::Door, TransformComp, comp::BoxShape, BodyComp>().each([](comp::Door& door, TransformComp& transform, comp::BoxShape& shape, BodyComp& body) {
        door.doorState += door.stateDelta * hub::GetDeltaTime() * door.openingSpeed;
        door.doorState = glm::clamp(door.doorState, 0.f, 1.f);

        transform.rotation = transform.originRotation * Quat({0.f, glm::radians(tun::EaseLerp(door.closeAngle, door.openAngle, door.doorState)), 0.f});
        transform.Update();
        phys::State::Get().physicsSystem.GetBodyInterface().SetRotation(body.id, Convert(transform.rotation), JPH::EActivation::Activate);
    });
}
