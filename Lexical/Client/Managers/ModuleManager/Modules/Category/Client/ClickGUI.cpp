#include "ClickGUI.h"
#include "../../../../../../Libs/json.hpp"
#include "../../../ModuleManager.h"

ClickGUI::ClickGUI() : Module(Category::CLIENT, "ClickGUI", "Display all modules", VK_INSERT, ModuleType::ToggleAllowed)
 {
	registerSetting(new ColorSetting("Color", "NULL", &mainColor, mainColor));
	registerSetting(new BoolSetting("Description", "Show Description", &showDescription, true));
}

ClickGUI::~ClickGUI() {
	for (auto& window : windowList) {
		delete window;
	}
	windowList.clear();
}

ClickGUI::ClickWindow::ClickWindow(std::string windowName, Vec2<float> startPos, Category c) {
	this->name = windowName;
	this->category = c;
	this->pos = startPos;
	this->extended = true;

	for (auto& mod : ModuleManager::moduleList) {
		if (mod->getCategory() == c) {
			this->moduleList.push_back(mod);
		}
	}

	std::sort(this->moduleList.begin(), this->moduleList.end(), [](Module* lhs, Module* rhs) {
		return lhs->getModuleName() < rhs->getModuleName();
	});
}

void ClickGUI::onDisable() {
	Game::clientInstance->grabMouse();
	
	isLeftClickDown = false;
	isRightClickDown = false;
	isHoldingLeftClick = false;
	isHoldingRightClick = false;

	draggingWindowPtr = nullptr;

	capturingKbSettingPtr = nullptr;
	draggingSliderSettingPtr = nullptr;

	openAnim = 0.0f;
}

void ClickGUI::onEnable() {
	Game::clientInstance->releasebMouse();
	openAnim = 0.0f;
}

bool ClickGUI::isVisible() {
	return false;
}

void ClickGUI::onKeyUpdate(int key, bool isDown) {
	if (!isEnabled()) {
		if (key == getKeybind() && isDown) {
			setEnabled(true);
		}
	}
	else {
		if (isDown) {
			if (key < 192) {
				if (capturingKbSettingPtr != nullptr) {
					if (key != VK_ESCAPE)
						*capturingKbSettingPtr->value = key;
					capturingKbSettingPtr = nullptr;
					return;
				}
			}
			if (key == getKeybind() || key == VK_ESCAPE) {
				setEnabled(false);
			}
		}
	}
}

void ClickGUI::onMouseUpdate(Vec2<float> mousePosA, char mouseButton, char isDown) {

	//MouseButtons
	//0 = mouse move
	//1 = left click
	//2 = right click
	//3 = middle click
	//4 = scroll   (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))

	switch (mouseButton) {
	case 0:
		mousePos = mousePosA;
		break;
	case 1:
		isLeftClickDown = isDown;
		isHoldingLeftClick = isDown;
		break;
	case 2:
		isRightClickDown = isDown;
		isHoldingRightClick = isDown;
		break;
	case 4:
		float moveVec = (isDown < 0) ? -15.f : 15.f;
		for (auto& window : windowList) {
			if (window == draggingWindowPtr)
				continue;

			window->pos.y += moveVec;
		}
		break;
	}

	if (draggingWindowPtr != nullptr) {
		if (!isHoldingLeftClick)
			draggingWindowPtr = nullptr;
	}

	if (capturingKbSettingPtr != nullptr) {
		if (isRightClickDown) {
			*capturingKbSettingPtr->value = 0;
			capturingKbSettingPtr = nullptr;
			isRightClickDown = false;
		}
	}

	if (draggingSliderSettingPtr != nullptr) {
		if (!isHoldingLeftClick)
			draggingSliderSettingPtr = nullptr;
	}
}

void ClickGUI::InitClickGUI() {
	setEnabled(false);

	Vec2<float> startPos = Vec2<float>(25.f, 35.f);
	windowList.push_back(new ClickWindow("Combat", startPos, Category::COMBAT));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Movement", startPos, Category::MOVEMENT));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Render", startPos, Category::RENDER));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Player", startPos, Category::PLAYER));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("World", startPos, Category::WORLD));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Misc", startPos, Category::MISC));
	startPos.x += 265.f;
	windowList.push_back(new ClickWindow("Client", startPos, Category::CLIENT));

	initialized = true;
}

inline float lerpSync(const float& a, const float& b, const float& duration) {
	const float result = (float)(a + (b - a) * duration * (60.f / 165.f));
	if (a < b && result > b) return b;
	else if (a > b && result < b) return b;
	return result;
}

void ClickGUI::Render() {
	if (!initialized)
		return;

	if (Game::canUseMoveKeys())
		Game::clientInstance->releasebMouse();

	static Vec2 oldMousePos = mousePos;
	mouseDelta = mousePos.sub(oldMousePos);
	oldMousePos = mousePos;
	std::string descriptionText = "NULL";

	Vec2 screenSize = Game::clientInstance->guiData->windowSizeReal;
	float deltaTime = D2D::deltaTime;
	static float opacity = 0.f;

	opacity = lerpSync(opacity, this->isEnabled() ? 1.f : 0.f, 0.15f);
	float textSize = 1.f;
	float textPaddingX = 3.f * textSize;
	float textPaddingY = 0.f * textSize;
	float textHeight = D2D::getTextHeight("", textSize);

	D2D::fillRectangle(Vec4(0.f, 0.f, screenSize.x, screenSize.y), UIColor(0, 0, 0, (int)(110 * openAnim)));

	for (auto& window : windowList) {
		if (draggingWindowPtr == window) {
			window->pos = window->pos.add(mouseDelta);
		}

		static CustomFont* customFontMod = ModuleManager::getModule<CustomFont>();
		float fontPercent = (float)customFontMod->fontSize / 25.f;
		float moduleSpace = 2.0f;
		Vec4 hRectPos = Vec4(window->pos.x,
			window->pos.y,
			window->pos.x + (int)(250.f * fontPercent) + (textPaddingX * 2.f),
			window->pos.y + textHeight + (textPaddingY * 2.f));

		Vec2 hTextPos = Vec2(hRectPos.x + textPaddingX, hRectPos.y + textPaddingY);

		if (hRectPos.contains(mousePos)) {
			if (isLeftClickDown) {
				draggingWindowPtr = window;
				isLeftClickDown = false;
			}
			else if (isRightClickDown) {
				window->extended = !window->extended;
				isRightClickDown = false;
			}
		}

		updateSelectedAnimRect(hRectPos, window->selectedAnim);

		D2D::fillRectangle(hRectPos, mainColor);
		D2D::drawText(hTextPos, window->name, UIColor(255, 255, 255), textSize);
		D2D::fillRectangle(hRectPos, UIColor(255, 255, 255, (int)(25 * window->selectedAnim)));

		if (window->extended) {
			float moduleHeight = textHeight + (textPaddingY * 2.f);


			float yOffset = hRectPos.w;

			float wbgPaddingX = 2.f * textSize * fontPercent;
			float yHeight = 0.f;

			for (auto& mod : window->moduleList) {
				yHeight += textHeight + (textPaddingY * 2.f);

				if (mod->extended) {
					float settingSpace = 2.f * textSize * fontPercent;
					yHeight += settingSpace;

					for (auto& setting : mod->getSettingList()) {
						if (setting->type != SettingType::COLOR_S) {
							yHeight += textHeight + (textPaddingY * 2.f);
						}
						else {
							ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
							yHeight += textHeight + (textPaddingY * 2.f);
							if (colorSetting->extended) {
								yHeight += settingSpace;
								for (auto& slider : colorSetting->colorSliders) {
									yHeight += textHeight + (textPaddingY * 2.f) + settingSpace;
								}
								yHeight -= settingSpace;
							}
						}
						yHeight += settingSpace;
					}
				}

				yHeight += moduleSpace;
			}

			Vec4 wbgRectPos = Vec4(hRectPos.x + wbgPaddingX,
				hRectPos.w,
				hRectPos.z - wbgPaddingX,
				hRectPos.w + yHeight);
			D2D::fillRectangle(wbgRectPos, UIColor(0, 0, 0, 75));

			for (auto& mod : window->moduleList) {
				float modPaddingX = wbgPaddingX + (2.f * textSize * fontPercent);
				Vec4 mRectPos = Vec4(hRectPos.x,
					yOffset,
					hRectPos.z,
					yOffset + textHeight + (textPaddingY * 2.f));

				Vec2 mTextPos = Vec2(mRectPos.x + textPaddingX, mRectPos.y + textPaddingY);

				if (mRectPos.contains(mousePos)) {
					descriptionText = mod->getDescription();
					if (isLeftClickDown) {
						mod->toggle();
						isLeftClickDown = false;
					}
					else if (isRightClickDown) {
						mod->extended = !mod->extended;
						isRightClickDown = false;
					}
				}

				updateSelectedAnimRect(mRectPos, mod->selectedAnim);

				D2D::fillRectangle(mRectPos, mod->isEnabled() ? mainColor : UIColor(0, 0, 0, 35));
				D2D::drawText(mTextPos, mod->getModuleName(), mod->isEnabled() ? UIColor(255, 255, 255) : ColorUtil::lerp(UIColor(175, 175, 175), UIColor(255, 255, 255), mod->selectedAnim), textSize);

				yOffset += textHeight + (textPaddingY * 2.f);

				if (mod->extended) {
					float settingSpace = 2.f * textSize * fontPercent;
					yOffset += settingSpace;
					float startY = yOffset;

					float settingPaddingX = 7.5f * textSize * fontPercent;
					float settingPaddingZ = 3.5f * textSize * fontPercent;

					for (auto& setting : mod->getSettingList()) {
						std::string settingName = setting->name;

						Vec4 sRectPos = Vec4(mRectPos.x + settingPaddingX,
							yOffset,
							mRectPos.z - settingPaddingZ,
							yOffset + textHeight + (textPaddingY * 2.f));

						Vec2 sTextPos = Vec2(sRectPos.x + textPaddingX, sRectPos.y + textPaddingY);

						if (sRectPos.contains(mousePos)) {
							descriptionText = setting->description;
						}

						updateSelectedAnimRect(sRectPos, setting->selectedAnim);

						switch (setting->type) {
						case SettingType::BOOL_S: {
							BoolSetting* boolSetting = static_cast<BoolSetting*>(setting);
							bool& boolValue = (*boolSetting->value);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									boolValue = !boolValue;
									isLeftClickDown = false;
								}
							}

							if (boolValue)
								D2D::fillRectangle(sRectPos, mainColor);

							D2D::drawText(sTextPos, settingName, UIColor(255, 255, 255), textSize);

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}
						case SettingType::KEYBIND_S: {
							KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
							int& keyValue = (*keybindSetting->value);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									if (capturingKbSettingPtr == keybindSetting)
										capturingKbSettingPtr = nullptr;
									else
										capturingKbSettingPtr = keybindSetting;

									isLeftClickDown = false;
								}
							}

							std::string keybindName;
							if (setting == capturingKbSettingPtr) {
								keybindName = "...";
							}
							else {
								if (keyValue != 0)
									keybindName = KeyNames[keyValue];
								else
									keybindName = "None";
							}

							Vec2 keybindTextPos = Vec2(sRectPos.z - textPaddingX - D2D::getTextWidth(keybindName, textSize), sTextPos.y);

							D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
							D2D::drawText(keybindTextPos, keybindName, UIColor(255, 255, 255), textSize);

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}
						case SettingType::ENUM_S: {
							EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
							int& enumValue = (*enumSetting->value);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									enumValue++;
									if (enumValue > enumSetting->enumList.size() - 1)
										enumValue = 0;
									isLeftClickDown = false;
								}
								else if (isRightClickDown) {
									enumValue--;
									if (enumValue < 0)
										enumValue = (int)enumSetting->enumList.size() - 1;
									isRightClickDown = false;
								}
							}

							std::string modeName = enumSetting->enumList[enumValue];
							Vec2 modeTextPos = Vec2(sRectPos.z - textPaddingX - D2D::getTextWidth(modeName, textSize), sTextPos.y);

							D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
							D2D::drawText(modeTextPos, modeName, UIColor(255, 255, 255), textSize);

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}

						case SettingType::COLOR_S: {
							ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);

							if (sRectPos.contains(mousePos)) {
								if (isRightClickDown) {
									colorSetting->extended = !colorSetting->extended;
									isRightClickDown = false;
								}
							}

							float colorBoxSize = std::round(textHeight / 1.5f);
							float colorBoxPaddingX = textPaddingX + (2.f * textSize);
							Vec4 colorBoxRect = Vec4(sRectPos.z - colorBoxPaddingX - colorBoxSize,
								(sRectPos.y + sRectPos.w - colorBoxSize) / 2.f,
								sRectPos.z - colorBoxPaddingX,
								(sRectPos.y + sRectPos.w + colorBoxSize) / 2.f);

							D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
							D2D::fillRectangle(colorBoxRect, (*colorSetting->colorPtr));

							yOffset += textHeight + (textPaddingY * 2.f);

							if (colorSetting->extended) {
								yOffset += settingSpace;

								float colorStartX = sRectPos.x;
								float colorStartY = yOffset;

								for (auto& slider : colorSetting->colorSliders) {
									Vec4 colorSliderRect = Vec4(sRectPos.x + settingPaddingX,
										yOffset,
										sRectPos.z - settingPaddingZ,
										yOffset + textHeight + (textPaddingY * 2.f));

									Vec2 colorSliderTextPos = Vec2(colorSliderRect.x + textPaddingX, colorSliderRect.y + textPaddingY);

									updateSelectedAnimRect(colorSliderRect, slider->selectedAnim);

									if (colorSliderRect.contains(mousePos)) {
										if (isLeftClickDown) {
											draggingSliderSettingPtr = slider;
											isLeftClickDown = false;
										}
									}

									uint8_t& value = (*slider->valuePtr);
									float minValue = (float)slider->minValue;
									float maxValue = (float)slider->maxValue;

									if (draggingSliderSettingPtr == slider) {
										float draggingPercent = (mousePos.x - colorSliderRect.x) / (colorSliderRect.z - colorSliderRect.x);

										if (draggingPercent > 1.f)
											draggingPercent = 1.f;
										if (draggingPercent < 0.f)
											draggingPercent = 0.f;

										value = (int)minValue + (int)std::round((maxValue - minValue) * draggingPercent);
									}

									float valuePercent = (value - minValue) / (maxValue - minValue);

									if (valuePercent > 1.f)
										valuePercent = 1.f;
									if (valuePercent < 0.f)
										valuePercent = 0.f;

									Vec4 valueRectPos = Vec4(colorSliderRect.x, colorSliderRect.y,
										colorSliderRect.x + (colorSliderRect.z - colorSliderRect.x) * valuePercent,
										colorSliderRect.w);

									char valueText[10];
									sprintf_s(valueText, 10, "%i", (int)value);
									std::string valueTextStr(valueText);

									Vec2 valueTextPos = Vec2(colorSliderRect.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != slider)), colorSliderTextPos.y);

									D2D::fillRectangle(valueRectPos, mainColor);
									D2D::drawText(colorSliderTextPos, slider->name + ":", UIColor(255, 255, 255), textSize);
									D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != slider));

									D2D::fillRectangle(colorSliderRect, UIColor(255, 255, 255, (int)(25 * slider->selectedAnim)));

									yOffset += textHeight + (textPaddingY * 2.f) + settingSpace;
								}

								yOffset -= settingSpace;

								float colorEndY = yOffset;
								float colorSLineWidth = 4.f * textSize * fontPercent;
								float colorSLinePaddingX = 1.f * textSize;
								Vec4 colorSLineRect = Vec4(colorStartX + colorSLinePaddingX,
									colorStartY,
									colorStartX + colorSLinePaddingX + colorSLineWidth,
									colorEndY);

								D2D::fillRectangle(colorSLineRect, mainColor);
							}
							break;
						}

						case SettingType::SLIDER_S: {
							SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);

							if (sRectPos.contains(mousePos)) {
								if (isLeftClickDown) {
									draggingSliderSettingPtr = sliderSettingBase;
									isLeftClickDown = false;
								}
							}

							if (sliderSettingBase->valueType == ValueType::INT_T) {
								SliderSetting<int>* intSlider = static_cast<SliderSetting<int> *>(sliderSettingBase);
								int& value = (*intSlider->valuePtr);
								float minValue = (float)intSlider->minValue;
								float maxValue = (float)intSlider->maxValue;

								if (draggingSliderSettingPtr == sliderSettingBase) {
									float draggingPercent = (mousePos.x - sRectPos.x) / (sRectPos.z - sRectPos.x);

									if (draggingPercent > 1.f)
										draggingPercent = 1.f;
									if (draggingPercent < 0.f)
										draggingPercent = 0.f;

									value = (int)minValue + (int)std::round((maxValue - minValue) * draggingPercent);
								}

								float valuePercent = (value - minValue) / (maxValue - minValue);

								if (valuePercent > 1.f)
									valuePercent = 1.f;
								if (valuePercent < 0.f)
									valuePercent = 0.f;

								Vec4 valueRectPos = Vec4(sRectPos.x, sRectPos.y,
									sRectPos.x + (sRectPos.z - sRectPos.x) * valuePercent,
									sRectPos.w);

								char valueText[10];
								sprintf_s(valueText, 10, "%i", value);
								std::string valueTextStr(valueText);

								Vec2 valueTextPos = Vec2(sRectPos.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != sliderSettingBase)), sTextPos.y);

								D2D::fillRectangle(valueRectPos, mainColor);
								D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
								D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != sliderSettingBase));
							}
							else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
								SliderSetting<float>* floatSlider = static_cast<SliderSetting<float> *>(sliderSettingBase);
								float& value = (*floatSlider->valuePtr);
								float minValue = floatSlider->minValue;
								float maxValue = floatSlider->maxValue;

								if (draggingSliderSettingPtr == sliderSettingBase) {
									float draggingPercent = (mousePos.x - sRectPos.x) / (sRectPos.z - sRectPos.x);

									if (draggingPercent > 1.f)
										draggingPercent = 1.f;
									if (draggingPercent < 0.f)
										draggingPercent = 0.f;

									value = minValue + (maxValue - minValue) * draggingPercent;
								}

								float valuePercent = (value - minValue) / (maxValue - minValue);

								if (valuePercent > 1.f)
									valuePercent = 1.f;
								if (valuePercent < 0.f)
									valuePercent = 0.f;

								Vec4 valueRectPos = Vec4(sRectPos.x, sRectPos.y,
									sRectPos.x + (sRectPos.z - sRectPos.x) * valuePercent,
									sRectPos.w);

								char valueText[10];
								sprintf_s(valueText, 10, "%.2f", value);
								std::string valueTextStr(valueText);

								Vec2 valueTextPos = Vec2(sRectPos.z - textPaddingX - D2D::getTextWidth(valueTextStr, textSize, (draggingSliderSettingPtr != sliderSettingBase)), sTextPos.y);

								D2D::fillRectangle(valueRectPos, mainColor);
								D2D::drawText(sTextPos, settingName + ":", UIColor(255, 255, 255), textSize);
								D2D::drawText(valueTextPos, valueTextStr, UIColor(255, 255, 255), textSize, (draggingSliderSettingPtr != sliderSettingBase));
							}

							yOffset += textHeight + (textPaddingY * 2.f);
							break;
						}

						}

						D2D::fillRectangle(sRectPos, UIColor(255, 255, 255, (int)(25 * setting->selectedAnim)));
						yOffset += settingSpace;
					}

					float endY = yOffset - settingSpace;
					float sLineWidth = 4.f * textSize * fontPercent;
					float sLinePaddingX = 1.f * textSize;
					Vec4 sLineRect = Vec4(mRectPos.x + sLinePaddingX,
						startY,
						mRectPos.x + sLinePaddingX + sLineWidth,
						endY);

					D2D::fillRectangle(sLineRect, mainColor);
				}

				yOffset += moduleSpace;
			}
		}
	}

	if (showDescription && descriptionText != "NULL" && draggingWindowPtr == nullptr && draggingSliderSettingPtr == nullptr) {
		Vec2 mousePadding = Vec2(15.f, 15.f);
		Vec4 rectPos = Vec4(mousePos.x + mousePadding.x,
			mousePos.y + mousePadding.y,
			mousePos.x + mousePadding.x + D2D::getTextWidth(descriptionText, 0.8f) + 2.f * 2.f,
			mousePos.y + mousePadding.y + D2D::getTextHeight(descriptionText, 0.8f));

		Vec2 textPos = Vec2(rectPos.x, rectPos.y).add(Vec2(2.f, 0.f));
		D2D::fillRectangle(rectPos, UIColor(0, 0, 0, 125));
		D2D::drawText(textPos, descriptionText, UIColor(255, 255, 255), 0.8f);
	}

	isLeftClickDown = false;
	isRightClickDown = false;
}

void ClickGUI::updateSelectedAnimRect(Vec4<float>& rect, float& anim) {
	bool shouldUp = rect.contains(mousePos);

	if (draggingWindowPtr != nullptr)
		shouldUp = false;

	if (draggingSliderSettingPtr != nullptr) {
		if (&draggingSliderSettingPtr->selectedAnim != &anim)
			shouldUp = false;
		else
			shouldUp = true;
	}

	if (shouldUp)
		anim = 1.f;
	else
		anim -= D2D::deltaTime * 2.f;

	if (anim > 1.f)
		anim = 1.f;
	if (anim < 0.f)
		anim = 0.f;
}

using json = nlohmann::json;

void ClickGUI::onLoadConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = this->getModuleName();

	if (conf->contains(modName)) {
		json obj = conf->at(modName);
		if (obj.is_null())
			return;

		if (obj.contains("enabled")) {
			this->setEnabled(obj.at("enabled").get<bool>());
		}

		for (auto& setting : getSettingList()) {
			std::string settingName = setting->name;

			if (obj.contains(settingName)) {
				json confValue = obj.at(settingName);
				if (confValue.is_null())
					continue;

				switch (setting->type) {
				case SettingType::BOOL_S: {
					BoolSetting* boolSetting = static_cast<BoolSetting*>(setting);
					(*boolSetting->value) = confValue.get<bool>();
					break;
				}
				case SettingType::KEYBIND_S: {
					KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
					(*keybindSetting->value) = confValue.get<int>();
					break;
				}
				case SettingType::ENUM_S: {
					EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
					(*enumSetting->value) = confValue.get<int>();
					break;
				}
				case SettingType::COLOR_S: {
					ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
					(*colorSetting->colorPtr) = ColorUtil::HexStringToColor(confValue.get<std::string>());
					break;
				}
				case SettingType::SLIDER_S: {
					SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);
					if (sliderSettingBase->valueType == ValueType::INT_T) {
						SliderSetting<int>* intSlider = static_cast<SliderSetting<int>*>(sliderSettingBase);
						(*intSlider->valuePtr) = confValue.get<int>();
					}
					else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
						SliderSetting<float>* floatSlider = static_cast<SliderSetting<float>*>(sliderSettingBase);
						(*floatSlider->valuePtr) = confValue.get<float>();
					}
					break;
				}
				}
			}
		}

		for (auto& window : windowList) {
			std::string windowName = window->name;

			if (obj.contains(windowName)) {
				json confValue = obj.at(windowName);
				if (confValue.is_null())
					continue;

				if (confValue.contains("isExtended")) {
					window->extended = confValue["isExtended"].get<bool>();
				}

				if (confValue.contains("pos")) {
					window->pos.x = confValue["pos"]["x"].get<float>();
					window->pos.y = confValue["pos"]["y"].get<float>();
				}
			}
		}
	}
}

void ClickGUI::onSaveConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = this->getModuleName();
	json obj = (*conf)[modName];

	obj["enabled"] = this->isEnabled();

	for (auto& setting : getSettingList()) {
		std::string settingName = setting->name;

		switch (setting->type) {
		case SettingType::BOOL_S: {
			BoolSetting* boolSetting = static_cast<BoolSetting*>(setting);
			obj[settingName] = (*boolSetting->value);
			break;
		}
		case SettingType::KEYBIND_S: {
			KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(setting);
			obj[settingName] = (*keybindSetting->value);
			break;
		}
		case SettingType::ENUM_S: {
			EnumSetting* enumSetting = static_cast<EnumSetting*>(setting);
			obj[settingName] = (*enumSetting->value);
			break;
		}
		case SettingType::COLOR_S: {
			ColorSetting* colorSetting = static_cast<ColorSetting*>(setting);
			obj[settingName] = ColorUtil::ColorToHexString((*colorSetting->colorPtr));
			break;
		}
		case SettingType::SLIDER_S: {
			SliderSettingBase* sliderSettingBase = static_cast<SliderSettingBase*>(setting);
			if (sliderSettingBase->valueType == ValueType::INT_T) {
				SliderSetting<int>* intSlider = static_cast<SliderSetting<int>*>(sliderSettingBase);
				obj[settingName] = (*intSlider->valuePtr);
			}
			else if (sliderSettingBase->valueType == ValueType::FLOAT_T) {
				SliderSetting<float>* floatSlider = static_cast<SliderSetting<float>*>(sliderSettingBase);
				obj[settingName] = (*floatSlider->valuePtr);
			}
			break;
		}
		}
	}

	for (auto& window : windowList) {
		obj[window->name]["isExtended"] = window->extended;
		obj[window->name]["pos"]["x"] = window->pos.x;
		obj[window->name]["pos"]["y"] = window->pos.y;
	}

	(*conf)[modName] = obj;
}