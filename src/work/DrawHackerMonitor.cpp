#include "Work.h"
#include "tun/math.h"
#include "tun/builder.h"
#include "comp/TextWidget.h"
#include "comp/ButtonWidget.h"
#include "comp/BoundsWidget.h"
#include "comp/Font.h"
#include "comp/MeshAsset.h"
#include "comp/HackerKey.h"
#include "comp/HackerBrick.h"
#include "comp/Camera.h"
#include "comp/Transform.h"
#include "comp/BrickWall.h"
#include "tun/tunstring.h"
#include "Tags.h"

void work::DrawHackerMonitor() {
    using comp::TextWidget;
    using comp::ButtonWidget;
    using comp::BoundsWidget;
    using comp::Font;
    using comp::HackerKey;
    using comp::HackerBrick;
    using comp::Camera;
    using comp::Transform;

    gl::UseDrawMaterial();

    hub::Reg().view<HackerBrick, tag::Hacker>().each([](HackerBrick& brick) {
        auto& state = gl::State();
        auto* brickWall = hub::Reg().try_get<comp::BrickWall>(State::Get().currentObject);
        if (!brickWall) return;

        Matrix projection = glm::perspective(glm::radians(45.f), hub::GetScreenSize().x / hub::GetScreenSize().y, 0.1f, 100.f);
        Vec target = tun::vecZero;
        static float tt = 0.f;
        tt += hub::GetDeltaTime() * 0.01f;
        float cx = glm::sin(tt);
        float cy = glm::cos(tt);
        Vec cameraPos = Vec(cx, 0.2f, cy);
        Vec2 pitchYaw = tun::LookAtPitchYaw(cameraPos, target);
        Quat q = Quat({pitchYaw.x, pitchYaw.y, 0.f});
        Matrix view = tun::LookAt(cameraPos, q);
        Matrix viewProj = projection * view;

        comp::Transform transform {};
        transform.rotation = tun::quatIdentity;
        transform.translation = Vec((brick.x - 5) * 0.025f, brick.y * 0.025f, 0.f);
        transform.scale = Vec(0.02f, 0.02f, 0.02f);
        transform.Update();
        Matrix offset = glm::translate(Matrix(1.f), Vec(0.f, 0.25f, 0.f));
        state.drawMaterial.vsParams.mvp = offset * viewProj * transform.transform;
        state.drawMaterial.vsParams.displaceFactor = 0.f;
        int brickIndex = brick.x + brick.y * 10;
        if (brickIndex < brickWall->progress) {
            state.drawMaterial.fsParams.color = Vec4(tun::green, 0.75f);
        } else {
            state.drawMaterial.fsParams.color = Vec4(tun::red, 0.75f);
        }

        auto& meshAsset = hub::Reg().get<comp::MeshAsset>(hub::Reg().view<tag::CubeMesh>().back());
        gl::UseMesh(meshAsset.vertexBuffer, meshAsset.indexBuffer, meshAsset.elementCount);
        gl::UpdateDrawMaterial();
        gl::Draw();


        static float scaleT = 0.f;
        scaleT += hub::GetDeltaTime() * 0.02f;
        float charScale = glm::sin(scaleT) * 0.01f;
        auto& wolfMeshAsset = hub::Reg().get<comp::MeshAsset>(hub::Reg().view<tag::WolfSharpMesh>().back());
        comp::Transform wolfTransform {};
        wolfTransform.rotation = tun::quatIdentity;
        wolfTransform.translation = Vec(0.f, 0.1f, 0.15f);
        wolfTransform.scale = Vec(0.1f + charScale, 0.1f + charScale, 0.1f + charScale);
        wolfTransform.Update();
        state.drawMaterial.vsParams.mvp = offset * viewProj * wolfTransform.transform;
        state.drawMaterial.fsParams.color = Vec4(tun::red, 0.75f);
        state.drawMaterial.vsParams.displaceFactor = 0.f;
        gl::UseMesh(wolfMeshAsset.vertexBuffer, wolfMeshAsset.indexBuffer, wolfMeshAsset.elementCount);
        gl::UpdateDrawMaterial();
        gl::Draw();

        auto& pigletMeshAsset = hub::Reg().get<comp::MeshAsset>(hub::Reg().view<tag::PigletMesh>().back());
        comp::Transform pigletTransform {};
        pigletTransform.rotation = Quat({0.f, tun::pi, 0.f});
        pigletTransform.translation = Vec(0.f, 0.1f, -0.15f);
        pigletTransform.scale = Vec(0.07f + charScale, 0.07f + charScale, 0.07f + charScale);
        pigletTransform.Update();
        state.drawMaterial.vsParams.mvp = offset * viewProj * pigletTransform.transform;
        state.drawMaterial.fsParams.color = Vec4(tun::green, 0.75f);
        state.drawMaterial.vsParams.displaceFactor = 0.f;
        gl::UseMesh(pigletMeshAsset.vertexBuffer, pigletMeshAsset.indexBuffer, pigletMeshAsset.elementCount);
        gl::UpdateDrawMaterial();
        gl::Draw();
    });
}
