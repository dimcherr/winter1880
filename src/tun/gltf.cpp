#include "gltf.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Mesh.h"
#include "comp/Model.h"
#include "comp/TransformComp.h"
#include "comp/Door.h"
#include "comp/MeshAsset.h"
#include "comp/ModelAsset.h"
#include "comp/MaterialGrid.h"
#include "comp/MaterialColor.h"
#include "comp/MaterialPBR.h"
#include "comp/TextureAsset.h"
#include "comp/PointLightComp.h"
#include "comp/BoxShape.h"
#include "comp/BodyComp.h"
#include "prefab/PhysicsSceneLib.h"
#include <unordered_map>
#include "Tags.h"
#include "sokol_gfx.h"
#include "basisu/sokol_basisu.h"

struct ModelDesc {
    String prefix {}; 
    void(*func)(Entity entity, Entity modelAsset);
};

List<ModelDesc> modelDescs {
    {"Decor", [](Entity, Entity) {}},
    {"Static", prefab::StaticBody},
    {"Door", [](Entity entity, Entity modelAsset) { prefab::Door(entity, modelAsset, -80.f); }},
    {"RightMainGate", [](Entity entity, Entity modelAsset) { prefab::MainGate(entity, modelAsset, -80.f); }},
    {"LeftMainGate", [](Entity entity, Entity modelAsset) { prefab::MainGate(entity, modelAsset, 80.f); }},
    {"Collision", prefab::Collision},
    {"SlidingManhole", prefab::SlidingManhole},
    {"GearCW", [](Entity entity, Entity modelAsset) { prefab::Gear(entity, modelAsset, 1.f); }},
    {"GearCCW", [](Entity entity, Entity modelAsset) { prefab::Gear(entity, modelAsset, -1.f); }},
    {"GearCorrect", [](Entity entity, Entity modelAsset) { prefab::GearPickable(entity, modelAsset); }},
    {"GearSlot", [](Entity entity, Entity modelAsset) { prefab::GearSlot(entity, modelAsset, 1.f); }},
};

static void Process(cgltf_data* data);
static void SpawnModel(const cgltf_node& node, Entity parentEntity, std::unordered_map<cgltf_mesh*, Entity>& modelAssets, std::unordered_map<cgltf_primitive*, Entity>& meshAssets);
static void FlattenTransformHierarchy(TransformComp& transform, const TransformComp& parentTransform);
static gltf::Data LoadInternal(StringView path);

void gltf::Load(StringView path) {
    gltf::Data data = LoadInternal(path);
    if (data.data) {
        Process(data.data);
    }
    Close(data.data);
}

void gltf::LoadPrims(StringView path) {
    gltf::Data data = LoadInternal(path);
    if (data.data) {
        for (int i = 0; i < data.data->nodes_count; ++i) {
            auto& node = data.data->nodes[i];
            auto* meshPtr = data.data->nodes[i].mesh;
            if (!meshPtr) continue;
            auto& mesh = *meshPtr;
            for (int k = 0; k < mesh.primitives_count; ++k) {
                auto& prim = mesh.primitives[k];
                cgltf_accessor pos {};
                cgltf_accessor texcoord {};
                for (int j = 0; j < prim.attributes_count; ++j) {
                    switch (prim.attributes[j].type) {
                        case cgltf_attribute_type_position:
                            pos = *prim.attributes[j].data;
                        break;
                        case cgltf_attribute_type_texcoord:
                            texcoord = *prim.attributes[j].data;
                        break;
                        default:
                            // ignore
                        break;
                    }
                }

                List<float> vertices(pos.count * 5);
                float* posPtr = (float*)((Byte*)pos.buffer_view->buffer->data + pos.buffer_view->offset);
                float* texcoordPtr = (float*)((Byte*)texcoord.buffer_view->buffer->data + texcoord.buffer_view->offset);
                for (int v = 0; v < pos.count; ++v) {
                    vertices[v * 5] = posPtr[v * 3];
                    vertices[v * 5 + 1] = posPtr[v * 3 + 1];
                    vertices[v * 5 + 2] = -posPtr[v * 3 + 2];
                    vertices[v * 5 + 3] = texcoordPtr[v * 2];
                    vertices[v * 5 + 4] = texcoordPtr[v * 2 + 1];
                }

                List<uint16_t> indices(prim.indices->count);
                memcpy((void*)indices.data(), (Byte*)prim.indices->buffer_view->buffer->data + prim.indices->buffer_view->offset, prim.indices->buffer_view->size);
                Entity meshEntity = hub::Create()
                    .Add<comp::MeshAsset>()
                        .indexBuffer(indices)
                        .vertexBuffer(vertices)
                        .Next()
                    .GetEntity();

                if (strcmp(node.name, "Sphere") == 0) {
                    hub::Modify(meshEntity).Tag<tag::CapsuleMesh>();
                }
            }
        }
        Close(data.data);
    }
}

void gltf::Close(cgltf_data* gltf) {
    if (gltf) {
        cgltf_free(gltf);
    }
}

static gltf::Data LoadInternal(StringView path) {
    gltf::Data data {};
    data.fileData = tun::ReadFileBinary(path);
    cgltf_options options {};
    if (data.fileData.size() > 0) {
        cgltf_result result = cgltf_parse(&options, (void*)data.fileData.data(), data.fileData.size(), &data.data);
        if (result == cgltf_result_success) {
            StringView jsonData {data.data->json, data.data->json_size};
            tun::WriteFile("glb.json", jsonData);
            cgltf_result bufres = cgltf_load_buffers(&options, data.data, path.data());
            if (bufres == cgltf_result_success) {
                return std::move(data);
            } else {
                tun::error("GLTF ERROR {}", (int)bufres);
                return {};
            }
        } else {
            tun::error("GLTF loading Failed {}", (int)result);
            return {};
        }
    }
    tun::error("GLTF file {} not found", path);
    return {};
}

static void Process(cgltf_data* data) {
    std::unordered_map<cgltf_texture*, Entity> textureAssets {};
    for (int i = 0; i < data->textures_count; ++i) {
        auto& texture = data->textures[i];
        auto& image = data->images[i];
        const cgltf_size size = image.buffer_view->size;
        const cgltf_size offset = image.buffer_view->offset;
        Bytes imageData(size);
        memcpy((void*)imageData.data(), (Byte*)image.buffer_view->buffer->data + offset, size);
        gl::Image img = gl::CreateImage(imageData);
        Entity textureAssetEntity = hub::Create()
            .Add<comp::TextureAsset>().name(image.name).image(img).Next()
            .GetEntity();
        textureAssets[&texture] = textureAssetEntity;
    }

    std::unordered_map<cgltf_material*, Entity> materials {};
    for (int i = 0; i < data->materials_count; ++i) {
        auto& material = data->materials[i];
        Entity materialEntity = hub::Create().GetEntity();
        if (material.pbr_metallic_roughness.metallic_roughness_texture.texture) {
            hub::Modify(materialEntity)
                .Add<comp::MaterialPBR>()
                    .name(material.name)
                    .baseColorTexture(textureAssets[material.pbr_metallic_roughness.base_color_texture.texture])
                    .normalTexture(textureAssets[material.normal_texture.texture])
                    .ormTexture(textureAssets[material.pbr_metallic_roughness.metallic_roughness_texture.texture])
                    .metallicFactor(material.pbr_metallic_roughness.metallic_factor)
                    .roughnessFactor(material.pbr_metallic_roughness.roughness_factor)
                    .normalScale(material.normal_texture.scale)
                    .Next();
            if (material.emissive_texture.texture) {
                hub::Modify(materialEntity)
                    .Modify<comp::MaterialPBR>().emissiveTexture(textureAssets[material.emissive_texture.texture]).emissiveFactor(1.f).Next();
            }
            materials[&material] = materialEntity;
        }
    }

    std::unordered_map<cgltf_mesh*, Entity> modelAssets {};
    std::unordered_map<cgltf_primitive*, Entity> meshAssets {};
    for (int i = 0; i < data->meshes_count; ++i) {
        auto& mesh = data->meshes[i];

        Entity modelEntity = hub::Create()
            .Add<comp::ModelAsset>().Next()
            .GetEntity();

        modelAssets[&mesh] = modelEntity;

        BoundingBox bb {};
        for (int k = 0; k < mesh.primitives_count; ++k) {
            auto& prim = mesh.primitives[k];

            cgltf_accessor pos {};
            cgltf_accessor norm {};
            cgltf_accessor texcoord {};
            cgltf_accessor tangent {};
            for (int j = 0; j < prim.attributes_count; ++j) {
                switch (prim.attributes[j].type) {
                    case cgltf_attribute_type_position:
                        pos = *prim.attributes[j].data;
                    break;
                    case cgltf_attribute_type_normal:
                        norm = *prim.attributes[j].data;
                    break;
                    case cgltf_attribute_type_texcoord:
                        texcoord = *prim.attributes[j].data;
                    break;
                    case cgltf_attribute_type_tangent:
                        tangent = *prim.attributes[j].data;
                    break;
                    default:
                        // ignore
                    break;
                }
            }

            List<float> vertices(pos.count * 12);
            float* posPtr = (float*)((Byte*)pos.buffer_view->buffer->data + pos.buffer_view->offset + pos.offset);
            float* normPtr = (float*)((Byte*)norm.buffer_view->buffer->data + norm.buffer_view->offset + norm.offset);
            float* texcoordPtr = (float*)((Byte*)texcoord.buffer_view->buffer->data + texcoord.buffer_view->offset + texcoord.offset);
            float* tangentPtr = (float*)((Byte*)tangent.buffer_view->buffer->data + tangent.buffer_view->offset + tangent.offset);
            for (int v = 0; v < pos.count; ++v) {
                float x = posPtr[v * 3];
                float y = posPtr[v * 3 + 1];
                float z = -posPtr[v * 3 + 2];
                bb.Stretch({x, y, -z});
                vertices[v * 12] = x;
                vertices[v * 12 + 1] = y;
                vertices[v * 12 + 2] = z;
                vertices[v * 12 + 3] = normPtr[v * 3];
                vertices[v * 12 + 4] = normPtr[v * 3 + 1];
                vertices[v * 12 + 5] = -normPtr[v * 3 + 2];
                vertices[v * 12 + 6] = texcoordPtr[v * 2];
                vertices[v * 12 + 7] = texcoordPtr[v * 2 + 1];
                vertices[v * 12 + 8] = tangentPtr[v * 4];
                vertices[v * 12 + 9] = tangentPtr[v * 4 + 1];
                vertices[v * 12 + 10] = tangentPtr[v * 4 + 2];
                vertices[v * 12 + 11] = tangentPtr[v * 4 + 3];
            }

            List<uint16_t> indices(prim.indices->count);
            memcpy((void*)indices.data(), (Byte*)prim.indices->buffer_view->buffer->data + prim.indices->buffer_view->offset + prim.indices->offset, prim.indices->count * 2);
            Entity meshEntity = hub::Create()
                .Add<comp::MeshAsset>()
                    .modelAsset(modelEntity)
                    .material(materials[prim.material])
                    .indexBuffer(indices)
                    .vertexBuffer(vertices)
                    .Next()
                .GetEntity();

            meshAssets[&prim] = meshEntity;
        }

        Vec offset = (bb.max + bb.min) * 0.5f;
        offset.z *= -1.f;
        hub::Modify(modelEntity)
            .Add<comp::BoxShape>().size(bb.max - bb.min).offset(offset).Next();
    }

    for (int i = 0; i < data->scenes_count; ++i) {
        auto& scene = data->scenes[i];
        for (int k = 0; k < scene.nodes_count; ++k) {
            auto& node = scene.nodes[k];
            SpawnModel(*node, entt::null, modelAssets, meshAssets);
        }
    }

    hub::Reg().view<TransformComp>().each([](TransformComp& transform) {
        if (!hub::Reg().valid(transform.parent)) {
            for (Entity child : transform.children) {
                if (auto* childTransform = hub::Reg().try_get<TransformComp>(child)) {
                    FlattenTransformHierarchy(*childTransform, transform);
                }
            }
        } 
    });
}

static void SpawnModel(const cgltf_node& node, Entity parentEntity, std::unordered_map<cgltf_mesh*, Entity>& modelAssets, std::unordered_map<cgltf_primitive*, Entity>& meshAssets) {
    if (glm::length(Vec {node.translation[0], node.translation[1], -node.translation[2]}) > 300) return;
    Entity modelEntity {};
    if (node.mesh) {
        modelEntity = hub::Create()
            .Add<comp::Model>().modelAsset(modelAssets[node.mesh]).Next()
            .GetEntity();
        auto& modelTransform = hub::AddComp<TransformComp>(modelEntity);
        modelTransform.translation = {node.translation[0], node.translation[1], -node.translation[2]};
        modelTransform.rotation = {node.rotation[3], -node.rotation[0], -node.rotation[1], node.rotation[2]};
        modelTransform.scale = {node.scale[0], node.scale[1], node.scale[2]};
        modelTransform.SetParent(modelEntity, parentEntity);
        modelTransform.Update();

        if (node.name) {
            StringView nodeName = StringView(node.name);
            for (auto& modelDesc : modelDescs) {
                if (nodeName.starts_with(modelDesc.prefix)) {
                    modelDesc.func(modelEntity, modelAssets[node.mesh]);
                    break;
                }
            }
        }

        for (int i = 0; i < node.mesh->primitives_count; ++i) {
            auto& prim = node.mesh->primitives[i];
            Entity meshAssetEntity = meshAssets[&prim];
            const auto& parentMaterial = hub::Reg().get<comp::MaterialPBR>(hub::Reg().get<comp::MeshAsset>(meshAssetEntity).material);

            Entity meshEntity = hub::Create()
                .Add<comp::Mesh>().asset(meshAssetEntity).model(modelEntity).Next()
                .GetEntity();
            hub::Reg().emplace<comp::MaterialPBR>(meshEntity, parentMaterial);
        }
    } else if (node.light) {
        Color colors[] {
            Color(1.f, 1.f, 1.f),
            Color(1.f, 1.f, 1.f),
            Color(1.f, 1.f, 1.f),
            Color(1.f, 1.f, 1.f),
            //Color(0.5f, 0.5f, 1.f),
            //Color(1.f, 0.5f, 0.5f),
            //Color(1.f, 0.5f, 1.f),
        };
        static int lightIndex = 0;
        ++lightIndex;
        modelEntity = hub::Create()
            .Add<comp::MaterialColor>()
                .color(colors[lightIndex % 4])
                .Next()
            .GetEntity();
        auto& lightTransform = hub::AddComp<TransformComp>(modelEntity);
        lightTransform.translation = {node.translation[0], node.translation[1], -node.translation[2]};
        lightTransform.rotation = {node.rotation[3], -node.rotation[0], -node.rotation[1], node.rotation[2]};
        lightTransform.scale = {node.scale[0], node.scale[1], node.scale[2]};
        lightTransform.SetParent(modelEntity, parentEntity);
        lightTransform.Update();

        auto& pointLight = hub::AddComp<PointLightComp>(modelEntity);
        pointLight.color = colors[lightIndex % 4];
        pointLight.intensity = node.light->intensity * 0.00001f;
        pointLight.range = 5.f;
    }
    
    for (int i = 0; i < node.children_count; ++i) {
        auto& child = node.children[i];
        SpawnModel(*child, modelEntity, modelAssets, meshAssets);
    }
}

static void FlattenTransformHierarchy(TransformComp& transform, const TransformComp& parentTransform) {
    transform.transform = parentTransform.transform * transform.transform;
    transform.translation = parentTransform.translation + transform.translation;
    transform.scale = parentTransform.scale * transform.scale;
    transform.rotation = parentTransform.rotation * transform.rotation;
    for (Entity child : transform.children) {
        if (auto* childTransform = hub::Reg().try_get<TransformComp>(child)) {
            FlattenTransformHierarchy(*childTransform, transform);
        }
    }
}