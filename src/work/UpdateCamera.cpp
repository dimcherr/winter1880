#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Camera.h"
#include "comp/TransformComp.h"
#include "Tags.h"

void work::UpdateCamera() {
    using comp::Camera;

    auto [camera, transform] = hub::Reg().get<Camera, TransformComp>(hub::Reg().view<tag::Current, Camera, TransformComp>().back());
    const auto& screenSize = hub::GetScreenSize();
    camera.Update(screenSize.x, screenSize.y);
    transform.rotation = Quat({camera.pitch, camera.yaw, 0.f});
    transform.Update();
    hub::SetViewProj(camera.projection * tun::LookAt(transform.translation + camera.offset, transform.rotation));
    hub::SetViewPos(transform.translation + camera.offset);
}
