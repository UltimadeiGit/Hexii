#include "Hex.h"
#include "Maths.h"

#include <format>

USING_NS_CC;

Hex::Hex(const unsigned int layer) : m_layer(layer), m_active(false),
	m_baseYieldSpeed((BigReal)1.0 / (layer + 1)), role(layer == 0 ? Role::HOME_L0 : Role::HOME)
{}

bool Hex::init() {
	m_hex = Sprite::create("gameplay/HexInactive.png"); //Sprite::create(AutoPolygon::generatePolygon("HexagonInactive.png"));
	m_hex->setAnchorPoint(Vec2(0, 0));
	
	setContentSize(m_hex->getContentSize());
	auto& size = getContentSize();

	m_shader = SimpleShader::createWithFragmentShader("shaders/hexProgress.frag");
	m_shader->setUniform<float>("progress", 0.0f);
	m_shader->setUniform<Vec2>("hexCenter", Vec2(size.height * HEXAGON_HEIGHT_TO_WIDTH * 0.5f, size.height * 0.5f));
	m_shader->setUniform<Vec2>("screenCenter", Director::getInstance()->getVisibleSize() / 2);
	m_shader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlay.png"));

	// The shader applies to the hex sprite
	m_hex->setProgramState(m_shader->programState);

	m_purchaseCostLabel = CompoundLabel::create("", "fonts/OCR.ttf", "fonts/OCR.ttf");
	m_purchaseCostLabel->setStyle(false, true, 45, Color4B::WHITE, Color4B::WHITE, 0, Color4B::WHITE, Size(1, -1), 4);
	m_purchaseCostLabel->setIconTexture("icons/GreenMatter.png");
	m_purchaseCostLabel->setSpacing(12.5f);
	m_purchaseCostLabel->setCascadeOpacityEnabled(true);
	m_purchaseCostLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_purchaseCostLabel->setPosition(Vec2(size.width / 2 - 12.5f, size.height / 2));

	m_hex->addChild(m_purchaseCostLabel);

	m_shaded = RenderTexture::create(size.width, size.height);
	m_shaded->getSprite()->setAnchorPoint(Vec2(0.5, 0.5));

	this->addChild(m_hex);
	this->addChild(m_shaded);
	
	return true;
}

void Hex::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) {
	m_shaded->beginWithClear(0, 0, 0, 0);

	m_hex->setVisible(true);
	m_hex->visit(renderer, Mat4::IDENTITY, parentFlags);
	// Prevents the sprite from being drawn in addition to the shaded version later on
	m_hex->setVisible(false);

	m_shaded->end();

	Node::visit(renderer, parentTransform, parentFlags);
}

void Hex::addEXP(BigReal exp) {
	m_exp += exp;
	BigReal expRequired = 0;
	while (m_exp >= (expRequired = getEXPRequiredForLevel(m_level + 1))) {
		m_level++;
		m_exp -= expRequired;
	}
}

void Hex::unlockUpgrade(const std::string& name) {
	m_upgrades[name] = true;
}

// +0.5 per level
BigReal Hex::getYieldFromYieldUp1Upgrade() const {
	return std::ceil(0.5 * m_level) * m_upgrades("YieldUp1");
}

// +5% per level
BigReal Hex::getYieldFromYieldUp2Upgrade() const {
	return 1.0 + (0.05 * m_level) * m_upgrades("YieldUp2");
}

// +25%
BigReal Hex::getYieldSpeedFactorFromSpeedUp1Upgrade() const {
	return 0.25 * m_upgrades("SpeedUp1");
}

// +25%
BigReal Hex::getYieldSpeedFactorFromSpeedUp2Upgrade() const {
	return 0.25 * m_upgrades("SpeedUp2");
}

BigReal Hex::getEXPCost() const {
	// Cost = 6^(layer) * (level + 1)
	return std::powl(6, m_layer) * (m_level + 1);
}

BigReal Hex::getYield() const {
	if (role == Role::HOME_L0) return 1e100;

	return ((1 +
	getYieldFromYieldUp1Upgrade()
	) 
		* (
	getYieldFromYieldUp2Upgrade()
	))
	;
}

BigReal Hex::getYieldSpeed() const {
	return m_baseYieldSpeed * (
		1 +
	getYieldSpeedFactorFromSpeedUp1Upgrade() + 
	getYieldSpeedFactorFromSpeedUp2Upgrade()
	)
	;
}

void Hex::setActive(bool active) {
	if (m_active == active) return;

	m_active = active;
	// Use the appropriate active texture
	if (active) m_hex->setTexture(_director->getTextureCache()->addImage("gameplay/L" + std::to_string(m_layer) + "Hex.png"));
	else m_hex->setTexture(_director->getTextureCache()->addImage("gameplay/HexInactive.png"));

	// Cost label is only visible when the hex is inactive
	m_purchaseCostLabel->setVisible(!active);

	// TODO: Investigate this
	// m_hex->setProgramState(nullptr)
	m_hex->setProgramState(m_shader->programState);
}

void Hex::setPurchaseCost(BigReal cost) {
	m_purchaseCostLabel->setVariablePartString(formatBigReal(cost));
}

void Hex::onTouchBegan() {
	if (m_upgrades("StrengthToStrength")) addEXP(1);
	
	m_shader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlayPressed.png"));

	m_isPressed = true;
}

void Hex::onTouchEnded() {
	m_shader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlay.png"));

	m_isPressed = false;
}

void Hex::onHoverBegan() {
	m_isHovered = true;
}

void Hex::onHoverEnd() {
	m_isHovered = false;
}

BigReal Hex::getEXPRequiredForLevel(uint layer, BigInt level)
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
	/// Update label

	if (!m_active) {
		if (m_isHovered) m_purchaseCostLabelOpacity += dt * 3;
		else m_purchaseCostLabelOpacity -= dt * 2.5;

		if (m_purchaseCostLabelOpacity < 0) m_purchaseCostLabelOpacity = 0.0f;
		else if (m_purchaseCostLabelOpacity > 1) m_purchaseCostLabelOpacity = 1.0f;

		printf("Opacity: %f\n", m_purchaseCostLabelOpacity);

		m_purchaseCostLabel->setOpacity(m_purchaseCostLabelOpacity * 255);
	}

	// No further actions beyond this point if inactive
	if (!m_active) return;

	/// Increase progress

	m_progress += ((BigReal)dt * (1 + m_isPressed * 3)) * getYieldSpeed();
	while (m_progress > 1.0f) {
		m_progress = m_progress - floor(m_progress);
		if (yieldFunction) yieldFunction(this);
	}

	/// Update shader uniforms

	m_shader->setUniform<float>("progress", m_progress);
}