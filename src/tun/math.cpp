#include "math.h"

Matrix tun::LookAt(const Vec& position, const Quat& rotation) {
    const Vec look = rotation * tun::back;
    const Vec right = rotation * tun::right;
    const Vec up = rotation * tun::up;
    Matrix m(1);
    m[0][0] = right.x;
    m[1][0] = right.y;
    m[2][0] = right.z;
    m[0][1] = up.x;
    m[1][1] = up.y;
    m[2][1] = up.z;
    m[0][2] = look.x;
    m[1][2] = look.y;
    m[2][2] = look.z;
    m[3][0] = -dot(right, position);
    m[3][1] = -dot(up, position);
    m[3][2] = dot(-look, position);
    return m;
}

Vec2 tun::LookAtPitchYaw(const Vec& position, const Vec& target) {
    Vec direction = target - position;
    float len = glm::length(direction);
    float pitch = len < 0.001f ? 0.f : -glm::asin(direction.y / len);
    float yaw = glm::atan(direction.x, direction.z);
    return Vec2(pitch, yaw);
}

float tun::EaseInOut(float t) {
    t = std::max(0.0f, std::min(1.0f, t));
    return t * t * (3.0f - 2.0f * t);
}

float tun::EaseQuickStartEnd(float t) {
    t = std::max(0.0f, std::min(1.0f, t));
    return t * t * t * (10.0f - 15.0f * t + 6.0f * t * t);
}

float tun::EaseLerp(float a, float b, float t) {
    float easedT = tun::EaseQuickStartEnd(t);
    return a * (1.f - t) + b * t;
}

float tun::Lerp(float a, float b, float t) {
    return a * (1.f - t) + b * t;
}

float tun::Clamp(float x, float min, float max) {
    return glm::clamp(x, min, max);
}

Vec4 tun::Lerp(const Vec4& a, const Vec4& b, float t) {
    return a * (1.f - t) + b * t;
}

Color tun::Lerp(const Color& a, const Color& b, float t) {
    return a * (1.f - t) + b * t;
}

Color tun::LerpThrough(const Color& a, const Color& c, const Color& b, float t) {
    if (t < 0.5f) {
        return tun::Lerp(a, c, t * 2.f);
    } else {
        return tun::Lerp(c, b, (t - 0.5f) * 2.f);
    }
}
