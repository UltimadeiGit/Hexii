#include "Hex.h"

#include <format>

USING_NS_CC;

Hex::Hex(const unsigned int layer, const Vec2& pos) : m_layer(layer), m_pos(pos), m_active(false),
	m_baseYieldDelay(layer + 1), role(layer == 0 ? Role::HOME_L0 : Role::HOME), m_upgrades {0}
{}

bool Hex::init() {
	if (!Sprite::initWithPolygon(AutoPolygon::generatePolygon("HexagonInactive.png"))) return false;
	Sprite::setPosition(m_pos);

	m_shader = SimpleShader::createWithFragmentShader("shaders/hexProgress.frag");
	m_shader->setUniform<float>("progress", 0.0f);
	m_shader->setUniform<Vec2>("hexCenter", m_pos);
	m_shader->setUniform<Vec2>("screenCenter", Director::getInstance()->getVisibleSize() / 2);
	m_shader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("HexProgressOverlay.png"));

	this->setProgramState(m_shader->programState);

	m_debugLabel = Label::createWithTTF("TMP", "fonts/OCR.ttf", 20, Size::ZERO, TextHAlignment::CENTER);
	m_debugLabel->setPosition(Sprite::getContentSize() / 2);
	m_debugLabel->setTextColor(Color4B(160, 0, 120, 255));
	m_debugLabel->enableShadow(Color4B::BLACK, Size(2, -2), 5);

	this->addChild(m_debugLabel);
	
	return true;
}

void Hex::addEXP(unsigned int exp) {
	m_exp += exp;
	while (m_exp >= getEXPRequiredForLevel(m_level + 1)) m_level++;
}

void Hex::onTouchBegan() {
	// A mouse press provides an immediate boost 1 second boost to progress
	m_progress += 1 / m_baseYieldDelay;

	m_isPressed = true;
}

void Hex::onTouchEnded() {
	m_isPressed = false;
}

void Hex::setActive(bool active) {
	if (m_active == active) return;

	this->getProgramState();

	m_active = active;
	this->setTexture(_director->getTextureCache()->addImage(active ? "HexagonActive.png" : "HexagonInactive.png"));
	this->setProgramState(m_shader->programState);
}

unsigned int Hex::getEXPRequiredForLevel(unsigned int layer, unsigned int level)
{
	// n = level
	// a_1 = 6 * (layer + 1)
	// d = 6 * (layer + 2)

	// a_n = a_1 + (n - 1)d
	// a_n = [6 * (layer + 1)] + level(12 + 6 * (layer + 1))
	// a_n = [6 * (layer + 1)](level + 1) + (level * 12)

	if (level == 0) return 0;
	return (6 * (layer + 1)) + (level - 1) * (6 * (layer + 2));
}

void Hex::update(float dt) {
	/*
	m_debugLabel->setString(std::format("\
Layer: {}\n \
EXP: {}\n\
LEVEL: {}",
		m_layer, m_exp, m_level
	));
	*/

	if (!m_active) return;

	// Increase progress

	m_progress += (dt * (1 + m_isPressed * 3)) / m_baseYieldDelay;
	if (m_progress > 1.0f) {
		m_progress = m_progress - floor(m_progress);
		if (yieldFunction) yieldFunction(this);
	}

	// Update shader uniforms

	m_shader->setUniform<float>("progress", m_progress);
}

/*

cocos2d::PolygonInfo& Hex::getActivePinfo() {
	/// TODO: Replace static variables with resource stuff
	static cocos2d::PolygonInfo pinfo = AutoPolygon::generatePolygon("HexagonActive.png");
	return pinfo;
}

cocos2d::PolygonInfo& Hex::getInactivePinfo() {
	/// TODO: Replace static variables with resource stuff
	static cocos2d::PolygonInfo pinfo = AutoPolygon::generatePolygon("HexagonInactive.png");
	return pinfo;
}

cocos2d::PolygonInfo& Hex::getPinfo() {
	return m_active ? getActivePinfo() : getInactivePinfo();
}


*/
