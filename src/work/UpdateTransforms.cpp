#include "Work.h"
#include "tun/math.h"
#include "tun/log.h"
#include "tun/builder.h"
#include "comp/Transform.h"

static void UpdateWorldTransform(comp::Transform& transform, const comp::Transform& parentTransform);

void work::UpdateTransforms() {
    using comp::Transform;

    hub::Reg().view<Transform>().each([](Transform& transform) {
        if (!hub::Reg().valid(transform.parent)) {
            for (Entity child : transform.children) {
                if (auto* childTransform = hub::Reg().try_get<comp::Transform>(child)) {
                    UpdateWorldTransform(*childTransform, transform);
                }
            }
        } 
    });
}

static void UpdateWorldTransform(comp::Transform& transform, const comp::Transform& parentTransform) {
    for (Entity child : transform.children) {
        if (auto* childTransform = hub::Reg().try_get<comp::Transform>(child)) {
            UpdateWorldTransform(*childTransform, transform);
        }
    }
}