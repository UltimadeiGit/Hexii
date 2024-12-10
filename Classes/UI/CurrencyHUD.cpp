#include "CurrencyHUD.h"
#include "Resources.h"
#include "ColourSchemes.h"
#include "SimpleShader.h"
#include "EventUtility.h"
#include "GameplayCommon.h"
#include "Progression.h"

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

	// Set the content size to match the background
	setContentSize(m_background->getContentSize());

	// The currency HUDs become visible after certain game events
	
	constexpr float HUD_SPACING = 0;

	// Green matter is the only currency type that is visible from the beginning of the game
	toggleHidden(m_currencyType != CurrencyType::GREEN_MATTER, false);
	// CurrencyHUD changes after the first sacrifice 
	Progression::firstSacrificeCompleted()->when(ProgressionEvent::State::ACHIEVED, [this, HUD_SPACING]() {
		switch (m_currencyType) {
			// For green matter, the HUD should shift to the left
		case CurrencyType::GREEN_MATTER:
			translate(0.8f, { -(getContentSize().width / 2.0f + HUD_SPACING) * getScale(), 0 });
			break;
		case CurrencyType::RED_MATTER:
			// For red matter, the HUD should become visible
			toggleHidden(false);
			// And shift to the right (instantly)
			translate(0.0f, { +(getContentSize().width / 2.0f + HUD_SPACING) * getScale(), 0 });
			break;
		default:
			break;
		}
	});

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

void CurrencyHUD::toggleHidden(bool hidden, bool animate) {
	if (m_hidden == hidden) return;

	// Start an action to move the HUD offscreen
	translate(animate ? 0.8f : 0.0f, { 0, getContentSize().height * getScale() * (hidden ? 1 : -1) });
	//this->runAction(cocos2d::EaseQuadraticActionInOut::create(cocos2d::MoveBy::create(animate ? 0.8f : 0.0f, { 0, getContentSize().height * getScale() * (hidden ? 1 : -1), 0})));
	//else this->setPosition(getPosition() + Vec2{ 0, getContentSize().height * getScale() * (hidden ? 1 : -1) });

	m_hidden = hidden;
}

void CurrencyHUD::translate(float duration, cocos2d::Vec2 offset) {
	if(duration == 0.0f) this->setPosition(getPosition() + Vec2{ offset.x, offset.y });
	else this->runAction(cocos2d::EaseQuadraticActionInOut::create(cocos2d::MoveBy::create(duration, {offset.x, offset.y, 0})));
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
