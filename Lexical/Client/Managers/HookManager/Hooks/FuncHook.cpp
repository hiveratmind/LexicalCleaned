#include "FuncHook.h"
#include <MinHook.h>

bool FuncHook::enableHook() {
	if (!enable) {
		if (!address) {
			return false;
		}
		if (MH_CreateHook((void*)address, func, reinterpret_cast<LPVOID*>(OriginFunc)) != MH_OK) {
			return false;
		}
		if (MH_EnableHook((void*)address) != MH_OK) {
			return false;
		}
		enable = true;
		return true;
	}
	return true;
}

void FuncHook::onHookRequest() {
}