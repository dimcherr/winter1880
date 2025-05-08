#pragma once
#include "Lang.h"
#include "tun/entity.h"
#include "tun/math.h"

struct TextWidgetComp {
    String text {};
    Entity font {entt::null};
    Color color {};
};
