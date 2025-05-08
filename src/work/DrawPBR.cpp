#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/gl.h"
#include "comp/TransformComp.h"
#include "comp/MaterialPBR.h"
#include "comp/Mesh.h"
#include "comp/MeshAsset.h"
#include "comp/TextureAsset.h"
#include "comp/Model.h"
#include "comp/ModelAsset.h"
#include "comp/PointLightComp.h"
#include "pbr.glsl.h"

void work::DrawPBR() {
    using comp::Mesh;
    using comp::Model;
    using comp::ModelAsset;
    using comp::MeshAsset;
    using comp::MaterialPBR;
    using comp::TextureAsset;

    gl::UsePBRMaterial();

    hub::Reg().view<Mesh, MaterialPBR>().each([](Entity entity, const Mesh& mesh, const MaterialPBR& material) {
        auto& state = gl::State();
        const auto& viewProj = hub::GetViewProj();
        const auto& viewPos = hub::GetViewPos();
        auto& transform = hub::Reg().get<TransformComp>(mesh.model);
        auto& model = hub::Reg().get<comp::Model>(mesh.model);
        if (!model.active || !model.visible) return;

        state.pbrMaterial.vsParams.mvp = viewProj * transform.transform;
        state.pbrMaterial.vsParams.matModel = transform.transform;
        state.pbrMaterial.vsParams.matNormal = glm::transpose(glm::inverse(transform.transform));

        state.pbrMaterial.fsParams.viewPos = viewPos;
        state.pbrMaterial.fsParams.tint = model.tint;
        state.pbrMaterial.fsParams.highlight = model.highlight;
        state.pbrMaterial.fsParams.ambientColor = tun::black;
        state.pbrMaterial.fsParams.ambientFactor = 0.001f;
        state.pbrMaterial.fsParams.metallicFactor = 1.f;
        state.pbrMaterial.fsParams.roughnessFactor = 1.f;

        state.pbrMaterial.fsParams.offset = {0.f, 0.f};
        state.pbrMaterial.fsParams.tiling = {1.f, 1.f};
        state.pbrMaterial.fsParams.time = hub::GetTime() * 0.3f;

        state.pbrMaterial.fsParams.pointLightCount = 0;
        int pointLightIndex = 0;
        hub::Reg().view<PointLightComp, TransformComp>().each([&pointLightIndex, &entity](Entity lightEntity, PointLightComp& light, TransformComp& lightTransform) {
            auto& state = gl::State();
            if (pointLightIndex < 30) {
                state.pbrMaterial.fsParams.pointLightColor[pointLightIndex] = Vec4(light.color, light.intensity);
                state.pbrMaterial.fsParams.pointLightPos[pointLightIndex] = Vec4(lightTransform.translation, light.range);
                ++state.pbrMaterial.fsParams.pointLightCount;
            }
            ++pointLightIndex;
        });

        state.bindings.samplers[SMP_smp] = state.pbrMaterial.sampler;
        state.bindings.images[IMG_albedoMap] = hub::Reg().get<TextureAsset>(material.baseColorTexture).image;
        state.bindings.images[IMG_normalMap] = hub::Reg().get<TextureAsset>(material.normalTexture).image;
        state.bindings.images[IMG_ormMap] = hub::Reg().get<TextureAsset>(material.ormTexture).image;
        state.bindings.images[IMG_cloudTex] = hub::Reg().get<TextureAsset>(State::Get().cloudTexture).image;

        auto& meshAsset = hub::Get<MeshAsset>(mesh.asset);
        gl::UseMesh(meshAsset.vertexBuffer, meshAsset.indexBuffer, meshAsset.elementCount);
        gl::UpdatePBRMaterial();
        gl::Draw();
    });
}
