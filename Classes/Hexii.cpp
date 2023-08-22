#include "Hexii.h"
#include "Maths.h"
#include "Resources.h"
#include "ColourSchemes.h"
#include "HexiiPlane.h"
#include "JSON.hpp"

USING_NS_CC;
using namespace nlohmann;

Hexii::Hexii(const uint district, const Vec2 posAxial) : 
	Hexagon(posAxial), 
	m_district(district),
	m_type(district == 0 ? HexiiType::QueensChamber : HexiiType::WorkersCell)
{}

Hexii::Hexii(const json& data) : 
	Hexagon(data.at("posAxial")),
	m_district(data.at("district")), 
	m_type(m_district == 0 ? HexiiType::QueensChamber : HexiiType::WorkersCell) 
{	
	try {
		auto beeGroups = data.at("beeGroups");

		if (!beeGroups.is_null()) {
			for (json::iterator it = beeGroups.begin(); it != beeGroups.end(); it++) {
				auto& groupData = *it;

				BeeGroup group(groupData, &m_upgrades, m_district);

				m_beeGroups.push_back(group);
			}
		}
	}
	catch (json::out_of_range) { /* No bee groups stored */	}
	
	try { addEXP(data.at("exp"), true); }
	catch (json::out_of_range) {}

	m_upgrades = BoolMap(data.at("upgrades"));
}

bool Hexii::init() {
	Vec2 axialPos = getAxialPosition();

	switch (m_type) {
	case HexiiType::QueensChamber:
		setName("Queen's Chamber");
		break;
	case HexiiType::WorkersCell:
		setName("Worker's Cell");
		break;
	}

	if (m_beeGroups.size() == 0) addBeeGroup(UpgradePath::getWorkerPath(), BeeGroup::BeeType::WORKER);

	/// Hex sprite init

	m_hex = Sprite::create("gameplay/L" + std::to_string(m_district) + "HexiiInactive.png");
	// TODO: Investigate the use of anchor points with the hexii
	m_hex->setAnchorPoint(Vec2(0, 0));

	setContentSize(m_hex->getContentSize());
	auto& size = getContentSize();

	/// Shader init

	m_progressShader = SimpleShaderPtr((SimpleShader::createWithFragmentShader("shaders/hexProgress.frag")));
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

	m_purchaseCostLabel = CompoundLabel::create("", "fonts/OCR.ttf", "fonts/OCR.ttf");
	m_purchaseCostLabel->setStyle(false, true, 45, Color4B::WHITE, Color4B::WHITE, 0, Color4B::WHITE, Size(1, -1), 4);
	m_purchaseCostLabel->setIconTexture("icons/Nectar.png");
	m_purchaseCostLabel->setSpacingConstraint(12.5f);
	m_purchaseCostLabel->setCascadeOpacityEnabled(true);
	m_purchaseCostLabel->setAnchorPoint(Vec2(0.5, 0.5));
	//m_purchaseCostLabel->setPosition(Vec2(size.width / 2 - 12.5f, size.height / 2));

	this->addChild(m_purchaseCostLabel, 1);
	
	/// Particles

	if (m_type == HexiiType::QueensChamber) {
		ParticleSystemQuad* d0Particles = ParticleSystemQuad::createWithTotalParticles(18);
		d0Particles->setPosition(Vec2(0, 0));

		// Gravity
		d0Particles->setEmitterMode(ParticleSystem::Mode::GRAVITY);
		d0Particles->setGravity(Vec2(0, 0));
		d0Particles->setPosVar(Vec2(20, 20));

		// Radial acceleration
		d0Particles->setRadialAccel(200);
		d0Particles->setRadialAccelVar(0);

		// Speed of particles
		d0Particles->setSpeed(140);
		d0Particles->setSpeedVar(14);

		// Angle
		d0Particles->setAngle(90);
		d0Particles->setAngleVar(360);

		d0Particles->setLife(0.6);

		// Size
		d0Particles->setStartSize(100.0f);
		d0Particles->setStartSizeVar(5.0f);
		d0Particles->setEndSize(50.0f);

		// Color
		d0Particles->setStartColor(Color4F(1.0f, 1.0f, 1.0f, 10.0));
		d0Particles->setStartColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
		d0Particles->setEndColor(Color4F(0.0f, 1.0f, 0.0f, 0.0));
		d0Particles->setEndColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));

		d0Particles->setTexture(Director::getInstance()->getTextureCache()->addImage("icons/Nectar.png"));

		m_yieldParticles.push_back(d0Particles);
		this->addChild(m_yieldParticles[0], 2);
	}

	/// Events

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [](cocos2d::Touch*, cocos2d::Event*) { return true; };
	touchListener->onTouchEnded = CC_CALLBACK_2(Hexii::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	EventUtility::addGlobalEventListener(BeeGroup::EVENT_NONLOCAL_UPGRADE_PURCHASED, this, &Hexii::onUpgradePurchased);

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

	bool affordable = Resources::getInstance()->getNectar() >= getPurchaseCost();
	// TODO: Why is this recalculated every frame?!
	BigReal cost = getPurchaseCostFromDistrict(m_district);

	/// Cost label

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	time_t currentTime;
	time(&currentTime);

	if (currentTime - m_timeOfLastPress < 1.0f) m_isHovered = true;
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

void Hexii::addEXP(BigReal exp, bool suppressEvent) {
	m_exp += exp;

	for (auto& group : m_beeGroups) {
		group.updateBeeCount(m_exp, suppressEvent);
	}

	// Nothing further to do if not leveling up
	//if (m_totalEXP >= m_expRequiredForNextLevel) levelUp(suppressEvent);
}

/*
void Hexii::levelUp(bool suppressEvent) {
	// Before beeCount up, record current beeCount
	BigInt beeCountBefore = m_district;
	m_district = getLevelFromEXP(m_totalEXP, m_district);

	// Before updating the exp requirement for the next beeCount, update how much exp into the current beeCount this hex is now
	if (m_district - beeCountBefore == 1) m_exp = m_totalEXP - m_expRequiredForNextLevel;
	else m_exp = m_totalEXP - getEXPRequiredToReachLevelFromdistrict(m_district, m_district);

	m_expRequiredForNextLevel = getEXPRequiredToReachLevelFromdistrict(m_district + 1, m_district);

	// Dispatch event

	// This event will be moved down into BeeGroup
	//if (!suppressEvent)
		//_eventDispatcher->dispatchCustomEvent("onBeeGained", new EventBeeGainedData{ this, beeCountBefore, m_district, m_exp, m_totalEXP, m_expRequiredForNextLevel });
}
*/

void Hexii::addYieldTarget(Hexii* hex, float angleBetween) {
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

BigReal Hexii::getPurchaseCostFromDistrict(uint district) {
	BigReal cost = 6;

	switch (district) {
	case 0:
		break;
	case 1:
		cost = (BigReal)900 * (std::powl(3, Resources::getInstance()->getHexiiCountInDistrict(1)));
		break;
	case 2:
		cost = (BigReal)120000 * std::powl(1.5, Resources::getInstance()->getHexiiCountInDistrict(2));
		break;
	case 3:
		cost = (BigReal)3e7 * std::powl(1.25, Resources::getInstance()->getHexiiCountInDistrict(3));
		break;
	default:
		cost = 1e20;
		break;
	}
	cost = std::floorl(cost);

	return cost;
}

BigReal Hexii::getEXPCost() const {
	// TODO: Adjust the formula

	// Cost =  10 * 6^(district) * (beeCount + 1)
	BigReal royalAffluenceMultiplier = m_upgrades("RoyalAffluence") ? (BigReal)1 / (10 * (m_district + 1)) : 1;
	return 10 * std::powl(6, m_district) * (m_beeGroups[0].getBeeCount() + 1) * royalAffluenceMultiplier;
}

BigReal Hexii::getYield() const {
	if (m_isYieldDirty) {
		BigReal constant = 1;
		BigReal additive = 1;
		BigReal multiplicative = 1;

		for (auto& beeGroup : m_beeGroups) {
			constant += beeGroup.getConstantYield();
			additive += (beeGroup.getAdditiveYieldMultiplier() - 1);
			multiplicative *= (beeGroup.getMultiplicativeYieldMultiplier());
		}

		m_yield = constant
			* additive * multiplicative
			* (1 + HexiiPlane::getInstance()->getAdjacencyBonuses(this))
			* (1 + Resources::getInstance()->getUnity2UpgradeBonus())
			;

		m_isYieldDirty = false;
	}

	return m_yield;
}

BigReal Hexii::getYieldSpeed(bool includeActiveBonus) const {
	if (m_isYieldSpeedDirty) {
		const BigReal base = 1.0 / std::powl(2, m_district);
		BigReal constant = 1;
		BigReal additive = 1;
		BigReal multiplicative = 1;

		m_activeYieldSpeedMultiplier = 2;

		for (auto& beeGroup : m_beeGroups) {
			constant += beeGroup.getConstantYieldSpeed();
			additive += (beeGroup.getAdditiveYieldSpeedMultiplier() - 1);
			multiplicative *= beeGroup.getMultiplicativeYieldSpeedMultiplier();

			m_activeYieldSpeedMultiplier *= beeGroup.getActiveYieldSpeedMultiplier();
		}

		m_yieldSpeed = base * constant * additive * multiplicative;
		
		m_isYieldSpeedDirty = false;
	}

	return includeActiveBonus && m_isPressed ? m_yieldSpeed * m_activeYieldSpeedMultiplier : m_yieldSpeed;
}

BigReal Hexii::getCritical(uint times) const {
	if (m_isCritDirty) {
		m_criticalChance = 0;
		m_criticalYieldMultiplier = 1;

		for (auto& beeGroup : m_beeGroups) {
			m_criticalChance += beeGroup.getCriticalChance();
			m_criticalYieldMultiplier *= beeGroup.getCriticalYieldMultiplier();
		}

		m_isCritDirty = false;
	}

	if (times > 1) {
		BigReal expectedCrits = times * m_criticalChance;
		// Average crit multiplier
		return (expectedCrits * m_criticalYieldMultiplier + (times - expectedCrits)) / times;
	}
	else return cocos2d::rand_0_1() < m_criticalChance ? m_criticalYieldMultiplier : 1;
}

BigReal Hexii::getAdjacencyYieldMultiplier() const {
	if (m_isAdjacencyDirty) {
		m_adjacencyYieldMultiplier = 1;

		for (auto& beeGroup : m_beeGroups) {
			m_adjacencyYieldMultiplier *= beeGroup.getAdjacencyYieldMultiplier();
		}

		m_isAdjacencyDirty = false;
	}

	return m_adjacencyYieldMultiplier;
}

void Hexii::setActive(bool active) {
	if (m_active == active) return;

	m_active = active;

	// Use the appropriate active texture
	if (active) m_hex->setTexture(_director->getTextureCache()->addImage("gameplay/L" + std::to_string(m_district) + "Hexii.png"));
	else m_hex->setTexture(_director->getTextureCache()->addImage("gameplay/L" + std::to_string(m_district) + "HexInactive.png"));

	// Cost label is only visible when the hex is inactive
	m_purchaseCostLabel->setVisible(!active);

	// TODO: Investigate this
	// m_hex->setProgramState(nullptr)
	m_hex->setProgramState(m_progressShader->programState);
}

void Hexii::addBeeGroup(UpgradePath* upgradePath, BeeGroup::BeeType beeType) {
	// Create a new bee group
	m_beeGroups.push_back(BeeGroup(upgradePath, &m_upgrades, beeType, m_district));
	BeeGroup& beeGroup = m_beeGroups.back();

	// Setup listeners
	EventUtility::addTargetedEventListener(beeGroup.EVENT_UPGRADE_PURCHASED, this, beeGroup.eventID, &Hexii::onUpgradePurchased);
	EventUtility::addTargetedEventListener(beeGroup.EVENT_BEE_GAINED, this, beeGroup.eventID, &Hexii::onBeeGained);
}

void Hexii::onBeeGained(cocos2d::EventCustom* evnt) {
	auto data = EventUtility::getEventData<BeeGroup::EventBeeGainedData>(evnt);
	
	// Many upgrades are affected by the number of bees in the bee group, so everything needs to be recalculated
	m_isYieldDirty = m_isYieldSpeedDirty = m_isCritDirty = true;
}

void Hexii::onUpgradePurchased(cocos2d::EventCustom* evnt) {
	auto data = EventUtility::getEventData<BeeGroup::EventUpgradePurchasedData>(evnt);

	if(data->upgrade->actsOnFlags & Upgrade::ACTS_ON_YIELD) m_isYieldDirty = true;
	if(data->upgrade->actsOnFlags & Upgrade::ACTS_ON_SPEED) m_isYieldSpeedDirty = true;
	if(data->upgrade->actsOnFlags & Upgrade::ACTS_ON_CRITS) m_isCritDirty = true;
	if(data->upgrade->actsOnFlags & Upgrade::ACTS_ON_OTHER_HEXII) m_isAdjacencyDirty = true;
}

void Hexii::onTouchBegan() {
	time(&m_timeOfLastPress);
		
	m_progressShader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlayPressed.png"));
	m_isPressed = true;

	if(m_active) _eventDispatcher->dispatchCustomEvent("onHexiiFocus", new EventHexiiFocusData{ this, m_active, getAxialPosition(), m_district});
}

void Hexii::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* evnt) {
	// Ignore this event if it doesn't apply to this hex
	if (!m_isPressed) return;

	m_progressShader->setUniform("overlayTex", Director::getInstance()->getTextureCache()->addImage("gameplay/HexProgressOverlay.png"));

	m_isPressed = false;
}

void Hexii::yield(uint times) {
	BigReal baseYield = getYield();
	BigReal yield = 0;

	// Compute directly if the number of times is small, otherwise use an average
	if (times < 5) for (uint i = 0; i < times; i++) yield += baseYield * getCritical(1);
	else yield = baseYield * getCritical(times);

	// District 0 produces nectar. Outer districts produce EXP for adjacent hexii of lower districts (stored in `m_yieldTargets`)
	if (m_type == HexiiType::QueensChamber) Resources::getInstance()->addNectar(yield);
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
	//_eventDispatcher->dispatchCustomEvent("onHexYield", new EventHexiiYieldData{ this, m_type, yield, getAxialPosition(), m_district});
}

void Hexii::purchase(BigReal cost) {
	// Note: at this point it is assumed that cost is affordable

	m_purchaseCostLabelOpacity = 0.0f;
	setActive(true);

	Resources::getInstance()->addNectar(-cost);
	Resources::getInstance()->addHexiiInDistrict(m_district);

	// TODO: Replace with the improved EventUtility
	// Trigger the purchase event and also then focus to this hex
	_eventDispatcher->dispatchCustomEvent("onHexPurchase", new EventHexiiPurchaseData{ this, m_active, getAxialPosition(), m_district});
	_eventDispatcher->dispatchCustomEvent("onHexiiFocus", new EventHexiiFocusData{ this, m_active, getAxialPosition(), m_district });
}

void to_json(json& j, const Hexii& hex) {
	//auto& yieldTargets = hex.getYieldTargets();
	//json yieldTargetsJSON = json::array();
	//for (uint i = 0; i < yieldTargets.size(); i++) yieldTargetsJSON.push_back(json::object({ {"posAxial", yieldTargets[i]->getAxialPosition()} }));

	j = json{ 
		{"active", hex.getActive() }, 
		{"exp", hex.getEXP()}, 
		{"upgrades", hex.getUpgrades() }, 
		{"district", hex.getDistrict() },
		{"posAxial", hex.getAxialPosition() },
		{"beeGroups", hex.getBeeGroups() }
		//{"yieldTargets", yieldTargetsJSON}
	};
}