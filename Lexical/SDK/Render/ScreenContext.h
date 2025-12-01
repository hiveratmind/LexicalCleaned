#pragma once
#include "../../Utils/MemoryUtil.h"
#include "ShaderColor.h"
#include "Tessellator.h"

class ScreenContext {
public:
	CLASS_MEMBER(mce::Camera*, camera, Offsets::camera);
	CLASS_MEMBER(ShaderColor*, currentShaderColor, Offsets::currentShaderColor);
	CLASS_MEMBER(Tessellator*, tessellator, Offsets::tessellator);
};