#include "Hex.h"
#include "Maths.h"
#include "Currencies.h"
#include "ColorSchemes.h"

USING_NS_CC;

Hex::Hex(const uint layer, const Vec2 posAxial) : m_layer(layer), m_posAxial(posAxial),
	m_baseYieldSpeed((BigReal)1.0 / (layer + 1)), m_role(layer == 0 ? Role::HOME_L0 : Role::HOME)
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
	
	/// Particles
	if (m_role == Role::HOME_L0) {
		ParticleSystemQuad* l0Particles = ParticleSystemQuad::createWithTotalParticles(18);
		l0Particles->setPosition(Vec2(0, 0));
		l0Particles->setCameraMask(2);

		// Gravity
		l0Particles->setEmitterMode(ParticleSystem::Mode::GRAVITY);
		l0Particles->setGravity(Vec2(0, 0));
		l0Particles->setPosVar(Vec2(20, 20));

		// Radial acceleration
		l0Particles->setRadialAccel(200);
		l0Particles->setRadialAccelVar(0);

		// Speed of particles
		l0Particles->setSpeed(140);
		l0Particles->setSpeedVar(14);

		// Angle
		l0Particles->setAngle(90);
		l0Particles->setAngleVar(30);

		l0Particles->setLife(0.6);

		// Size
		l0Particles->setStartSize(100.0f);
		l0Particles->setStartSizeVar(5.0f);
		l0Particles->setEndSize(50.0f);

		// Color
		l0Particles->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 10.0));
		l0Particles->setStartColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
		l0Particles->setEndColor(Color4F(0.0f, 1.0f, 0.0f, 0.0));
		l0Particles->setEndColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));

		l0Particles->setTexture(Director::getInstance()->getTextureCache()->addImage("icons/GreenMatterB.png"));

		m_yieldParticles.push_back(l0Particles);
	}

	/// Shaded

	m_shaded = RenderTexture::create(size.width, size.height);
	m_shaded->getSprite()->setAnchorPoint(Vec2(0.5, 0.5));

	/// Events

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [](cocos2d::Touch*, cocos2d::Event*) { return true; };
	touchListener->onTouchEnded = CC_CALLBACK_2(Hex::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->addChild(m_hex);
	this->addChild(m_shaded);
	if (m_yieldParticles.size() > 0) this->addChild(m_yieldParticles[0]);
	
	return true;
}

void Hex::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) {
	// Only the default camera should be used to render to the render texture.
	if (!isVisitableByVisitingCamera()) {
		m_shaded->beginWithClear(0, 0, 0, 0);

		m_hex->setVisible(true);
		m_hex->visit(renderer, Mat4::IDENTITY, parentFlags);
		// Prevents the sprite from being drawn in addition to the shaded version later on
		m_hex->setVisible(false);

		m_shaded->end();
	}
	else Node::visit(renderer, parentTransform, parentFlags);
}

void Hex::update(float dt) {
	if (m_active) updateActive(dt);
	else updateInactive(dt);

	/// Update shader uniforms

	m_shader->setUniform<float>("progress", m_progress);
}

void Hex::updateActive(float dt) {
	/// Increase progress

	m_progress += dt * (1 + m_isPressed * 3) * getYieldSpeed();

	// Trigger yield
	if (m_progress >= 1.0f) {
		// Fractional part
		uint timesToYield = (uint)std::floor(m_progress);
		m_progress -= timesToYield;

		yield(timesToYield);
	}
}

void Hex::updateInactive(float dt) {
	// While inactive, hovering over the hex will show the cost to buy it and clicking on it will progress the purchase

	bool affordable = Currencies::getGreenMatter() >= getPurchaseCost();
	BigReal cost = getPurchaseCostFromLayer(m_layer);

	/// Cost label

	// Increase opacity while hovering, decrease when the user moves away
	if (m_isHovered) m_purchaseCostLabelOpacity += dt * 3;
	else m_purchaseCostLabelOpacity -= dt * 2.5;

	// Clamp to [0, 1]
	if (m_purchaseCostLabelOpacity < 0) m_purchaseCostLabelOpacity = 0.0f;
	else if (m_purchaseCostLabelOpacity > 1) m_purchaseCostLabelOpacity = 1.0f;

	m_purchaseCostLabel->setOpacity(m_purchaseCostLabelOpacity * 255);
	m_purchaseCostLabel->setStyle(false, true, 0, affordable ? AFFORDABLE_COLOR : UNAFFORDABLE_COLOR);

	// While the cost label is visible, continuously update its string where necessary
	if (m_purchaseCostLabelOpacity > 0) m_purchaseCostLabel->setVariablePartString(formatBigReal(cost));

	/// Purchasing

	// If unaffordable, this function has finished
	if (!affordable) return;

	BigReal yieldSpeed = 5 * dt * 0.5;

	// Reverse progress if the user decides to cancel the purchase by depressing \
	cannot be cancelled after the reverse animation has started (which is where `1 / 1.1` comes from)
	if (!m_isPressed && m_progress > 0 && m_progress < (1 / 1.15)) yieldSpeed *= -1;

	if (m_isPressed || m_progress > 0) m_progress += yieldSpeed;
	// Clamp to [0, ..
	if (m_progress < 0) m_progress = 0.0f;

	if (m_progress >= 1.0f) {
		m_progress = 0.0f;

		purchase(cost);
	}
}

void Hex::addEXP(BigReal exp) {
	m_totalEXP += exp;
	m_exp += exp;

	// Nothing further to do if not leveling up
	if (m_totalEXP < m_expRequiredForNextLevel) return;

	// Before level up, record current level
	BigInt levelBefore = m_level;
	m_level = getLevelFromEXP(m_totalEXP, m_layer);

	// Before updating the exp requirement for the next level, update how much exp into the current level this hex is now
	if (m_level - levelBefore == 1) m_exp = m_totalEXP - m_expRequiredForNextLevel;
	else m_exp = m_totalEXP - getEXPRequiredToReachLevelFromLayer(m_level - 1, m_layer);

	m_expRequiredForNextLevel = getEXPRequiredToReachLevelFromLayer(m_level + 1, m_layer);

	_eventDispatcher->dispatchCustomEvent("onHexLevelUp", new EventHexLevelUpData{ this, levelBefore, m_level, m_exp, m_totalEXP, m_expRequiredForNextLevel });
}

void Hex::unlockUpgrade(UpgradePtr upgrade) {
	m_upgrades[fmt::to_string(upgrade->name)] = true;
	_eventDispatcher->dispatchCustomEvent("onHexUpgradePurchase", new EventHexUpgradePurchaseData{ this, upgrade });
}

void Hex::addYieldTarget(Hex* hex, float angleBetween) {
	m_yieldTargets.push_back(hex);

	/// Particles

	ParticleSystemQuad* yieldTargetParticles = ParticleSystemQuad::createWithTotalParticles(12);
	yieldTargetParticles->setPosition(Vec2(0, 0));
	yieldTargetParticles->setCameraMask(getCameraMask());

	// Gravity
	yieldTargetParticles->setEmitterMode(ParticleSystem::Mode::GRAVITY);
	yieldTargetParticles->setGravity(Vec2(0, 0));

	// Radial acceleration
	yieldTargetParticles->setRadialAccel(-20);
	yieldTargetParticles->setRadialAccelVar(0);

	// Speed of particles
	yieldTargetParticles->setSpeed(140);
	yieldTargetParticles->setSpeedVar(0);

	// Angle
	yieldTargetParticles->setAngle(angleBetween);
	yieldTargetParticles->setAngleVar(0);

	yieldTargetParticles->setLife(1.2);

	// Size
	yieldTargetParticles->setStartSize(90.0f);
	yieldTargetParticles->setStartSizeVar(5.0f);
	yieldTargetParticles->setEndSize(110.0f);

	// Color
	yieldTargetParticles->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 1.4));
	yieldTargetParticles->setStartColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	yieldTargetParticles->setEndColor(Color4F(0.8f, 1.0f, 0.8f, 0.0));
	yieldTargetParticles->setEndColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));

	yieldTargetParticles->setTexture(Director::getInstance()->getTextureCache()->addImage("particles/EXPParticle.png"));

	m_yieldParticles.push_back(yieldTargetParticles);

	this->addChild(yieldTargetParticles);
}

BigReal Hex::getPurchaseCostFromLayer(uint layer) {
	BigReal cost = 6;

	switch (layer) {
	case 0:
		break;
	case 1:
		cost = (BigReal)300 * (std::powl(3, Currencies::getHexiiCountInLayer(1)));
		break;
	case 2:
		cost = (BigReal)120000 * std::powl(1.5, Currencies::getHexiiCountInLayer(2));
		break;
	case 3:
		cost = (BigReal)3e7 * std::powl(1.25, Currencies::getHexiiCountInLayer(3));
		break;
	default:
		cost = 1e20;
		break;
	}
	cost = std::floorl(cost);

	return cost;
}

BigReal Hex::getEXPRequiredToReachLevelFromLayer(BigReal level, uint layer) {
	// = A(b^level - 1) where \
		b = (1.2 + layer/50) \
		A = 6b / (b - 1)

	// First level is hardcoded
	if (level == 0) return 0;
	level -= 1;

	// Derived from EXP required at `level` to level up, which is simply 6 * b^level

	BigReal base = 1.2 + (layer * 0.02);
	return std::ceill(6 + ((6 * base) / (base - 1)) * (std::powl(base, level) - 1));
}

BigReal Hex::getLevelFromEXP(BigInt exp, uint layer) {
	// = log_b(exp/A + 1) 
	// Inverse function of getEXPRequiredToReachLevelFromLayer

	// The first level is hardcoded
	if (exp < 6) return 0;
	exp -= 6;

	const BigReal base = 1.2 + (layer * 0.02);
	const BigReal operand = 1 + exp / ((6 * base) / (base - 1));
	return std::floorl(1 + std::logl(operand) / std::logl(base));
}

// +0.5 per level
BigReal Hex::getYieldFromYieldUp1Upgrade() const {
	return std::ceil(0.5 * m_level) * m_upgrades("YieldUp1");
}

// +5% per level
BigReal Hex::getYieldFromYieldUp2Upgrade() const {
	return 1.0 + (0.05 * m_level) * m_upgrades("YieldUp2");
}

// +50%
BigReal Hex::getYieldSpeedFactorFromSpeedUp1Upgrade() const {
	return 2.5 * m_upgrades("SpeedUp1");
}

// +50%
BigReal Hex::getYieldSpeedFactorFromSpeedUp2Upgrade() const {
	return 2.5 * m_upgrades("SpeedUp2");
}

BigReal Hex::getContributionFromUpgrade(const std::string& upgradeName, bool asConstant) const {
	// Constant yield upgrades will always return constants (of course)

	BigReal contribution = 0;

	if (upgradeName == "YieldUp1") contribution = getYieldFromYieldUp1Upgrade();

	else if (!asConstant) {
		if (upgradeName == "YieldUp2") contribution = getYieldFromYieldUp2Upgrade();
		else return 0;

		contribution = multiplierToPercentageContribution(contribution);
	}
	else {
		if (upgradeName == "YieldUp2") contribution = (getYieldFromYieldUp2Upgrade() - 1) * getConstantYield();
	}

	return contribution;
}

BigReal Hex::getEXPCost() const {
	// Cost = 6^(layer) * (level + 1)
	return std::powl(6, m_layer) * (m_level + 1);
}

BigReal Hex::getConstantYield() const {
	return 
		1 +
		getYieldFromYieldUp1Upgrade()
	;
}

BigReal Hex::getAdditiveYield() const {
	return getYieldFromYieldUp2Upgrade();
}

BigReal Hex::getMultiplicativeYield() const {
	return 1;
}

BigReal Hex::getYield() const {
	if (m_role == Role::HOME_L0) return 1e100;

	return getConstantYield() * getAdditiveYield() * getMultiplicativeYield();
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

void Hex::setCameraMask(unsigned short mask, bool applyChildren) {
	Node::setCameraMask(mask, false);
	for(uint i = 0; i < m_yieldParticles.size(); i++) m_yieldParticles[i]->setCameraMask(mask, true);
	m_shaded->setCameraMask(mask, true);
}

void Hex::onTouchBegan() {
	if (m_upgrades("StrengthToStrength")) addEXP(1);
	
	m_shader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlayPressed.png"));
	m_isPressed = true;

	if(m_active) _eventDispatcher->dispatchCustomEvent("onHexFocus", new EventHexFocusData{ this, m_active, m_posAxial, m_layer});
}

void Hex::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt) {
	// Ignore this event if it doesn't apply to this hex
	if (!m_isPressed) return;

	m_shader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlay.png"));

	m_isPressed = false;
}

void Hex::onHoverBegan() {
	m_isHovered = true;
}

void Hex::onHoverEnd() {
	m_isHovered = false;
}

void Hex::yield(uint times) {
	BigReal yield = getYield() * times;

	// Layer 0 produces green matter. Outer layers produce EXP for adjacent hexii of lower layers (stored in `m_yieldTargets`)
	if (m_role == Hex::Role::HOME_L0) Currencies::instance()->addGreenMatter(yield);
	else {
		uint yieldTargetCount = m_yieldTargets.size();
		// The yield is split evenly across its targets
		yield /= yieldTargetCount;

		for (uint i = 0; i < yieldTargetCount; i++) {
			m_yieldTargets[i]->addEXP(yield);
		}
	}

	// Update particles
	for (uint i = 0; i < m_yieldParticles.size(); i++) {
		if (m_yieldParticles[i]->getParticleCount() < m_yieldParticles[i]->getTotalParticles()) m_yieldParticles[i]->addParticles(1);
		auto a = m_yieldParticles[i]->getCameraMask();
		auto b = this->getCameraMask();
		printf("%d\n", m_yieldParticles[i]->getParticleCount());
	}
	// Dispatch the event for any other nodes to pick up on and respond to (e.g to update UI label values)
	_eventDispatcher->dispatchCustomEvent("onHexYield", new EventHexYieldData{ this, m_role, yield, m_posAxial, m_layer });
}

void Hex::purchase(BigReal cost) {
	// Note: at this point it is assumed that cost is affordable

	m_purchaseCostLabelOpacity = 0.0f;
	setActive(true);

	Currencies::instance()->addGreenMatter(-cost);
	Currencies::instance()->addHexInLayer(m_layer);

	// Trigger the purchase event and also then focus to this hex
	_eventDispatcher->dispatchCustomEvent("onHexPurchase", new EventHexPurchaseData{ this, m_active, m_posAxial, m_layer });
	_eventDispatcher->dispatchCustomEvent("onHexFocus", new EventHexFocusData{ this, m_active, m_posAxial, m_layer });
}