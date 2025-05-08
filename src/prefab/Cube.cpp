#include "Prefab.h"
#include "comp/MeshAsset.h"
#include "Tags.h"

Entity prefab::Cube() {
    return hub::Create()
        .Add<comp::MeshAsset>().vertexBuffer({
            // front face
            -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,    1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,    1.0f,  0.0f, 
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,

            // back face
            0.5f, 0.5f,  -0.5f,    0.0f,  0.0f,
            -0.5f, 0.5f,  -0.5f,    1.0f,  0.0f, 
            -0.5f,  -0.5f,  -0.5f,    1.0f,  1.0f,
            0.5f,  -0.5f,  -0.5f,    0.0f,  1.0f,

            // left face
            -0.5f,  0.5f,  -0.5f,    0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,    1.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,    1.0f,  0.0f, 
            -0.5f, -0.5f,  -0.5f,    0.0f,  0.0f,

            // right face
            0.5f,  0.5f,  0.5f,    0.0f,  1.0f,
            0.5f,  0.5f,  -0.5f,    1.0f,  1.0f,
            0.5f, -0.5f,  -0.5f,    1.0f,  0.0f, 
            0.5f, -0.5f,  0.5f,    0.0f,  0.0f,

            // top face
            -0.5f,  0.5f,  -0.5f,    0.0f,  1.0f,
            0.5f,  0.5f,  -0.5f,    1.0f,  1.0f,
            0.5f, 0.5f,  0.5f,    1.0f,  0.0f, 
            -0.5f, 0.5f,  0.5f,    0.0f,  0.0f,

            // bottom face
            0.5f,  -0.5f,  0.5f,    0.0f,  1.0f,
            0.5f,  -0.5f,  -0.5f,    1.0f,  1.0f,
            -0.5f, -0.5f,  -0.5f,    1.0f,  0.0f, 
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,
        }).indexBuffer({
            0, 1, 2,
            0, 2, 3,

            4, 5, 6,
            4, 6, 7,

            8, 9, 10,
            8, 10, 11,

            12, 13, 14,
            12, 14, 15,

            16, 17, 18,
            16, 18, 19,

            20, 21, 22,
            20, 22, 23,
        }).Next()
        .Tag<tag::CubeMesh>()
        .GetEntity();
}
