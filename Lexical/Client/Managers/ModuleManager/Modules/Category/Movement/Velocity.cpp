#include "Velocity.h"

Velocity::Velocity() :Module(Category::MOVEMENT, "Velocity", "Prevent you from knockback", 0x0, ModuleType::ToggleAllowed)
 {
	registerSetting(new SliderSetting<float>("Horizontal", "NULL", &horizontal, horizontal, 0.f, 100.f));
	registerSetting(new SliderSetting<float>("Vertical", "NULL", &vertical, vertical, 0.f, 100.f));
}

std::string Velocity::getModeText() {
	static char textStr[15];
	sprintf_s(textStr, 15, "H%i%%V%i%%", (int)(std::round(horizontal)), (int)(std::round(vertical)));
	return std::string(textStr);
}