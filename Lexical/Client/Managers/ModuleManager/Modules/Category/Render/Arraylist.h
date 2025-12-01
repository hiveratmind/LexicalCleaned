#pragma once
#include "../../ModuleBase/Module.h"

class Arraylist : public Module {
private:
    bool showModes = true;
    bool outline = false;
    bool rainbow = false;
    int modeOutline = 0;
    int opacity = 130;
    int offset = 10;
    float size = 1.0f;
    float slider = 0.0f;
    UIColor color;
    static bool sortByLength(Module* lhs, Module* rhs);

public:
    Arraylist();
    bool bottom = true;
    void onD2DRender() override;
};
