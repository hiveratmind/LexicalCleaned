#pragma once
#include "../../ModuleBase/Module.h"

class HUD : public Module {
private:
    bool showPosition = true;
    bool showDirection = false;
    bool showEffects = true;
    int opacity = 130;
    int offset = 10;

	inline std::string getEffectTimeLeftStr(MobEffectInstance* mEffectInstance) {
		uint32_t timeLeft = mEffectInstance->mDuration;
		uint32_t timeReal = (uint32_t)(timeLeft / 20);
		std::string m = std::to_string(timeReal / 60);
		std::string s;
		if (timeReal % 60 < 10)
			s += "0";
		s += std::to_string(timeReal % 60);
		return m + ":" + s;
	}
public:
    HUD();
    ~HUD();

    void onD2DRender() override;
};
