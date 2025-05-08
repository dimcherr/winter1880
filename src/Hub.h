#pragma once
#include "tun/entity.h"
#include "tun/tunstring.h"
#include "tun/math.h"
#include "tun/input.h"
#include "State.h"

class EntityBuilder;

namespace hub {

Entity AddEntity();
EntityBuilder Create();
EntityBuilder Modify(Entity entity);
entt::registry& Reg();
double GetDeltaTime();
double GetTime();
Vec2 GetScreenSize();
void SetViewProj(const Matrix& viewProj);
Matrix GetViewProj();
void SetViewPos(const Vec& viewPos);
Vec GetViewPos();
bool IsKeyPressed(Key key);
bool IsMouseButtonPressed(MouseButton mouseButton);
Vec2 GetMouseDelta();
void LockMouse(bool locked);
void UpdateTime();

template <typename T>
T& AddComp(Entity entity, const T& comp) {
    return State::Get().reg.emplace<T>(entity, comp);
}

template <typename T>
T& AddComp(Entity entity) {
    return State::Get().reg.emplace<T>(entity);
}

template <typename T>
T& GetComp(Entity entity) {
    return State::Get().reg.get<T>(entity);
}

template <typename T>
void AddTag(Entity entity) {
    State::Get().reg.emplace<T>(entity);
}

template<typename ...T>
decltype(auto) Get(Entity entity) {
    auto& reg = hub::Reg();
    if constexpr(sizeof...(T) == 1u) {
        return (reg.get<T>(entity), ...);
    } else {
        return std::forward_as_tuple(reg.get<T>(entity)...);
    }
}

}

#define GET State::Get().reg.get
#define GETOneEntity(__viewClasses__) State::Get().reg.view<__viewClasses__>().back()
#define GETOne(__classes__, __viewClasses__) State::Get().reg.get<__classes__>(State::Get().reg.view<__viewClasses__>().back())
#define TRYGETOne(__classes__, __viewClasses__) State::Get().reg.try_get<__classes__>(State::Get().reg.view<__viewClasses__>().back())
