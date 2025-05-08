#include "Work.h"
#include "tun/math.h"
#include "tun/builder.h"
#include "comp/TextWidget.h"
#include "comp/ButtonWidget.h"
#include "comp/BoundsWidget.h"
#include "comp/Font.h"
#include "comp/MeshAsset.h"
#include "comp/HackerKey.h"
#include "tun/tunstring.h"
#include "comp/Healthbar.h"
#include "Tags.h"

void work::DrawHealthbar() {
    using comp::TextWidget;
    using comp::ButtonWidget;
    using comp::BoundsWidget;
    using comp::Font;
    using comp::HackerKey;

    hub::Reg().view<BoundsWidget, comp::Healthbar>().each([](BoundsWidget& bounds, comp::Healthbar& healthbar) {
        if (!bounds.visible) return;

        auto& state = State::Get();
        auto screenSize = hub::GetScreenSize();
        Vec2 panelSize {bounds.minSize.x, bounds.minSize.y};

        Vec2 anchorOffset {};
        Vec2 panelAnchorOffset {};
        Vec2 parentAnchorOffset {};
        switch(bounds.parentAnchors.horizontal) {
            case tun::begin:
                parentAnchorOffset.x = 0.f;
            break;
            case tun::center:
                parentAnchorOffset.x = screenSize.x * 0.5f;
            break;
            case tun::end:
                parentAnchorOffset.x = screenSize.x * 1.f;
            break;
        }

        switch(bounds.parentAnchors.vertical) {
            case tun::begin:
                parentAnchorOffset.y = 0.f;
            break;
            case tun::center:
                parentAnchorOffset.y = screenSize.y * 0.5f;
            break;
            case tun::end:
                parentAnchorOffset.y = screenSize.y * 1.f;
            break;
        }

        switch(bounds.anchors.horizontal) {
            case tun::begin:
                panelAnchorOffset.x = 0.f;
            break;
            case tun::center:
                panelAnchorOffset.x = panelSize.x * -0.5f;
            break;
            case tun::end:
                panelAnchorOffset.x = panelSize.x * -1.f;
            break;
        }

        switch(bounds.anchors.vertical) {
            case tun::begin:
                panelAnchorOffset.y = 0.f;
            break;
            case tun::center:
                panelAnchorOffset.y = panelSize.y * -0.5f;
            break;
            case tun::end:
                panelAnchorOffset.y = panelSize.y * -1.f;
            break;
        }

        Vec2 panelPos {parentAnchorOffset.x + bounds.pos.x + panelAnchorOffset.x, parentAnchorOffset.y + bounds.pos.y + panelAnchorOffset.y};

        static float scaleTime = 0.f;
        scaleTime += hub::GetDeltaTime() * 2.4f;
        float scaleAnim = glm::sin(scaleTime) * 0.002f;

        gl::UseDrawMaterial();
        Entity rectMeshEntity = hub::Reg().view<tag::RectMesh>().back();
        auto& rectMesh = hub::Reg().get<comp::MeshAsset>(rectMeshEntity);
        Matrix sm = glm::scale(Matrix(1.f), Vec3((panelSize.x / screenSize.x) + scaleAnim, (panelSize.y / screenSize.y) + scaleAnim, 1.f));
        Matrix tm = glm::translate(Matrix(1.f), Vec3(((panelPos.x + panelSize.x * 0.5f) * 2.f) / screenSize.x - 1.f, -((panelPos.y + panelSize.y * 0.5f) * 2.f) / screenSize.y + 1.f, 0.f));
        Matrix m = tm * sm;
        gl::State().drawMaterial.vsParams.mvp = m;
        gl::State().drawMaterial.vsParams.displaceFactor = 0.f;
        gl::State().drawMaterial.fsParams.color = Vec4(tun::black, 0.75f);
        gl::UseMesh(rectMesh.vertexBuffer, rectMesh.indexBuffer, rectMesh.elementCount);
        gl::UpdateDrawMaterial();
        gl::Draw();

        float alpha = (healthbar.health / 5.f) * 0.99f;
        Color healthbarColor = tun::LerpThrough(tun::red, tun::orange, tun::green, alpha);
        sm = glm::scale(Matrix(1.f), Vec3((panelSize.x / screenSize.x) * alpha + scaleAnim, (panelSize.y / screenSize.y) * 0.9f + scaleAnim, 1.f));
        m = tm * sm;
        gl::State().drawMaterial.vsParams.mvp = m;
        gl::State().drawMaterial.vsParams.displaceFactor = 0.f;
        gl::State().drawMaterial.fsParams.color = Vec4(healthbarColor, 1.f);
        gl::UseMesh(rectMesh.vertexBuffer, rectMesh.indexBuffer, rectMesh.elementCount);
        gl::UpdateDrawMaterial();
        gl::Draw();

        auto& healthbarLabel = hub::Reg().get<comp::TextWidget>(hub::Reg().view<tag::HealthbarLabel>().back());
        healthbarLabel.color = healthbarColor;
    });
}
