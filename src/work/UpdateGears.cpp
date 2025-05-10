#include "Work.h"
#include "Hub.h"
#include "comp/GearComp.h"
#include "comp/TransformComp.h"
#include "comp/LeverComp.h"
#include "comp/GearSlotComp.h"
#include "comp/SlidingManholeComp.h"
#include "comp/BodyComp.h"
#include "Tags.h"
#include "tun/physics.h"

void work::UpdateGears() {
    if (
        hub::Reg().get<GearSlotComp>(hub::Reg().view<tag::LeverMainLeft, GearSlotComp>().back()).filled &&
        hub::Reg().get<GearSlotComp>(hub::Reg().view<tag::LeverMainRight, GearSlotComp>().back()).filled &&
        hub::Reg().get<LeverComp>(hub::Reg().view<tag::LeverMainLeft, LeverComp>().back()).state > 0.5f &&
        hub::Reg().get<LeverComp>(hub::Reg().view<tag::LeverMainRight, LeverComp>().back()).state > 0.5f
    ) {
        hub::Reg().view<SlidingManholeComp>().each([](SlidingManholeComp& manhole) {
            manhole.delta = 1.f;
        });
    }

    hub::Reg().view<SlidingManholeComp, TransformComp, BodyComp>().each([] (SlidingManholeComp& manhole, TransformComp& transform, BodyComp& body) {
        manhole.state += manhole.delta * manhole.speed * hub::GetDeltaTime();
        if (manhole.state > 1.f) {
            manhole.state = 1.f;
            manhole.delta = 0.f;
        }

        transform.translation = manhole.originTranslation + Vec(0.f, 0.f, tun::EaseLerp(manhole.closeDistance, manhole.openDistance, manhole.state));
        transform.Update();
        phys::State::Get().physicsSystem.GetBodyInterface().SetPosition(body.id, Convert(transform.translation), JPH::EActivation::Activate);
    });

    hub::Reg().view<GearComp, TransformComp>().each([](GearComp& gear, TransformComp& transform) {

        if (hub::Reg().get<LeverComp>(hub::Reg().view<tag::LeverMainLeft, LeverComp>().back()).state > 0.5f) {
            if (transform.translation.z < 5.f) {
                // right
                transform.rotation = transform.rotation * Quat({0.f, 0.f, gear.rotationSpeed * hub::GetDeltaTime()});
                transform.Update();
            }
        }

        if (hub::Reg().get<LeverComp>(hub::Reg().view<tag::LeverMainRight, LeverComp>().back()).state > 0.5f) {
            if (transform.translation.z > 5.f) {
                // right
                transform.rotation = transform.rotation * Quat({0.f, 0.f, gear.rotationSpeed * hub::GetDeltaTime()});
                transform.Update();
            }
        }

    });
}
