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
void DrawUI();
void UpdateRaycast();
void UpdateDoors();
void DrawMenu();
void DrawSliders();
void UpdateSubtitles();
void UpdateLanguage();
void UpdateFlashlight();
void UpdateGears();
void DrawText3D();
void DrawBlackScreen();
void UpdateTimeline();

void SetMusicMenuPlayed(bool played);
void SetMusicWindPlayed(bool played);

}