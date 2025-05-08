#pragma once
#include "tun/entity.h"
#include "comp/SliderWidget.h"
#include "Lang.h"

namespace prefab {

Entity Tooltip();
Entity Button(LangString* text, void(*onClick)(Entity), int uiSlot = 0);
Entity Subtitle(LangString* text, float time);
Entity Slider(LangString* text, float value, SliderType sliderType, int uiSlot = 0);

}