#include "CustomFont.h"

CustomFont::CustomFont() :Module(Category::CLIENT, "Fonts", "Font of Client", 0x0, ModuleType::AlwaysOn)
 {
	fontEnumSetting = (EnumSetting*)registerSetting(new EnumSetting("Font", "NULL", { "Arial", "Verdana", "Tahoma"}, &fontMode, 0));
	registerSetting(new SliderSetting<int>("FontSize", "NULL", &fontSize, 25, 20, 30));
	registerSetting(new BoolSetting("Italic", "NULL", &italic, false));
	registerSetting(new BoolSetting("Shadow", "NULL", &shadow, false));
}

bool CustomFont::isVisible() {
	return false;
}

std::string CustomFont::getSelectedFont() {
	return fontEnumSetting->enumList[fontMode];
}