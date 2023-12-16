#include "EXPInfoTab.h"

#include "Maths.h"
#include <Resources.h>

USING_NS_CC;

EXPInfoTab::EXPInfoTab() {}

bool EXPInfoTab::init() {
    // Purchase EXP

    m_purchaseEXPPlate = cocos2d::Sprite::create("UI/parts/ImagePlate.png");
    m_purchaseEXPPlate->setAnchorPoint({ 0.0f, 0.0f });
    m_purchaseEXPPlate->setPosition(0, 69);
    this->addChild(m_purchaseEXPPlate, 1);

    m_purchaseEXPIcon = cocos2d::Sprite::create("UI/HexiiInfoTab/PurchaseEXP.png");
    m_purchaseEXPIcon->setAnchorPoint({ 0.5f, 0.5f });
    m_purchaseEXPIcon->setPosition(104, 119);
    m_purchaseEXPPlate->addChild(m_purchaseEXPIcon);

    m_purchaseEXPLabel = cocos2d::Label::createWithTTF("Purchase EXP", "fonts/BreeSerif.ttf", 28, Size::ZERO, TextHAlignment::CENTER);
    m_purchaseEXPLabel->setTextColor(Color4B::WHITE);
    m_purchaseEXPLabel->enableOutline(Color4B::BLACK, 1);
    m_purchaseEXPLabel->enableShadow(Color4B::BLACK, Size::ZERO, 2);
    m_purchaseEXPLabel->setAnchorPoint({ 0.5f, 0.5f });
    m_purchaseEXPLabel->setPosition({ 104, 219 });
    m_purchaseEXPPlate->addChild(m_purchaseEXPLabel);

    m_purchaseEXPGainLabel = CompoundLabel::create("+ ", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
    m_purchaseEXPGainLabel->setStyle(true, true, 28, Color4B::WHITE, Color4B::BLACK, 2, Color4B::BLACK, Size(2, -2), 2);
    m_purchaseEXPGainLabel->setSpacingConstraint(5.0f);
    m_purchaseEXPGainLabel->setIconTexture("icons/EXPSmall.png");
    m_purchaseEXPGainLabel->setAnchorPoint({ 0.5, 0.5 });
    m_purchaseEXPGainLabel->setPosition({ 104, 19 });
    m_purchaseEXPPlate->addChild(m_purchaseEXPGainLabel);

    m_purchaseEXPButton = cocos2d::ui::Button::create("UI/buttons/PurchaseButton.png", "UI/buttons/PurchaseButtonSelected.png", "UI/buttons/PurchaseButtonDisabled.png");
    m_purchaseEXPButton->ignoreContentAdaptWithSize(false);
    m_purchaseEXPButton->setContentSize(Size(186, 70));
    m_purchaseEXPButton->setAnchorPoint({ 0.5f, 1.0f });
    m_purchaseEXPButton->setPosition({ 104, 0 });
    m_purchaseEXPButton->addTouchEventListener(CC_CALLBACK_2(EXPInfoTab::onPurchaseEXPButtonPressed, this));
    m_purchaseEXPPlate->addChild(m_purchaseEXPButton);

    m_purchaseEXPButtonCostLabel = CompoundLabel::create("", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
    m_purchaseEXPButtonCostLabel->setStyle(true, true, 28, Color4B::WHITE, Color4B::BLACK, 2, Color4B::BLACK, Size(2, -2), 2);
    m_purchaseEXPButtonCostLabel->setSpacingConstraint(0.0f, 10.0f);
    m_purchaseEXPButtonCostLabel->setIconTexture("icons/GreenMatterSmall.png");
    m_purchaseEXPButtonCostLabel->setAnchorPoint({ 0.5, 0.5 });
    m_purchaseEXPButtonCostLabel->setPosition({ 93, 35 });
    m_purchaseEXPButton->addChild(m_purchaseEXPButtonCostLabel);

    //m_purchaseEXPGainLabel->setVariablePartString(formatBigReal(BigReal(1), true, 3, 0));
    //m_purchaseEXPButtonCostLabel->setVariablePartString(formatBigReal(BigReal(3.59e100), true, 3, 0));

    setContentSize(Size(208, 237));

    return true;
}

void EXPInfoTab::update(float dt) {}

void EXPInfoTab::updateLabels(BigReal exp, BigReal expForNextLevel) {
    /// Update button labels

    // Since exp costs increase after a gaining a level, only buy at most the exp required to reach the next level
    BigReal desiredEXP = std::min(std::floorl(std::max((exp + expForNextLevel) * 0.05, (BigReal)1.0)), expForNextLevel);
    BigReal costToPurchaseDesiredEXP = desiredEXP * m_focusHexii->getEXPCost();

    if (desiredEXP != m_purchaseEXPButtonDesiredEXP || costToPurchaseDesiredEXP != m_costToPurchaseDesiredEXP) {
        m_purchaseEXPButtonDesiredEXP = desiredEXP;
        m_costToPurchaseDesiredEXP = costToPurchaseDesiredEXP;

        // Update the relevant labels
        m_purchaseEXPGainLabel->setVariablePartString(formatBigReal(desiredEXP));
        m_purchaseEXPButtonCostLabel->setVariablePartString(formatBigReal(costToPurchaseDesiredEXP));
    }

    /// Update button state
    // TODO: Make purchase button logic its own class

    // If the exp provided by the button is unaffordable, grey out the button. Otherwise, enabled it again
    if (Resources::getInstance()->getGreenMatter() < costToPurchaseDesiredEXP) {
        if (m_purchaseEXPButton->isEnabled()) {
            m_purchaseEXPButton->setBright(false);
            m_purchaseEXPButton->setEnabled(false);
        }
    }
    else if (!m_purchaseEXPButton->isEnabled()) {
        m_purchaseEXPButton->setBright(true);
        m_purchaseEXPButton->setEnabled(true);
    }
}

void EXPInfoTab::onPurchaseEXPButtonPressed(Ref*, cocos2d::ui::Widget::TouchEventType evntType) {
    // Only care about the release
    if (evntType != ui::Widget::TouchEventType::ENDED) return;

    if (m_purchaseEXPButton->isEnabled()) purchaseEXP();
}

void EXPInfoTab::purchaseEXP() {
    BigReal cost = m_purchaseEXPButtonDesiredEXP * m_focusHexii->getEXPCost();

    // Pay
    Resources::getInstance()->addGreenMatter(-cost);

    m_focusHexii->addEXP(m_purchaseEXPButtonDesiredEXP);
}
