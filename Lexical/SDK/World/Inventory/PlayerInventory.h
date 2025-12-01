#pragma once
#include "Container.h"

class PlayerInventory {
public:
	CLASS_MEMBER(int32_t, selectedSlot, Offsets::selectedSlot);
	CLASS_MEMBER(Container*, container, Offsets::container);
};