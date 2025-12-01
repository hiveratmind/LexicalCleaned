#pragma once
#include "../../ModuleBase/Module.h"

class NameTags : public Module {
private:
    float opacity = 1.f;
    bool showSelf = true;
    bool showItems = true;
public:
    NameTags();
    void onD2DRender() override;
    void onMCRender(MinecraftUIRenderContext* renderCtx) override;
};