#include "Prefab.h"
#include "comp/Camera.h"
#include "comp/Transform.h"
#include "comp/MaterialGrid.h"
#include "comp/Mesh.h"
#include "comp/MeshAsset.h"
#include "Tags.h"

Entity prefab::Grid(float segmentSize, int segmentCount, const Color& color) {
    float scale = segmentSize * segmentCount;
    Entity planeMesh = GETOneEntity(tag::PlaneMesh);
    return hub::Create()
        .Add<comp::Transform>().pos(tun::vecZero).rot(-90.f, 0.f, 0.f).scale(scale).update().Next()
        .Add<comp::MaterialGrid>().segmentSize(scale).segmentCount(segmentCount).color(color).Next()
        .Add<comp::Mesh>().asset(planeMesh).Next()
        .GetEntity();
}
