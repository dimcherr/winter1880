#include "Work.h"
#include "Hub.h"
#include "comp/LeverComp.h"
#include "comp/TransformComp.h"
#include "comp/GearSlotComp.h"
#include "comp/Music.h"
#include "Tags.h"

void work::UpdateLevers() {

    auto& gearsMusic = hub::Reg().get<comp::Music>(hub::Reg().view<comp::Music, tag::MusicGears>().back());
    gearsMusic.volume = 0.f;
    gearsMusic.currentVolume = 0.f;
    hub::Reg().view<LeverComp, TransformComp>().each([&gearsMusic](Entity entity, LeverComp& lever, TransformComp& transform) {
        lever.state += lever.delta * lever.rotationSpeed * hub::GetDeltaTime();
        if (lever.state > 1.f) {
            lever.state = 1.f;
            lever.delta = 0.f;
            // check mechanism
            if (hub::Reg().any_of<tag::LeverMainLeft>(entity)) {
                if (!hub::Reg().get<GearSlotComp>(hub::Reg().view<GearSlotComp, tag::LeverMainLeft>().back()).filled) {
                    lever.delta = -1.f;
                }
            } else if (hub::Reg().any_of<tag::LeverMainRight>(entity)) {
                if (!hub::Reg().get<GearSlotComp>(hub::Reg().view<GearSlotComp, tag::LeverMainRight>().back()).filled) {
                    lever.delta = -1.f;
                }
            }
        } else if (lever.state < 0.f) {
            lever.state = 0.f;
            lever.delta = 0.f;
        }

        gearsMusic.volume += lever.state * 0.5f;
        gearsMusic.currentVolume += lever.state * 0.5f;

        transform.rotation = transform.originRotation * Quat({glm::radians(tun::EaseLerp(lever.offAngle, lever.onAngle, lever.state)), 0.f, 0.f});
        transform.Update();
    });
}
