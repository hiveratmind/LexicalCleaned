#include "Swing.h"

Swing::Swing() : Module(Category::RENDER, "Swing", "Swing animation", 0x0, ModuleType::ToggleAllowed) {
	registerSetting(new SliderSetting<int>("Speed", "Swing speed", &swingSpeed, swingSpeed, 1, 20));
}