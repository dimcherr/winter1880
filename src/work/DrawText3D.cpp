#include "Work.h"
#include "Hub.h"
#include "comp/Text3DComp.h"
#include "comp/TransformComp.h"
#include "comp/Font.h"

void work::DrawText3D() {
    hub::Reg().view<Text3DComp, TransformComp>().each([](Entity entity, Text3DComp& text, TransformComp& transform) {
        auto& state = State::Get();
        auto screenSize = hub::GetScreenSize();
        ImFont* font = hub::Reg().get<comp::Font>(text.font).font;
        ImGui::PushFont(font);

        //float deformAmp = tun::Lerp(0.0001f, 0.00005f, tun::Clamp(bounds.timeSinceClick * 5.f, 0.f, 1.f)) * 0.25f;
        float deformAmp = 0.f;
        float scaleAmp = 1.f;
        //float scaleAmp = tun::Lerp(0.5f, 0.1f, tun::Clamp(bounds.timeSinceClick * 5.f, 0.f, 1.f));
        float deformTime = 1.f;
        //float deformTime = tun::Lerp(1.f, (float)hub::GetTime(), tun::Clamp(bounds.timeSinceClick * 5.f, 0.f, 1.f));
        //ImGui::SetCursorScreenPos(ToImVec2(bounds.pos + parentAnchorOffset + anchorOffset + textAnchorOffset));
        ImGui::PushStyleColor(ImGuiCol_Text, ToImVec(text.color));
        ImGui::PushStyleColor(TundraCol_Offset, {0.f, 0.0f, state.mouseX, state.mouseY});
        ImGui::PushStyleColor(TundraCol_Deform, {5.f, deformAmp * font->FontSize, deformTime, scaleAmp * font->FontSize});
        ImGui::Text("%s", text.text.c_str());
        ImGui::PopStyleColor(3);
        ImGui::PopFont();
    });
}
