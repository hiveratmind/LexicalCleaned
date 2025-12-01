#pragma once
#include "../../FuncHook.h"

class ActorNormalTickHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Actor*);
	static inline func_t oFunc;

	static void ActorNormalTickCallback(Actor* _this) {
		if (!_this) {
			if (oFunc) oFunc(_this);
			return;
		}

		auto localPlayer = Game::getLocalPlayer();
		if (localPlayer == _this) {
			ModuleManager::onNormalTick(localPlayer);
		}

		if (oFunc) oFunc(_this);
	}

public:
	ActorNormalTickHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorNormalTickCallback;
	}
};
