#include "HexiiEXPTab.h"
#include "UICommon.h"

#include "Maths.h"
#include <Resources.h>

USING_NS_CC;

HexiiEXPTab::HexiiEXPTab() {}

bool HexiiEXPTab::init() {

    m_expPurchaseBox = EXPPurchaseBox::create();
    this->addChild(m_expPurchaseBox, 1);

    m_investRedMatterBox = InvestRedMatterBox::create();
    m_investRedMatterBox->setPosition({ UICommon::PURCHASABLE_BOX_SPACING + UICommon::PURCHASABLE_BOX_WIDTH, 0 });
    this->addChild(m_investRedMatterBox, 1);

    setContentSize(m_expPurchaseBox->getContentSize());

    return true;
}

void HexiiEXPTab::setFocus(Hexii* focus) {
    m_focusHexii = focus;
	m_expPurchaseBox->setFocusHexii(focus);
    m_investRedMatterBox->setFocusHexii(focus);
}

void HexiiEXPTab::update(float dt) {
    m_expPurchaseBox->update(dt);
    m_investRedMatterBox->update(dt);
}

void HexiiEXPTab::updateEXP(BigReal exp, BigReal expForNextLevel) {
    m_expPurchaseBox->updateValues(exp, expForNextLevel);
}

void HexiiEXPTab::updateRedMatter(BigReal redMatter) {
    m_investRedMatterBox->updateValues(redMatter);
}