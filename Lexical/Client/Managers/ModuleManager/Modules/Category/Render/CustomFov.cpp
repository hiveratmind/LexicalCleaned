#include "CustomFov.h"

CustomFov::CustomFov() : Module(Category::RENDER, "CustomFov", "Allow you modify FOV without limitation", 0x0, ModuleType::ToggleAllowed) {
	registerSetting(new SliderSetting<float>("Fov", "NULL", &fov, 120.f, 1.f, 359.f));
}