#include "Prefab.h"
#include "comp/Camera.h"
#include "comp/TransformComp.h"
#include "Tags.h"

Entity prefab::CameraFly(const Vec& position, const Vec& target) {
    const Vec2 angles = tun::LookAtPitchYaw(position, target);
    const Vec2 screenSize = hub::GetScreenSize();
    Entity entity = hub::Create()
        .Add<comp::Camera>().rotationSensitivity(0.5f).rotation(angles.x, angles.y).update(screenSize.x, screenSize.y).Next()
        .Tag<tag::Fly>()
        .GetEntity();
    
    auto& transform = hub::AddComp<TransformComp>(entity);
    transform.translation = position;
    transform.Update();

    return entity;
}
