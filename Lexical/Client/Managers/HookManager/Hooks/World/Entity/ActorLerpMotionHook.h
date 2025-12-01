#pragma once
#include "../../FuncHook.h"

class ActorLerpMotionHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Actor*, Vec3<float>*);
	static inline func_t oFunc;

	static void ActorLerpMotionCallback(Actor* _this, Vec3<float>* delta) {
		if (!_this || !delta) {
			if (oFunc) oFunc(_this, delta);
			return;
		}

		auto velocityMod = ModuleManager::getModule<Velocity>();
		auto localPlayer = Game::getLocalPlayer();
		if (velocityMod && velocityMod->isEnabled() && localPlayer == _this) {
			static void* networkSender = reinterpret_cast<void*>(MemoryUtil::findSignature("48 8D 57 38 48 8B CE 48") + Offsets::networkSenderPart);
			if (_ReturnAddress() == networkSender && _this->stateVector) {
				float hPercent = velocityMod->horizontal / 100.f;
				float vPercent = velocityMod->vertical / 100.f;
				*delta = _this->stateVector->velocity.lerp(*delta, hPercent, vPercent, hPercent);
			}
		}

		if (oFunc) oFunc(_this, delta);
	}

public:
	ActorLerpMotionHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorLerpMotionCallback;
	}
};
