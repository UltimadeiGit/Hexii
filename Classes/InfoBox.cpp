#include "InfoBox.h"

USING_NS_CC;

bool InfoBox::init() {
	Sprite::initWithFile("InfoBox.png");

	const auto& size = getContentSize();

	m_greenMatterAmount = Label::createWithTTF("0", "fonts/AgencyBold.ttf", 52, Size::ZERO, TextHAlignment::CENTER);
	m_greenMatterAmount->setTextColor(Color4B(188, 255, 0, 255));
	m_greenMatterAmount->enableOutline(Color4B::BLACK, 1);
	m_greenMatterAmount->enableShadow(Color4B::BLACK, Size::ZERO, 2);
	m_greenMatterAmount->setPosition(m_greenMatterBoxAnchor + Vec2(272, size.height - 55));
	// m_greenMatterAmount->setPosition(m_greenMatterBoxAnchor + Vec2(275, 55 - size.height));

	m_greenMatterIncreaseAmount = Label::createWithTTF("+ 0", "fonts/AgencyBold.ttf", 36, Size::ZERO, TextHAlignment::CENTER);
	m_greenMatterIncreaseAmount->setTextColor(Color4B(255, 255, 255, 255));
	m_greenMatterIncreaseAmount->enableOutline(Color4B::BLACK, 1);
	m_greenMatterIncreaseAmount->enableShadow(Color4B::BLACK, Size::ZERO, 1);
	m_greenMatterIncreaseAmount->setPosition(m_greenMatterBoxAnchor + Vec2(272 - 32, size.height - 115));

	m_perSecLabel = Label::createWithTTF("/ sec", "fonts/AgencyBold.ttf", 30, Size::ZERO, TextHAlignment::LEFT);
	m_perSecLabel->setPosition(Vec2(5 + m_greenMatterIncreaseAmount->getContentSize().width, 0.0f));
	m_perSecLabel->enableOutline(Color4B::BLACK, 1);
	m_perSecLabel->enableShadow(Color4B::BLACK, Size::ZERO, 1);
	m_perSecLabel->setAnchorPoint(Vec2(0, 0));
	m_greenMatterIncreaseAmount->addChild(m_perSecLabel);

	m_upgradesMenu = Menu::create();

	m_hexEXPBar = ProgressBar::create(400.0f, 6);
	m_hexEXPBar->setPosition(Vec2(0.0f, size.height * 0.75f));

	this->addChild(m_greenMatterAmount);
	this->addChild(m_greenMatterIncreaseAmount);
	this->addChild(m_upgradesMenu);
	this->addChild(m_hexEXPBar);

	return true;
}

void InfoBox::update(float dt) {
	if (m_focus == nullptr) {
		m_hexEXPBar->setVisible(false);
		m_upgradesMenu->setVisible(false);
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

void InfoBox::updateGreenMatterAmount(const BigInt& amount) {
	m_greenMatterAmount->setString(amount.to_string());
}

void InfoBox::updateGreenMatterIncreaseAmount(const BigInt& amount) {
	m_greenMatterIncreaseAmount->setString("+ " + amount.to_string());
	m_perSecLabel->setPosition(Vec2(5 + m_greenMatterIncreaseAmount->getContentSize().width, 0.0f));
}
