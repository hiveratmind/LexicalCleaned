#pragma once
#include "Mob.h"
#include "GameMode.h"
#include "Inventory/PlayerInventory.h"
#include "Inventory/Transaction/InventoryTransactionManager.h"
#include "Inventory/Transaction/ComplexInventoryTransaction.h"

class Player : public Mob {
public:
	CLASS_MEMBER(PlayerInventory*, playerInventory, Offsets::playerInventory);
	CLASS_MEMBER(GameMode*, gameMode, Offsets::gameMode);
public:
	InventoryTransactionManager* getTransactionManager() {
		return reinterpret_cast<InventoryTransactionManager*>((uintptr_t)(this) + Offsets::getTransactionManager);
	}
	 void playEmote(std::string const& sex, bool t){
		return MemoryUtil::CallVFunc<70, void, std::string const&, bool>(this, sex, t);
	}
	int getItemUseDuration() {
		return MemoryUtil::CallVFunc<162, int>(this);
	}
};
