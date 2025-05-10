#include "Work.h"
#include "tun/builder.h"
#include "comp/Music.h"
#include "comp/Sound.h"
#include "tun/sound.h"
#include "Tags.h"

void work::SetMusicMenuPlayed(bool played) {
    hub::Reg().view<tag::MenuMusic, comp::Music>().each([&played](comp::Music& music) {
        music.SetPlayed(played);
    });
}

void work::SetMusicWindPlayed(bool played) {
    hub::Reg().view<tag::MusicWind, comp::Music>().each([&played](comp::Music& music) {
        music.SetPlayed(played);
    });
}
