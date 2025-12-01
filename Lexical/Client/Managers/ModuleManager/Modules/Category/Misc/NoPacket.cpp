#include "NoPacket.h"

NoPacket::NoPacket() : Module(Category::MISC, "NoPacket", "Stop sending packets to servers\n(This module commonly used for bug/dupe on server)", 0x0, ModuleType::ToggleAllowed)
 {
}