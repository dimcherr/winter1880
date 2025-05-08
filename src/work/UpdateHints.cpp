#include "Work.h"
#include "tun/math.h"
#include "tun/builder.h"
#include "comp/TextWidget.h"
#include "comp/ButtonWidget.h"
#include "comp/BoundsWidget.h"
#include "comp/Font.h"
#include "comp/MeshAsset.h"
#include "comp/HackerKey.h"
#include "comp/HintLabel.h"
#include "comp/BrickWall.h"
#include "Tags.h"

void work::UpdateHints() {

    int hintIndex = 0;
    for (auto& hintLabelEntity : State::Get().hintLabels) {
        auto& hintBounds = hub::Reg().get<comp::BoundsWidget>(hintLabelEntity);
        hintBounds.visible = false;
    }
    hub::Reg().view<comp::BrickWall>().each([&hintIndex](comp::BrickWall& brickWall) {
        if (brickWall.underAttack) {
            Entity hintLabelEntity = State::Get().hintLabels[hintIndex];
            auto& hintText = hub::Reg().get<comp::TextWidget>(hintLabelEntity);
            auto& hintBounds = hub::Reg().get<comp::BoundsWidget>(hintLabelEntity);
            hintBounds.visible = true;

            hintBounds.color = Vec4(tun::black, 0.75f);

            if (brickWall.access[0].required) {
                hintBounds.color = Vec4(State::Get().accessColors[0], 0.75f);
            }
            if (brickWall.access[1].required) {
                hintBounds.color = Vec4(State::Get().accessColors[1], 0.75f);
            }
            if (brickWall.access[2].required) {
                hintBounds.color = Vec4(State::Get().accessColors[2], 0.75f);
            }

            StringView face {};
            hintText.color = tun::white;
            if (brickWall.progress > 50) {
                face = State::Get().goodFace;
            } else if (brickWall.progress > 30) {
                face = State::Get().badFace;
            } else if (brickWall.progress > 10) {
                face = State::Get().veryBadFace;
            } else {
                face = State::Get().veryVeryBadFace;
            }

            hintText.text = tun::formatToString("{} {} {}%", face, brickWall.name, brickWall.progress);
            hintIndex++;
        }
    });
}
