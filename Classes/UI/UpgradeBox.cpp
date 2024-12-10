#include "UpgradeBox.h"
#include "ColourSchemes.h"
#include "Resources.h"

#include "UICommon.h"

USING_NS_CC;   

bool UpgradeBox::init() {
    PurchasableBox::init();

    togglePurchaseButtonHoldable(false);

    auto* descriptionButton = Sprite::create("UI/Tooltip/Button.png");
    descriptionButton->setContentSize({ 80, 80 });

    m_descriptionWidget = TooltipWidget::create(descriptionButton, Tooltip::create());
    m_descriptionWidget->setAnchorPoint(Vec2(0.2, 0.5));
    m_descriptionWidget->setPosition({
        UICommon::PURCHASABLE_BOX_WIDTH, 
        UICommon::PURCHASABLE_BOX_HEIGHT - (UICommon::PURCHASABLE_BOX_NAMEPLATE_HEIGHT / 2)
    });
    m_descriptionWidget->getTooltip()->setAnchorPoint({ 0.0, 1.0 });
    this->addChild(m_descriptionWidget, 2);

    setState(Upgrade::State::LOCKED);

    return true;
}

void UpgradeBox::update(float dt) {
    m_descriptionWidget->update(dt);

    // Update whether or not upgrade is affordable. Only relevant when in the AVAILABLE state
    if (m_upgradeState != Upgrade::State::AVAILABLE) return;

    updateIsAffordable();
}

void UpgradeBox::setUpgrade(UpgradePtr upgrade, Hexii* subject, UpgradeTrackerPtr tracker) {
    m_focus = upgrade;
    m_subject = subject;
    m_subjectUpgradeTracker = tracker;

    // Most of the details for this box can be set as long as the upgrade is known
    if (upgrade == nullptr) return;

    m_nameLabel->setString(fmt::to_string(m_focus->friendlyName));
    m_icon->setTexture(m_focus->icon);
    m_icon->setTextureRect(Rect({ 0, 0 }, m_focus->icon->getContentSize()));

    auto descriptionContent = cocos2d::ui::RichText::create();
    descriptionContent->pushBackElement(cocos2d::ui::RichElementText::create(0, { 255, 255, 255 }, 255, fmt::to_string(m_focus->description), "fonts/BreeSerif.ttf", 38));
    m_descriptionWidget->getTooltip()->setContent(descriptionContent);

    //m_description->setString(fmt::to_string(m_focus->description));
    m_lockedLabel->setString("Need level " + formatBigInt(m_focus->levelRequirement) + "\nto reveal");
    m_focusHasContribution = m_focus->contributionDescription != "";
    
    // The rest of the details for this box cannot be set until the subject and tracker are known
    if (subject == nullptr || tracker == nullptr) return;
    
    updateLabels();
    setState(tracker->getState(upgrade));
}

void UpgradeBox::setState(Upgrade::State state) {
    // The button goes away after purchase

    m_upgradeState = state;

    // Most parts aren't visible when locked
    if (m_icon) m_icon->setVisible(m_upgradeState != Upgrade::State::LOCKED);
    //if (m_description) m_description->setVisible(m_upgradeState != Upgrade::State::LOCKED);

    // The purchase button is only visible in the reveal state
    togglePurchaseable(m_upgradeState == Upgrade::State::AVAILABLE);

    // Contribution label is only visible in the OWNED state and only when not blank
    m_footerLabel->setVisible(m_upgradeState == Upgrade::State::OWNED && m_focusHasContribution);

    /*
    // If the contribution label is blank, instead center the description
    if (m_upgradeState == Upgrade::State::OWNED && !m_footerLabel->isVisible()) {
        m_description->setAnchorPoint(Vec2(0.5, 0.5));
        m_description->setPosition(Vec2(273, 67));
        m_description->setHorizontalAlignment(TextHAlignment::CENTER);
        // Note: an upgrade can't be unpurchased (at least right now), so there doesn't need to be a way to change the description back
    }
    */
    
    if(m_lockedLabel) m_lockedLabel->setVisible(m_upgradeState == Upgrade::State::LOCKED);

    //if (m_upgradeState == Upgrade::State::LOCKED) m_background->setTexture("components/HexUpgradeBoxLockedBackground.png");
    //else m_background->setTexture("components/HexUpgradeBoxBackground.png");

    updateLabels();
}

void UpgradeBox::tryPurchase() {
    // Verify again that the upgrade is affordable
    updateIsAffordable();
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
    updateContributionLabel();
}
*/

void UpgradeBox::updateIsAffordable() {
    bool affordable = Resources::getInstance()->getGreenMatter() >= getCost();

    if (m_isAffordable == affordable) return;

    m_isAffordable = affordable;

    m_purchaseCostLabel->setStyle(false, true, 0, m_isAffordable ? AFFORDABLE_GREEN : UNAFFORDABLE_RED);
}

void UpgradeBox::updateLabels() {
    if (m_focus == nullptr) return;

    // Update the cost label
    m_purchaseCostLabel->setVariablePartString(formatBigReal(getCost()));

    // Update the contribution label (footer)
    if (m_focusHasContribution && m_footerLabel->isVisible()) {
        std::string upgradeName = fmt::to_string(m_focus->name);
        m_footerLabel->setVariablePartString(fmt::format(m_focus->contributionDescription,
            formatBigReal(m_subject->getContributionFromUpgrade(upgradeName/*, false*/), false, 2, 0)
            //,formatBigReal(m_subject->getContributionFromUpgrade(upgradeName, true), true, 2, 0)
        ));
    }
}