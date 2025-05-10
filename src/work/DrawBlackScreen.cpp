#include "Work.h"
#include "tun/math.h"
#include "tun/builder.h"
#include "comp/TextWidgetComp.h"
#include "comp/ButtonWidget.h"
#include "comp/BoundsWidget.h"
#include "comp/Font.h"
#include "comp/MeshAsset.h"
#include "comp/SliderWidget.h"
#include "comp/BlackScreenComp.h"
#include "Tags.h"

void work::DrawBlackScreen() {
    using comp::ButtonWidget;
    using comp::BoundsWidget;
    using comp::Font;

    hub::Reg().view<BlackScreenComp, BoundsWidget>().each([](Entity entity, BlackScreenComp& blackScreen, BoundsWidget& bounds) {
        blackScreen.alpha += blackScreen.delta * blackScreen.speed * hub::GetDeltaTime();
        if (blackScreen.alpha > 1.f) {
            blackScreen.alpha = 1.f;
            blackScreen.delta = 0.f;
        } else if (blackScreen.alpha < 0.f) {
            blackScreen.alpha = 0.f;
            blackScreen.delta = 0.f;
        }
        if (!bounds.visible) return;

        bounds.color = Vec4(bounds.color.r, bounds.color.g, bounds.color.b, blackScreen.alpha);

        auto& state = State::Get();
        auto screenSize = hub::GetScreenSize();
        Vec2 panelSize {bounds.minSize.x, bounds.minSize.y};
        Vec2 panelPos {0.f, 0.f};

        gl::UseDrawMaterial();
        Entity rectMeshEntity = hub::Reg().view<tag::RectMesh>().back();
        auto& rectMesh = hub::Reg().get<comp::MeshAsset>(rectMeshEntity);
        Matrix sm = glm::scale(Matrix(1.f), Vec3(panelSize.x / screenSize.x, panelSize.y / screenSize.y, 1.f));
        Matrix tm = glm::translate(Matrix(1.f), Vec3(((panelPos.x + panelSize.x * 0.5f) * 2.f) / screenSize.x - 1.f, -((panelPos.y + panelSize.y * 0.5f) * 2.f) / screenSize.y + 1.f, 0.f));
        Matrix m = tm * sm;
        gl::State().drawMaterial.vsParams.mvp = m;
        gl::State().drawMaterial.vsParams.displaceFactor = 0.f;
        gl::State().drawMaterial.fsParams.color = tun::Lerp(Vec4(tun::red, 1.f), bounds.color, bounds.timeSinceClick);
        gl::UseMesh(rectMesh.vertexBuffer, rectMesh.indexBuffer, rectMesh.elementCount);
        gl::UpdateDrawMaterial();
        gl::Draw();
    });
}
