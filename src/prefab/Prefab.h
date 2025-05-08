#pragma once
#include "tun/log.h"
#include "tun/entity.h"
#include "tun/builder.h"
#include "tun/math.h"

namespace prefab {

Entity Font(StringView path, float fontSize);
Entity CameraFly(const Vec& position, const Vec& target);
Entity Grid(float segmentSize = 1.f, int segmentCount = 30, const Color& color = tun::black);
Entity Plane();
Entity Rect();
Entity Cube();

}
