#include "CurrencyHUD.h"
#include "Resources.h"
#include "ColourSchemes.h"
#include "SimpleShader.h"

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

	//m_currencyIcon = Sprite::create("icons/RedMatter.png");
	m_currencyIcon = Sprite::create("icons/" + currencyName + ".png");
	//static SimpleShaderPtr s = SimpleShader::create(SimpleShader::createShaderProgramWithFragmentShader("shaders/dents.frag"));
	//m_currencyIcon->setProgramState(s->getProgramState());
	m_currencyIcon->setContentSize({ 112, 117 });
	m_currencyIcon->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_currencyIcon->setPosition({ 95, -56 });

	m_currencyAmount = Label::createWithTTF("0", "fonts/BreeSerif.ttf", 60, Size::ZERO, TextHAlignment::RIGHT);
	m_currencyAmount->setAnchorPoint(Vec2(0.0f, 0.4f));
	m_currencyAmount->setTextColor(COMMON_GREEN);
	m_currencyAmount->enableOutline(Color4B::BLACK, 5);
	m_currencyAmount->enableShadow(Color4B::BLACK, Size(1, -1), 3);
	m_currencyAmount->setPosition(Vec2( (desiredSize.width / 2) - 25, -(desiredSize.height / 2)));

	setContentSize(m_background->getContentSize());

	this->addChild(m_background);
	this->addChild(m_currencyIcon);
	this->addChild(m_currencyAmount);

    return true;
}

void CurrencyHUD::update(float dt) {
	switch (m_currencyType) {
	case CurrencyType::GREEN_MATTER:
		m_currencyAmount->setString(formatBigReal(Resources::getInstance()->getGreenMatter()));
		break;
	}
}

std::string CurrencyHUD::getCurrencyName() const {
	switch (m_currencyType) {
		case CurrencyType::GREEN_MATTER:
			return "GreenMatter";
	}
}
