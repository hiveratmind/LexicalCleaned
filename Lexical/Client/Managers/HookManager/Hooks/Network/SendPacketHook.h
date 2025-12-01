#pragma once
#include "../FuncHook.h"

int lastSlot = 0;

class SendPacketHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(PacketSender*, Packet*);
	static inline func_t oFunc;

	static void SendPacketCallback(PacketSender* _this, Packet* packet) {
		if (!_this || !packet) {
			if (oFunc) oFunc(_this, packet);
			return;
		}

		auto noPacketMod = ModuleManager::getModule<NoPacket>();
		auto localPlayer = Game::getLocalPlayer();

		if (noPacketMod && noPacketMod->isEnabled() && localPlayer) 
			return;

		ModuleManager::onSendPacket(packet);

		if (oFunc) oFunc(_this, packet);
	}

public:
	SendPacketHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&SendPacketCallback;
	}
};
