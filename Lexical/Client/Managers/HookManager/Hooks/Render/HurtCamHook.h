#pragma once
#include "../FuncHook.h"

class HurtCamHook : public FuncHook {
private:
	using func_t = __int64(__thiscall*)(Actor*, char, uint32_t);
	static inline func_t oFunc;

	static __int64 CauseHurtCamCallback(Actor* a1, char a2, uint32_t a3) {
		if (!a1) return oFunc ? oFunc(a1, a2, a3) : 0;

		__int64 result = oFunc ? oFunc(a1, a2, a3) : 0;

		auto noHurtCamMod = ModuleManager::getModule<NoHurtCam>();
		auto localPlayer = Game::getLocalPlayer();

		if (noHurtCamMod && noHurtCamMod->isEnabled() && a2 == 2 && a1 == localPlayer) {
			uint32_t* hurtTimePtr = *(uint32_t**)((uintptr_t)a1 + Offsets::MobAnimation_decrementHurtTime);
			if (hurtTimePtr) *hurtTimePtr = 0;
		}

		return result;
	}

public:
	HurtCamHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&CauseHurtCamCallback;
	}
};
