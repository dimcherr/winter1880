#include "Work.h"
#include "tun/math.h"
#include "tun/builder.h"
#include "comp/TextWidget.h"
#include "comp/ButtonWidget.h"
#include "comp/BoundsWidget.h"
#include "comp/BrickWallKey.h"
#include "comp/Font.h"
#include "comp/MeshAsset.h"
#include "comp/HackerKey.h"
#include "comp/HackerBrick.h"
#include "comp/BrickWall.h"
#include "comp/Camera.h"
#include "comp/Transform.h"
#include "comp/BrickWall.h"
#include "comp/Model.h"
#include "tun/tunstring.h"
#include "Tags.h"

void work::HighlightBrickWall() {
    using comp::BrickWall;
    using comp::ButtonWidget;
    using comp::BoundsWidget;
    using comp::Font;
    using comp::HackerKey;
    using comp::HackerBrick;
    using comp::Camera;
    using comp::Model;

    hub::Reg().view<BrickWall, Model>().each([](BrickWall& brickWall, Model& model) {
        if (brickWall.destroyed) {
            model.highlight = tun::darkRed;
            model.tint = tun::black;
        } else if (brickWall.progress < 100 && brickWall.requiredCount > 0) {
            brickWall.highlightTime += hub::GetDeltaTime() * 1.2f;
            if (brickWall.highlightTime > brickWall.requiredCount) {
                brickWall.highlightTime -= brickWall.requiredCount;
            }
            int highlightSegment = (int)glm::floor(brickWall.highlightTime);
            float timeNormalized = brickWall.highlightTime - highlightSegment;
            Color currentColor = tun::black;
            int c = 0;
            for (int i = 0; i < (int)AccessType::count; ++i) {
                if (brickWall.access[i].required) {
                    if (highlightSegment == c) {
                        currentColor = State::Get().accessColors[(int)brickWall.access[i].type];
                        break;
                    }
                    ++c;
                }
            }

            model.highlight = tun::Lerp(tun::black, currentColor, glm::sin(timeNormalized * tun::pi * 0.5f));
            model.tint = tun::Lerp(tun::white, currentColor, glm::sin(timeNormalized * tun::pi * 0.5f) * 0.5f);
        } else {
            model.highlight = tun::black;
            model.tint = tun::white;
        }
    });

    hub::Reg().view<comp::BrickWallKey, Model>().each([](comp::BrickWallKey& brickWallKey, Model& model) {
        model.highlight = tun::Lerp(tun::black, State::Get().accessColors[(int)brickWallKey.type], (glm::sin(State::Get().time * tun::pi * 2.4f) + 1.f) * 0.5f);
    });
}
