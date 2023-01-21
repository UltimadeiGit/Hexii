#include "CurrencyHUD.h"
#include "Currencies.h"

USING_NS_CC;

bool CurrencyHUD::init() {
	m_background = Sprite::create("CurrencyHUDBackground.png");
	m_background->setAnchorPoint(Vec2(0.5, 1.0));
	m_background->setPosition(Vec2(0.0f, 0.0f));

	m_greenMatterIcon = Sprite::create("icons/GreenMatter.png");
	m_greenMatterIcon->setScale(1.0 / 3);
	m_greenMatterIcon->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_greenMatterIcon->setPosition(Vec2(-100, -85));

	m_greenMatterAmount = Label::createWithTTF("0", "fonts/AgencyBold.ttf", 52, Size::ZERO, TextHAlignment::CENTER);
	m_greenMatterAmount->setTextColor(Color4B(188, 255, 0, 255));
	m_greenMatterAmount->enableOutline(Color4B::BLACK, 1);
	m_greenMatterAmount->enableShadow(Color4B::BLACK, Size::ZERO, 2);
	m_greenMatterAmount->setPosition(Vec2(100, -85));

	// m_greenMatterAmount->setPosition(m_greenMatterBoxAnchor + Vec2(275, 55 - size.height));

	// GM PER SECOND NOT CURRENTLY BEING USED

	/*

	m_greenMatterIncreaseAmount = Label::createWithTTF("+ 0", "fonts/AgencyBold.ttf", 36, Size::ZERO, TextHAlignment::CENTER);
	m_greenMatterIncreaseAmount->setTextColor(Color4B(255, 255, 255, 255));
	m_greenMatterIncreaseAmount->enableOutline(Color4B::BLACK, 1);
	m_greenMatterIncreaseAmount->enableShadow(Color4B::BLACK, Size::ZERO, 1);
	m_greenMatterIncreaseAmount->setPosition(Vec2(465, 85));

	m_perSecLabel = Label::createWithTTF("/ sec", "fonts/AgencyBold.ttf", 30, Size::ZERO, TextHAlignment::LEFT);
	m_perSecLabel->setPosition(Vec2(5 + m_greenMatterIncreaseAmount->getContentSize().width, 0.0f));
	m_perSecLabel->enableOutline(Color4B::BLACK, 1);
	m_perSecLabel->enableShadow(Color4B::BLACK, Size::ZERO, 1);
	m_perSecLabel->setAnchorPoint(Vec2(0, 0));
	m_greenMatterIncreaseAmount->addChild(m_perSecLabel);

	this->addChild(m_greenMatterIncreaseAmount);

	*/

	setContentSize(m_background->getContentSize());

	this->addChild(m_background);
	this->addChild(m_greenMatterIcon);
	this->addChild(m_greenMatterAmount);

    return true;
}

void CurrencyHUD::update(float dt) {
	m_greenMatterAmount->setString(Currencies::getGreenMatter().to_string());

	/*
	m_greenMatterIncreaseAmount->setString("+ " + Currencies::getGreenMatter().to_string());
	m_perSecLabel->setPosition(Vec2(5 + m_greenMatterIncreaseAmount->getContentSize().width, 0.0f));
	*/
}
