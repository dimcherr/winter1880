#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/gl.h"
#include "comp/Transform.h"
#include "comp/MaterialGrid.h"
#include "comp/Mesh.h"
#include "comp/MeshAsset.h"
#include "grid.glsl.h"

void work::DrawGrid() {
    using comp::Mesh;
    using comp::MeshAsset;
    using comp::MaterialGrid;
    using comp::Transform;

    gl::UseGridMaterial();
    
    hub::Reg().view<Mesh, MaterialGrid, Transform>().each([](const Mesh& mesh, const MaterialGrid& material, const Transform& transform) {
        auto& state = gl::State();
        const auto& viewProj = hub::GetViewProj();
        state.gridMaterial.vsParams.mvp = viewProj * transform.transform;
        state.gridMaterial.fsParams.segmentSize = material.segmentSize;
        state.gridMaterial.fsParams.segmentCount = material.segmentCount;
        state.gridMaterial.fsParams.color = material.color;

        auto& meshAsset = hub::Get<MeshAsset>(mesh.asset);
        gl::UseMesh(meshAsset.vertexBuffer, meshAsset.indexBuffer, meshAsset.elementCount);
        gl::UpdateGridMaterial();
        gl::Draw();
    });
}
