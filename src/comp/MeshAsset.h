#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"
#include "tun/gl.h"
#include "tun/list.h"

namespace comp {

struct MeshAsset {
    Entity modelAsset {};
    Entity material {};
    gl::Buffer vertexBuffer {};
    gl::Buffer indexBuffer {};
    int elementCount {};
};

}

template <>
struct CompBuilder<comp::MeshAsset> : public BaseCompBuilder<comp::MeshAsset> {
    CompBuilder& material(Entity material) {
        comp->material = material;
        return *this;
    }

    CompBuilder& modelAsset(Entity modelAsset) {
        comp->modelAsset = modelAsset;
        return *this;
    }

    CompBuilder& vertexBuffer(const List<float>& vertices) {
        comp->vertexBuffer = gl::CreateVertexBuffer(vertices);
        return *this;
    }

    CompBuilder& indexBuffer(const List<uint16_t>& indices) {
        comp->indexBuffer = gl::CreateIndexBuffer(indices);
        comp->elementCount = indices.size();
        return *this;
    }
};

template <>
struct std::formatter<comp::MeshAsset> : std::formatter<std::string> {
    auto format(comp::MeshAsset mesh, format_context& ctx) const {
        return formatter<string>::format(
            std::format("[MeshAsset VB={}, IB={}, Count={}, Model={}, Material={}]",
                mesh.vertexBuffer, mesh.indexBuffer, mesh.elementCount, mesh.modelAsset, mesh.material), ctx);
    }
};