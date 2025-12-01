#pragma once
#include <string>
#include <vector>
#include "../../../../../SDK/Game.h"
#include "../../../../../Renderer/D2D.h"
#include "../../../../../Renderer/MCR.h"
#include "../../../../../Utils/Minecraft/InventoryUtil.h"
#include "../../../../../Utils/Minecraft/PlayerUtil.h"
#include "../../../../../Utils/Minecraft/TargetUtil.h"
#include "../../../../../Utils/Minecraft/WorldUtil.h"
#include "../../../../../Libs/json.hpp"

#include "Settings/Setting.h"
#include "Settings/BoolSetting.h"
#include "Settings/ColorSetting.h"
#include "Settings/EnumSetting.h"
#include "Settings/KeybindSetting.h"
#include "Settings/SliderSetting.h"

enum class Category {
    COMBAT = 0,
    MOVEMENT = 1,
    RENDER = 2,
    PLAYER = 3,
    WORLD = 4,
    MISC = 5,
    CLIENT = 6
};

enum class ModuleType {
    ToggleAllowed = 0,
    AlwaysOn = 1
};

class Module {
private:
    std::string name;
    std::string description;
    Category category;
    ModuleType type;
    bool enabled = false;
    bool visible = true;
    int keybind = 0x0;
    int toggleMode = 0;
    std::vector<Setting*> settings;
public:
    float arraylistAnim = 0.f;
    float selectedAnim = 0.f;
    bool extended = false;
protected:
    inline Setting* registerSetting(Setting* setting) {
        this->settings.push_back(setting);
        return setting;
    }
public:
    Module(Category c, std::string moduleName, std::string des, int k = 0x0, ModuleType t = ModuleType::ToggleAllowed);
    ~Module();

    inline std::string getModuleName() {
        return this->name;
    }

    inline std::string getDescription() {
        return this->description;
    }

    inline Category getCategory() { 
        return this->category;
    }

    inline ModuleType getModuleType() {
        return this->type;
    }

    inline std::vector<Setting*>& getSettingList() {
        return this->settings;
    }
public:
    virtual std::string getModeText();
    virtual bool isEnabled();
    virtual bool isVisible();
    virtual bool isHoldMode();
    virtual int getKeybind();
    virtual void setKeybind(int key);
    virtual bool runOnBackground();
    virtual void setEnabled(bool enable);
    virtual void toggle();
    virtual void onDisable();
    virtual void onEnable();
    virtual void onKeyUpdate(int key, bool isDown);
    virtual void onClientTick();
    virtual void onNormalTick(LocalPlayer* localPlayer);
    virtual void onLevelTick(Level* level);
    virtual void onUpdateRotation(LocalPlayer* localPlayer);
    virtual void onSendPacket(Packet* packet);
    virtual void onD2DRender();
    virtual void onMCRender(MinecraftUIRenderContext* renderCtx);
    virtual void onLevelRender();
    virtual void onLoadConfig(void* conf);
    virtual void onSaveConfig(void* conf);
};
