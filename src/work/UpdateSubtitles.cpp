#include "Work.h"
#include "tun/builder.h"
#include "comp/Subtitle.h"
#include "comp/BoundsWidget.h"
#include "comp/Sound.h"
#include "tun/sound.h"

void work::UpdateSubtitles() {
    Entity chosenSubtitle = entt::null;
    for (Entity subtitleEntity : State::Get().subtitles) {
        auto& bounds = hub::Reg().get<comp::BoundsWidget>(subtitleEntity);
        bounds.visible = false;
    }
    if (State::Get().gameTime > 45.f) {
        return;
    }

    for (Entity subtitleEntity : State::Get().subtitles) {
        auto& subtitle = hub::Reg().get<comp::Subtitle>(subtitleEntity);
        if (subtitle.startTime < State::Get().gameTime) {
            chosenSubtitle = subtitleEntity;
        }
    }

    if (hub::Reg().valid(chosenSubtitle)) {
        auto& bounds = hub::Reg().get<comp::BoundsWidget>(chosenSubtitle);
        bounds.visible = true;
    }
}
