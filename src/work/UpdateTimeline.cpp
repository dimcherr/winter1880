#include "Work.h"
#include "Hub.h"
#include "comp/BlackScreenComp.h"
#include "comp/SubtitleComp.h"
#include "comp/Character.h"
#include "comp/TransformComp.h"
#include "comp/Sound.h"
#include "comp/Door.h"
#include "comp/Camera.h"
#include "Tags.h"

void work::UpdateTimeline() {
    auto& state = State::Get();
    if (state.phase == GamePhase::intro) {
        if (state.gameTime > 1.f && state.introStage == 0) {
            hub::Reg().view<SubtitleComp>().each([](SubtitleComp& subtitle) {
                subtitle.running = false;
                subtitle.time = 0.f;
            });
            auto& introSub = hub::Reg().get<SubtitleComp>(hub::Reg().view<tag::CueIntro, SubtitleComp>().back());
            introSub.running = true;
            introSub.time = 0.f;
            state.introStage = 1;
        }

        if (state.gameTime > 2.f && state.introStage == 1) {
            hub::Reg().view<BlackScreenComp>().each([](BlackScreenComp& blackScreen) {
                blackScreen.alpha = 1.f;
                blackScreen.delta = -1.f;
                State::Get().introStage = 2;
            });
        }

        Entity characterEntity = hub::Reg().view<comp::Character, TransformComp>().back();
        auto& characterTransform = hub::Reg().get<TransformComp>(characterEntity);
        auto& character = hub::Reg().get<comp::Character>(characterEntity);
        if (state.gameTime > 25.f && characterTransform.translation.z > -220.f && State::Get().introStage == 2) {
            hub::Reg().view<BlackScreenComp>().each([](BlackScreenComp& blackScreen) {
                blackScreen.alpha = 0.f;
                blackScreen.delta = 1.f;
                State::Get().introStage = 3;
            });
        }

        if (State::Get().introStage == 3 && hub::Reg().get<BlackScreenComp>(hub::Reg().view<BlackScreenComp>().back()).alpha == 1.f) {
            State::Get().introStage = 4;
            character.character->SetPosition(Convert(Vec(0.f, character.character->GetPosition().GetY(), -30.f)));
            hub::Reg().view<BlackScreenComp>().each([](BlackScreenComp& blackScreen) {
                blackScreen.alpha = 1.f;
                blackScreen.delta = -1.f;
            });
        }

        if (State::Get().introStage == 4) {
            hub::Reg().view<tag::MainGate, comp::Door>().each([](comp::Door& door) {
                door.stateDelta = 1.f;
                auto& doorOpenSound = hub::Reg().get<comp::Sound>(hub::Reg().view<tag::SoundDoorOpen>().back());
                doorOpenSound.Play();
            });

            State::Get().introStage = 5;
        }

        if (State::Get().introStage == 5 && characterTransform.translation.z > -10.f) {
            hub::Reg().view<tag::MainGate, comp::Door>().each([](comp::Door& door) {
                door.stateDelta = -1.f;
            });

            State::Get().phase = GamePhase::instructions;
            hub::Reg().view<comp::Camera>().each([](comp::Camera& camera) {
                camera.minYaw = 0.f;
                camera.maxYaw = 0.f;
                camera.minPitch = -70.f;
                camera.maxPitch = 70.f;
            });
        }
    }

    //hub::Reg().view<comp::Character, TransformComp>().each([](comp::Character& character) {

    //});

    //if ()

                    //if (transform.translation.z > -240.f && State::Get().introStage == 0) {
                        //State::Get().introStage = 1;
                        //character.character->SetPosition(Convert(Vec(0.f, character.character->GetPosition().GetY(), -140.f)));
                    //}
                    //tun::log("character z pos {}", transform.translation);
                    //tun::log("intro stage {}", State::Get().introStage);
}