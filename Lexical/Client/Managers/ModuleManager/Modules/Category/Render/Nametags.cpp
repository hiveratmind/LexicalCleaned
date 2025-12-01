#include "NameTags.h"

NameTags::NameTags() : Module(Category::RENDER, "NameTags", "Better nametags.", 0x0, ModuleType::ToggleAllowed) {
    registerSetting(new SliderSetting<float>("Opacity", "NULL", &opacity, 1.f, 0.f, 1.f));
    registerSetting(new BoolSetting("Self", "Render urself", &showSelf, true));
}

static bool isInvalidChar(char c) {
    return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
}

static std::string sanitizeString(const std::string& text) {
    std::string out;
    bool wasValid = true;
    for (char c : text) {
        bool isValid = !isInvalidChar(c);
        if (wasValid) {
            if (!isValid) wasValid = false;
            else out += c;
        } else {
            wasValid = isValid;
        }
    }
    return out;
}

void NameTags::onD2DRender() {
    LocalPlayer* lp = Game::getLocalPlayer();
    if (!lp) return;

    for (Actor* actor : lp->getlevel()->getRuntimeActorList()) {
        if (!TargetUtil::isTargetValid(actor) && !(actor == lp && showSelf)) continue;

        Vec2<float> pos;
        if (!Game::clientInstance->WorldToScreen(actor->getEyePos().add2(0.f, 0.75f, 0.f), pos)) continue;

        std::string name = *actor->getNameTag();
        std::string sanitized = sanitizeString(name);

        float textSize = 1.f;
        float textWidth = D2D::getTextWidth(sanitized, textSize);
        float textHeight = D2D::getTextHeight(sanitized, textSize);
        float paddingX = 3.f * textSize;
        float paddingY = 1.f * textSize;

        Vec2<float> textPos(pos.x - textWidth / 2.f, pos.y - textHeight / 2.f);
        Vec4<float> rectPos(textPos.x - paddingX, textPos.y - paddingY, textPos.x + textWidth + paddingX, textPos.y + textHeight + paddingY);

        D2D::fillRectangle(rectPos, UIColor(12, 27, 46, static_cast<int>(255 * opacity)));
        D2D::drawText(textPos, sanitized, UIColor(255, 255, 255, 255), textSize, true);
    }
}

static int waitForInit = 0;

void NameTags::onMCRender(MinecraftUIRenderContext* renderCtx) {
    LocalPlayer* lp = Game::getLocalPlayer();
    if (!lp) return;

    if (waitForInit < 20) {
        waitForInit++;
        return;
    }

    for (Actor* actor : lp->getlevel()->getRuntimeActorList()) {
        if (!TargetUtil::isTargetValid(actor) && !(actor == lp && showSelf)) continue;

        Vec2<float> pos;
        if (!MCR::worldToScreen(actor->getEyePos().add2(0.f, 0.85f, 0.f), pos)) continue;

        std::string name = *actor->getNameTag();
        float textSize = 1.f;
        float textWidth = MCR::getTextWidth(name, textSize);
        float textHeight = MCR::getTextHeight(textSize);
        float paddingX = 2.f * textSize;
        float paddingY = 1.f * textSize;

        Vec2<float> textPos(pos.x - textWidth / 2.f, pos.y - textHeight / 2.f);
        Vec4<float> rectPos(textPos.x - paddingX, textPos.y - paddingY, textPos.x + textWidth + paddingX, textPos.y + textHeight + paddingY);

        if (!showItems) continue;

        float scale = 1.f;
        float spacing = 15.f * scale;
        float totalWidth = 0.f;

        if (actor->getOffhandSlot() && actor->getOffhandSlot()->isValid()) totalWidth += spacing;
        for (int i = 0; i < 4; i++) {
            ItemStack* armor = actor->getArmor(i);
            if (armor && armor->isValid()) totalWidth += spacing;
        }
        if (actor->getCarriedItem() && actor->getCarriedItem()->isValid()) totalWidth += spacing;

        float startX = (rectPos.x + rectPos.z - totalWidth) / 2.f;
        float y = rectPos.y - 5.f;

        if (actor->getOffhandSlot() && actor->getOffhandSlot()->isValid()) {
            MCR::drawItem(actor->getOffhandSlot(), Vec2<float>(startX, y), 1.f, scale);
            startX += spacing;
        }

        for (int i = 0; i < 4; i++) {
            ItemStack* armor = actor->getArmor(i);
            if (armor && armor->isValid()) {
                MCR::drawItem(armor, Vec2<float>(startX, y), 1.f, scale);
                startX += spacing;
            }
        }

        if (actor->getCarriedItem() && actor->getCarriedItem()->isValid()) {
            MCR::drawItem(actor->getCarriedItem(), Vec2<float>(startX, y), 1.f, scale);
        }
    }
}
