#include "Prefab.h"
#include "comp/Camera.h"
#include "comp/TransformComp.h"
#include "comp/MaterialGrid.h"
#include "comp/Mesh.h"
#include "comp/MeshAsset.h"
#include "Tags.h"

Entity prefab::Grid(float segmentSize, int segmentCount, const Color& color) {
    float scale = segmentSize * segmentCount;
    Entity planeMesh = hub::Reg().view<tag::PlaneMesh>().back();
    Entity entity = hub::Create()
        .Add<comp::MaterialGrid>().segmentSize(scale).segmentCount(segmentCount).color(color).Next()
        .Add<comp::Mesh>().asset(planeMesh).Next()
        .GetEntity();

    auto& transform = hub::AddComp<TransformComp>(entity);
    transform.SetRotationDegrees(-90.f, 0.f, 0.f);
    transform.scale = Vec(scale, scale, scale);
    transform.Update();

    return entity;
}
