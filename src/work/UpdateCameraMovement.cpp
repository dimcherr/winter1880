#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Camera.h"
#include "comp/TransformComp.h"
#include "Tags.h"

void work::UpdateCameraMovement() {
    using comp::Camera;
    auto [camera, transform] = hub::Reg().get<Camera, TransformComp>(hub::Reg().view<tag::Current, Camera, TransformComp>().back());
    if (hub::IsKeyPressed(Key::w)) {
        transform.translation += transform.rotation * tun::forward * camera.movementSpeed * (float)hub::GetDeltaTime();
    }
    if (hub::IsKeyPressed(Key::s)) {
        transform.translation += transform.rotation * tun::back * camera.movementSpeed * (float)hub::GetDeltaTime();
    }
    if (hub::IsKeyPressed(Key::a)) {
        transform.translation += transform.rotation * tun::left * camera.movementSpeed * (float)hub::GetDeltaTime();
    }
    if (hub::IsKeyPressed(Key::d)) {
        transform.translation += transform.rotation * tun::right * camera.movementSpeed * (float)hub::GetDeltaTime();
    }
    if (hub::IsKeyPressed(Key::e)) {
        transform.translation += transform.rotation * tun::up * camera.movementSpeed * (float)hub::GetDeltaTime();
    }
    if (hub::IsKeyPressed(Key::q)) {
        transform.translation += transform.rotation * tun::down * camera.movementSpeed * (float)hub::GetDeltaTime();
    }
}
