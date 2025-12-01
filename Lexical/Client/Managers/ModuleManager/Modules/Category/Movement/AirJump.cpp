#include "AirJump.h"

AirJump::AirJump() : Module(Category::MOVEMENT, "AirJump", "Allow you to jump even you're not on the ground", 0x0, ModuleType::ToggleAllowed) {
}

void AirJump::onLevelTick(Level* level) {
	if (!level || !Game::getLocalPlayer()) return;
	Game::getLocalPlayer()->setIsOnGround(true);
}