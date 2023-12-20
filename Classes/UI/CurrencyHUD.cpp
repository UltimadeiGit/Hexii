#include "CurrencyHUD.h"
#include "Resources.h"
#include "ColourSchemes.h"
#include "SimpleShader.h"
#include "EventUtility.h"
#include "GameplayCommon.h"

USING_NS_CC;

CurrencyHUD::CurrencyHUD(CurrencyType currencyType)
	: m_currencyType(currencyType)
{}

bool CurrencyHUD::init() {
	const std::string currencyName = getCurrencyName();

	//m_background = Sprite::create("HUD/RedMatterResourcePane.png");
	m_background = Sprite::create("HUD/" + currencyName + "ResourcePane.png");
	const cocos2d::Size desiredSize { 438, 125 };

	//m_background->setContentSize(Size(356, 145));
	m_background->setContentSize(desiredSize);
	m_background->setAnchorPoint(Vec2(0.0, 1.0));
	m_background->setPosition(Vec2(0.0f, 0.0f));
	this->addChild(m_background);

	//m_currencyIcon = Sprite::create("icons/RedMatter.png");
	m_currencyIcon = Sprite::create("icons/" + currencyName + ".png");
	//static SimpleShaderPtr s = SimpleShader::create(SimpleShader::createShaderProgramWithFragmentShader("shaders/dents.frag"));
	//m_currencyIcon->setProgramState(s->getProgramState());
	m_currencyIcon->setContentSize({ 112, 117 });
	m_currencyIcon->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_currencyIcon->setPosition({ 95, -56 });
	this->addChild(m_currencyIcon);

	m_currencyAmount = Label::createWithTTF("0", "fonts/BreeSerif.ttf", 60, Size::ZERO, TextHAlignment::CENTER);
	m_currencyAmount->setAnchorPoint(Vec2(0.5f, 0.5f));
	switch (m_currencyType) {
	case CurrencyType::GREEN_MATTER:
		m_currencyAmount->setTextColor(COMMON_GREEN);
		break;
	case CurrencyType::RED_MATTER:
		m_currencyAmount->setTextColor(COMMON_RED);
		break;
	}
	m_currencyAmount->enableOutline(Color4B::BLACK, 5);
	m_currencyAmount->enableShadow(Color4B::BLACK, Size(1, -1), 3);
	m_currencyAmount->setPosition(Vec2( (desiredSize.width / 2) + (m_currencyIcon->getContentSize().width / 2), 10 -(desiredSize.height / 2)));
	this->addChild(m_currencyAmount);

	// Register for events
	EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_INITIATED, this, &CurrencyHUD::onSacrificeInitiated);
	EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_CANCELLED, this, &CurrencyHUD::onSacrificeCancelled);
	EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_CONFIRMED, this, &CurrencyHUD::onSacrificeConfirmed);

	setContentSize(m_background->getContentSize());

    return true;
}

void CurrencyHUD::update(float dt) {
	switch (m_currencyType) {
	case CurrencyType::GREEN_MATTER:
		m_currencyAmount->setString(formatBigReal(Resources::getInstance()->getGreenMatter()));
		break;
	case CurrencyType::RED_MATTER:
		m_currencyAmount->setString(formatBigReal(Resources::getInstance()->getRedMatter()));
		break;
	default:
		err("Unimplemented currency type");
	}
}

std::string CurrencyHUD::getCurrencyName() const {
	switch (m_currencyType) {
	case CurrencyType::GREEN_MATTER:
		return "GreenMatter";
	case CurrencyType::RED_MATTER:
		return "RedMatter";
	default:
		err("Unimplemented currency type");
	}
}

void CurrencyHUD::toggleHidden(bool hidden) {
	if (m_hidden == hidden) return;

	// Start an action to move the HUD offscreen
	this->runAction(cocos2d::EaseQuadraticActionInOut::create(cocos2d::MoveBy::create(0.8f, { 0, getContentSize().height * getScale() * (hidden ? 1 : -1), 0})));

	m_hidden = hidden;
}

void CurrencyHUD::onSacrificeInitiated(cocos2d::EventCustom* evnt) {
	// When the user initiates a sacrifice, hide the HUD so that it doesn't get in the way
	toggleHidden(true);
}

void CurrencyHUD::onSacrificeCancelled(cocos2d::EventCustom* evnt) {
	// When the user cancels a sacrifice, show the HUD again
	toggleHidden(false);
}

void CurrencyHUD::onSacrificeConfirmed(cocos2d::EventCustom* evnt) {
	// When the user completes a sacrifice, show the HUD again
	toggleHidden(false);
}
