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
#include "comp/BoxShape.h"
#include "comp/Model.h"
#include "comp/BoundsWidget.h"
#include "comp/TextWidgetComp.h"
#include "comp/Door.h"
#include "comp/Character.h"
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
        text.text = "[G] Положить";
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

            auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
            if (door.doorState < 0.5f) {
                text.text = "[E] Открыть";
                text.color = tun::white;
            } else {
                text.text = "[E] Закрыть";
                text.color = tun::white;
            }
            bounds.visible = true;
            bounds.parentAnchors.vertical = tun::center;
            bounds.pos = {0.f, 128.f};
            State::Get().currentObject = entity;
        });

        hub::Reg().view<BodyComp, BoxShape, Model, TransformComp, tag::Pickable>().each([&bodyID, &hit](Entity entity, const BodyComp& body, const BoxShape& shape, const Model& model, const TransformComp& modelTransform) {
            if (!State::Get().firstPerson || body.id != bodyID) return;

            auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
            text.text = "[E] Взять";
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
                    text.text = "[E] Вставить";
                    text.color = tun::white;
                    bounds.visible = true;
                    bounds.parentAnchors.vertical = tun::center;
                    bounds.pos = {0.f, 128.f};
                    State::Get().currentObject = entity;
                } else {
                    auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
                    text.text = "Не подходит";
                    text.color = tun::white;
                    bounds.visible = true;
                    bounds.parentAnchors.vertical = tun::center;
                    bounds.pos = {0.f, 128.f};
                    State::Get().currentObject = entity;
                }
            } else {
                auto [text, bounds] = hub::Reg().get<TextWidgetComp, comp::BoundsWidget>(hub::Reg().view<tag::Tooltip>().back());
                text.text = "Нужна деталь";
                text.color = tun::white;
                bounds.visible = true;
                bounds.parentAnchors.vertical = tun::center;
                bounds.pos = {0.f, 128.f};
                State::Get().currentObject = entity;
            }
        });
    } 
}
