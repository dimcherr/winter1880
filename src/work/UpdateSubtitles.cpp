#include "Work.h"
#include "tun/builder.h"
#include "comp/SubtitleComp.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "comp/Sound.h"
#include "tun/sound.h"
#include "Tags.h"
#include "tun/tunstring.h"

void work::UpdateSubtitles() {
    hub::Reg().view<SubtitleComp, comp::BoundsWidget, TextWidgetComp>().each([](Entity entity, SubtitleComp& subtitle, comp::BoundsWidget& bounds, TextWidgetComp& text) {
        if (subtitle.running) {
            subtitle.time += hub::GetDeltaTime();
            if (subtitle.time >= subtitle.totalDuration) {
                subtitle.time = subtitle.totalDuration;
                subtitle.running = false;
                if (subtitle.onEnd) {
                    subtitle.onEnd(entity);
                }
                if (hub::Reg().valid(subtitle.next)) {
                    auto& nextSubtitle = hub::Reg().get<SubtitleComp>(subtitle.next);
                    nextSubtitle.time = 0.f;
                    nextSubtitle.running = true;
                }
            }
            if (subtitle.time < subtitle.speakingDuration) {
                subtitle.soundElapsedTime += hub::GetDeltaTime();
                if (subtitle.soundElapsedTime > 0.2f) {
                    auto& booSound = hub::Reg().get<comp::Sound>(hub::Reg().view<tag::SoundBoo, comp::Sound>().back());
                    booSound.Play();
                    subtitle.soundElapsedTime = 0.f;
                }
            }
            text.text = Substring(text.langString->Get(), subtitle.GetSpeakingPercent());
            bounds.visible = true;
        } else {
            bounds.visible = false;
        }
    });
}
