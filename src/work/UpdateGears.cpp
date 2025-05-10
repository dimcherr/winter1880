#include "Work.h"
#include "Hub.h"
#include "comp/GearComp.h"
#include "comp/TransformComp.h"

void work::UpdateGears() {
    hub::Reg().view<GearComp, TransformComp>().each([](GearComp& gear, TransformComp& transform) {
        transform.rotation = transform.rotation * Quat({0.f, 0.f, gear.rotationSpeed * hub::GetDeltaTime()});
        transform.Update();
    });
}
