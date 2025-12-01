#pragma once
#include "../../FuncHook.h"

class LevelTickHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Level*);
	static inline func_t oFunc;

	static void LevelTickCallback(Level* _this) {
		if (!_this) return;
		auto localPlayer = Game::getLocalPlayer();
		if (localPlayer && localPlayer->level == _this) {
			ModuleManager::onLevelTick(_this);
		}
		if (oFunc) oFunc(_this);
	}

public:
	LevelTickHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&LevelTickCallback;
	}
};
