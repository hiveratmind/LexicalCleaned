#pragma once
#include "../../Utils/MemoryUtil.h"
#include "ActorRenderDispatcher.h"
#include "ItemRenderer.h"

class ScreenContext;
class ClientInstance;
class MinecraftGame;

class BaseActorRenderContext {
private:
	char pad_0x0[0x2A0];
public:
	CLASS_MEMBER(ScreenContext*, screenContext, Offsets::screenContext);
	CLASS_MEMBER(ActorRenderDispatcher*, actorRenderDispatcher, Offsets::actorRenderDispatcher);
	CLASS_MEMBER(ItemRenderer*, itemRenderer, Offsets::itemRenderer);
public:
	BaseActorRenderContext() {
		memset(this, 0x0, sizeof(BaseActorRenderContext));
	}

	BaseActorRenderContext(ScreenContext* screenContext, ClientInstance* clientInstance, MinecraftGame* mcGame) {
		memset(this, 0x0, sizeof(BaseActorRenderContext));
		using BaseActorRenderContext_t = void(__fastcall*)(BaseActorRenderContext*, ScreenContext*, ClientInstance*, MinecraftGame*);
		static BaseActorRenderContext_t BaseActorRenderContext_constructor = reinterpret_cast<BaseActorRenderContext_t>(Addresses::BaseActorRenderContext_Constructor);
		BaseActorRenderContext_constructor(this, screenContext, clientInstance, mcGame);
	}
};