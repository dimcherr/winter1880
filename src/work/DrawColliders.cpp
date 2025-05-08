#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/gl.h"
#include "comp/TransformComp.h"
#include "comp/MaterialGrid.h"
#include "comp/Mesh.h"
#include "comp/MeshAsset.h"
#include "comp/BoxShape.h"
#include "comp/CapsuleShape.h"
#include "comp/BodyComp.h"
#include "Tags.h"
#include "grid.glsl.h"

void work::DrawColliders() {
    using comp::Mesh;
    using comp::MeshAsset;
    using comp::BoxShape;
    using comp::CapsuleShape;
    using comp::MaterialGrid;

    gl::UseGridMaterial();
    
    hub::Reg().view<BodyComp, BoxShape, TransformComp>().each([](const BodyComp& body, const BoxShape& shape, const TransformComp& transform) {
        auto& state = gl::State();
        const auto& viewProj = hub::GetViewProj();
        Matrix off = glm::translate({1.f}, shape.offset);
        Matrix ss = glm::scale({1.f}, shape.size);
        Matrix t = glm::translate({1.f}, transform.translation);
        Matrix r = glm::mat4_cast(transform.rotation);
        Matrix s = glm::scale({1.f}, transform.scale);
        Matrix m = t * r * s * off * ss;
        state.gridMaterial.vsParams.mvp = viewProj * m;

        if (body.layer == phys::Layers::moving) {
            state.gridMaterial.fsParams.color = tun::red;
        } else {
            state.gridMaterial.fsParams.color = tun::blue;
        }

        float maxSideLength = glm::max(shape.size.x * transform.scale.x, glm::max(shape.size.y * transform.scale.y, shape.size.z * transform.scale.z));
        state.gridMaterial.fsParams.segmentCount = (int)glm::round(maxSideLength / 0.2f);
        state.gridMaterial.fsParams.segmentSize = 0.2f;

        auto& meshAsset = hub::Reg().get<comp::MeshAsset>(hub::Reg().view<tag::CubeMesh>().back());
        gl::UseMesh(meshAsset.vertexBuffer, meshAsset.indexBuffer, meshAsset.elementCount);
        gl::UpdateGridMaterial();
        gl::Draw();
    });

    hub::Reg().view<CapsuleShape, TransformComp>().each([](const CapsuleShape& shape, const TransformComp& transform) {
        auto& state = gl::State();
        const auto& viewProj = hub::GetViewProj();
        Matrix t = glm::translate({1.f}, transform.translation);
        Matrix s = glm::scale({1.f}, transform.scale);
        Matrix ss = glm::scale({1.f}, Vec(shape.radius, (shape.halfHeight + shape.radius) * 2.f, shape.radius));
        Matrix m = t * s * ss;
        state.gridMaterial.vsParams.mvp = viewProj * m;
        state.gridMaterial.fsParams.color = Vec4(tun::red, 1.f);

        float maxSideLength = glm::max(shape.radius * transform.scale.x, glm::max(shape.radius * transform.scale.y, shape.radius * transform.scale.z));
        state.gridMaterial.fsParams.segmentCount = (int)glm::round(maxSideLength * 30.f);
        state.gridMaterial.fsParams.segmentSize = 0.2f;

        auto& meshAsset = hub::Reg().get<comp::MeshAsset>(hub::Reg().view<tag::CapsuleMesh>().back());
        gl::UseMesh(meshAsset.vertexBuffer, meshAsset.indexBuffer, meshAsset.elementCount);
        gl::UpdateGridMaterial();
        gl::Draw();
    });
}
