#pragma once
#include "../../FuncHook.h"

class ActorIsOnFireHook : public FuncHook {
private:
	using func_t = bool(__thiscall*)(Actor*);
	static inline func_t oFunc;

	static bool ActorIsOnFireCallback(Actor* _this) {
		if (!_this) return false;

		auto noRenderMod = ModuleManager::getModule<NoRender>();
		auto localPlayer = Game::getLocalPlayer();
		if (noRenderMod && localPlayer == _this && noRenderMod->isEnabled() && noRenderMod->noFire)
			return false;

		return oFunc ? oFunc(_this) : false;
	}

public:
	ActorIsOnFireHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorIsOnFireCallback;
	}
};
