#include "Hub.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "sokol_app.h"

Entity hub::AddEntity() {
    return State::Get().reg.create();
}

EntityBuilder hub::Create() {
    return EntityBuilder();
}

EntityBuilder hub::Modify(Entity entity) {
    return EntityBuilder(entity);
}

entt::registry& hub::Reg() {
    return State::Get().reg;
}

double hub::GetDeltaTime() {
    return sapp_frame_duration();
}

double hub::GetTime() {
    return State::Get().time;
}

void hub::UpdateTime() {
    State::Get().time += hub::GetDeltaTime();
    if (!State::Get().paused) {
        State::Get().gameTime += hub::GetDeltaTime();
    }
}

Vec2 hub::GetScreenSize() {
    return {sapp_widthf(), sapp_heightf()};
}

void hub::SetViewProj(const Matrix& viewProj) {
    State::Get().viewProj = viewProj;
}

Matrix hub::GetViewProj() {
    return State::Get().viewProj;
}

void hub::SetViewPos(const Vec& viewPos) {
    State::Get().viewPos = viewPos;
}

Vec hub::GetViewPos() {
    return State::Get().viewPos;
}

bool hub::IsKeyPressed(Key key) {
    return State::Get().keys[(int)key];
}

bool hub::IsMouseButtonPressed(MouseButton mouseButton) {
    return State::Get().mouse[(int)mouseButton];
}

Vec2 hub::GetMouseDelta() {
    const auto& state = State::Get();
    return {state.mouseDeltaX, state.mouseDeltaY};
}

void hub::LockMouse(bool locked) {
    sapp_lock_mouse(locked);
    sapp_show_mouse(!locked);
}
