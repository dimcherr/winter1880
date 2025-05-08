#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Camera.h"
#include "comp/Transform.h"
#include "Tags.h"

void work::UpdateCameraRotation() {
    using comp::Camera;
    using comp::Transform;
    const auto mouseDelta = hub::GetMouseDelta();
    auto [camera, transform] = hub::Reg().get<Camera, Transform>(hub::Reg().view<tag::Current, Camera, Transform>().back());
    camera.yaw += mouseDelta.x * camera.rotationSensitivity * State::Get().sensitivityFactor * hub::GetDeltaTime();
    camera.pitch += mouseDelta.y * camera.rotationSensitivity * State::Get().sensitivityFactor * hub::GetDeltaTime();
    camera.pitch = glm::clamp(camera.pitch, glm::radians(-60.f), glm::radians(60.f));

}
