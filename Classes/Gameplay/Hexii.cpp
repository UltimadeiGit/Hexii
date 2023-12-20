#include "Hexii.h"
#include "Maths.h"
#include "Resources.h"
#include "ColourSchemes.h"
#include "HexiiPlane.h"
#include "JSON.hpp"
#include "Console.h"
#include "GameplayCommon.h"
#include "GameplayCommon.h"

USING_NS_CC;
using namespace nlohmann;

Hexii::Hexii(const uint layer, const Vec2 posAxial) : 
	Hexagon(posAxial), 
	m_layer(layer),
	m_type(layer == 0 ? HexiiType::HOME_L0 : HexiiType::HOME)
{}

Hexii::Hexii(const json& data) : 
	Hexagon(data.at("posAxial")),
	m_layer(data.at("layer")), 
	m_type(m_layer == 0 ? HexiiType::HOME_L0 : HexiiType::HOME) 
{	
	/*
	try {
		auto specGroups = data.at("specGroups");

		if (!specGroups.is_null()) {
			for (json::iterator it = specGroups.begin(); it != specGroups.end(); it++) {
				auto& groupData = *it;

				Specialization group(groupData, &m_upgrades, m_layer);

				m_specGroups.push_back(group);
			}
		}
	}
	catch (json::out_of_range) { /* No specs stored */	//}

	try { m_standardPathTracker = std::make_shared<UpgradeTracker>(data.at("standardUpgrades")); }
	catch (json::out_of_range) { /* No standard path stored */ }
	
	try { addEXP(data.at("exp"), true); } catch (json::out_of_range) {}

	try { addRedMatter(data.at("redMatterInvested"), true); } catch (json::out_of_range) {}
}

bool Hexii::init() {
	Vec2 axialPos = getAxialPosition();

	switch (m_type) {
	case HexiiType::HOME_L0:
		setName("Centre Hexii");
		if (m_standardPathTracker == nullptr) m_standardPathTracker = std::make_shared<UpgradeTracker>(UpgradePath::getStandardL0Path());
		break;
	case HexiiType::HOME:
		setName("Standard Hexii");
		if (m_standardPathTracker == nullptr) m_standardPathTracker = std::make_shared<UpgradeTracker>(UpgradePath::getStandardPath());
		break;
	default:
		err("Unhandled hexii type");
		break;
	}
	
	/// Hex sprite init

	m_hex = Sprite::create("gameplay/L" + std::to_string(m_layer) + "HexiiInactive.png");
	// TODO: Investigate the use of anchor points with the hexii
	m_hex->setAnchorPoint(Vec2(0, 0));

	setContentSize(m_hex->getContentSize());
	auto& size = getContentSize();

	/// Shader init

	m_progressShader = std::make_shared<SimpleShader>(SimpleShaderManager::getProgressShaderProgram());
	m_progressShader->setUniform<float>("progress", 0.0f);
	m_progressShader->setUniform<Vec2>("hexCenter", Vec2(size.height * HEXAGON_HEIGHT_TO_WIDTH * 0.5f, size.height * 0.5f));
	// TODO: Investigate the impact of resizing given that this uniform is only set here...
	m_progressShader->setUniform<Vec2>("screenCenter", Director::getInstance()->getVisibleSize() / 2);
	m_progressShader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlay.png"));

	m_shaded = ShadedElement::create(m_progressShader, size.width, size.height);
	// TODO: Move this up into HexiiPlane
	m_shaded->setAnchorPoint(Vec2(0.5, 0.5));
	// The shader applies to the hex sprite
	m_shaded->addShadedChild(m_hex);

	this->addChild(m_shaded, 0);

	/// Label init

	m_purchaseCostLabel = CompoundLabel::create("", "fonts/BreeSerif.ttf", "fonts/BreeSerif.ttf");
	m_purchaseCostLabel->setStyle(false, true, 45, Color4B::WHITE, Color4B::BLACK, 3, Color4B::WHITE, Size(2, -2), 4);
	m_purchaseCostLabel->setIconTexture("icons/GreenMatterSmall.png");
	m_purchaseCostLabel->setSpacingConstraint(12.5f);
	m_purchaseCostLabel->setCascadeOpacityEnabled(true);
	m_purchaseCostLabel->setAnchorPoint(Vec2(0.5, 0.5));
	//m_purchaseCostLabel->setPosition(Vec2(size.width / 2 - 12.5f, size.height / 2));

	this->addChild(m_purchaseCostLabel, 1);
	
	/// Particles

	if (m_type == HexiiType::HOME_L0) {
		YieldParticleSystem* d0Particles = YieldParticleSystem::create();
		d0Particles->setPosition(Vec2(0, 0));

		// Particle should "pop" up from the hexii, 

		// Gravity
		d0Particles->setEmitterMode(ParticleSystem::Mode::RADIUS);
		d0Particles->setStartRadius(10.0f);
		//d0Particles->setGravity(Vec2(0, 0));
		d0Particles->setPosVar(Vec2(20, 20));

		// Radial acceleration
		//d0Particles->setRadialAccel(200);
		//d0Particles->setRadialAccelVar(0);

		// Speed of particles
		//d0Particles->setSpeed(140);
		//d0Particles->setSpeedVar(14);

		d0Particles->setEndRadius(0.0f);

		// Angle
		d0Particles->setAngle(90);
		d0Particles->setAngleVar(360);

		d0Particles->setLife(0.6);

		// Size
		d0Particles->setStartSize(100.0f);
		//d0Particles->setStartSizeVar(5.0f);
		//d0Particles->setEndSize(50.0f);

		// Color
		d0Particles->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 1.0));
		d0Particles->setEndColor(Color4F(1.0f, 1.0f, 1.0f, 1.0));

		d0Particles->setTexture(Director::getInstance()->getTextureCache()->addImage("particles/GreenMatter.png"));

		m_yieldParticles.push_back(d0Particles);
		this->addChild(m_yieldParticles[0], 2);
	}

	/// Events

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [](cocos2d::Touch*, cocos2d::Event*) { return true; };
	touchListener->onTouchEnded = CC_CALLBACK_2(Hexii::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	//EventUtility::addGlobalEventListener(Specialization::EVENT_NONLOCAL_UPGRADE_PURCHASED, this, &Hexii::onUpgradePurchased);

	EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_INITIATED, this, &Hexii::onSacrificeInitiated);
	EventUtility::addGlobalEventListener(GameplayCommon::GameEvent::EVENT_SACRIFICE_CANCELLED, this, &Hexii::onSacrificeCancelled);

	return true;
}

bool Hexii::init(const json& data) {
	if (!init()) {
		return false;
	}

	setActive(data.at("active"));

	return true;
}

void Hexii::update(float dt) {
	if (m_active) updateActive(dt);
	else updateInactive(dt);

	/// Update shader uniforms

	m_progressShader->setUniform<float>("progress", m_progress);
}

UpgradeTrackerPtr Hexii::getUpgradeTrackerFromPath(UpgradePathPtr path) const {
	if (m_standardPathTracker->getPath() == path) {
		return m_standardPathTracker;
	}
	
	warn("Attempted to get upgrade tracker for invalid path");
	return nullptr;
}

void Hexii::updateActive(float dt) {
	/// Increase progress

	m_progress += dt * getYieldSpeed(true);

	// Trigger yield
	if (m_progress >= 1.0f) {
		// Fractional part
		uint timesToYield = (uint)std::floor(m_progress);
		m_progress -= timesToYield;

		yield(timesToYield);
	}
}

void Hexii::updateInactive(float dt) {
	// While inactive, hovering over the hex will show the cost to buy it and clicking on it will progress the purchase

	bool affordable = Resources::getInstance()->getGreenMatter() >= getPurchaseCost();
	// TODO: Why is this recalculated every frame?!
	BigReal cost = getPurchaseCostFromlayer(m_layer);

	/// Cost label

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	time_t currentTime;
	time(&currentTime);

	if (currentTime - m_timeOfLastPress < 3.0f) m_isHovered = true;
	else m_isHovered = false;
#endif

	// Increase opacity while hovering, decrease when the user moves away
	if (m_isHovered) m_purchaseCostLabelOpacity += dt * 3;
	else m_purchaseCostLabelOpacity -= dt * 2.5;

	// Clamp to [0, 1]
	if (m_purchaseCostLabelOpacity < 0) m_purchaseCostLabelOpacity = 0.0f;
	else if (m_purchaseCostLabelOpacity > 1) m_purchaseCostLabelOpacity = 1.0f;

	m_purchaseCostLabel->setOpacity(m_purchaseCostLabelOpacity * 255);
	m_purchaseCostLabel->setStyle(false, true, 0, affordable ? AFFORDABLE_GREEN : UNAFFORDABLE_RED);

	// While the cost label is visible, continuously update its string where necessary
	if (m_purchaseCostLabelOpacity > 0) {
		// TODO: Investigate why the cost label is off centre
		m_purchaseCostLabel->setVariablePartString(formatBigReal(cost));
	}

	/// Purchasing

	// If unaffordable, this function has finished
	if (!affordable) return;

	BigReal yieldSpeed = dt * 0.5;

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


BigReal Hexii::getEXPRequiredForLevel(BigInt level, uint layer) {
	if (level >= GameplayCommon::LevelUps::MAX_LEVEL) {
		err("Max level exceeded!");
	}
	if (layer >= GameplayCommon::MAX_LAYER) {
		err("Max layer exceeded!");
	}
	return GameplayCommon::LevelUps::EXP_REQUIRED_FOR_LEVEL[layer][level];
}

BigInt Hexii::getLevelFromEXP(BigReal exp, uint layer) {
	/*
	// = log_b(exp/A + 1) 
	// Inverse function of getEXPRequiredForLevel

	// First is hardcoded
	if (exp < 6) return 0;
	exp -= 6;

	const BigReal base = 1.2 + (layer * 0.02);
	const BigReal operand = 1 + exp / ((6 * base) / (base - 1));
	BigReal level = 1 + std::logl(operand) / std::logl(base);

	// Scaling becomes worse after level 60
	// TODO: Handle levels over 60

	return (BigInt)std::floorl(level);
	*/

	// TODO: Profile this

	for (int i = 1; i < GameplayCommon::LevelUps::MAX_LEVEL; i++) {
		if (exp < GameplayCommon::LevelUps::EXP_REQUIRED_FOR_LEVEL[layer][i]) return (BigInt)i - 1;
	}

	return GameplayCommon::LevelUps::MAX_LEVEL;
}

BigReal Hexii::getUpgradePurchaseCostMultiplier(uint layer) {
	// Multiplier = 20 ^ layer
	if (layer == 0) return 1;
	return std::powl(20, layer);
}

void Hexii::addEXP(BigReal exp, bool suppressEvent) {
	m_exp += exp;

	updateLevel(true, false, suppressEvent);
}

void Hexii::addRedMatter(BigReal redMatter, bool suppressEvent) {
	m_redMatterInvested += redMatter;

	updateLevel(false, true, suppressEvent);
}

void Hexii::updateLevel(bool expChanged, bool redMatterChanged, bool suppressEvent) {
	BigInt rawLevelChange = 0;
	BigInt totalLevelChange = 0;

	// If the EXP is greater than the EXP required for the next level, increase raw level
	if (expChanged && m_exp >= m_expForNextLevel) {
		BigInt level = getLevelFromEXP(m_exp, m_layer);
		rawLevelChange = level - m_rawLevel;

		if (rawLevelChange == 0) throw std::runtime_error("Level change is unexpectedly 0");

		// Update state

		m_rawLevel = level;
		m_expForNextLevel = getEXPRequiredForLevel(m_rawLevel + 1, m_layer);
	}

	// Recalculate total level if the raw level changed or red matter changed
	if (rawLevelChange > 0 || redMatterChanged) {
		BigInt totalLevel = std::ceill(m_rawLevel * getLevelScale());
	
		totalLevelChange = totalLevel - m_totalLevel;

		// Update state

		m_totalLevel = totalLevel;
	}

	if (totalLevelChange > 0) {
		/// Check upgrade states

		m_standardPathTracker->updateStatesDueToLevel(m_totalLevel);

		/// Dispatch events

		if (!suppressEvent)
			EventUtility::dispatchEvent(EVENT_LEVEL_GAINED, eventID, EventHexiiLevelGainedData{ this, totalLevelChange });

		// Many upgrades are affected by level, so everything needs to be recalculated
		m_isYieldDirty = m_isYieldSpeedDirty = m_isCritDirty = m_isAdjacencyDirty = m_isCritDirty = true;
	}
}

/*
void Hexii::levelUp(bool suppressEvent) {
	// Before level up, record current level
	BigInt levelBefore = m_layer;
	m_layer = getLevelFromEXP(m_totalEXP, m_layer);

	// Before updating the exp requirement for the next level, update how much exp into the current level this hex is now
	if (m_layer - levelBefore == 1) m_exp = m_totalEXP - m_expRequiredForNextLevel;
	else m_exp = m_totalEXP - getEXPRequiredToReachLevelFromlayer(m_layer, m_layer);

	m_expRequiredForNextLevel = getEXPRequiredToReachLevelFromlayer(m_layer + 1, m_layer);

	// Dispatch event

	// This event will be moved down into Specialization
	//if (!suppressEvent)
		//_eventDispatcher->dispatchCustomEvent("onLevelGained", new EventLevelGainedData{ this, levelBefore, m_layer, m_exp, m_totalEXP, m_expRequiredForNextLevel });
}
*/

void Hexii::addYieldTarget(Hexii* hex, float angleBetween) {
	m_yieldTargets.push_back(hex);

	/// Particles

	YieldParticleSystem* yieldTargetParticles = YieldParticleSystem::create();
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

	yieldTargetParticles->setTexture(Director::getInstance()->getTextureCache()->addImage("particles/EXP.png"));

	m_yieldParticles.push_back(yieldTargetParticles);

	this->addChild(yieldTargetParticles);
}

BigReal Hexii::getPurchaseCostFromlayer(uint layer) {
	BigReal cost = 6;

	switch (layer) {
	case 0:
		break;
	case 1:
		cost = (BigReal)900 * (std::powl(3, Resources::getInstance()->getHexiiCountInlayer(1)));
		break;
	case 2:
		cost = (BigReal)120000 * std::powl(1.5, Resources::getInstance()->getHexiiCountInlayer(2));
		break;
	case 3:
		cost = (BigReal)3e7 * std::powl(1.25, Resources::getInstance()->getHexiiCountInlayer(3));
		break;
	default:
		cost = 1e20;
		break;
	}
	cost = std::floorl(cost);

	return cost;
}

BigReal Hexii::getYield() const {
	if (m_isYieldDirty) {
		BigReal constant = 1 + getConstantYield();
		BigReal additive = getAdditiveYieldMultiplier();
		BigReal multiplicative = getMultiplicativeYieldMultiplier();

		m_yield = constant
			* additive * multiplicative
			* (1 + HexiiPlane::getInstance()->getAdjacencyBonuses(this))
			* (1 + Resources::getInstance()->getGlobalPowerUpgradeBonus())
			;

		m_isYieldDirty = false;
	}

	return m_yield;
}

BigReal Hexii::getYieldSpeed(bool includeActiveBonus) const {
	if (m_isYieldSpeedDirty) {
		const BigReal base = 1.0 / std::powl(2, m_layer);
		BigReal constant = getConstantYieldSpeed();
		BigReal additive = getAdditiveYieldSpeedMultiplier();
		BigReal multiplicative = getMultiplicativeYieldSpeedMultiplier();

		m_activeYieldSpeedMultiplier = 2 * getActiveYieldSpeedMultiplier();

		m_yieldSpeed = (base + constant) * additive * multiplicative;
		
		m_isYieldSpeedDirty = false;
	}

	return includeActiveBonus && m_isPressed ? m_yieldSpeed * m_activeYieldSpeedMultiplier : m_yieldSpeed;
}



BigReal Hexii::getAdjacencyYieldMultiplier() const {
	if (m_isAdjacencyDirty) {
		m_adjacencyYieldMultiplier = 1 +
			getAdjacencyYieldMultiplierFromSupport1Upgrade();

		m_isAdjacencyDirty = false;
	}

	return m_adjacencyYieldMultiplier;
}

void Hexii::setActive(bool active) {
	if (m_active == active) return;

	m_active = active;

	// Use the appropriate active texture
	if (active) m_hex->setTexture(_director->getTextureCache()->addImage("gameplay/L" + std::to_string(m_layer) + "Hexii.png"));
	else m_hex->setTexture(_director->getTextureCache()->addImage("gameplay/L" + std::to_string(m_layer) + "HexInactive.png"));

	// Cost label is only visible when the hex is inactive
	m_purchaseCostLabel->setVisible(!active);

	m_hex->setProgramState(m_progressShader->getProgramState());
}

void Hexii::purchaseUpgrade(UpgradeTrackerPtr tracker, UpgradePtr upgrade) {
	tracker->purchaseUpgrade(upgrade);

	if (upgrade->actsOnFlags & Upgrade::ACTS_ON_YIELD) m_isYieldDirty = true;
	if (upgrade->actsOnFlags & Upgrade::ACTS_ON_SPEED) m_isYieldSpeedDirty = true;
	if (upgrade->actsOnFlags & Upgrade::ACTS_ON_CRITS) m_isCritDirty = true;
	if (upgrade->actsOnFlags & Upgrade::ACTS_ON_OTHER_HEXII) m_isAdjacencyDirty = true;

	// Dispatch event
	EventUtility::dispatchEvent<EventHexiiUpgradePurchasedData>(Hexii::EVENT_UPGRADE_PURCHASED, eventID, { this, tracker, upgrade });
}

void Hexii::onTouchBegan() {
	time(&m_timeOfLastPress);
		
	m_progressShader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlayPressed.png"));
	m_isPressed = true;
}

void Hexii::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt) {
	// Ignore this event if it doesn't apply to this hex
	if (!m_isPressed) return;

	m_progressShader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlay.png"));

	m_isPressed = false;
}

void Hexii::onSacrificeInitiated(cocos2d::EventCustom* evnt) {
	if (m_active) {
		// Use the dents shader overall
		this->setShaderEffect(SimpleShader::create(SimpleShaderManager::getDentsProgram()));

		// Temporarily disable the progress shader
		m_shaded->toggleShader(false);
	}
	else {
		setVisible(false);
	}
}

void Hexii::onSacrificeCancelled(cocos2d::EventCustom* evnt) {
	if (m_active) {
		// Disable the dents shader
		this->setShaderEffect(nullptr);

		// Re-enable the progress shader
		m_shaded->toggleShader(true);
	}
	else {
		setVisible(true);
	}
}

void Hexii::onShaderEffectChanged(SimpleShaderPtr shader) {
	// For hexii, changing the shader effect means applying it to the `m_shaded` sprite
	m_shaded->setProgramState(shader ? shader->getProgramState() : nullptr);
}

void Hexii::focus() {
	if (m_active) EventUtility::dispatchEvent(Hexii::EVENT_FOCUS, eventID, EventHexiiFocusData{ this, m_active, getAxialPosition(), m_layer });
	//if (m_active) _eventDispatcher->dispatchCustomEvent("onHexiiFocus", new EventHexiiFocusData{ this, m_active, getAxialPosition(), m_layer });
}

BigReal Hexii::getCritical(uint times) const {
	if (m_isCritDirty) {
		m_criticalChance = 0.005 + getCriticalChance();
		m_criticalYieldMultiplier = 6 * getCriticalYieldMultiplier();

		m_isCritDirty = false;
	}

	if (m_criticalChance == 0) return 1;
	if (times > 1) {
		BigReal expectedCrits = times * m_criticalChance;
		// Average crit multiplier
		return (expectedCrits * m_criticalYieldMultiplier + (times - expectedCrits)) / times;
	}
	else return cocos2d::rand_0_1() <= m_criticalChance ? m_criticalYieldMultiplier : 1;
}

void Hexii::yield(uint times) {
	BigReal baseYield = getYield();
	BigReal yield = 0;

	// Compute directly if the number of times is small, otherwise use an average
	if (times < 5) for (uint i = 0; i < times; i++) yield += baseYield * getCritical(1);
	else yield = baseYield * getCritical(times);

	// layer 0 produces greenMatter. Outer layers produce EXP for adjacent hexii of lower layers (stored in `m_yieldTargets`)
	if (m_type == HexiiType::HOME_L0) Resources::getInstance()->addGreenMatter(yield);
	else {
		uint yieldTargetCount = m_yieldTargets.size();
		// The yield is split evenly across its targets
		yield /= yieldTargetCount;

		for (uint i = 0; i < yieldTargetCount; i++) m_yieldTargets[i]->addEXP(yield);
	}

	// Update particles
	for (uint i = 0; i < m_yieldParticles.size(); i++) {
		int particlesAvailable = m_yieldParticles[i]->getTotalParticles() - m_yieldParticles[i]->getParticleCount();

		if (particlesAvailable > 0) {
			m_yieldParticles[i]->addParticles(1);
			particlesAvailable -= 1;
			// Crit particle
			//if (particlesAvailable > 0 && critical) m_yieldParticles[i]->addParticles(1);
		}
	}

	// TODO: Optimize this by instead having an 'opt in' event system
	// Dispatch the event for any other nodes to pick up on and respond to (e.g to update UI label values)
	//_eventDispatcher->dispatchCustomEvent("onHexiiYield", new EventHexiiYieldData{ this, m_type, yield, getAxialPosition(), m_layer});
}

void Hexii::purchase(BigReal cost) {
	// Note: at this point it is assumed that cost is affordable

	m_purchaseCostLabelOpacity = 0.0f;
	setActive(true);

	Resources::getInstance()->addGreenMatter(-cost);
	Resources::getInstance()->addHexiiInlayer(m_layer);

	// Trigger the purchase event
	EventUtility::dispatchEvent(Hexii::EVENT_PURCHASE, eventID, EventHexiiPurchaseData{ this, m_active, getAxialPosition(), m_layer });
}

void to_json(json& j, const Hexii& hex) {
	//auto& yieldTargets = hex.getYieldTargets();
	//json yieldTargetsJSON = json::array();
	//for (uint i = 0; i < yieldTargets.size(); i++) yieldTargetsJSON.push_back(json::object({ {"posAxial", yieldTargets[i]->getAxialPosition()} }));

	j = json{ 
		{"active", hex.getActive() }, 
		{"exp", hex.getEXP()},
		{"redMatterInvested", hex.getRedMatterInvested()},
		{"standardUpgrades", *hex.getStandardUpgradeTracker()},
		{"layer", hex.getLayer() },
		{"posAxial", hex.getAxialPosition() }
		//{"yieldTargets", yieldTargetsJSON}
	};
}