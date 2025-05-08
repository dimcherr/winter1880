#include "Work.h"
#include "tun/math.h"
#include "tun/builder.h"
#include "comp/TextWidget.h"
#include "comp/ButtonWidget.h"
#include "comp/BoundsWidget.h"
#include "comp/Font.h"
#include "comp/MeshAsset.h"
#include "comp/SliderWidget.h"
#include "Tags.h"

void work::DrawUI() {
    using comp::TextWidget;
    using comp::ButtonWidget;
    using comp::BoundsWidget;
    using comp::Font;

    hub::Reg().view<TextWidget, BoundsWidget>(entt::exclude<tag::Menu>).each([](Entity entity, const TextWidget& text, BoundsWidget& bounds) {
        if (!bounds.visible) return;
        if (hub::Reg().any_of<comp::SliderWidget>(entity)) return;

        if (bounds.timeToActualClick > 0.f && bounds.clickPending) {
            bounds.timeToActualClick -= hub::GetDeltaTime();
        } else if (bounds.clickPending) {
            bounds.timeToActualClick = 0.f;
            bounds.clickPending = false;
            if (bounds.onClick) {
                bounds.onClick(entity);
            }
        }

        if (bounds.timeSinceClick < 1.f) {
            bounds.timeSinceClick += hub::GetDeltaTime();
        } else {
            bounds.timeSinceClick = 1.f;
        }
        auto& state = State::Get();
        auto screenSize = hub::GetScreenSize();
        ImFont* font = hub::Reg().get<Font>(text.font).font;
        ImGui::PushFont(font);
        ImVec2 textSize = ImGui::CalcTextSize(text.text.c_str());
        Vec2 panelSize {glm::max(bounds.minSize.x, textSize.x + bounds.padding.x), glm::max(bounds.minSize.y, textSize.y + bounds.padding.y)};

        Vec2 anchorOffset {};
        Vec2 panelAnchorOffset {};
        Vec2 parentAnchorOffset {};
        Vec2 textAnchorOffset {};
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

        switch(bounds.textAnchors.horizontal) {
            case tun::begin:
                anchorOffset.x = 0.f;
            break;
            case tun::center:
                anchorOffset.x = textSize.x * -0.5f;
            break;
            case tun::end:
                anchorOffset.x = textSize.x * -1.f;
            break;
        }

        switch(bounds.textAnchors.vertical) {
            case tun::begin:
                anchorOffset.y = 0.f;
            break;
            case tun::center:
                anchorOffset.y = textSize.y * -0.5f;
            break;
            case tun::end:
                anchorOffset.y = textSize.y * -1.f;
            break;
        }

        switch(bounds.anchors.horizontal) {
            case tun::begin:
                panelAnchorOffset.x = 0.f;
                textAnchorOffset.x = panelSize.x * 0.5f;
            break;
            case tun::center:
                panelAnchorOffset.x = panelSize.x * -0.5f;
                textAnchorOffset.x = 0.f;
            break;
            case tun::end:
                panelAnchorOffset.x = panelSize.x * -1.f;
                textAnchorOffset.x = panelSize.x * -0.5f;
            break;
        }

        switch(bounds.anchors.vertical) {
            case tun::begin:
                panelAnchorOffset.y = 0.f;
                textAnchorOffset.y = panelSize.y * 0.5f;
            break;
            case tun::center:
                panelAnchorOffset.y = panelSize.y * -0.5f;
                textAnchorOffset.y = 0.f;
            break;
            case tun::end:
                panelAnchorOffset.y = panelSize.y * -1.f;
                textAnchorOffset.y = panelSize.y * -0.5f;
            break;
        }


        Vec2 panelPos {parentAnchorOffset.x + bounds.pos.x + panelAnchorOffset.x, parentAnchorOffset.y + bounds.pos.y + panelAnchorOffset.y};

        if (state.mouseX >= panelPos.x && state.mouseX <= panelPos.x + panelSize.x && state.mouseY >= panelPos.y && state.mouseY <= panelPos.y + panelSize.y) {
            if (state.leftClick) {
                bounds.timeSinceClick = 0.f;
                bounds.timeToActualClick = 0.2f;
                bounds.clickPending = true;
            }
        }

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



        float deformAmp = tun::Lerp(0.0001f, 0.00005f, tun::Clamp(bounds.timeSinceClick * 5.f, 0.f, 1.f));
        float scaleAmp = tun::Lerp(0.5f, 0.1f, tun::Clamp(bounds.timeSinceClick * 5.f, 0.f, 1.f));
        float deformTime = tun::Lerp(1.f, (float)hub::GetTime(), tun::Clamp(bounds.timeSinceClick * 5.f, 0.f, 1.f));
        ImGui::SetCursorScreenPos(ToImVec2(bounds.pos + parentAnchorOffset + anchorOffset + textAnchorOffset));
        ImGui::PushStyleColor(ImGuiCol_Text, ToImVec(text.color));
        ImGui::PushStyleColor(TundraCol_Offset, {0.f, 0.0f, state.mouseX, state.mouseY});
        ImGui::PushStyleColor(TundraCol_Deform, {5.f, deformAmp * font->FontSize, deformTime, scaleAmp * font->FontSize});
        ImGui::Text("%s", text.text.c_str());
        ImGui::PopStyleColor(3);
        ImGui::PopFont();
    });
}
