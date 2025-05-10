#pragma once
#include "tun/entity.h"
#include "comp/SliderWidget.h"
#include "Lang.h"

namespace prefab {

Entity Tooltip();
Entity BlackScreen();
Entity Button(LangString* text, void(*onClick)(Entity), int uiSlot = 0);
Entity Subtitle(LangString* text, float duration);
Entity Slider(LangString* text, float value, SliderType sliderType, int uiSlot = 0);
Entity Text3D(LangString* text, Color color, Vec translation, Quat rotation, Vec scale);

}