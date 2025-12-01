#include "HUD.h"
#include "../../../ModuleManager.h"

HUD::HUD() : Module(Category::RENDER, "HUD", "Displays HUD elements on screen", 0x0, ModuleType::ToggleAllowed) {
    registerSetting(new BoolSetting("Position", "Show player coordinates", &showPosition, true));
    registerSetting(new BoolSetting("Direction", "Show facing direction", &showDirection, false));
    registerSetting(new BoolSetting("Effects", "Show active status effects", &showEffects, true));
    registerSetting(new SliderSetting<int>("Opacity", "Set HUD element opacity", &opacity, 130, 0, 255));
    registerSetting(new SliderSetting<int>("Offset", "Set HUD element offset from screen edge", &offset, 10, 0, 30));
}

HUD::~HUD() {
}

void HUD::onD2DRender() {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (!localPlayer) return;

    Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;
    float textSize = 1.f;
    float textPaddingX = 1.f * textSize;
    float textPaddingY = 1.f * textSize;
    float textHeight = D2D::getTextHeight("", textSize);
    float posX = static_cast<float>(offset);
    float posY = windowSize.y - (textHeight + textPaddingY * 2.f) - static_cast<float>(offset);

    if (showPosition) {
        Vec3<float> lpPos = localPlayer->getPos();
        int dimensionId = localPlayer->getDimensionTypeComponent()->type;

        if (dimensionId == 0) {
            char netherText[50];
            sprintf_s(netherText, 50, "Nether: %.1f, %.1f, %.1f", lpPos.x / 8.f, lpPos.y - 1.6f, lpPos.z / 8.f);
            D2D::fillRectangle(Vec4<float>(posX, posY, posX + D2D::getTextWidth(netherText, textSize, false) + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
            D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), netherText, UIColor(255, 255, 255), textSize, false);
            posY -= textHeight + textPaddingY * 2.f;
        } else if (dimensionId == 1) {
            char overworldText[50];
            sprintf_s(overworldText, 50, "Overworld: %.1f, %.1f, %.1f", lpPos.x * 8.f, lpPos.y - 1.6f, lpPos.z * 8.f);
            D2D::fillRectangle(Vec4<float>(posX, posY, posX + D2D::getTextWidth(overworldText, textSize, false) + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
            D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), overworldText, UIColor(255, 255, 255), textSize, false);
            posY -= textHeight + textPaddingY * 2.f;
        }

        char posText[50];
        sprintf_s(posText, 50, "Position: %.1f, %.1f, %.1f", lpPos.x, lpPos.y - 1.6f, lpPos.z);
        D2D::fillRectangle(Vec4<float>(posX, posY, posX + D2D::getTextWidth(posText, textSize, false) + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
        D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), posText, UIColor(255, 255, 255), textSize, false);
        posY -= textHeight + textPaddingY * 2.f;
    }

    if (showDirection) {
        float rotation = localPlayer->rotation->presentRot.y;
        if (rotation < 0) rotation += 360.0f;

        std::string direction;
        if ((0 <= rotation && rotation < 45) || (315 <= rotation && rotation < 360)) direction = "South (+Z)";
        else if (45 <= rotation && rotation < 135) direction = "West (-X)";
        else if (135 <= rotation && rotation < 225) direction = "North (-Z)";
        else if (225 <= rotation && rotation < 315) direction = "East (+X)";
        else direction = "NULL";

        char dirText[25];
        sprintf_s(dirText, 25, "Direction: %s", direction.c_str());
        D2D::fillRectangle(Vec4<float>(posX, posY, posX + D2D::getTextWidth(dirText, textSize) + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
        D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), dirText, UIColor(255, 255, 255), textSize);
        posY -= textHeight + textPaddingY * 2.f;
    }

    if (showEffects) {
        static Arraylist* arrayListMod = ModuleManager::getModule<Arraylist>();
        Vec2<float> effectPos = Vec2<float>(windowSize.x - static_cast<float>(offset), static_cast<float>(offset));
        if (!arrayListMod->bottom) effectPos.y = windowSize.y - (textHeight + textPaddingY * 2.f) - static_cast<float>(offset);

        for (uint32_t effectId = 1; effectId < 37; effectId++) {
            MobEffect* mEffect = MobEffect::getById(effectId);
            if (!mEffect || !localPlayer->hasEffect(mEffect)) continue;

            MobEffectInstance* mEffectInstance = localPlayer->getEffect(mEffect);
            if (!mEffectInstance) continue;

            std::string name = mEffectInstance->getDisplayName() + " ";
            std::string duration = getEffectTimeLeftStr(mEffectInstance);
            std::string fullText = name + duration;

            Vec4<float> rectPos = Vec4<float>(effectPos.x - D2D::getTextWidth(fullText, textSize, false) - textPaddingX * 2.f,
                                             effectPos.y,
                                             effectPos.x,
                                             effectPos.y + textHeight + textPaddingY * 2.f);

            Vec2<float> textPos = Vec2<float>(rectPos.x + textPaddingX, rectPos.y + textPaddingY);
            Vec2<float> durationPos = Vec2<float>(textPos.x + D2D::getTextWidth(name, textSize, true), textPos.y);

            D2D::fillRectangle(rectPos, UIColor(0, 0, 0, opacity));
            D2D::drawText(textPos, name, mEffect->color, textSize, true);
            D2D::drawText(durationPos, duration, UIColor(255, 255, 255), textSize, false);

            effectPos.y -= (textHeight + textPaddingY * 2.f) * (arrayListMod->bottom ? -1.f : 1.f);
        }
    }
}
