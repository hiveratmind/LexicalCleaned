#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../../Utils/Math.h"
#include <functional>

class Actor;
class Player;
class InputMode;
class ItemStack;
class Block;

class GameMode {
public:
	CLASS_MEMBER(Player*, player, Offsets::player);
	CLASS_MEMBER(BlockPos, lastBreakPos, Offsets::lastBreakPos);
	CLASS_MEMBER(int32_t, lastBreakFace, Offsets::lastBreakFace);
	CLASS_MEMBER(float, lastDestroyProgress, Offsets::lastDestroyProgress);
	CLASS_MEMBER(float, destroyProgress, Offsets::destroyProgress);
private:
    virtual void Destructor(); // 0 // GameMode Destructor
public:
    virtual __int64 startDestroyBlock(Vec3<int> const& pos, unsigned char blockSide, bool& isDestroyedOut); // 1
    virtual __int64 destroyBlock(const Vec3<int>&, unsigned char blockSide); // 2
    virtual __int64 continueDestroyBlock(Vec3<int> const& a1, unsigned char a2, Vec3<float> const& a3, bool& a4); // 3
    virtual __int64 stopDestroyBlock(Vec3<int> const&); // 4
    virtual __int64 startBuildBlock(Vec3<int> const& a1, unsigned char a2, bool auth); // 5
    virtual __int64 buildBlock(Vec3<int> const& a1, unsigned char a2, bool auth); // 6
    virtual __int64 continueBuildBlock(Vec3<int> const& a1, unsigned char a2); // 7
    virtual __int64 stopBuildBlock(); // 8
    virtual __int64 tick(); // 9
    virtual __int64 getPickRange(InputMode const& a1, bool a2); // 10
    virtual __int64 useItem(class ItemStack& a1); // 11
    virtual __int64 useItemOn(ItemStack& a1, Vec3<int> const& a2, unsigned char a3, Vec3<float> const& a4, class Block const* a5); // 12
    virtual __int64 interact(Actor* a1, Vec3<float> const& a2); // 13
    virtual __int64 attack(Actor*); // 14
    virtual __int64 releaseUsingItem(); // 15
    virtual void setTrialMode(bool a1); // 16
    virtual void isInTrialMode(); // 17
    virtual __int64 registerUpsellScreenCallback(std::function<void> a3); // 18
public:
	bool baseUseItem(ItemStack* item) {
		using func_t = bool(__thiscall*)(GameMode*, ItemStack*);
		static func_t Func = reinterpret_cast<func_t>(Addresses::GameMode_baseUseItem);
		return Func(this, item);
	}

	float getDestroyRate(Block* block) {
		using func_t = float(__thiscall*)(GameMode*, Block*);
		static func_t Func = reinterpret_cast<func_t>(Addresses::GameMode_getDestroyRate);
		return Func(this, block);
	}
};
