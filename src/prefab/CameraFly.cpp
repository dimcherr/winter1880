#include "Prefab.h"
#include "comp/Camera.h"
#include "comp/Transform.h"
#include "Tags.h"

Entity prefab::CameraFly(const Vec& position, const Vec& target) {
    const Vec2 angles = tun::LookAtPitchYaw(position, target);
    const Vec2 screenSize = hub::GetScreenSize();
    return hub::Create()
        .Add<comp::Camera>().rotationSensitivity(0.5f).rotation(angles.x, angles.y).update(screenSize.x, screenSize.y).Next()
        .Add<comp::Transform>().pos(position).update().Next()
        .Tag<tag::Fly>()
        .GetEntity();
}
