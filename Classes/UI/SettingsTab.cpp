#include "SettingsTab.h"
#include "SaveData.h"
#include "Settings.h"
#include "UICommon.h"

USING_NS_CC;

bool SettingsTab::init() {
    this->setContentSize(UICommon::SIDEBAR_SIZE);

    /// Save button

    m_saveButton = ui::Button::create("widgets/buttons/ButtonGreenNeutral.png", "widgets/buttons/ButtonGreenSelected.png");
    m_saveButton->setAnchorPoint(Vec2(0, 0.5));
    m_saveButton->setPosition(Vec2(213, 163));
    m_saveButton->addTouchEventListener(CC_CALLBACK_2(SettingsTab::onSaveButtonPressed, this));

    Label* saveButtonLabel = Label::createWithTTF("Save", "fonts/SpeakPro.ttf", 48, Size::ZERO, TextHAlignment::CENTER);
    saveButtonLabel->setAnchorPoint(Vec2(0.5, 0.5));
    saveButtonLabel->setPosition(Vec2(78, 34));
    CompoundLabel::setSingleStyle(saveButtonLabel, 48, Color4B::WHITE, Color4B::BLACK, 3);

    m_saveButton->addChild(saveButtonLabel);
    this->addChild(m_saveButton);

    /// Autosave label

    m_autosaveLabel = CompoundLabel::create("Next autosave in: ", "fonts/SpeakPro.ttf", "fonts/SpeakPro.ttf");
    m_autosaveLabel->setStyle(true, true, 48, Color4B::WHITE, Color4B::BLACK, 3);
    m_autosaveLabel->setAnchorPoint(Vec2(0, 0.5));
    m_autosaveLabel->setPosition(Vec2(213, 243));

    this->addChild(m_autosaveLabel);

    return true;
}

void SettingsTab::update(float dt) {
    m_autosaveLabel->setVariablePartString(
        formatBigReal(Settings::getInstance()->getSetting("autosaveFrequencySeconds").get<float>() - SaveData::getInstance()->getTimeSinceLastSave()));
}

void SettingsTab::onSaveButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
    // Only care about release
    if (evntType != ui::Widget::TouchEventType::ENDED) return;
    
    SaveData::getInstance()->save();
}
