#include "Game.h"
#include "Hub.h"
#include "comp/Mesh.h"
#include "comp/TransformComp.h"
#include "comp/MaterialGrid.h"
#include "comp/Subtitle.h"
#include "comp/Model.h"
#include "comp/SliderWidget.h"
#include "comp/MaterialColor.h"
#include "comp/Camera.h"
#include "comp/TextureAsset.h"
#include "comp/Countdown.h"
#include "comp/BoxShape.h"
#include "comp/Door.h"
#include "comp/CapsuleShape.h"
#include "comp/Character.h"
#include "comp/Music.h"
#include "comp/Sound.h"
#include "comp/FlashlightComp.h"
#include "prefab/UIPrefabLib.h"
#include "comp/BoundsWidget.h"
#include "comp/Font.h"
#include "comp/ButtonWidget.h"
#include "Tags.h"
#include "tun/gl.h"
#include "tun/log.h"
#include "tun/gltf.h"
#include "tun/tunrandom.h"
#include "tun/physics.h"
#include "tun/sound.h"
#include "work/Work.h"
#include "prefab/Prefab.h"
#include "prefab/GameplayPrefab.h"
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

static void OnClickPlayButton(Entity entity);
static void RestartGame(Entity entity);

void game::Create() {
    tun::InitLogging("out.log", "err.log", "cerr.log", "stderr.log");
    phys::Init();
    gl::SetupRenderer();
    sound::Init();



    auto& state = State::Get();
    state.regularFont = prefab::Font("res/fonts/Serati.ttf", 32.f);
    state.boldFont = state.regularFont;
    state.secondaryFont = state.regularFont;

    state.cloudTexture = hub::Create()
        .Add<comp::TextureAsset>().image(gl::CreateImageSimple("res/textures/cloud1.png")).Next()
        .GetEntity();
    //state.boldFont = prefab::Font("res/fonts/Serati.ttf", 64.f);
    //state.secondaryFont = prefab::Font("res/fonts/M6.ttf", 24.f);

    Entity plane = prefab::Plane();
    Entity cube = prefab::Cube();
    Entity rect = prefab::Rect();
    Entity grid = prefab::Grid();
    gltf::LoadPrims("res/models/Prims.glb");
    gltf::Load("res/models/Winter1880.glb");



    Entity tooltip = prefab::Tooltip();

    Entity buttonPlay = prefab::Button(&LangStrings::play, &OnClickPlayButton, 0);
    hub::AddTag<tag::Menu>(buttonPlay);

    Entity sliderMouseSense = prefab::Slider(&LangStrings::mouseSense, 0.5f, SliderType::mouse, 1);
    Entity sliderSoundVolume = prefab::Slider(&LangStrings::soundVolume, 1.f, SliderType::sound, 2);
    Entity sliderMusicVolume = prefab::Slider(&LangStrings::musicVolume, 0.75f, SliderType::music, 3);

    Entity buttonChangeLanguage = prefab::Button(&LangStrings::currentLang, [](Entity entity) { work::UpdateLanguage(); }, 4);
    hub::AddTag<tag::Menu>(buttonChangeLanguage);

    Entity buttonReplay = prefab::Button(&LangStrings::replay, &RestartGame, 0);
    hub::Reg().get<comp::BoundsWidget>(buttonReplay).visible = false;

    prefab::CameraFly({0.f, 10.f, 10.f}, tun::vecZero);



    Entity character = hub::Create()
        .Add<comp::Camera>().rotationSensitivity(0.5f).update(hub::GetScreenSize().x, hub::GetScreenSize().y).Next()
        .Tag<tag::FirstPerson>()
        .Tag<tag::Current>()
        .Add<comp::Character>().mass(70.f).maxSlopeAngle(60.f).maxStrength(100.f).speed(300.f, 600.f).jumpStrength(300.f).Next()
        .Add<comp::CapsuleShape>().halfHeight(0.6f).radius(0.3f).Next()
        .Add<comp::Sound>().foleys("res/sounds/steps/step", 3).Next()
        .GetEntity();
    auto& characterTransform = hub::AddComp<TransformComp>(character);
    characterTransform.translation = {5.38f, 4.85f, -7.6f};
    characterTransform.Update();
    
    Entity flashlight = prefab::Flashlight();
    
    Entity backgroundMusic = hub::Create()
        .Add<comp::Music>().path("res/sounds/theme.ogg").volume(0.5f).Next()
        .Tag<tag::GameMusic>()
        .GetEntity();
    Entity backgroundMusicMenu = hub::Create()
        .Add<comp::Music>().path("res/sounds/FourthPigletOstMenu.ogg").volume(0.5f).Next()
        .Tag<tag::MenuMusic>()
        .GetEntity();

    State::Get().subtitles.push_back(prefab::Subtitle(&LangStrings::testSubtitle0, 1.f));
    State::Get().subtitles.push_back(prefab::Subtitle(&LangStrings::testSubtitle1, 4.f));

    work::SetMusicPlaying(false);
    State::Get().paused = true;
    onWebLoad();
}

void game::Update() {
    auto& state = State::Get();
    work::UpdateSounds();
    work::UpdateSubtitles();

    if (!state.paused) {
        work::UpdateDoors();
        work::UpdatePhysics();
        work::UpdateCountdown();
        work::UpdateFlashlight();
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
    //work::DrawColliders();
    work::DrawPBR();

    if (gl::BeginDrawUI()) {
        if (state.paused) {
            work::DrawMenu();
            work::DrawSliders();
        } else {
            work::DrawUI();
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
    if (key == Key::tab) {
        state.paused = !state.paused;
        work::SetMusicPlaying(!state.paused);
        hub::LockMouse(!state.paused);
        return;
    }
    if (state.paused) return;


    //if (key == Key::v) {
        //work::SwitchCamera();
    //}
    if (key == Key::f) {
        hub::Reg().view<FlashlightComp>().each([](FlashlightComp& flashlight) {
            flashlight.SetTurnedOn(flashlight.enabled < 0.5f);
        });
    }

    if (key == Key::e && state.firstPerson) {
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

static void OnClickPlayButton(Entity entity) {
    State::Get().paused = false;
    work::SetMusicPlaying(true);
    hub::LockMouse(true);
}

static void RestartGame(Entity entity) {
    auto& state = State::Get();
    state.paused = false;
    state.time = 0.0;
    state.gameTime = 50.f;
    state.leftClick = false;
    state.viewProj = Matrix(1.f);
    state.viewPos = {};
    state.firstPerson = true;
    state.flyMode = false;
    state.musicPlaying = false;
    state.currentObject = entt::null;

    hub::Reg().view<comp::Music>().each([](comp::Music& music) {
        sound::StopMusic(music.music);
        sound::PlayMusic(music.music, 20.f);
    });
    work::SetMusicPlaying(true);
    hub::LockMouse(true);
}