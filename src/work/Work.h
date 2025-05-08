#pragma once
#include "tun/entity.h"
#include "tun/tunstring.h"
#include "tun/math.h"
#include "tun/list.h"

namespace work {

void DrawGrid();
void DrawLights();
void DrawColliders();
void DrawPBR();
void UpdateCamera();
void UpdateCameraMovement();
void UpdateCameraRotation();
void UpdateTransforms();
void UpdatePhysics();
void SwitchCamera();
void UpdateSounds();
void SetMusicPlaying(bool playing);
void DrawUI();
void UpdateRaycast();
void UpdateDoors();
void DrawMenu();
void UpdateCountdown();
void DrawSliders();
void UpdateSubtitles();

}