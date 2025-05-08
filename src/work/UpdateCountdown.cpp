#include "Work.h"
#include "tun/builder.h"
#include "comp/Countdown.h"
#include "comp/TextWidget.h"
#include "comp/BoundsWidget.h"
#include "comp/Sound.h"
#include "tun/sound.h"

void work::UpdateCountdown() {
    hub::Reg().view<comp::BoundsWidget, comp::TextWidget, comp::Countdown>().each([](comp::BoundsWidget& bounds, comp::TextWidget& text, comp::Countdown& countdown) {
        if (!countdown.running) {
            bounds.visible = false;
            return;
        }
        bounds.visible = true;
        countdown.countdown -= hub::GetDeltaTime();
        if (countdown.countdown < 0.f) {
            countdown.countdown = 0.f;
            State::Get().win = true;
            State::Get().gameover = true;
            State::Get().paused = true;
            work::SetMusicPlaying(false);
            hub::LockMouse(false);
        }

        int minutes = (int)(countdown.countdown / 60.f);
        int seconds = (int)countdown.countdown % 60;
        String s = tun::formatToString("{:02d}:{:02d}", minutes, seconds);
        text.text = s;
    });
}
