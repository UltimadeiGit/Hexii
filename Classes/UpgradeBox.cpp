#include "UpgradeBox.h"
#include "ColourSchemes.h"
#include "Resources.h"
#include "Specialization.h"

#include <fmt/format.h>

USING_NS_CC;   

bool UpgradeBox::init() {
    //m_background = Sprite::create("components/HexUpgradeBoxBackground.png");
    //m_background->setAnchorPoint(Vec2(0, 0));

    m_upgradePlate = cocos2d::Sprite::create("UI/parts/ImagePlate.png");
    m_upgradePlate->setAnchorPoint({ 0.0f, 0.0f });
    m_upgradePlate->setPosition({ 0, 69 });
    this->addChild(m_upgradePlate, 1);

    m_icon = cocos2d::Sprite::create("icons/Missing.png");
    m_icon->setScale(0.32);
    m_icon->setAnchorPoint({ 0.5f, 0.5f });
    m_icon->setPosition(104, 119);
    m_upgradePlate->addChild(m_icon);

    //Widget::setFlippedY(true);

    m_nameLabel = cocos2d::Label::createWithTTF("???", "fonts/BreeSerif.ttf", 28, Size::ZERO, TextHAlignment::CENTER);
    m_nameLabel->setTextColor(Color4B::WHITE);
    m_nameLabel->enableOutline(Color4B::BLACK, 2);
    m_nameLabel->enableShadow(Color4B::BLACK, Size(1, 1), 2);
    m_nameLabel->setAnchorPoint({ 0.5f, 0.5f });
    m_nameLabel->setPosition({ 104, 219 });
    m_upgradePlate->addChild(m_nameLabel);

    m_contributionLabel = CompoundLabel::create("", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
    m_contributionLabel->setStyle(true, true, 28, Color4B(182, 255, 0, 255), Color4B::BLACK, 2, Color4B::BLACK, Size(2, -2), 2);
    m_contributionLabel->setSpacingConstraint(5.0f);
    m_contributionLabel->setAnchorPoint({ 0.5, 0.5 });
    m_contributionLabel->setPosition({ 104, 19 });
    m_upgradePlate->addChild(m_contributionLabel);

    m_purchaseUpgradeButton = cocos2d::ui::Button::create("UI/buttons/PurchaseButton.png", "UI/buttons/PurchaseButtonSelected.png", "UI/buttons/PurchaseButtonDisabled.png");
    m_purchaseUpgradeButton->ignoreContentAdaptWithSize(false);
    m_purchaseUpgradeButton->setContentSize(Size(186, 70));
    m_purchaseUpgradeButton->setAnchorPoint({ 0.5f, 1.0f });
    m_purchaseUpgradeButton->setPosition({ 104, 0 });
    m_purchaseUpgradeButton->addTouchEventListener(CC_CALLBACK_2(UpgradeBox::onPurchaseUpgradeButtonPressed, this)); 
    m_purchaseUpgradeButton->setCascadeColorEnabled(true);
    m_upgradePlate->addChild(m_purchaseUpgradeButton);

    m_purchaseUpgradeCostLabel = CompoundLabel::create("", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
    m_purchaseUpgradeCostLabel->setStyle(true, true, 34, UNAFFORDABLE_RED, Color4B::BLACK, 2, Color4B::BLACK, Size(2, -2), 2);
    m_purchaseUpgradeCostLabel->setSpacingConstraint(0.0f, 10.0f);
    m_purchaseUpgradeCostLabel->setIconTexture("icons/GreenMatterSmall.png");
    m_purchaseUpgradeCostLabel->setAnchorPoint({ 0.5, 0.5 });
    m_purchaseUpgradeCostLabel->setPosition({ 93, 35 });
    m_purchaseUpgradeButton->addChild(m_purchaseUpgradeCostLabel);

    m_lockedLabel = Label::createWithTTF("--", "fonts/BreeSerif.ttf", 28, Size(140, 140), TextHAlignment::CENTER);
    m_lockedLabel->setContentSize(Size(140, 140));
    m_lockedLabel->setAnchorPoint({0.5, 0.5});
    m_lockedLabel->setPosition({104, 110});
    CompoundLabel::setSingleStyle(m_lockedLabel, 28, Color4B::WHITE, Color4B::BLACK, 4);
    m_upgradePlate->addChild(m_lockedLabel);

    /*m_description = Label::createWithTTF("--", "fonts/OCR.ttf", 24, Size(280, 0));
    m_description->setAnchorPoint(Vec2(0, 1));
    m_description->setPosition(Vec2(144, 126));
    CompoundLabel::setSingleStyle(m_description, 24, Color4B::WHITE, Color4B::BLACK, 3, Color4B::WHITE, Size(1, 1), 0);
    */
    /*
    m_contributionLabel = Label::createWithTTF("--", "fonts/OCR.ttf", 24, Size(280, 0), TextHAlignment::RIGHT);
    m_contributionLabel->setAnchorPoint(Vec2(1.0, 0.0));
    m_contributionLabel->setPosition(Vec2(405, 15));
    m_contributionLabel->setVisible(false);
    CompoundLabel::setSingleStyle(m_contributionLabel, 24, Color4B(182, 255, 0, 255), Color4B::BLACK, 3, Color4B::WHITE, Size(1, 1), 0);
    */

    //this->addChild(m_background);
    //this->addChild(m_lockedLabel);
    //this->addChild(m_icon);
    //this->addChild(m_description);
    //this->addChild(m_purchaseUpgradeButton);
    //this->addChild(m_contributionLabel);

    setContentSize({ 208, 307 });

    setState(Upgrade::State::LOCKED);

    return true;
}

void UpgradeBox::update(float dt) {
    // Update whether or not upgrade is affordable. Only relevant when in the AVAILABLE state

    if (m_upgradeState != Upgrade::State::AVAILABLE) return;

    if (Resources::getInstance()->getGreenMatter() >= getCost()) {
        // Currently marked as unaffordable but it has become affordable, so change the style
        if (!m_isAffordable) {
            m_isAffordable = true;
            m_purchaseUpgradeCostLabel->setStyle(false, true, 0, AFFORDABLE_GREEN);
        }
    } // Currently marked as affordable but it has become unaffordable, so change the style
    else if (m_isAffordable) {
        m_isAffordable = false;
        m_purchaseUpgradeCostLabel->setStyle(false, true, 0, UNAFFORDABLE_RED);
    }
}

BigReal UpgradeBox::getCost() {
    return m_focus->greenMatterCost * Hexii::getUpgradePurchaseCostMultiplier(m_subject->getLayer());
}

void UpgradeBox::setUpgrade(UpgradePtr upgrade, Hexii* subject, UpgradeTrackerPtr tracker) {
    //_eventDispatcher->removeEventListenersForTarget(this);

    m_focus = upgrade;
    m_subject = subject;
    m_subjectUpgradeTracker = tracker;

    //EventUtility::addTargetedEventListener(Hexii::EVENT_LEVEL_GAINED, this, m_subject->eventID, &UpgradeBox::onLevelGained);

    // Most of the details for this box can be set as long as the upgrade is known
    if (upgrade == nullptr) return;

    m_nameLabel->setString(fmt::to_string(m_focus->friendlyName));
    m_icon->setTexture(m_focus->icon);
    m_icon->setTextureRect(Rect({ 0, 0 }, m_focus->icon->getContentSize()));
    //m_description->setString(fmt::to_string(m_focus->description));
    m_lockedLabel->setString("Need level " + formatBigInt(m_focus->levelRequirement) + "\nto reveal");
    m_focusHasContribution = m_focus->contributionDescription != "--";
    
    // The rest of the details for this box cannot be set until the subject and tracker are known
    if (subject == nullptr || tracker == nullptr) return;

    m_purchaseUpgradeCostLabel->setVariablePartString(formatBigReal(getCost()));
    
    updateContributionLabelString();
    setState(tracker->getState(upgrade));
}

void UpgradeBox::setState(Upgrade::State state) {
    // The button goes away after purchase

    m_upgradeState = state;

    // Most parts aren't visible when locked
    if (m_icon) m_icon->setVisible(m_upgradeState != Upgrade::State::LOCKED);
    //if (m_description) m_description->setVisible(m_upgradeState != Upgrade::State::LOCKED);

    // The purchase button is only visible in the reveal state
    m_purchaseUpgradeButton->setEnabled(m_upgradeState == Upgrade::State::AVAILABLE);
    m_purchaseUpgradeButton->setVisible(m_upgradeState == Upgrade::State::AVAILABLE);

    // Contribution label is only visible in the OWNED state and only when not blank
    m_contributionLabel->setVisible(m_upgradeState == Upgrade::State::OWNED && m_contributionLabel->getVariablePart()->getString() != "--");

    /*
    // If the contribution label is blank, instead center the description
    if (m_upgradeState == Upgrade::State::OWNED && !m_contributionLabel->isVisible()) {
        m_description->setAnchorPoint(Vec2(0.5, 0.5));
        m_description->setPosition(Vec2(273, 67));
        m_description->setHorizontalAlignment(TextHAlignment::CENTER);
        // Note: an upgrade can't be unpurchased (at least right now), so there doesn't need to be a way to change the description back
    }
    */
    
    if(m_lockedLabel) m_lockedLabel->setVisible(m_upgradeState == Upgrade::State::LOCKED);

    //if (m_upgradeState == Upgrade::State::LOCKED) m_background->setTexture("components/HexUpgradeBoxLockedBackground.png");
    //else m_background->setTexture("components/HexUpgradeBoxBackground.png");

    updateContributionLabelString();
}

void UpgradeBox::onPurchaseUpgradeButtonPressed(Ref*, ui::Widget::TouchEventType evntType) {
    // Nothing to do except on the mouse release
    if (evntType != Widget::TouchEventType::ENDED) return;
    if (!m_isAffordable) return;

    // Guaranteed to be affordable from this point

    // Pay and unlock
    Resources::getInstance()->addGreenMatter(-getCost());
    m_subject->purchaseUpgrade(m_subjectUpgradeTracker, m_focus);

    setState(Upgrade::State::OWNED);    
}

/*
void UpgradeBox::onLevelGained(cocos2d::EventCustom* evnt) {
    auto data = EventUtility::getEventData<Specialization::EventLevelGainedData>(evnt);

    // Check if state changed
    for (uint i = 0; i < data->changedStates.size(); i++) {
        if (m_focus == data->changedStateUpgrades[i]) {
            setState(data->changedStates[i]);
        }
    }

    // Contribution may need to be refreshed
    updateContributionLabelString();
}
*/

void UpgradeBox::updateContributionLabelString() {
    if (m_focusHasContribution && m_contributionLabel->isVisible()) {
        std::string upgradeName = fmt::to_string(m_focus->name);
        m_contributionLabel->setVariablePartString(fmt::format(m_focus->contributionDescription,
            formatBigReal(m_subject->getContributionFromUpgrade(upgradeName, false), false, 2, 0),
            formatBigReal(m_subject->getContributionFromUpgrade(upgradeName, true), true, 2, 0)
        ));
    }        
}