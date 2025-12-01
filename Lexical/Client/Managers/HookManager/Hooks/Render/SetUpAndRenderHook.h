#pragma once
#include "../FuncHook.h"
#include "../DirectX/PresentHook.h"
#include "../../../../../Renderer/MCR.h"

class SetUpAndRenderHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(ScreenView*, MinecraftUIRenderContext*);
	static inline func_t oFunc;

	static void ScreenView_SetUpAndRenderCallback(ScreenView* _this, MinecraftUIRenderContext* renderCtx) {
		if (!_this || !renderCtx) {
			if (oFunc) oFunc(_this, renderCtx);
			return;
		}

		if (oFunc) oFunc(_this, renderCtx);

		uintptr_t* visualTree = nullptr;
		std::string rootControlName;

		auto visualTreePtr = (uintptr_t**)((uintptr_t)_this + Offsets::visualTree);
		if (visualTreePtr) visualTree = *visualTreePtr;
		if (visualTree) {
			auto rootNamePtr = (std::string*)((uintptr_t)visualTree + Offsets::rootControlName);
			if (rootNamePtr) rootControlName = *rootNamePtr;
		}

		static const std::string debugScreen = "debug_screen.debug_screen";
		if (!rootControlName.empty() && strcmp(rootControlName.c_str(), debugScreen.c_str()) == 0) {
			MCR::onRenderScreen(renderCtx);
			MCR::deltaTime = _this->deltaTime;
		}
	}

public:
	SetUpAndRenderHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ScreenView_SetUpAndRenderCallback;
	}
};
