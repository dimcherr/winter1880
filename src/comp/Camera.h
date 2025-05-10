#pragma once
#include "tun/log.h"
#include "tun/builder.h"
#include "tun/math.h"

namespace comp {

struct Camera {
    Matrix projection {1.f};
    tun::ProjectionType type {tun::perspective};
    float fovy {glm::radians(70.f)};
    float znear {0.1f};
    float zfar {100.f};
    float yaw {0.f};
    float pitch {0.f};
    float rotationSensitivity {0.05f};
    float movementSpeed {5.f};
    Vec offset {0.f, 1.2f, 0.f};

    float minPitch {-30.f};
    float maxPitch {30.f};
    float minYaw {-15.f};
    float maxYaw {15.f};

    float bobbingTime {0.f};
    float bobbingDelta {0.f};
    float bobbingSpeed {5.f};
    float bobbingActive {false};
    Vec bobbingOffset {0.f, -0.025f, 0.f};

    void Update(float width, float height) {
        if (type == tun::perspective) {
            if (height > 0.f) {
                projection = glm::perspective(fovy, width / height, znear, zfar);
            } else {
                tun::error("Screen height should be > 0 in Camera.Update()");
            }
        } else {
            projection = glm::ortho(0.f, width, 0.f, height, znear, zfar);
        }
    }
};

}

template <>
struct CompBuilder<comp::Camera> : public BaseCompBuilder<comp::Camera> {
    CompBuilder& fovy(float degrees) {
        comp->fovy = glm::radians(degrees);
        return *this;
    }

    CompBuilder& zplanes(float znear, float zfar) {
        comp->znear = znear;
        comp->zfar = zfar;
        return *this;
    }

    CompBuilder& type(tun::ProjectionType type) {
        comp->type = type;
        return *this;
    }

    CompBuilder& rotation(float pitch, float yaw) {
        comp->pitch = pitch;
        comp->yaw = yaw;
        return *this;
    }

    CompBuilder& update(float width, float height) {
        comp->Update(width, height);
        return *this;
    }

    CompBuilder& rotationSensitivity(float sensitivity) {
        comp->rotationSensitivity = sensitivity;
        return *this;
    }
};

template <>
struct std::formatter<comp::Camera> : std::formatter<std::string> {
    auto format(comp::Camera camera, format_context& ctx) const {
        return formatter<string>::format(std::format("[Camera Type={}, fovy={}, zplanes={}->{}, yaw={}, pitch={}]", 
            camera.type == tun::perspective ? "perspective" : "ortho", glm::degrees(camera.fovy), camera.znear, camera.zfar, glm::degrees(camera.yaw), glm::degrees(camera.pitch)), ctx);
    }
};