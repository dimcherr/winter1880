#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Camera.h"
#include "comp/TransformComp.h"
#include "comp/BoxShape.h"
#include "comp/BodyComp.h"
#include "comp/Door.h"
#include "comp/Sound.h"
#include "Tags.h"

void work::UpdateDoors() {

    hub::Reg().view<comp::Door, TransformComp, comp::BoxShape, BodyComp>().each([](comp::Door& door, TransformComp& transform, comp::BoxShape& shape, BodyComp& body) {
        if (door.stateDelta > 0.f) {
            door.doorState += door.stateDelta * hub::GetDeltaTime() * door.openingSpeed;
        } else if (door.stateDelta < 0.f) {
            door.doorState += door.stateDelta * hub::GetDeltaTime() * door.closingSpeed;
        }

        if (door.doorState <= 0.f && door.stateDelta != 0.f) {
            door.doorState = 0.f;
            auto& doorCloseSound = hub::Reg().get<comp::Sound>(hub::Reg().view<tag::SoundDoorClose>().back());
            doorCloseSound.Play();
            door.stateDelta = 0.f;
        }

        //if (door.doorState < 0.f) {
            //door.doorState = 0.f;
            //auto& doorCloseSound = hub::Reg().get<comp::Sound>(hub::Reg().view<tag::SoundDoorClose>().back());
            //doorCloseSound.Play();
        //} else if (door.doorState > 1.f) {
            //door.doorState = 1.f;
            //auto& doorOpenSound = hub::Reg().get<comp::Sound>(hub::Reg().view<tag::SoundDoorClose>().back());
            //doorOpenSound.Play();
        //}
        door.doorState = glm::clamp(door.doorState, 0.f, 1.f);

        transform.rotation = transform.originRotation * Quat({0.f, glm::radians(tun::EaseLerp(door.closeAngle, door.openAngle, door.doorState)), 0.f});
        transform.Update();
        phys::State::Get().physicsSystem.GetBodyInterface().SetRotation(body.id, Convert(transform.rotation), JPH::EActivation::Activate);
    });
}
