#include "CurrencyHUD.h"
#include "Resources.h"

USING_NS_CC;

bool CurrencyHUD::init() {
	m_background = Sprite::create("components/CurrencyHUDBackground.png");
	m_background->setAnchorPoint(Vec2(0.5, 1.0));
	m_background->setPosition(Vec2(0.0f, 0.0f));

	m_nectarIcon = Sprite::create("icons/Nectar.png");
	m_nectarIcon->setScale(1.0 / 3);
	m_nectarIcon->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_nectarIcon->setPosition(Vec2(-100, -85));

	m_nectarAmount = Label::createWithTTF("0", "fonts/AgencyBold.ttf", 52, Size::ZERO, TextHAlignment::CENTER);
	m_nectarAmount->setTextColor(Color4B(188, 255, 0, 255));
	m_nectarAmount->enableOutline(Color4B::BLACK, 1);
	m_nectarAmount->enableShadow(Color4B::BLACK, Size::ZERO, 2);
	m_nectarAmount->setPosition(Vec2(100, -85));

	// m_nectarAmount->setPosition(m_nectarBoxAnchor + Vec2(275, 55 - size.height));

	// GM PER SECOND NOT CURRENTLY BEING USED

	/*

	m_nectarIncreaseAmount = Label::createWithTTF("+ 0", "fonts/AgencyBold.ttf", 36, Size::ZERO, TextHAlignment::CENTER);
	m_nectarIncreaseAmount->setTextColor(Color4B(255, 255, 255, 255));
	m_nectarIncreaseAmount->enableOutline(Color4B::BLACK, 1);
	m_nectarIncreaseAmount->enableShadow(Color4B::BLACK, Size::ZERO, 1);
	m_nectarIncreaseAmount->setPosition(Vec2(465, 85));

	m_perSecLabel = Label::createWithTTF("/ sec", "fonts/AgencyBold.ttf", 30, Size::ZERO, TextHAlignment::LEFT);
	m_perSecLabel->setPosition(Vec2(5 + m_nectarIncreaseAmount->getContentSize().width, 0.0f));
	m_perSecLabel->enableOutline(Color4B::BLACK, 1);
	m_perSecLabel->enableShadow(Color4B::BLACK, Size::ZERO, 1);
	m_perSecLabel->setAnchorPoint(Vec2(0, 0));
	m_nectarIncreaseAmount->addChild(m_perSecLabel);

	this->addChild(m_nectarIncreaseAmount);

	*/

	setContentSize(m_background->getContentSize());

	this->addChild(m_background);
	this->addChild(m_nectarIcon);
	this->addChild(m_nectarAmount);

    return true;
}

void CurrencyHUD::update(float dt) {
	m_nectarAmount->setString(formatBigReal(Resources::getInstance()->getNectar()));

	/*
	m_nectarIncreaseAmount->setString("+ " + Resources::getInstance()->getNectar().to_string());
	m_perSecLabel->setPosition(Vec2(5 + m_nectarIncreaseAmount->getContentSize().width, 0.0f));
	*/
}
