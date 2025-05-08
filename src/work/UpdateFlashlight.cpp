#include "Work.h"
#include "tun/builder.h"
#include "comp/TransformComp.h"
#include "comp/FlashlightComp.h"
#include "comp/PointLightComp.h"
#include "comp/Camera.h"
#include "Tags.h"

void work::UpdateFlashlight() {
    hub::Reg().view<FlashlightComp, PointLightComp, TransformComp>().each([](FlashlightComp& flashlight, PointLightComp& light, TransformComp& lightTransform) {
        flashlight.Update();
        auto& cameraTransform = hub::Reg().get<TransformComp>(hub::Reg().view<tag::Current, comp::Camera>().back());
        lightTransform = cameraTransform;
        lightTransform.translation.y += 1.f;
        lightTransform.Update();
        light.color = tun::Lerp(tun::black, tun::white, flashlight.enabled);
    });
}
