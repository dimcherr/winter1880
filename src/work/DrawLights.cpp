#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/gl.h"
#include "comp/Transform.h"
#include "comp/MaterialColor.h"
#include "comp/Mesh.h"
#include "comp/MeshAsset.h"
#include "comp/PointLight.h"
#include "Tags.h"
#include "grid.glsl.h"

void work::DrawLights() {
    using comp::Mesh;
    using comp::MeshAsset;
    using comp::PointLight;
    using comp::Transform;
    using comp::MaterialColor;

    gl::UseColorMaterial();
    
    hub::Reg().view<PointLight, MaterialColor, Transform>().each([](const PointLight& light, const MaterialColor& material, const Transform& transform) {
        auto& state = gl::State();
        const auto& viewProj = hub::GetViewProj();
        state.colorMaterial.vsParams.mvp = viewProj * transform.transform;
        state.colorMaterial.fsParams.color = Vec4(material.color, 1.f);

        auto& meshAsset = hub::Reg().get<comp::MeshAsset>(hub::Reg().view<tag::PlaneMesh>().back());
        gl::UseMesh(meshAsset.vertexBuffer, meshAsset.indexBuffer, meshAsset.elementCount);
        gl::UpdateColorMaterial();
        gl::Draw();
    });
}
