#include "Prefab.h"
#include "comp/MeshAsset.h"
#include "Tags.h"

Entity prefab::Rect() {
    return hub::Create()
        .Add<comp::MeshAsset>().vertexBuffer({
            -1.f,  1.f,  0.0f,    0.0f,  1.0f,
            1.f,  1.f,  0.0f,    1.0f,  1.0f,
            1.f, -1.f,  0.0f,    1.0f,  0.0f, 
            -1.f, -1.f,  0.0f,    0.0f,  0.0f,
        }).indexBuffer({
            0, 1, 2,
            0, 2, 3,
        }).Next()
        .Tag<tag::RectMesh>()
        .GetEntity();
}
