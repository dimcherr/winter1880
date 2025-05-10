#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Camera.h"
#include "comp/TransformComp.h"
#include "Tags.h"

void work::UpdateCameraRotation() {
    using comp::Camera;
    const auto mouseDelta = hub::GetMouseDelta();
    auto [camera, transform] = hub::Reg().get<Camera, TransformComp>(hub::Reg().view<tag::Current, Camera, TransformComp>().back());
    camera.yaw += mouseDelta.x * camera.rotationSensitivity * State::Get().sensitivityFactor * hub::GetDeltaTime();
    camera.pitch += mouseDelta.y * camera.rotationSensitivity * State::Get().sensitivityFactor * hub::GetDeltaTime();

    if (camera.minPitch != 0.f || camera.maxPitch != 0.f) {
        camera.pitch = glm::clamp(camera.pitch, glm::radians(camera.minPitch), glm::radians(camera.maxPitch));
    }

    if (camera.minYaw != 0.f || camera.maxYaw != 0.f) {
        camera.yaw = glm::clamp(camera.yaw, glm::radians(camera.minYaw), glm::radians(camera.maxYaw));
    }
}
