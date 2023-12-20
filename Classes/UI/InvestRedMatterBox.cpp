#include "InvestRedMatterBox.h"

#include "Resources.h"

USING_NS_CC;

bool InvestRedMatterBox::init() {
    PurchasableBox::init();

    m_nameLabel->setString("Scale Level");
    m_footerLabel->getConstPart()->setTextColor(cocos2d::Color4B::WHITE);
    m_footerLabel->getVariablePart()->setTextColor(cocos2d::Color4B::WHITE);
    m_footerLabel->setScale(0.8f);
    //m_footerLabel->setIconTexture("icons/EXPSmall.png");
    m_icon->setScale(1.0f);
    m_icon->setTexture("UI/HexiiInfoTab/ScaleLevel.png");
    m_purchaseCostLabel->setIconTexture("icons/RedMatterSmall.png");
    m_purchaseCostLabel->getVariablePart()->setTextColor(cocos2d::Color4B::WHITE);
    //m_icon->setTextureRect(Rect({ 0,0 }, m_icon->getTexture()->getContentSize()));
    //m_icon->setContentSize(m_icon->getTexture()->getContentSize());

    return true;
}

void InvestRedMatterBox::setFocusHexii(Hexii* focus) {
    m_focusHexii = focus;

    if (m_focusHexii == nullptr) return;

    updateLabels();
}

void InvestRedMatterBox::update(float dt) {
    updateIsAffordable();
}

void InvestRedMatterBox::updateValues(BigReal redMatter) {
    BigReal desiredInvestment = 1 + (std::powl(redMatter, 0.5) * 2);

    bool didChange = desiredInvestment != m_desiredInvestment;

    m_desiredInvestment = desiredInvestment;

    // Just to save some computation, only update the labels if the values have changed
    if (didChange) updateLabels();
}

void InvestRedMatterBox::updateLabels() {
    // Update the relevant labels
    m_footerLabel->setVariablePartString(
        "x" + formatBigReal(m_focusHexii->getLevelScale(), false, 2, 3) + " -> " + "x" +
        formatBigReal(GameplayCommon::Sacrifice::redMatterLevelScale(m_focusHexii->getRedMatterInvested() + m_desiredInvestment, m_focusHexii->getLayer()), false, 2, 3)
    );
    m_purchaseCostLabel->setVariablePartString(formatBigReal(m_desiredInvestment));
}

void InvestRedMatterBox::onPurchaseButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
    // Only care about the release
    if (evntType != ui::Widget::TouchEventType::ENDED) return;

    if (m_purchaseButton->isEnabled()) investRedMatter();
}

void InvestRedMatterBox::updateIsAffordable() {
    m_isAffordable = Resources::getInstance()->getRedMatter() >= getCost();
    togglePurchaseable(m_isAffordable);
}

void InvestRedMatterBox::investRedMatter() {
    // Verify affordability
    updateIsAffordable();
    if (!m_isAffordable) return;

    // Guaranteed to be affordable at this point

    // Pay
    Resources::getInstance()->addRedMatter(-getCost());

    m_focusHexii->addRedMatter(m_desiredInvestment);
}
