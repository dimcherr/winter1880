#pragma once
#include "tun/entity.h"
#include "tun/tunstring.h"
#include "tun/math.h"
#include "tun/list.h"
#include "tun/input.h"
#include "Lang.h"

enum class GamePhase {
    intro,
    instructions,
    main,
    machinesRunning,
    outro,
};

class State {
public:
    entt::registry reg {};

    float mouseDeltaX {};
    float mouseDeltaY {};
    float mouseX {};
    float mouseY {};
    Array<bool, 1000> keys {};
    Array<bool, (size_t)MouseButton::count> mouse {};
    double time {};
    float gameTime {0.f};
    bool leftClick {false};
    bool paused {false};
    float sensitivityFactor {1.f};
    float masterMusicVolume {0.5f};
    float masterSoundVolume {0.5f};

    Matrix viewProj {1.f};
    Vec viewPos {};
    bool firstPerson {true};
    bool flyMode {false};
    bool musicPlaying {false};
    Entity currentObject {entt::null};


    Entity regularFont {entt::null};
    Entity boldFont {entt::null};
    Entity secondaryFont {entt::null};
    Entity cloudTexture {entt::null};
    Entity blackTexture {entt::null};

    Lang currentLang {Lang::eng};

    GamePhase phase {GamePhase::intro};
    int introStage {0};

    static State& Get() {
        static State state;
        return state;
    }

    State(const State&) = delete;
    State& operator=(const State&) = delete;

private:
    State() {}
};
