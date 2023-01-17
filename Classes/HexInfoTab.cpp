#include "HexInfoTab.h"

USING_NS_CC;

bool HexInfoTab::init() {
	m_upgradesList = ui::ScrollView::create();
	m_upgradesList->setAnchorPoint(Vec2(0.0, 0.0));
	m_upgradesList->setDirection(ui::ScrollView::Direction::VERTICAL);
	m_upgradesList->setInnerContainerSize(Size(420, 930));
	m_upgradesList->setContentSize(Size(420, 4650));
	m_upgradesList->setBounceEnabled(true);	
	m_upgradesList->setPosition(Vec2(15, 70));
	m_upgradesList->setBackGroundColor(Color3B::BLACK);

	m_hexEXPBar = ProgressBar::create(400.0f, 6);
	m_hexEXPBar->setAnchorPoint(Vec2(0.0, 1.0));
	m_hexEXPBar->setPosition(Vec2(0.0f, 600));
	
	setContentSize(Size(420, 1320));

	this->addChild(m_upgradesList);
	this->addChild(m_hexEXPBar);

	return true;
}

void HexInfoTab::update(float dt) {
	if (m_focus == nullptr) {
		m_hexEXPBar->setVisible(false);
		m_hexEXPBar->setVisible(false);
		return;
	}

	unsigned int level = m_focus->getLevel();
	unsigned int lastEXPRequirement = m_focus->getEXPRequiredForLevel(level);

	m_hexEXPBar->setProgress(
		// Ratio of current exp into this level over the exp needed to level up
		(float)(m_focus->getEXP() - lastEXPRequirement) /
		(m_focus->getEXPRequiredForLevel(level + 1) - lastEXPRequirement)
	);
}