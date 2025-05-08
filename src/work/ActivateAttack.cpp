#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/tunrandom.h"
#include "comp/BrickWall.h"
#include "comp/Healthbar.h"
#include "comp/BrickWallKey.h"
#include "comp/Model.h"
#include "comp/Countdown.h"
#include "Tags.h"

void work::ActivateAttack() {
    auto& state = State::Get();

    if (!state.firstAttackHappened && state.gameTime > 20.f) {
        hub::Reg().view<comp::BrickWallKey, comp::Model>().each([](comp::BrickWallKey& brickWallKey, comp::Model& model) {
            model.active = true;
        });
        hub::Reg().view<comp::Countdown>().each([](comp::Countdown& countdown) {
            countdown.running = true;
        });
        for (int i = 0; i < 3; ++i) {
            state.timeSinceAttack = 0.f;
            state.firstAttackHappened = true;
            auto view = hub::Reg().view<comp::BrickWall>();
            List<Entity> walls {};
            int underAttackCount = 0;
            for (Entity entity : view) {
                auto& brickWall = view.get<comp::BrickWall>(entity);
                if (brickWall.underAttack) {
                    underAttackCount++;
                }
                if (brickWall.progress >= 100 && !brickWall.destroyed) {
                    walls.push_back(entity);
                }
            }

            if (walls.size() > 0 && underAttackCount < 5) {
                int randomIndex = tun::GetRandomInt(0, walls.size() - 1);
                int randomAccess = i;
                Entity randomEntity = walls[randomIndex];
                auto& brickWall = view.get<comp::BrickWall>(randomEntity);
                brickWall.highlightTime = 0.f;
                brickWall.progress = 99;
                brickWall.access[randomAccess].required = true;
                brickWall.requiredCount = 1;
                brickWall.damage = tun::GetRandomFloat(15.f, 35.f);
                brickWall.underAttack = true;
            }
        }
    }



    state.timeSinceAttack += hub::GetDeltaTime();
    if (!state.firstAttackHappened) return;

    if (state.timeSinceAttack > state.startTimeSinceAttack) {
        state.timeSinceAttack = 0.f;

        auto view = hub::Reg().view<comp::BrickWall>();
        List<Entity> walls {};
        int underAttackCount = 0;
        for (Entity entity : view) {
            auto& brickWall = view.get<comp::BrickWall>(entity);
            if (brickWall.underAttack) {
                underAttackCount++;
            }
            if (brickWall.progress >= 100 && !brickWall.destroyed) {
                walls.push_back(entity);
            }
        }

        if (walls.size() > 0 && underAttackCount < 5) {
            int randomIndex = tun::GetRandomInt(0, walls.size() - 1);
            int randomAccess = tun::GetRandomInt(0, (int)AccessType::count - 1);
            Entity randomEntity = walls[randomIndex];
            auto& brickWall = view.get<comp::BrickWall>(randomEntity);
            brickWall.highlightTime = 0.f;
            brickWall.progress = 99;
            brickWall.access[randomAccess].required = true;
            brickWall.requiredCount = 1;
            brickWall.damage = tun::GetRandomFloat(15.f, 35.f);
            brickWall.underAttack = true;
            state.startTimeSinceAttack *= 0.9f;
        }
    }

    hub::Reg().view<comp::BrickWall>().each([](comp::BrickWall& brickWall) {
        if (brickWall.underAttack && !brickWall.destroyed) {
            brickWall.damage += hub::GetDeltaTime() * brickWall.damagePerSecond;
            brickWall.progress = 99 - (int)brickWall.damage;
            if (brickWall.progress < 1) {
                auto& healthbar = hub::Reg().get<comp::Healthbar>(hub::Reg().view<comp::Healthbar>().back());
                --healthbar.health;
                brickWall.destroyed = true;
                brickWall.underAttack = false;
                State::Get().timeSinceAttack = State::Get().startTimeSinceAttack * 0.5f;
                if (healthbar.health < 0) {
                    healthbar.health = 0;
                    State::Get().lose = true;
                    State::Get().gameover = true;
                    State::Get().paused = true;
                    work::SetMusicPlaying(false);
                    hub::LockMouse(false);
                }
            }
        }
    });
}