#include "Arraylist.h"
#include "../../../ModuleManager.h"

Arraylist::Arraylist() : Module(Category::RENDER, "Arraylist", "Display enabled modules on screen", 0x0, ModuleType::ToggleAllowed) {
    registerSetting(new BoolSetting("Bottom", "Position at the bottom of the screen", &bottom, true));
    registerSetting(new BoolSetting("Show Modes", "Display mode names next to modules", &showModes, true));
    registerSetting(new SliderSetting<float>("Size", "Control the size of modules", &size, 1.0f, 0.1f, 2.0f));
    registerSetting(new BoolSetting("Outline", "Enable module outline", &outline, false));
    registerSetting(new ColorSetting("MainColor", "Set the main color of modules", &color, color, false));
    registerSetting(new BoolSetting("Rainbow", "Enable rainbow coloring", &rainbow, false));
    registerSetting(new EnumSetting("ModeOutline", "Outline orientation", { "Horizontal", "Vertical" }, &modeOutline, 0));
    registerSetting(new SliderSetting<int>("Opacity", "Set module opacity", &opacity, 130, 0, 255));
    registerSetting(new SliderSetting<int>("Offset", "Set module offset from screen edge", &offset, 10, 0, 30));
}

bool Arraylist::sortByLength(Module* lhs, Module* rhs) {
    float textSize = 1.0f;
    float textWidth1 = D2D::getTextWidth(lhs->getModuleName(), textSize);
    float textWidth2 = D2D::getTextWidth(rhs->getModuleName(), textSize);

    static Arraylist* arraylistMod = ModuleManager::getModule<Arraylist>();

    if (arraylistMod->showModes) {
        if (lhs->getModeText() != "NULL") textWidth1 += D2D::getTextWidth(" " + lhs->getModeText(), textSize);
        if (rhs->getModeText() != "NULL") textWidth2 += D2D::getTextWidth(" " + rhs->getModeText(), textSize);
    }

    std::string keybindText1 = "[" + std::string(KeyNames[lhs->getKeybind()]) + "]";
    std::string keybindText2 = "[" + std::string(KeyNames[rhs->getKeybind()]) + "]";

    if (keybindText1 != "[Unknown]") textWidth1 += D2D::getTextWidth(" " + keybindText1, textSize);
    if (keybindText2 != "[Unknown]") textWidth2 += D2D::getTextWidth(" " + keybindText2, textSize);

    return textWidth1 > textWidth2;
}


void Arraylist::onD2DRender() {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (!localPlayer) return;

    Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;

    float textSize = size;
    float textPaddingX = 1.f * textSize;
    float textPaddingY = 1.f * textSize;
    float textHeight = D2D::getTextHeight("", textSize);
    float lineWidth = 1.5f * textSize;

    Vec2<float> pos = Vec2<float>(windowSize.x - static_cast<float>(offset), static_cast<float>(offset));
    if (bottom) pos.y = windowSize.y - (textHeight + textPaddingY * 2.f) - static_cast<float>(offset);

    UIColor mainColor = UIColor(color.r, color.g, color.b);
    UIColor modeColor = UIColor(255, 255, 255);

    static std::vector<Module*> moduleList = ModuleManager::moduleList;
    std::sort(moduleList.begin(), moduleList.end(), sortByLength);

    int index = 0;
    Vec4<float> lastRect;

    for (auto& mod : moduleList) {
        std::string moduleName = mod->getModuleName();
        std::string modeText = showModes ? mod->getModeText() : "NULL";
        bool shouldRender = mod->isEnabled() && mod->isVisible();
        std::string keybindText = "[" + std::string(KeyNames[mod->getKeybind()]) + "]";
        if (keybindText == "[Unknown]") keybindText = "";

        if (rainbow) {
            float saturation = 127.5f / 255.f;
            mainColor = ColorUtil::getRainbowColor(7.f, saturation, 1.f, static_cast<int>(index * 200 * (bottom ? -1.f : 1.f)));
        }

        modeColor = mainColor;

        mod->arraylistAnim = Math::moveTowards(mod->arraylistAnim, shouldRender ? 1.05f : -0.1f, D2D::deltaTime * 10.f);
        if (mod->arraylistAnim > 1.f) mod->arraylistAnim = 1.f;
        if (mod->arraylistAnim < 0.f) mod->arraylistAnim = 0.f;
        slider = mod->arraylistAnim;

        if (mod->arraylistAnim > 0.f) {
            float fullTextWidth = D2D::getTextWidth(moduleName, textSize);
            if (modeText != "NULL") fullTextWidth += D2D::getTextWidth(" " + modeText, textSize);
            if (!keybindText.empty()) fullTextWidth += D2D::getTextWidth(" " + keybindText, textSize);

            Vec4<float> rectPos = Vec4<float>(
                pos.x - textPaddingX - fullTextWidth - textPaddingX,
                pos.y,
                pos.x,
                pos.y + textPaddingY + textHeight + textPaddingY
            );

            float animRect = (windowSize.x + 25.f - rectPos.x) * (1.f - mod->arraylistAnim);
            rectPos.x += animRect;
            rectPos.z += animRect;

            float offsetY = 2.0f;
            float offsetX = 0.8f;
            Vec2<float> textPos = Vec2<float>(rectPos.x + textPaddingX + offsetX, rectPos.y + textPaddingY - offsetY);

            D2D::fillRectangle(rectPos, UIColor(0, 0, 0, opacity));

            if (outline) {
                float outlineWidth = lineWidth * 1.7f;
                if (modeOutline == 1) {
                    D2D::fillRectangle(Vec4<float>(rectPos.x - outlineWidth, rectPos.y, rectPos.x, rectPos.w), mainColor);
                    D2D::fillRectangle(Vec4<float>(rectPos.z, rectPos.y, rectPos.z + outlineWidth, rectPos.w), mainColor);
                } else {
                    D2D::fillRectangle(Vec4<float>(rectPos.x - outlineWidth, rectPos.y, rectPos.x, rectPos.w), mainColor);
                    D2D::fillRectangle(Vec4<float>(rectPos.z, rectPos.y, rectPos.z + outlineWidth, rectPos.w), mainColor);
                }
            }

            D2D::drawText(textPos, moduleName, mainColor, textSize, true);
            textPos.x += D2D::getTextWidth(moduleName, textSize);

            if (modeText != "NULL") {
                textPos.x += D2D::getTextWidth(" ", textSize);
                D2D::drawText(textPos, modeText, modeColor, textSize, true);
                textPos.x += D2D::getTextWidth(modeText, textSize);
            }

            if (!keybindText.empty()) {
                textPos.x += D2D::getTextWidth(" ", textSize);
                D2D::drawText(textPos, keybindText, modeColor, textSize, true);
            }

            lastRect = rectPos;
        }

        float yAnim = mod->arraylistAnim * 979.25f;
        if (yAnim > 1.f) yAnim = 1.f;
        pos.y += (textHeight + textPaddingY * 2.f) * yAnim * (bottom ? -1.f : 1.f);

        if (shouldRender) index++;
    }
}
