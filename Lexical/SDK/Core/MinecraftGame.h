#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../Render/Font.h"

class MinecraftGame {
public:
	CLASS_MEMBER(bool, mouseGrabbed, Offsets::mouseGrabbed); // CE dissect structures
	CLASS_MEMBER(Font*, mcFont, Offsets::mcFont);	// CE dissect structures
};