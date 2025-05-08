#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/TransformComp.h"

static void UpdateWorldTransform(TransformComp& transform, const TransformComp& parentTransform);

void work::UpdateTransforms() {
    hub::Reg().view<TransformComp>().each([](TransformComp& transform) {
        if (!hub::Reg().valid(transform.parent)) {
            for (Entity child : transform.children) {
                if (auto* childTransform = hub::Reg().try_get<TransformComp>(child)) {
                    UpdateWorldTransform(*childTransform, transform);
                }
            }
        } 
    });
}

static void UpdateWorldTransform(TransformComp& transform, const TransformComp& parentTransform) {
    for (Entity child : transform.children) {
        if (auto* childTransform = hub::Reg().try_get<TransformComp>(child)) {
            UpdateWorldTransform(*childTransform, transform);
        }
    }
}