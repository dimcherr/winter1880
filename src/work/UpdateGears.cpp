#include "Work.h"
#include "Hub.h"
#include "comp/GearComp.h"
#include "comp/TransformComp.h"
#include "comp/LeverComp.h"
#include "Tags.h"

void work::UpdateGears() {
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
