#pragma once
#include "../FuncHook.h"

class GetGammaHook : public FuncHook {
private:
	using func_t = float(__thiscall*)(__int64);
	static inline func_t oFunc;

	static float GetGammaCallback(__int64 a1) {
		if (!a1) return oFunc ? oFunc(a1) : 1.f;

		static bool initPtr = false;
		if (!initPtr) {
			uintptr_t** list = (uintptr_t**)a1;
			auto noRenderMod = ModuleManager::getModule<NoRender>();
			if (noRenderMod) {
				int obtainedSettings = 0;
				for (uint16_t i = 3; i < 450; i++) {
					if (!list[i]) continue;
					uintptr_t* info = *(uintptr_t**)((uintptr_t)list[i] + Offsets::info);
					if (!info) continue;

					std::string* translateName = (std::string*)((uintptr_t)info + Offsets::translateName);
					bool* boolSetting = (bool*)((uintptr_t)list[i] + 16);

					if (!translateName || !boolSetting) continue;

					if (strcmp(translateName->c_str(), "options.dev_disableRenderWeather") == 0) {
						noRenderMod->noWeatherBoolPtr = boolSetting;
						obtainedSettings++;
					} else if (strcmp(translateName->c_str(), "options.dev_disableRenderEntities") == 0) {
						noRenderMod->noEntitiesBoolPtr = boolSetting;
						obtainedSettings++;
					} else if (strcmp(translateName->c_str(), "options.dev_disableRenderBlockEntities") == 0) {
						noRenderMod->noBlockEntitiesBoolPtr = boolSetting;
						obtainedSettings++;
					} else if (strcmp(translateName->c_str(), "options.dev_disableRenderParticles") == 0) {
						noRenderMod->noParticlesBoolPtr = boolSetting;
						obtainedSettings++;
					}

					if (obtainedSettings == 4) break;
				}
			}
			initPtr = true;
		}

		auto fullBrightMod = ModuleManager::getModule<Fullbright>();
		if (fullBrightMod && fullBrightMod->isEnabled()) return 12.f;

		return oFunc ? oFunc(a1) : 1.f;
	}

public:
	GetGammaHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&GetGammaCallback;
	}
};
