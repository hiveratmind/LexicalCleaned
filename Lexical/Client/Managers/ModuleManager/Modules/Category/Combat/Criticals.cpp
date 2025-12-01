#include "Criticals.h"

Criticals::Criticals() : Module(Category::COMBAT, "Criticals", "Each hit is a critical hit", 0x0, ModuleType::ToggleAllowed) {
	registerSetting(new EnumSetting("Mode", "NULL", { "Local" }, &mode, 0));
}

std::string Criticals::getModeText() {
	switch (mode) {
	case 0: {
		return "Local";
		break;
	}
	}
	return "NULL";
}

void Criticals::onSendPacket(Packet* packet) {
	LocalPlayer* localPlayer = Game::getLocalPlayer();
	if (localPlayer == nullptr) {
		return;
	}

	if (mode == 0) {
		if (packet->getId() == PacketID::PlayerAuthInput) {
			PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
			paip->position.y += 3.f;
		}
	}
}