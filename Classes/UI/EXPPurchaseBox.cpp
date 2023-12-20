#include "EXPPurchaseBox.h"
#include "Resources.h"

USING_NS_CC;

bool EXPPurchaseBox::init() {
    PurchasableBox::init();

    m_nameLabel->setString("Purchase EXP");
    m_footerLabel->getConstPart()->setTextColor(cocos2d::Color4B::WHITE);
    m_footerLabel->getVariablePart()->setTextColor(cocos2d::Color4B::WHITE);
    m_footerLabel->setIconTexture("icons/EXPSmall.png");
    m_icon->setScale(1.0f);
    m_icon->setTexture("UI/HexiiInfoTab/PurchaseEXP.png");
    m_purchaseCostLabel->getVariablePart()->setTextColor(cocos2d::Color4B::WHITE);
    //m_icon->setTextureRect(Rect({ 0,0 }, m_icon->getTexture()->getContentSize()));
    //m_icon->setContentSize(m_icon->getTexture()->getContentSize());

    return true;
}

void EXPPurchaseBox::setFocusHexii(Hexii* focus) {
    m_focusHexii = focus;

    if (m_focusHexii == nullptr) return;

    updateLabels();
}

void EXPPurchaseBox::update(float dt) {
    updateIsAffordable();
}

void EXPPurchaseBox::updateValues(BigReal exp, BigReal expForNextLevel) {
    // Since exp costs increase after a gaining a level, only buy at most the exp required to reach the next level
    BigReal desiredEXP = std::min(std::floorl(std::max((exp + expForNextLevel) * 0.05, (BigReal)1.0)), expForNextLevel);
    BigReal cost = m_desiredEXP * GameplayCommon::LevelUps::getEXPCost(m_focusHexii->getRawLevel(), m_focusHexii->getLayer());

    bool didChange = desiredEXP != m_desiredEXP || cost != m_cost;

    m_desiredEXP = desiredEXP;
    m_cost = cost;

    // Just to save some computation, only update the labels if the values have changed
    if (didChange) updateLabels();
}

void EXPPurchaseBox::updateLabels() {
    // Update the relevant labels
    m_footerLabel->setVariablePartString(formatBigReal(m_desiredEXP));
    m_purchaseCostLabel->setVariablePartString(formatBigReal(m_cost));
}

void EXPPurchaseBox::onPurchaseButtonPressed(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
    // Only care about the release
    if (evntType != ui::Widget::TouchEventType::ENDED) return;

    if (m_purchaseButton->isEnabled()) purchaseEXP();
}

void EXPPurchaseBox::updateIsAffordable() {
    m_isAffordable = Resources::getInstance()->getGreenMatter() >= getCost(); 
    togglePurchaseable(m_isAffordable);
}

void EXPPurchaseBox::purchaseEXP() {
    m_cost = m_desiredEXP * GameplayCommon::LevelUps::getEXPCost(m_focusHexii->getRawLevel(), m_focusHexii->getLayer());
    updateIsAffordable();
    if(!m_isAffordable) return;

    // Guaranteed to be affordable at this point

    // Pay
    Resources::getInstance()->addGreenMatter(-m_cost);

    m_focusHexii->addEXP(m_desiredEXP);
}
