#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/gl.h"
#include "comp/Transform.h"
#include "comp/MaterialColor.h"
#include "comp/Mesh.h"
#include "comp/MeshAsset.h"
#include "Tags.h"
#include "grid.glsl.h"

void work::DrawColored() {
    using comp::Mesh;
    using comp::MeshAsset;
    using comp::Transform;
    using comp::MaterialColor;

    gl::UseColorMaterial();
    
    hub::Reg().view<MaterialColor, Mesh, Transform>().each([](const MaterialColor& material, const Mesh& mesh, const Transform& transform) {
        auto& state = gl::State();
        const auto& viewProj = hub::GetViewProj();
        state.colorMaterial.vsParams.mvp = viewProj * transform.transform;
        state.colorMaterial.fsParams.color = Vec4(material.color, 1.f);

        auto& meshAsset = hub::Reg().get<comp::MeshAsset>(mesh.asset);
        gl::UseMesh(meshAsset.vertexBuffer, meshAsset.indexBuffer, meshAsset.elementCount);
        gl::UpdateColorMaterial();
        gl::Draw();
    });
}
