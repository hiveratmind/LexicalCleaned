#include "NoSlowDown.h"

NoSlowDown::NoSlowDown() : Module(Category::MOVEMENT, "NoSlowDown", "Don't get slowed down when eating/using item", 0x0, ModuleType::ToggleAllowed) {
}