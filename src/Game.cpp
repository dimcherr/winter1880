#include "Game.h"
#include "Hub.h"
#include "comp/Mesh.h"
#include "comp/Transform.h"
#include "comp/MaterialGrid.h"
#include "comp/Subtitle.h"
#include "comp/Model.h"
#include "comp/SliderWidget.h"
#include "comp/MaterialColor.h"
#include "comp/HintLabel.h"
#include "comp/Healthbar.h"
#include "comp/Camera.h"
#include "comp/Countdown.h"
#include "comp/BrickWallKey.h"
#include "comp/SphereShape.h"
#include "comp/BoxShape.h"
#include "comp/Door.h"
#include "comp/CapsuleShape.h"
#include "comp/Character.h"
#include "comp/Music.h"
#include "comp/Sound.h"
#include "comp/TextWidget.h"
#include "comp/BoundsWidget.h"
#include "comp/Font.h"
#include "comp/ButtonWidget.h"
#include "comp/HackerKey.h"
#include "comp/HackerBrick.h"
#include "comp/BrickWall.h"
#include "Tags.h"
#include "tun/gl.h"
#include "tun/log.h"
#include "tun/gltf.h"
#include "tun/tunrandom.h"
#include "tun/physics.h"
#include "tun/sound.h"
#include "work/Work.h"
#include "prefab/Prefab.h"
#ifdef OS_WEB
#include <emscripten.h>
#endif

#ifdef OS_WEB
EMSCRIPTEN_KEEPALIVE
void onWebLoad() {
    EM_ASM({
        Module.onWebLoad();
    });
}
#else
void onWebLoad() {}
#endif

static void OnHackerKeyPress(Key key);
static void OnClickPlayButton(Entity entity);
static void RestartGame(Entity entity);

void game::Create() {
    tun::InitLogging("out.log", "err.log", "cerr.log", "stderr.log");
    phys::Init();
    gl::SetupRenderer();
    sound::Init();



    auto& state = State::Get();
    state.regularFont = prefab::Font("res/fonts/Serati.ttf", 32.f);
    state.boldFont = prefab::Font("res/fonts/Serati.ttf", 64.f);
    state.secondaryFont = prefab::Font("res/fonts/M6.ttf", 24.f);

    Entity plane = prefab::Plane();
    Entity cube = prefab::Cube();
    Entity rect = prefab::Rect();
    Entity grid = prefab::Grid();
    gltf::LoadPrims("res/models/Prims.glb");
    gltf::Load("res/models/Winter1880.glb");



    Entity tooltip = hub::Create()
        .Add<comp::BoundsWidget>().color(tun::black, 0.25f).minSize({512.f, 64.f}).pos({0.f, 256.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.regularFont).color(tun::white).text("Что это?").Next()
        .Tag<tag::Tooltip>()
        .GetEntity();

    Entity buttonPlay = hub::Create()
        .Add<comp::BoundsWidget>().color(tun::black, 1.f).minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).visible(true).onClick(&OnClickPlayButton).Next()
        .Add<comp::TextWidget>().font(state.regularFont).color(tun::green).text("Играть").Next()
        .Tag<tag::Menu>()
        .GetEntity();

    Entity buttonSettings = hub::Create()
        .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, 0.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.regularFont).color(tun::white).text("Настройки").Next()
        .GetEntity();

    Entity buttonButton = hub::Create()
        .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, 128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.regularFont).color(tun::white).text("Нажми на меня").Next()
        .GetEntity();

    prefab::CameraFly({0.f, 10.f, 10.f}, tun::vecZero);



    Entity character = hub::Create()
        .Add<comp::Camera>().rotationSensitivity(0.5f).update(hub::GetScreenSize().x, hub::GetScreenSize().y).Next()
        .Tag<tag::FirstPerson>()
        .Tag<tag::Current>()
        .Add<comp::Transform>().pos({5.38f, 4.85f, -7.6f}).rot(0.f, 0.f, 0.f).update().Next()
        .Add<comp::Character>().mass(70.f).maxSlopeAngle(60.f).maxStrength(100.f).speed(300.f, 600.f).jumpStrength(300.f).Next()
        .Add<comp::CapsuleShape>().halfHeight(0.6f).radius(0.3f).Next()
        .Add<comp::Sound>().foleys("res/sounds/steps/step", 3).Next()
        .GetEntity();
    
    Entity backgroundMusic = hub::Create()
        .Add<comp::Music>().path("res/sounds/theme.ogg").volume(0.5f).Next()
        .Tag<tag::GameMusic>()
        .GetEntity();
    Entity backgroundMusicMenu = hub::Create()
        .Add<comp::Music>().path("res/sounds/WinterOstMenu.ogg").volume(0.5f).Next()
        .Tag<tag::MenuMusic>()
        .GetEntity();
    Entity hruSound = hub::Create()
        .Add<comp::Sound>().foleys("res/sounds/hru/hru", 3).period(1.f, 3.f).Next()
        .Tag<tag::Hru>()
        .GetEntity();


    List<HackerKeyPair> row0 {
        {Key::q, "q"},
        {Key::w, "w"},
        {Key::e, "e"},
        {Key::r, "r"},
        {Key::t, "t"},
        {Key::y, "y"},
        {Key::u, "u"},
        {Key::i, "i"},
        {Key::o, "o"},
        {Key::p, "p"},
    };

    List<HackerKeyPair> row1 {
        {Key::a, "a"},
        {Key::s, "s"},
        {Key::d, "d"},
        {Key::f, "f"},
        {Key::g, "g"},
        {Key::h, "h"},
        {Key::j, "j"},
        {Key::k, "k"},
        {Key::l, "l"},
    };

    List<HackerKeyPair> row2 {
        {Key::z, "z"},
        {Key::x, "x"},
        {Key::c, "c"},
        {Key::v, "v"},
        {Key::b, "b"},
        {Key::n, "n"},
        {Key::m, "m"},
    };

    List<List<HackerKeyPair>> rows {
        row0,
        row1,
        row2,
    };

    for (int k = 0; k < rows.size(); ++k) {
        for (int i = 0; i < rows[k].size(); ++i) {
            hub::Create()
                .Add<comp::BoundsWidget>()
                    .color(tun::black, 0.75f)
                    .visible(true)
                    .minSize({48.f, 48.f})
                    .pos({i * 64.f - 256.f + k * 32.f, k * 72.f + 128.f})
                    .anchors({tun::center, tun::center})
                    .parentAnchors({tun::center, tun::center})
                    .Next()
                .Add<comp::TextWidget>().font(state.regularFont).color(tun::green).text("hey").Next()
                .Add<comp::HackerKey>().symbol(rows[k][i].symbol, rows[k][i].key).period(0.5f).animIntensity(1.f).onPress(&OnHackerKeyPress).Next()
                .Tag<tag::Hacker>()
                .GetEntity();
        }
    }

    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            hub::Create()
                .Add<comp::HackerBrick>().pos(x, y).Next()
                .Tag<tag::Hacker>()
                .GetEntity();
        }
    }

    Entity healthbarLabel = hub::Create()
        .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, 128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::begin}).visible(true).Next()
        .Add<comp::TextWidget>().font(state.regularFont).color(tun::white).text("Состояние умного дома").Next()
        .Tag<tag::HealthbarLabel>()
        .GetEntity();

    hub::Create()
        .Add<comp::BoundsWidget>().minSize({512.f, 32.f}).parentAnchors({tun::center, tun::begin}).pos({0.f, 64.f}).visible(true).Next()
        .Add<comp::Healthbar>().health(5).Next();

    Entity countdown = hub::Create()
        .Add<comp::BoundsWidget>().color(tun::black, 0.85f).visible(true).minSize({128.f, 64.f}).pos({128.f, 64.f}).anchors({tun::center, tun::center}).parentAnchors({tun::begin, tun::begin}).Next()
        .Add<comp::TextWidget>().font(state.regularFont).color(tun::white).text("05:00").Next()
        .Add<comp::Countdown>().countdown(State::Get().startCountdownTime).Next()
        .GetEntity();

    for (int i = 0; i < 5; ++i) {
        Entity hintLabelEntity = hub::Create()
            .Add<comp::BoundsWidget>().color(tun::darkPurple, 0.75f).visible(true).minSize({0.f, 0.f}).padding({16.f, 16.f}).pos({64.f, 128.f + 64.f * i}).textAnchors({tun::center, tun::center}).anchors({tun::begin, tun::center}).parentAnchors({tun::begin, tun::begin}).Next()
            .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("(-_-) Подвал Водонагреватель 80%").Next()
            .Add<comp::HintLabel>().Next()
            .GetEntity();
        State::Get().hintLabels.push_back(hintLabelEntity);
    }

    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("Прошло несколько лет с тех пор,").Next()
            .Add<comp::Subtitle>().time(1.f).Next()
            .GetEntity()
    );
    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("как волк уничтожил дома трёх его братьев-поросят.").Next()
            .Add<comp::Subtitle>().time(3.5f).Next()
            .GetEntity()
    );
    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("Четвёртый поросёнок подкопил денег и построил себе Умный дом.").Next()
            .Add<comp::Subtitle>().time(7.f).Next()
            .GetEntity()
    );
    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("Но и для его врага прогресс не стоял на месте.").Next()
            .Add<comp::Subtitle>().time(11.f).Next()
            .GetEntity()
    );
    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("Теперь нашему герою угрожает Киберволк!").Next()
            .Add<comp::Subtitle>().time(14.f).Next()
            .GetEntity()
    );

    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().color(tun::black).minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.regularFont).color(tun::red).text("Внимание! Умный дом обнаружил нападение на систему!").Next()
            .Add<comp::Subtitle>().time(20.f).Next()
            .GetEntity()
    );
    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().color(tun::black).minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.regularFont).color(tun::red).text("Киберволк атакует по всем электрическим приборам.").Next()
            .Add<comp::Subtitle>().time(23.5f).Next()
            .GetEntity()
    );
    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().color(tun::black).minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.regularFont).color(tun::red).text("Киберполиция прибудет через пять минут.").Next()
            .Add<comp::Subtitle>().time(27.f).Next()
            .GetEntity()
    );

    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().color(tun::black).minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.regularFont).color(tun::red).text("Пока не приехала Киберполиция нужно отражать удары Киберволка!").Next()
            .Add<comp::Subtitle>().time(30.5f).Next()
            .GetEntity()
    );
    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().color(tun::black).minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.regularFont).color(tun::red).text("Для этого нужно следить, что именно атакует враг").Next()
            .Add<comp::Subtitle>().time(34.f).Next()
            .GetEntity()
    );
    State::Get().subtitles.push_back(
        hub::Create()
            .Add<comp::BoundsWidget>().color(tun::black).minSize({512.f, 64.f}).pos({0.f, -128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::end}).visible(false).Next()
            .Add<comp::TextWidget>().font(state.regularFont).color(tun::red).text("и быстро чинить устройство с помощью ключа соответствующего цвета.").Next()
            .Add<comp::Subtitle>().time(38.f).Next()
            .GetEntity()
    );

    Entity winLabel = hub::Create()
        .Add<comp::BoundsWidget>().visible(false).minSize({0.f, 0.f}).pos({0.f, 0.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.boldFont).color(tun::green).text("ПОЗДРАВЛЯЕМ!").Next()
        .Tag<tag::Win>()
        .GetEntity();
    Entity winLabel2 = hub::Create()
        .Add<comp::BoundsWidget>().visible(false).minSize({0.f, 0.f}).pos({0.f, 256.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.boldFont).color(tun::green).text("КИБЕРВОЛК ПОЙМАН И АРЕСТОВАН!").Next()
        .Tag<tag::Win>()
        .GetEntity();

    Entity loseLabel = hub::Create()
        .Add<comp::BoundsWidget>().visible(false).minSize({0.f, 0.f}).pos({0.f, 0.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.boldFont).color(tun::red).text("ДОМ УНИЧТОЖЕН. ВЫ ПРОИГРАЛИ.").Next()
        .Tag<tag::Lose>()
        .GetEntity();

    Entity buttonReplay = hub::Create()
        .Add<comp::BoundsWidget>().color(tun::black).visible(false).minSize({512.f, 64.f}).pos({0.f, 128.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).onClick(&RestartGame).Next()
        .Add<comp::TextWidget>().font(state.regularFont).color(tun::white).text("Заново").Next()
        .Tag<tag::Win>()
        .Tag<tag::Lose>()
        .GetEntity();
    
    Entity sliderMouseSense = hub::Create()
        .Add<comp::BoundsWidget>().visible(true).color(tun::black, 0.75f).minSize({512.f, 64.f}).pos({0.f, 0.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("Чувствительность мыши").Next()
        .Add<comp::SliderWidget>().percent(0.5f).type(SliderType::mouse).Next()
        .GetEntity();

    Entity sliderSoundVolume = hub::Create()
        .Add<comp::BoundsWidget>().visible(true).color(tun::black, 0.75f).minSize({512.f, 64.f}).pos({0.f, 72.f}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("Громкость звуков").Next()
        .Add<comp::SliderWidget>().percent(0.5f).type(SliderType::sound).Next()
        .GetEntity();

    Entity sliderMusicVolume = hub::Create()
        .Add<comp::BoundsWidget>().visible(true).color(tun::black, 0.75f).minSize({512.f, 64.f}).pos({0.f, 72.f * 2}).anchors({tun::center, tun::center}).parentAnchors({tun::center, tun::center}).Next()
        .Add<comp::TextWidget>().font(state.secondaryFont).color(tun::white).text("Громкость музыки").Next()
        .Add<comp::SliderWidget>().percent(0.5f).type(SliderType::music).Next()
        .GetEntity();
    


    work::SetMusicPlaying(false);
    State::Get().paused = true;
    onWebLoad();
}

void game::Update() {
    auto& state = State::Get();
    work::UpdateSounds();
    work::UpdateSubtitles();

    if (state.gameover) {
        if (state.win) {
            hub::Reg().view<comp::BoundsWidget, tag::Win>().each([](comp::BoundsWidget& bounds) {
                bounds.visible = true;
            });
        } else if (state.lose) {
            hub::Reg().view<comp::BoundsWidget, tag::Lose>().each([](comp::BoundsWidget& bounds) {
                bounds.visible = true;
            });
        }
    } else {
        hub::Reg().view<comp::BoundsWidget, tag::Win>().each([](comp::BoundsWidget& bounds) {
            bounds.visible = false;
        });
        hub::Reg().view<comp::BoundsWidget, tag::Lose>().each([](comp::BoundsWidget& bounds) {
            bounds.visible = false;
        });
    }

    if (!state.paused) {
        work::HighlightBrickWall();
        work::ActivateAttack();
        work::UpdateDoors();
        work::UpdatePhysics();
        work::UpdateCountdown();
        work::UpdateHints();
    }

    work::UpdateTransforms();

    if (!state.paused) {
        if (state.firstPerson) {
            work::UpdateCameraRotation();
            work::UpdateRaycast();
            hub::LockMouse(true);
        } else if (state.flyMode) {
            if (hub::IsMouseButtonPressed(MouseButton::right)) {
                work::UpdateCameraRotation();
                work::UpdateCameraMovement();
                hub::LockMouse(true);
            } else {
                hub::LockMouse(false);
            }
        }
    }
    work::UpdateCamera();

    gl::BeginRenderPass({0.85f, 0.5f, 0.5f});
    //work::DrawGrid();
    //work::DrawLights();
    if (state.gameover && state.lose) {
        work::DrawColliders();
    }
    //work::DrawColored();
    work::DrawPBR();

    if (gl::BeginDrawUI()) {
        if (state.paused && !state.gameover) {
            work::DrawMenu();
            work::DrawSliders();
        } else {
            work::DrawUI();
            if (!state.gameover && state.firstAttackHappened) {
                work::DrawHealthbar();
                if (state.hackerMode) {
                    work::DrawHackerKeyboard();
                    work::DrawHackerMonitor();
                }
            }
        }
    }
    gl::EndDrawUI();
    gl::RenderUIBuffer();

    gl::EndRenderPass();
    gl::EndDrawing();

    state.mouseDeltaX = 0.f;
    state.mouseDeltaY = 0.f;
    state.leftClick = false;
    hub::UpdateTime();
}

void game::Destroy() {
    gl::Shutdown();
}

void game::OnEvent(const sapp_event* event) {
    gl::HandleUIEvent(event);
    auto& state = State::Get();
    switch (event->type) {
        case SAPP_EVENTTYPE_KEY_DOWN:
            if (event->key_code < state.keys.size()) {
                state.keys[event->key_code] = true;
                OnKeyDown((Key)event->key_code);
            }
        break;
        case SAPP_EVENTTYPE_KEY_UP:
            if (event->key_code < state.keys.size()) {
                state.keys[event->key_code] = false;
            }
        break;
        case SAPP_EVENTTYPE_MOUSE_DOWN:
            state.mouse[event->mouse_button] = true;
            state.leftClick = true;
        break;
        case SAPP_EVENTTYPE_MOUSE_UP:
            state.mouse[event->mouse_button] = false;
        break;
        case SAPP_EVENTTYPE_MOUSE_MOVE:
            state.mouseDeltaX = tun::Lerp(state.mouseDeltaX, event->mouse_dx, 0.05f);
            state.mouseDeltaY = tun::Lerp(state.mouseDeltaY, event->mouse_dy, 0.05f);
            state.mouseX = event->mouse_x;
            state.mouseY = event->mouse_y;
        break;
        default:
        break;
    }
}

void game::OnKeyDown(Key key) {
    auto& state = State::Get();
    //if (key == Key::v) {
        //work::SwitchCamera();
    //}

    if (key == Key::tab) {
        state.paused = !state.paused;
        work::SetMusicPlaying(!state.paused);
        hub::LockMouse(!state.paused);
        return;
    }
    if (state.paused) return;
    if (key == Key::e && state.firstPerson) {
        if (hub::Reg().all_of<tag::Kimchi>(state.currentObject)) {
            auto& kimchiModel = hub::Reg().get<comp::Model>(state.currentObject);
            kimchiModel.active = false;
            return;
        }

        if (hub::Reg().valid(state.currentObject) && hub::Reg().all_of<comp::BrickWall, tag::Hacker>(state.currentObject)) {
            auto& brickWall = hub::Reg().get<comp::BrickWall>(state.currentObject);
            if (brickWall.requiredCount == 0 && !brickWall.destroyed && brickWall.underAttack) {
                state.hackerMode = true;
                state.firstPerson = false;

                auto [text, bounds] = hub::Reg().get<comp::TextWidget, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
                bounds.visible = false;
            } else if (brickWall.requiredCount > 0 && !brickWall.destroyed) {
                auto& character = hub::Reg().get<comp::Character>(hub::Reg().view<comp::Character, tag::Current>().back());
                if (hub::Reg().valid(character.pickable)) {
                    auto& brickWallKey = hub::Reg().get<comp::BrickWallKey>(character.pickable);
                    auto& brickWallKeyModel = hub::Reg().get<comp::Model>(character.pickable);
                    if (brickWall.access[(int)brickWallKey.type].required) {
                        brickWall.access[(int)brickWallKey.type].required = false;
                        brickWall.highlightTime = 0.f;
                        brickWall.requiredCount--;
                        if (brickWall.requiredCount < 0) {
                            brickWall.requiredCount = 0;
                        }
                        character.pickable = entt::null;

                        if (brickWall.requiredCount == 0) {
                            state.hackerMode = true;
                            state.firstPerson = false;
                            auto [text, bounds] = hub::Reg().get<comp::TextWidget, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
                            bounds.visible = false;
                        }
                    }
                }
            }
        }

        auto* door = hub::Reg().try_get<comp::Door>(state.currentObject);
        if (door) {
            if (door->doorState < 0.5f) {
                door->stateDelta = 1.f;
            } else {
                door->stateDelta = -1.f;
            }
        }

        if (hub::Reg().any_of<tag::Pickable>(state.currentObject)) {
            auto* character = hub::Reg().try_get<comp::Character>(hub::Reg().view<comp::Character, tag::Current>().back());
            if (character) {
                character->pickable = state.currentObject;
            }
        }
    }

    if (key == Key::g && state.firstPerson) {
        auto* character = hub::Reg().try_get<comp::Character>(hub::Reg().view<comp::Character, tag::Current>().back());
        if (character) {
            if (hub::Reg().valid(character->pickable)) {
                character->pickable = entt::null;
            }
        }
    }
}

static void OnHackerKeyPress(Key key) {
    auto& state = State::Get();
    if (state.paused) return;
    ++state.totalHackerKeysPressed;
    auto* brickWall = hub::Reg().try_get<comp::BrickWall>(state.currentObject);
    if (state.hackerMode && brickWall) {
        if (brickWall->destroyed) {
            state.hackerMode = false;
            state.firstPerson = true;
            brickWall->progress = 0;
        } else if (brickWall->progress < 100) {
            brickWall->damage -= 1.f;
            if (brickWall->damage <= 0.f) {
                brickWall->underAttack = false;
                brickWall->damage = 0.f;
                brickWall->progress = 100;
                state.hackerMode = false;
                state.firstPerson = true;
                brickWall->underAttack = false;
                State::Get().timeSinceAttack = State::Get().startTimeSinceAttack * 0.5f;
            }
        } else {
            state.hackerMode = false;
            state.firstPerson = true;
            brickWall->progress = 100;
        }
    }
}

static void OnClickPlayButton(Entity entity) {
    State::Get().paused = false;
    work::SetMusicPlaying(true);
    hub::LockMouse(true);
}

static void RestartGame(Entity entity) {
    hub::Reg().view<comp::Countdown>().each([](comp::Countdown& countdown) {
        countdown.countdown = State::Get().startCountdownTime;
    });
    auto& state = State::Get();
    state.gameover = false;
    state.win = false;
    state.lose = false;
    state.paused = false;
    state.time = 0.0;
    state.gameTime = 50.f;
    state.timeSinceAttack = 0.f;
    state.leftClick = false;
    state.viewProj = Matrix(1.f);
    state.viewPos = {};
    state.firstPerson = true;
    state.hackerMode = false;
    state.flyMode = false;
    state.musicPlaying = false;
    state.currentObject = entt::null;
    state.totalHackerKeysPressed = 0;
    state.win = false;
    state.lose = false;
    state.gameover = false;
    state.firstAttackHappened = true;
    state.startTimeSinceAttack = 20.f;

    hub::Reg().view<comp::Healthbar>().each([](comp::Healthbar& healthbar) {
        healthbar.health = 5;
    });

    hub::Reg().view<comp::Model, tag::Kimchi>().each([](comp::Model& kimchi) {
        kimchi.active = true;
    });

    hub::Reg().view<comp::BrickWall>().each([](comp::BrickWall& brickWall) {
        brickWall.access[0].required = false;
        brickWall.access[1].required = false;
        brickWall.access[2].required = false;
        brickWall.damage = 0.f;
        brickWall.destroyed = false;
        brickWall.highlightTime = 0.f;
        brickWall.progress = 100;
        brickWall.requiredCount = 0;
        brickWall.underAttack = false;
    });

    hub::Reg().view<comp::Music>().each([](comp::Music& music) {
        sound::StopMusic(music.music);
        sound::PlayMusic(music.music, 20.f);
    });
    work::SetMusicPlaying(true);
    hub::LockMouse(true);


    for (int i = 0; i < 3; ++i) {
        hub::Reg().view<comp::Countdown>().each([](comp::Countdown& countdown) {
            countdown.running = true;
        });
        hub::Reg().view<comp::BrickWallKey, comp::Model>().each([](comp::BrickWallKey& brickWallKey, comp::Model& model) {
            model.active = true;
        });
        state.firstAttackHappened = true;
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