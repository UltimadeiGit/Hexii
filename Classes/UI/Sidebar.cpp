#include "Sidebar.h"
#include "GameplayCommon.h"
#include "Progression.h"
#include "CocosUtility.h"

bool Sidebar::init() {

    m_background = cocos2d::Sprite::create("UI/Sidebar/Background.png");
    m_background->setAnchorPoint({0, 1});
    this->addChild(m_background);

    // Buttons

    constexpr float TOP_BUTTON_Y = -49;
    constexpr float BUTTON_SCALE = 0.85f;
    constexpr float BUTTON_VERTICAL_SPACING = 198;
    int buttonIndex = 0;

    m_sacrificeButton = cocos2d::ui::Button::create("UI/Sidebar/SacrificeButton.png", "UI/Sidebar/SacrificeButtonSelected.png", "UI/Sidebar/SacrificeButtonDisabled.png");
    m_sacrificeButton->setAnchorPoint({0, 1});
    m_sacrificeButton->setScale(BUTTON_SCALE);
    m_sacrificeButton->setPosition({0, TOP_BUTTON_Y - BUTTON_VERTICAL_SPACING * buttonIndex++});
    m_sacrificeButton->addTouchEventListener(CC_CALLBACK_2(Sidebar::onSacrificeButtonPressed, this));
    m_sacrificeButton->setEnabled(false);
    m_sacrificeButton->setBright(false);
    // Sacrifice button should be disabled until the player has unlocked the ability to sacrifice
    Progression::sacrificeUnlocked()->when(ProgressionEvent::State::ACHIEVED, [this]() {
        m_sacrificeButton->setEnabled(true);
        m_sacrificeButton->setBright(true);
    });
    this->addChild(m_sacrificeButton);

    for (; buttonIndex < 4; buttonIndex++) {
        m_miscButtons[buttonIndex - 1] = cocos2d::ui::Button::create("UI/Sidebar/LockedButton.png", "UI/Sidebar/LockedButton.png", "UI/Sidebar/LockedButton.png");
        m_miscButtons[buttonIndex - 1]->setAnchorPoint({ 0, 1 });
        m_miscButtons[buttonIndex - 1]->setScale(BUTTON_SCALE);
        m_miscButtons[buttonIndex - 1]->setPosition({ 0, TOP_BUTTON_Y - BUTTON_VERTICAL_SPACING * buttonIndex });
        m_miscButtons[buttonIndex - 1]->setEnabled(false);
        this->addChild(m_miscButtons[buttonIndex - 1]);
    }

    // The vertical gap is 29 pixels for each button

    cocos2d::Size size = m_background->getContentSize();

    setContentSize(size);

    // Hide the sidebar until the player is about to unlock sacrifice (so that event is Encouraging)

    useVisibleOrigin;
    setPosition(origin.x - size.width, getPositionY());
    Progression::sacrificeUnlocked()->when(Progression::State::ENCOURAGING, [this]() {
        useVisibleOrigin;
        // Animate the sidebar in
        this->runAction(cocos2d::EaseQuadraticActionIn::create(
            cocos2d::MoveTo::create(0.8f, { origin.x, getPositionY() })
        ));
    });

    return true;
}

void Sidebar::onSacrificeButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
    // Only care about release
    if (evntType != cocos2d::ui::Widget::TouchEventType::ENDED) return;

    EventUtility::dispatchEvent(GameplayCommon::GameEvent::EVENT_SACRIFICE_INITIATED, eventID);
}
