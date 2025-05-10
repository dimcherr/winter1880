#pragma once
#include "Lang.h"
#include "tun/entity.h"
#include "tun/math.h"

struct Text3DComp {
    String text {};
    LangString* langString {};
    Entity font {entt::null};
    Color color {};
};
