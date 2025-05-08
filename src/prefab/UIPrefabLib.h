#pragma once
#include "tun/entity.h"
#include "comp/SliderWidget.h"
#include "Lang.h"

namespace prefab {

Entity Tooltip();
Entity Button(const LangString& text, void(*onClick)(Entity), int uiSlot = 0);
Entity Subtitle(const LangString& text, float time);
Entity Slider(const LangString& text, float value, SliderType sliderType, int uiSlot = 0);

}