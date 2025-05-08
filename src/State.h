#pragma once
#include "tun/entity.h"
#include "tun/tunstring.h"
#include "tun/math.h"
#include "tun/list.h"
#include "tun/input.h"

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
    float timeSinceAttack {0.f};
    //float startTimeSinceAttack {30.f};
    float startTimeSinceAttack {20.f};
    bool leftClick {false};
    bool paused {false};
    float sensitivityFactor {1.f};
    float masterMusicVolume {0.5f};
    float masterSoundVolume {0.5f};
    bool firstAttackHappened {false};

    Matrix viewProj {1.f};
    Vec viewPos {};
    bool firstPerson {true};
    bool hackerMode {false};
    bool flyMode {false};
    bool musicPlaying {false};
    Entity currentObject {entt::null};
    int totalHackerKeysPressed {0};
    Color accessColors[3] {tun::darkPurple, tun::darkBlue, tun::darkYellow};
    List<Entity> hintLabels {};
    List<Entity> subtitles {};
    bool win {false};
    bool lose {false};
    bool gameover {false};
    float startCountdownTime {300.f};
    String goodFace = "(-_-)";
    String badFace = "(o_o)";
    String veryBadFace = "(O_O)";
    String veryVeryBadFace = "(x_x)";

    float frametime {0.f};

    Entity regularFont {entt::null};
    Entity boldFont {entt::null};
    Entity secondaryFont {entt::null};

    static State& Get() {
        static State state;
        return state;
    }

    State(const State&) = delete;
    State& operator=(const State&) = delete;

private:
    State() {}
};
