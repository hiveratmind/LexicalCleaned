#pragma once
#include "../../FuncHook.h"

class MobSwingDurationHook : public FuncHook {
private:
	using func_t = uint32_t(__thiscall*)(Mob*);
	static inline func_t oFunc;

	static uint32_t MobGetCurrentSwingDurationCallback(Mob* _this) {
		if (!_this) return 0;

		auto swingMod = ModuleManager::getModule<Swing>();
		auto localPlayer = Game::getLocalPlayer();
		if (swingMod && swingMod->isEnabled() && localPlayer == _this) {
			return swingMod->swingSpeed;
		}

		if (oFunc) return oFunc(_this);
		return 0;
	}

public:
	MobSwingDurationHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&MobGetCurrentSwingDurationCallback;
	}
};
