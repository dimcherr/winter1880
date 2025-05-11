#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/physics.h"
#include "comp/Camera.h"
#include "comp/TransformComp.h"
#include "comp/BodyComp.h"
#include "comp/GearSlotComp.h"
#include "comp/GearPickableComp.h"
#include "comp/SubtitleComp.h"
#include "comp/BoxShape.h"
#include "comp/Model.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "comp/Door.h"
#include "comp/Character.h"
#include "comp/LeverBaseComp.h"
#include "comp/LeverComp.h"
#include "Tags.h"

void work::UpdateRaycast() {
    using comp::Camera;
    using comp::BoxShape;
    using comp::Model;

    auto& tooltipBounds = hub::Reg().get<comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
    tooltipBounds.visible = false;

    auto [camera, transform] = hub::Reg().get<Camera, TransformComp>(hub::Reg().view<tag::Current, Camera, TransformComp>().back());
    Vec cameraPos = transform.translation + camera.offset;

    Vec forward = glm::normalize(transform.rotation * tun::forward) * 3.f;

    JPH::RRayCast raycast(Convert(cameraPos), Convert(forward));
    JPH::RayCastResult hit {};
    auto& physState = phys::State::Get();
    auto& character = hub::Reg().get<comp::Character>(hub::Reg().view<comp::Character, tag::Current>().back());
    JPH::ObjectLayerFilter& layerFilter = hub::Reg().valid(character.pickable) ? physState.nonPickableObjectFilter : physState.characterObjectLayerFilter;
    bool hasHit = physState.physicsSystem.GetNarrowPhaseQuery().CastRay(
        raycast, 
        hit, 
        physState.characterBroadPhaseLayerFilter,
        layerFilter,
        physState.characterBodyFilter 
    );

    if (hub::Reg().valid(character.pickable)) {
        auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
        text.text = tun::formatToString("[G] {}", LangStrings::putDown.Get());
        text.color = tun::white;
        bounds.visible = true;
        bounds.parentAnchors.vertical = tun::center;
        bounds.pos = {0.f, 300.f};
    }

    if (hasHit) {
        JPH::BodyID bodyID = hit.mBodyID;
        State::Get().currentObject = entt::null;
        hub::Reg().view<BodyComp, BoxShape, Model, TransformComp, comp::Door>().each([&bodyID, &hit](Entity entity, const BodyComp& body, const BoxShape& shape, const Model& model, const TransformComp& modelTransform, comp::Door& door) {
            if (!State::Get().firstPerson || body.id != bodyID) return;
            if (hub::Reg().any_of<tag::MainGate>(entity)) return;

            auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
            if (door.doorState < 0.1f && State::Get().phase != GamePhase::outro) {
                //text.text = "[E] Открыть";
                text.text = tun::formatToString("[E] {}", LangStrings::open.Get());
                text.color = tun::white;
                bounds.visible = true;
                State::Get().currentObject = entity;
                bounds.parentAnchors.vertical = tun::center;
                bounds.pos = {0.f, 128.f};
            } else {
                //text.text = "[E] Закрыть";
                //text.text = tun::formatToString("[E] {}", LangStrings::close.Get());
                //text.color = tun::white;
            }
        });

        hub::Reg().view<BodyComp, BoxShape, Model, TransformComp, LeverBaseComp>().each([&bodyID, &hit](Entity entity, const BodyComp& body, const BoxShape& shape, const Model& model, const TransformComp& modelTransform, LeverBaseComp& leverBase) {
            if (!State::Get().firstPerson || body.id != bodyID) return;

            Entity leverEntity {entt::null};
            if (hub::Reg().any_of<tag::LeverMainLeft>(entity)) {
                leverEntity = hub::Reg().view<tag::LeverMainLeft, LeverComp>().back();
            }
            else if (hub::Reg().any_of<tag::LeverMainRight>(entity)) {
                leverEntity = hub::Reg().view<tag::LeverMainRight, LeverComp>().back();
            }

            auto& lever = hub::Reg().get<LeverComp>(leverEntity);

            auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
            if (lever.state == 0.f) {
                //text.text = "[E] Потянуть";
                text.text = tun::formatToString("[E] {}", LangStrings::pull.Get());
                text.color = tun::white;
                bounds.visible = true;
                bounds.parentAnchors.vertical = tun::center;
                bounds.pos = {0.f, 128.f};
                State::Get().currentObject = entity;
            }
        });

        hub::Reg().view<BodyComp, BoxShape, Model, TransformComp, tag::Pickable>().each([&bodyID, &hit](Entity entity, const BodyComp& body, const BoxShape& shape, const Model& model, const TransformComp& modelTransform) {
            if (!State::Get().firstPerson || body.id != bodyID) return;

            auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
            //text.text = "[E] Взять";
            text.text = tun::formatToString("[E] {}", LangStrings::take.Get());
            text.color = tun::white;
            bounds.visible = true;
            bounds.parentAnchors.vertical = tun::center;
            bounds.pos = {0.f, 128.f};
            State::Get().currentObject = entity;
        });

        hub::Reg().view<BodyComp, BoxShape, Model, TransformComp, GearSlotComp>().each([&bodyID, &hit](Entity entity, const BodyComp& body, const BoxShape& shape, const Model& model, const TransformComp& modelTransform, GearSlotComp& gearSlot) {
            if (!State::Get().firstPerson || body.id != bodyID) return;
            if (gearSlot.filled) return;
            auto& character = hub::Reg().get<comp::Character>(hub::Reg().view<comp::Character, tag::Current>().back());

            if (hub::Reg().valid(character.pickable)) {
                if (hub::Reg().any_of<GearPickableComp>(character.pickable)) {
                    auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
                    //text.text = "[E] Вставить";
                    text.text = tun::formatToString("[E] {}", LangStrings::placeInto.Get());
                    text.color = tun::white;
                    bounds.visible = true;
                    bounds.parentAnchors.vertical = tun::center;
                    bounds.pos = {0.f, 128.f};
                    State::Get().currentObject = entity;
                } else {
                    auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
                    //text.text = "Не подходит";
                    text.text = tun::formatToString("{}", LangStrings::wrongPiece.Get());
                    text.color = tun::white;
                    bounds.visible = true;
                    bounds.parentAnchors.vertical = tun::center;
                    bounds.pos = {0.f, 128.f};
                    State::Get().currentObject = entity;
                }
            } else {
                auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
                //text.text = "Нужна деталь";
                text.text = tun::formatToString("{}", LangStrings::needAPart.Get());
                text.color = tun::white;
                bounds.visible = true;
                bounds.parentAnchors.vertical = tun::center;
                bounds.pos = {0.f, 128.f};
                State::Get().currentObject = entity;

                if (!State::Get().lookedAtTheGearSlot) {
                    State::Get().lookedAtTheGearSlot = true;
                    // PLAY CUE
                    hub::Reg().view<SubtitleComp>().each([](SubtitleComp& subtitle) {
                        subtitle.running = false;
                        subtitle.time = 0.f;
                    });
                    auto& sub = hub::Reg().get<SubtitleComp>(hub::Reg().view<tag::CueGear, SubtitleComp>().back());
                    sub.running = true;
                    sub.time = 0.f;
                    // PLAY CUE
                }
            }
        });
    } else {
        State::Get().currentObject = entt::null;
    }
}
