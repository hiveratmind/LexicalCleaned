#pragma once
#include "Modules/ModuleBase/Module.h"
#include "Modules/Category/Combat/Criticals.h"
#include "Modules/Category/Movement/AirJump.h"
#include "Modules/Category/Movement/Velocity.h"
#include "Modules/Category/Movement/NoSlowDown.h"
#include "Modules/Category/Render/Arraylist.h"
#include "Modules/Category/Render/Nametags.h"
#include "Modules/Category/Render/CustomFov.h"
#include "Modules/Category/Render/Fullbright.h"
#include "Modules/Category/Render/HUD.h"
#include "Modules/Category/Render/NoHurtCam.h"
#include "Modules/Category/Render/NoRender.h"
#include "Modules/Category/Render/Swing.h"
#include "Modules/Category/Misc/NoPacket.h"
#include "Modules/Category/Client/ClickGUI.h"
#include "Modules/Category/Client/CustomFont.h"


class ModuleManager {
public:
	static inline std::vector<Module*> moduleList;

	static void init();
	static void shutdown();

	template <typename TRet>
	static TRet* getModule() {
		for (Module* mod : moduleList) {
			TRet* result = dynamic_cast<TRet*>(mod);
			if (result == nullptr)
				continue;
			return result;
		}
		return nullptr;
	}

	static void onKeyUpdate(int key, bool isDown);
	static void onClientTick();
	static void onClientTick(Packet* packet);
	static void onNormalTick(LocalPlayer* localPlayer);
	static void onLevelTick(Level* level);
	static void onUpdateRotation(LocalPlayer* localPlayer);
	static void onSendPacket(Packet* packet);
	static void onD2DRender();
	static void onMCRender(MinecraftUIRenderContext* renderCtx);
	static void onLevelRender();
	static void onLoadConfig(void* conf);
	static void onSaveConfig(void* conf);
};
