#include "HexUpgradeBox.h"
#include "ColorSchemes.h"
#include "Currencies.h"

USING_NS_CC;   

bool HexUpgradeBox::init() {
    m_background = Sprite::create("components/HexUpgradeBoxBackground.png");
    m_background->setAnchorPoint(Vec2(0, 0));

    setContentSize(m_background->getContentSize());
    Widget::setFlippedY(true);

    m_purchaseUpgradeButton = ui::Button::create("widgets/buttons/ButtonGreenNeutral.png", "widgets/buttons/ButtonGreenSelected.png", "widgets/buttons/ButtonDisabled.png");
    m_purchaseUpgradeButton->setAnchorPoint(Vec2(1.0, 0.0));
    m_purchaseUpgradeButton->setPosition(Vec2(415, 0));
    m_purchaseUpgradeButton->addClickEventListener(CC_CALLBACK_1(HexUpgradeBox::onPurchaseUpgradeButtonPressed, this));

    m_costLabel = CompoundLabel::create("", "fonts/Agency.ttf", "fonts/Agency.ttf");
    m_costLabel->setStyle(false, true, 39, UNAFFORDABLE_COLOR, Color4B::BLACK, 3, Color4B::BLACK, Size(0, 0), 0);
    m_costLabel->setIconTexture("icons/GreenMatter.png");
    m_costLabel->setSpacing(25.0f);
    m_costLabel->setAnchorPoint(Vec2(0.5, 0.5));
    m_costLabel->setPosition(Vec2(53, 34));

    m_purchaseUpgradeButton->addChild(m_costLabel);

    m_lockedLabel = Label::createWithTTF("--", "fonts/OCR.ttf", 36, Size::ZERO, TextHAlignment::CENTER);
    m_lockedLabel->setAnchorPoint(Vec2(0.5, 0.5));
    m_lockedLabel->setPosition(Vec2(210, 65));
    CompoundLabel::setSingleStyle(m_lockedLabel, 48, Color4B::WHITE, Color4B::BLACK, 4);

    m_icon = Sprite::create("icons/upgrades/Default.png");
    m_icon->setAnchorPoint(Vec2(0.5, 0.5));
    m_icon->setPosition(Vec2(65, 70));    

    m_description = Label::createWithTTF("--", "fonts/OCR.ttf", 24, Size(280, 0));
    m_description->setAnchorPoint(Vec2(0, 1));
    m_description->setPosition(Vec2(144, 126));
    CompoundLabel::setSingleStyle(m_description, 24, Color4B::WHITE, Color4B::BLACK, 3, Color4B::WHITE, Size(1, 1), 0);
    
    this->addChild(m_background);
    this->addChild(m_lockedLabel);
    this->addChild(m_icon);
    this->addChild(m_description);
    this->addChild(m_purchaseUpgradeButton);

    setState(State::LOCKED);

    return true;
}

void HexUpgradeBox::update(float dt) {
    if (Currencies::getGreenMatter() >= m_focus->greenMatterCost) {
        // Currently marked as unaffordable but it has become affordable, so change the style
        if (!m_isAffordable) {
            m_isAffordable = true;
            m_costLabel->setStyle(false, true, 0, AFFORDABLE_COLOR);
        }        
    } // Currently marked as affordable but it has become unaffordable, so change the style
    else if (m_isAffordable) {
        m_isAffordable = false;
        m_costLabel->setStyle(false, true, 0, UNAFFORDABLE_COLOR);
    }
}

void HexUpgradeBox::setUpgrade(UpgradePtr upgrade, State state) {
    m_focus = upgrade;
    
    m_icon->setTexture(m_focus->icon);
    m_description->setString(std::string(m_focus->description));
    m_lockedLabel->setString("Reveal at\nLevel " + formatBigInt(m_focus->unlockLevel));
    m_costLabel->setVariablePartString(formatBigReal(m_focus->greenMatterCost));

    setState(state);
}

void HexUpgradeBox::setState(State state) {
    // The button goes away after purchase

    m_upgradeState = state;

    // Most parts aren't visible when locked
    if(m_icon) m_icon->setVisible(m_upgradeState != State::LOCKED);
    if(m_description) m_description->setVisible(m_upgradeState != State::LOCKED);

    // The purchase button is only visible in the reveal state
    m_purchaseUpgradeButton->setEnabled(m_upgradeState == State::REVEALED);
    m_purchaseUpgradeButton->setVisible(m_upgradeState == State::REVEALED);

    if(m_lockedLabel) m_lockedLabel->setVisible(m_upgradeState == State::LOCKED);

    if (m_upgradeState == State::LOCKED) m_background->setTexture("components/HexUpgradeBoxLockedBackground.png");
    else m_background->setTexture("components/HexUpgradeBoxBackground.png");
}

void HexUpgradeBox::onPurchaseUpgradeButtonPressed(Ref*) {
    if (!m_isAffordable) return;
    // Guaranteed to be affordable from this point

    setState(State::PURCHASED);

    if (purchaseUpgradeFunction) purchaseUpgradeFunction(m_focus);
}
