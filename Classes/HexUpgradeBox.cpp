#include "HexUpgradeBox.h"
#include "ColorSchemes.h"
#include "Resources.h"

#include <fmt/format.h>

USING_NS_CC;   

bool HexUpgradeBox::init() {
    m_background = Sprite::create("components/HexUpgradeBoxBackground.png");
    m_background->setAnchorPoint(Vec2(0, 0));

    setContentSize(m_background->getContentSize());
    Widget::setFlippedY(true);

    m_purchaseUpgradeButton = ui::Button::create("widgets/buttons/ButtonGreenNeutral.png", "widgets/buttons/ButtonGreenSelected.png", "widgets/buttons/ButtonDisabled.png");
    m_purchaseUpgradeButton->setAnchorPoint(Vec2(1.0, 0.0));
    m_purchaseUpgradeButton->setPosition(Vec2(415, 0));
    m_purchaseUpgradeButton->addTouchEventListener(CC_CALLBACK_2(HexUpgradeBox::onPurchaseUpgradeButtonPressed, this));

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

    m_contributionLabel = Label::createWithTTF("--", "fonts/OCR.ttf", 24, Size(280, 0), TextHAlignment::RIGHT);
    m_contributionLabel->setAnchorPoint(Vec2(1.0, 0.0));
    m_contributionLabel->setPosition(Vec2(405, 15));
    m_contributionLabel->setVisible(false);
    CompoundLabel::setSingleStyle(m_contributionLabel, 24, Color4B(182, 255, 0, 255), Color4B::BLACK, 3, Color4B::WHITE, Size(1, 1), 0);
    
    this->addChild(m_background);
    this->addChild(m_lockedLabel);
    this->addChild(m_icon);
    this->addChild(m_description);
    this->addChild(m_purchaseUpgradeButton);
    this->addChild(m_contributionLabel);

    setState(State::LOCKED);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexLevelUp", CC_CALLBACK_1(HexUpgradeBox::onHexLevelUp, this)), this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("onHexUpgradePurchase", CC_CALLBACK_1(HexUpgradeBox::onHexUpgradePurchase, this)), this);

    return true;
}

void HexUpgradeBox::update(float dt) {
    // Update whether or not upgrade is affordable. Only relevant when in the REVEALED state

    if (m_upgradeState == State::REVEALED) {
        if (Resources::getInstance()->getGreenMatter() >= getCost()) {
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
}
    

void HexUpgradeBox::setUpgrade(UpgradePtr upgrade, Hex* upgradeOwner, State state) {
    m_focus = upgrade;
    m_owner = upgradeOwner;
    
    m_icon->setTexture(m_focus->icon);
    m_description->setString(fmt::to_string(m_focus->description));
    m_lockedLabel->setString("Reveal at\nLevel " + formatBigInt(m_focus->unlockLevel));
    m_costLabel->setVariablePartString(formatBigReal(getCost()));
    m_focusHasContribution = m_focus->contributionDescription != "--";
    updateContributionLabelString();

    setState(state);
}

void HexUpgradeBox::setState(State state) {
    // The button goes away after purchase

    m_upgradeState = state;

    // Most parts aren't visible when locked
    if (m_icon) m_icon->setVisible(m_upgradeState != State::LOCKED);
    if (m_description) m_description->setVisible(m_upgradeState != State::LOCKED);

    // The purchase button is only visible in the reveal state
    m_purchaseUpgradeButton->setEnabled(m_upgradeState == State::REVEALED);
    m_purchaseUpgradeButton->setVisible(m_upgradeState == State::REVEALED);

    // Contribution label is only visible in the purchased state and only when not blank
    m_contributionLabel->setVisible(m_upgradeState == State::PURCHASED && m_contributionLabel->getString() != "--");

    // If the contribution label is blank, instead center the description
    if (m_upgradeState == State::PURCHASED && !m_contributionLabel->isVisible()) {
        m_description->setAnchorPoint(Vec2(0.5, 0.5));
        m_description->setPosition(Vec2(273, 67));
        m_description->setHorizontalAlignment(TextHAlignment::CENTER);
        // Note: an upgrade can't be unpurchased (at least right now), so there doesn't need to be a way to change the description back
    }
    
    if(m_lockedLabel) m_lockedLabel->setVisible(m_upgradeState == State::LOCKED);

    if (m_upgradeState == State::LOCKED) m_background->setTexture("components/HexUpgradeBoxLockedBackground.png");
    else m_background->setTexture("components/HexUpgradeBoxBackground.png");
}

void HexUpgradeBox::onPurchaseUpgradeButtonPressed(Ref*, ui::Widget::TouchEventType evntType) {
    // Nothing to do except on the mouse release
    if (evntType != Widget::TouchEventType::ENDED) return;
    if (!m_isAffordable) return;

    // Guaranteed to be affordable from this point

    setState(State::PURCHASED);

    // Pay and unlock
    Resources::getInstance()->addGreenMatter(-getCost());
    m_owner->unlockUpgrade(m_focus);
}

void HexUpgradeBox::onHexLevelUp(cocos2d::EventCustom* evnt) {
    Hex::EventHexLevelUpData* data = static_cast<Hex::EventHexLevelUpData*>(evnt->getUserData());

    // Only update this box if the hex that leveled up was the owner of this upgrade
    if (data->subject != m_owner) return;

    // Contribution may need to be refreshed
    updateContributionLabelString();
}

void HexUpgradeBox::onHexUpgradePurchase(cocos2d::EventCustom* evnt) {
    Hex::EventHexUpgradePurchaseData* data = static_cast<Hex::EventHexUpgradePurchaseData*>(evnt->getUserData());

    if (data->subject != m_owner) return;

    // Upgrade purchase may require a contribution refresh
    updateContributionLabelString();
}

void HexUpgradeBox::updateContributionLabelString() {
    if (m_focusHasContribution) {
        std::string upgradeName = fmt::to_string(m_focus->name);
        m_contributionLabel->setString(fmt::format(m_focus->contributionDescription,
            formatBigReal(m_owner->getContributionFromUpgrade(upgradeName, false), false, 2, 0),
            formatBigReal(m_owner->getContributionFromUpgrade(upgradeName, true), true, 2, 0)
        ));
    }        
}