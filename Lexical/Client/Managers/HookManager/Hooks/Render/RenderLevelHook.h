#pragma once
#include "../FuncHook.h"
#include "../../../../../Renderer/MCR.h"

class RenderLevelHook : public FuncHook {
private:
	using func_t = __int64(__thiscall*)(LevelRenderer*, ScreenContext*, __int64);
	static inline func_t oFunc;

	static __int64 LevelRenderer_renderLevelCallback(LevelRenderer* _this, ScreenContext* a2, __int64 a3) {
		if (!_this || !a2) return oFunc ? oFunc(_this, a2, a3) : 0;

		__int64 result = oFunc ? oFunc(_this, a2, a3) : 0;

		auto localPlayer = Game::getLocalPlayer();
		if (localPlayer && _this->levelRendererPlayer && MCR::blendMaterial) {
			MCR::onRenderWorld(_this, a2);
		}

		return result;
	}

public:
	RenderLevelHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&LevelRenderer_renderLevelCallback;
	}
};
