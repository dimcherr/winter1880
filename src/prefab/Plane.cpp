#include "Prefab.h"
#include "comp/MeshAsset.h"
#include "Tags.h"

Entity prefab::Plane() {
    return hub::Create()
        .Add<comp::MeshAsset>().vertexBuffer({
            -0.5f,  0.5f,  0.0f,    0.0f,  1.0f,
            0.5f,  0.5f,  0.0f,    1.0f,  1.0f,
            0.5f, -0.5f,  0.0f,    1.0f,  0.0f, 
            -0.5f, -0.5f,  0.0f,    0.0f,  0.0f,
        }).indexBuffer({
            0, 1, 2,
            0, 2, 3,
        }).Next()
        .Tag<tag::PlaneMesh>()
        .GetEntity();
}
